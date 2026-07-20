
import polars as pl
import re


def cluster_logs_df(df: pl.DataFrame, depth: int = 2, max_child: int = 10) -> dict[str, list[str]]:
    df = df.with_columns(pl.col("Content").str.replace_all(r"[^a-zA-Z0-9\s]", " "))
    df = df.with_columns(pl.col("Content").str.replace_all(r"\d+", "<*>").str.replace_all(r"\s+", " "))
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


def cluster_logs_stream(log: str, depth: int = 2, max_child: int = 10) -> dict[str, list[str]]:
    log = re.sub(r"[^a-zA-Z0-9\s]", " ", log)
    log = re.sub(r"\d+", "<*>", log)
    log = re.sub(r"\s+", " ", log)
    log_split = log.split(" ")

    key = f"{n if (n := len(log_split)) < max_child else max_child}"
    for i in range(depth):
        key += f"_{'None' if i >= n else log_split[i]}"
    return {key: [log]}
