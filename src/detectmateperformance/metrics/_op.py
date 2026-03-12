from detectmateperformance.match_tree import TreeMatcher
from detectmateperformance.types_ import LogTemplates

import detectmateperformance.metrics._methods as m

from tqdm import tqdm
import polars as pl


methods = {
    "PA": m.calculate_pa,
    "NED": m.calculate_ned,
    "TS": m.calculate_ts,
    "GA": m.calculate_ga,
}


def __evaluation(
    logs: list[str],
    ground_matcher: TreeMatcher,
    predicted_matcher: TreeMatcher,
    n_workers: int = 1,
    batch: int = int(3e+6),
    regex: str = r"(?P<Content>.*)",
    metrics: list[str] = list(methods.keys()),
) -> pl.DataFrame:
    print("Running Ground Truth")
    ground_truth = ground_matcher(
        logs, get_var=False, n_workers=n_workers, batch=batch, regex=regex
    )["Templates"]

    print("Running Templates")
    predicted = predicted_matcher(
        logs, get_var=False, n_workers=n_workers, batch=batch, regex=regex
    )["Templates"]

    results = pl.DataFrame()
    results.insert_column(0, ground_truth.rename("GroundTruth"))
    results.insert_column(1, predicted)
    print(results)

    final = {}
    for m in tqdm(metrics, desc="Running metrics..."):
        final[m] = methods[m](results)

    return final


def evaluate(
    logs: list[str],
    ground_templates: list[str],
    templates: list[str],
    n_workers: int = 1,
    batch: int = int(3e+6),
    regex: str = r"(?P<Content>.*)",
    metrics: list[str] = list(methods.keys()),
) -> pl.DataFrame:
    return __evaluation(
        logs=logs,
        ground_matcher=TreeMatcher(templates=LogTemplates(ground_templates)),
        predicted_matcher=TreeMatcher(templates=LogTemplates(templates)),
        n_workers=n_workers,
        batch=batch,
        regex=regex,
        metrics=metrics,
    )


def evaluate_from_file(
    logs: list[str],
    ground_templates_path: str,
    templates_path: str,
    n_workers: int = 1,
    batch: int = int(3e+6),
    regex: str = r"(?P<Content>.*)",
    metrics: list[str] = list(methods.keys()),
) -> pl.DataFrame:
    return __evaluation(
        logs=logs,
        ground_matcher=TreeMatcher.from_file(ground_templates_path),
        predicted_matcher=TreeMatcher.from_file(templates_path),
        n_workers=n_workers,
        batch=batch,
        regex=regex,
        metrics=metrics,
    )
