from detectmateperformance._iparser import IParser


import polars as pl


def cluter_logs(logs: pl.DataFrame, weight: float = 0.5) -> pl.DataFrame:
    # Keep all tokens so we can inspect every position after grouping.
    tokens = (
        logs
        .with_row_index("log_id")
        .with_columns(word=pl.col("Content").str.extract_all(r"\S+"))
        .with_columns(length=pl.col("word").list.len())
        .with_columns(position=pl.int_ranges(0, pl.col("length"), dtype=pl.UInt32))
        .explode("word", "position")
        .with_columns(frequency=pl.len().over("position", "word"))
    )

    return (
        tokens
        .filter(pl.col("frequency") >= weight * pl.col("frequency").max().over("log_id"))
        .group_by("log_id", "frequency")
        .agg(
            size=pl.len(),
            pattern=pl.struct("position", "word").sort_by("position"),
        )
        .sort(["size", "frequency"], descending=[True, True])
        .unique(subset="log_id", keep="first")
        .select("log_id", "pattern")
        .join(tokens, on="log_id")
        .with_columns(
            constant=pl.col("word").n_unique().over("length", "pattern", "position") == 1
        )
        .filter(pl.col("constant"))
        .group_by("length", "pattern")
        .agg(
            Template=pl.struct("position", "word").unique(),
            members=pl.col("log_id").unique()
        )
        .select(Lenght="length", Template="Template")
    )


class Brain(IParser):
    def __init__(self, weight: float = 0.5) -> None:
        self.weight = weight
        super().__init__()

    def __str__(self) -> str:

        msg = "\033[46m >>>> Generating templates    \033[0m\n"
        msg += "\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m\n"
        msg += f"\033[46m  \033[0mDrain <Weight: {self.weight}>"
        msg += f"\n\033[46m  \033[0m  -> (N. logs in buffer {len(self)})\n"
        return msg + "\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m\n"
