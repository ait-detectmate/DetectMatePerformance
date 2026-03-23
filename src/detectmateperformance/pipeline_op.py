
from tqdm import tqdm
from typing import Callable

from detectmateperformance.types_ import ParsedLogs

import polars as pl
import gc


def preprocessing(logs: list[str], regex: str) -> pl.DataFrame:
    df = (
        pl.DataFrame({"Message": logs})
        .with_columns(
            pl.col("Message")
            .str.extract_groups(regex)
            .alias("parts")
        )
        .unnest("parts")
    ).drop("Message")
    df = df.drop_nulls()

    del logs
    gc.collect()

    return df


def add_parsed(df: pl.DataFrame, results: ParsedLogs) -> pl.DataFrame:
    vars = results.get_all_vars()
    if vars is not None:
        df.insert_column(df.shape[1], pl.Series("ParamList", vars))

    if "Templates" in df:
        df = df.with_columns(pl.Series("Templates", results.get_all_templates()))
        df = df.with_columns(pl.Series("EventIDs", results.get_all_events_ids()))
    else:
        df = df.insert_column(df.shape[1], pl.Series("Templates", results.get_all_templates()))
        df = df.insert_column(df.shape[1], pl.Series("EventIDs", results.get_all_events_ids()))

    return df


def postprocessing(df: pl.DataFrame) -> pl.DataFrame:
    df = df.with_columns(pl.col("Templates").str.replace_all("VAR", "<*>"))
    return df


def run_batches(
    func: Callable[[list[str], bool, int], ParsedLogs],
    table: pl.DataFrame,
    get_var: bool = False,
    n_workers: int = 1,
    batch: int = int(3e+6),
) -> pl.DataFrame:

    first = True
    print(">>> Matching data")
    for i in tqdm(range(batch, len(table) + batch, batch)):
        results = func(  # type: ignore
            table["Content"][i-batch: i].to_list(), get_var=get_var, n_workers=n_workers
        )
        if first:
            df = add_parsed(df=table[i-batch: i], results=results)
            first = False
        else:
            df = pl.concat([df, add_parsed(df=table[i-batch: i], results=results)])
        del results

    print(">>> Postprocessing results")
    return postprocessing(df)


def run_full_pipeline(
    func: Callable[[list[str], bool, int], ParsedLogs],
    logs: list[str],
    get_var: bool = False,
    n_workers: int = 1,
    batch: int = int(3e+6),
    regex: str = r"(?P<Content>.*)"
) -> pl.DataFrame:

    print(">>> Preprocesing logs")
    table = preprocessing(logs, regex=regex)

    return run_batches(
        func=func,
        table=table,
        get_var=get_var,
        n_workers=n_workers,
        batch=batch,
    )
