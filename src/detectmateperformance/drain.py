from detectmateperformance.match_tree import TreeMatcher
from detectmateperformance.types_ import LogTemplates

from detectmateperformance.pipeline_op import preprocessing

from detectmateperformance.lib.bind_class import drain_generator

import polars as pl
import warnings
import re


def replace_patterns(text: str) -> str:
    text = re.sub(r'\b(?:\d{1,3}\.){3}\d{1,3}\b', "VAR", text)
    text = re.sub(r'(?:[a-zA-Z]:\\.[^\\\-]*|/[^\\\-\s]*|\.\.[^\\\-]*|\.\/[^\\\-]*)\b', "VAR", text)
    text = re.sub(r'(?<!\w)(?:-\d+|\d+-\d+|\d+\s\d+)(?!\w)', "VAR", text)
    return text


def clean_strings(df: pl.DataFrame) -> pl.DataFrame:
    df = df.with_columns(
        pl.concat_str([pl.lit(" "), pl.col("Content"), pl.lit(" ")]).alias("Content")
    )
    return df.with_columns(
        pl.col("Content")
        .str.replace_all(r'[!"#$%&\'()*+,:;<=>?@\[\\\]^`{|}~\s]', " ")
        .str.replace_all(r'\.\s|\s\.', " ")
        .str.replace_all(r'-\s|\s-', " ")
        .map_elements(replace_patterns)
        .str.replace_all(r' \d+ ', " VAR ")
        .str.replace_all(r"\s+", " ")
        .str.strip_chars()
    )


def cluster_logs_df(df: pl.DataFrame, depth: int = 2, max_child: int = 10) -> dict[str, list[str]]:
    df = clean_strings(df)
    df = df.unique()

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

    dict_ = df.group_by("concat_list").agg(pl.col("Content")).rows_by_key("concat_list")
    return {k: v[0][0] for k, v in dict_.items()}


class Drain:
    def __init__(
        self, depth: int = 2, max_child: int = 10, sim: float = 0.5
    ) -> None:

        self.depth = depth
        self.max_child = max_child
        self.sim = sim
        warnings.warn("This method is still a prototype!!")

        self.reset()

    def __str__(self) -> str:
        msg = "\033[46m >>>> Generating templates    \033[0m\n"
        msg += "\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m\n"
        msg += f"\033[46m  \033[0mDrain <Depth: {self.depth} Max child: {self.max_child} Sim: {self.sim}>"
        msg += f"\n\033[46m  \033[0m  -> (N. logs in buffer {len(self)})\n"
        return msg + "\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m\n"

    def __len__(self) -> int:
        return len(self.buffer)

    def add(self, log: str) -> None:
        self.buffer.append(log)

    def reset(self) -> None:
        self.buffer: list[str] = []

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

    def generate(self) -> TreeMatcher:
        return self.generate_from_df(pl.DataFrame({"Content": self.buffer}))

    def __call__(
        self, logs: list[str] | pl.DataFrame | str, regex: str = r"(?P<Content>.*)"
    ) -> TreeMatcher:
        if not isinstance(logs, pl.DataFrame):
            logs = preprocessing(logs=logs, regex=regex)

        return self.generate_from_df(logs)
