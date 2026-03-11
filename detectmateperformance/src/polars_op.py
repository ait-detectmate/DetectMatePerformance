
from tqdm import tqdm
from typing import Callable
from detectmateperformance.src.types_ import ParsedLogs

import polars as pl
import gc

def generate_table(logs: list[str], regex: str) -> pl.DataFrame:
    return (
        pl.DataFrame({"Message": logs})
        .with_columns(
            pl.col("Message")
            .str.extract_groups(regex)  
            .alias("parts")
        )
        .unnest("parts") 
    ).drop("Message")


def add_parsed(df: pl.DataFrame, results: ParsedLogs) -> pl.DataFrame: 
    vars = results.get_all_vars()
    if vars is not None:
        df.insert_column(df.shape[1], pl.Series("ParamList", vars))
    df.insert_column(df.shape[1], pl.Series("Templates", results.get_all_templates()))

    return df


def postprocessing(df: pl.DataFrame) -> pl.DataFrame:
    df = df.with_columns(pl.col("Templates").str.replace_all("VAR", "<*>"))
    eventids = {e: i for i, e in enumerate(df["Templates"].unique())}
    df.insert_column(
        df.shape[1] - 1, df.map_rows(lambda x: eventids[x[-1]]).to_series().rename("EventIDs")
    )

    return df


def run_full_pipeline(
    func: Callable[[list[str], bool, int], ParsedLogs],
    logs: list[str],
    get_var: bool = False,
    n_workers: int = 1,
    batch = int(3e+6),
    regex: str = r"(?P<Content>.*)"
) -> pl.DataFrame:

    first = True
    print(">>> Preprocesing logs")
    table = generate_table(logs, regex=regex)
    table = table.drop_nulls()
    del logs
    gc.collect()

    for i in tqdm(range(batch, len(table) + batch, batch)):
        print(">>> Matching data")
        results = func(
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
