
from tqdm import tqdm
from typing import Callable

from detectmateperformance.types_ import ParsedLogs

import polars as pl
import gc


# %% Generic one
def load_logs(path: str) -> pl.DataFrame:
    try:
        return pl.read_csv(
            path,
            has_header=False,
            new_columns=['Message'],
            separator='\n',
            null_values=None,
        )
    except pl.exceptions.ComputeError:
        print("⚠️  Load logs failed, trying a slower method")
        with open(path, "r") as f:
            return pl.DataFrame({"Messag": f.readlines()})


def preprocessing(logs: list[str] | str, regex: str = r"(?P<Content>.*)") -> pl.DataFrame:
    df = load_logs(logs) if isinstance(logs, str) else pl.DataFrame({"Message": logs})
    df = (
        df.with_columns(pl.col("Message").str.extract_groups(regex).alias("parts")).unnest("parts")
    ).drop("Message")
    df = df.drop_nulls()

    del logs
    gc.collect()

    return df


# %% Pipeline of Tree Matcher
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
    if "ParamList" in df.columns:
        df = df.with_columns(pl.col("ParamList").str.split(by=" "))
    return df


def run_batches(
    func: Callable[[list[str], bool, int], ParsedLogs],
    table: pl.DataFrame,
    get_var: bool = False,
    n_workers: int = 1,
    batch: int = int(3e+6),
) -> pl.DataFrame:

    first = True
    print("\033[46m  \033[0m  🛠️  Matching data")
    for i in tqdm(
        range(batch, len(table) + batch, batch),
        desc="\033[46m  \033[0m  Matching logs ⏳",
        unit="logs",
        bar_format='{l_bar}{bar:10}{r_bar}{bar:-10b}'
    ):
        results = func(  # type: ignore
            table["Content"][i-batch: i].to_list(), get_var=get_var, n_workers=n_workers
        )
        if first:
            df = add_parsed(df=table[i-batch: i], results=results)
            first = False
        else:
            df = pl.concat([df, add_parsed(df=table[i-batch: i], results=results)])
        del results

    print("\033[46m  \033[0m  🛠️  Postprocessing results")
    df = postprocessing(df)
    print("\033[46m  \033[0m  ✅   Finish")
    print("\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m")
    return df


def run_full_pipeline(
    func: Callable[[list[str], bool, int], ParsedLogs],
    logs: list[str] | str,
    get_var: bool = False,
    n_workers: int = 1,
    batch: int = int(3e+6),
    regex: str = r"(?P<Content>.*)"
) -> pl.DataFrame:

    print("\033[46m  \033[0m  🛠️  Preprocesing logs")
    table = preprocessing(logs, regex=regex)

    if "Content" not in table.columns:
        print("\033[46m  \033[0m   ⚠️  Content not found")
        return table

    return run_batches(
        func=func,
        table=table,
        get_var=get_var,
        n_workers=n_workers,
        batch=batch,
    )
