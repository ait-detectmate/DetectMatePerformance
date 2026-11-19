from detectmateperformance.match_tree import TreeMatcher
from detectmateperformance.types_ import LogTemplates

from detectmateperformance._iparser import IParser

from detectmateperformance.lib.bind_class import drain_generator

import polars as pl


def cluster_logs_df(df: pl.DataFrame, depth: int = 2, max_child: int = 10) -> dict[str, list[str]]:
    # The first log in each group seeds native clustering.
    df = df.unique(keep="first", maintain_order=True)

    df = df.insert_column(-1, pl.col("Content").str.split(by=" ").list.len().alias("L"))
    df = df.with_columns(
        pl.when(pl.col("L") > max_child).then(max_child).otherwise(pl.col.L).alias("Length")
    ).drop("L")

    cols = ["Length"]
    for i in range(depth):
        cols.append(f"Depth {i}")
        df = df.insert_column(
            -1, pl.col("Content").str.split(by=" ").list.get(i, null_on_oob=True).alias(cols[-1])
        )

    df = df.fill_null("None")
    df = df.with_columns(concat_list=pl.concat_list(cols).list.join("_"))
    df = df.drop(cols)

    # The order of the groups determines template order and event IDs.
    dict_ = (
        df.group_by("concat_list", maintain_order=True)
        .agg(pl.col("Content"))
        .rows_by_key("concat_list")
    )
    return {k: v[0][0] for k, v in dict_.items()}


class Drain(IParser):
    def __init__(
        self, depth: int = 2, max_child: int = 10, sim: float = 0.5
    ) -> None:

        self.depth = depth
        self.max_child = max_child
        self.sim = sim

        super().__init__()

    def __str__(self) -> str:
        msg = "\033[46m >>>> Generating templates    \033[0m\n"
        msg += "\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m\n"
        msg += f"\033[46m  \033[0mDrain <Depth: {self.depth} Max child: {self.max_child} Sim: {self.sim}>"
        msg += f"\n\033[46m  \033[0m  -> (N. logs in buffer {len(self)})\n"
        return msg + "\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m\n"

    def generate_from_df(self, df: pl.DataFrame) -> TreeMatcher:

        print("\033[46m >>>> Generating templates    \033[0m")
        print("\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m")

        print("\033[46m  \033[0m  ⚙️ Clustering logs")
        set_groups = cluster_logs_df(df=df[["Content"]], depth=self.depth, max_child=self.max_child)

        print("\033[46m  \033[0m  ⚙️  Generating templates")
        templates = LogTemplates(drain_generator(list(set_groups.values()), self.sim))

        print("\033[46m  \033[0m  💻  Initializing tree matcher instance")
        tree_matcher = TreeMatcher(templates)

        print("\033[46m  \033[0m  ✅ Process complete!")
        print("\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m")
        return tree_matcher
