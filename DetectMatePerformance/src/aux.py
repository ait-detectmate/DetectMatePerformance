
from DetectMatePerformance.src.types_ import ParsedLogs

import polars as pl


def convet2polars(logs: list[str], results: ParsedLogs) -> pl.DataFrame:
    df = pl.DataFrame({
        "Content": logs,
        "Templates": results.get_all_templates()
    })
    df = df.with_columns(pl.col("Templates").str.replace_all("VAR", "<*>"))

    if (vars := results.get_all_vars()) is not None:
        df.insert_column(2, pl.Series("ParamList", vars))

    eventids = {e: i for i, e in enumerate(df["Templates"].unique())}
    df.insert_column(
        0, df.map_rows(lambda x: eventids[x[1]]).to_series().rename("EventIDs")
    )

    return df
