
from DetectMatePerformance.src.types_ import ParsedLogs

import polars as pl


def generate_table(logs: list[str], regex: str) -> pl.DataFrame:
    return (
        pl.DataFrame({"Message": logs})
        .with_columns(
            pl.col("Message")
            .str.extract_groups(regex)  
            .alias("parts")
        )
        .unnest("parts") 
    )


def add_parsed(df: pl.DataFrame, results: ParsedLogs) -> pl.DataFrame:
    df.insert_column(df.shape[1], pl.Series("Templates", results.get_all_templates()))
    df = df.with_columns(pl.col("Templates").str.replace_all("VAR", "<*>"))

    eventids = {e: i for i, e in enumerate(df["Templates"].unique())}
    df.insert_column(
        df.shape[1] - 1, df.map_rows(lambda x: eventids[x[-1]]).to_series().rename("EventIDs")
    )

    if (vars := results.get_all_vars()) is not None:
        df.insert_column(df.shape[1], pl.Series("ParamList", vars))


    return df
