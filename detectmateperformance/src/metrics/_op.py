from detectmateperformance.src.match_tree import TreeMatcher
from detectmateperformance.src.types_ import LogTemplates

import detectmateperformance.src.metrics._methods as m

from tqdm import tqdm
import polars as pl


methods = {
    "PA": m.calculate_pa,
    "NED": m.calculate_ned,
    "TS": m.calculate_ts
}


def evaluate(
    logs: list[str],
    ground_templates: list[str],
    templates: list[str],
    n_workers: int = 1,
    batch: int = int(3e+6),
    regex: str = r"(?P<Content>.*)",
    metrics: list[str] = list(methods.keys()),
) -> pl.DataFrame:
    print("Running Ground Truth")
    matcher = TreeMatcher(templates=LogTemplates(ground_templates))
    ground_truth = matcher(
        logs, get_var=False, n_workers=n_workers, batch=batch, regex=regex
    )["Templates"]

    print("Running Templates")
    matcher = TreeMatcher(templates=LogTemplates(templates))
    predicted = matcher(
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



