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
    "FR": m.calculate_fr,
}


def _init_temp(templates: list[str] | LogTemplates | str) -> LogTemplates:
    if isinstance(templates, LogTemplates):
        return templates
    if isinstance(templates, str):
        return LogTemplates.from_file(templates)
    return LogTemplates(templates)


class MultipleEvaluation:
    def __init__(
        self,
        logs: list[str],
        ground_templates: list[str] | LogTemplates | str,
        n_workers: int = 1,
        batch: int = int(3e+6),
        regex: str = r"(?P<Content>.*)",
        metrics: list[str] = list(methods.keys()),
    ) -> None:

        self.n_workers, self.batch = n_workers, batch
        self.metrics = metrics

        self.table = TreeMatcher(_init_temp(ground_templates))(
            logs=logs,
            get_var=False,
            n_workers=n_workers,
            batch=batch,
            regex=regex,
        )[["Templates", "Content"]]
        self.table.insert_column(0, self.table["Templates"].rename("GroundTruth"))

    def __call__(self, templates: list[str] | LogTemplates | str) -> dict[str, float]:

        self.table = TreeMatcher(_init_temp(templates))(
            logs=self.table,
            get_var=False,
            n_workers=self.n_workers,
            batch=self.batch
        )[["GroundTruth", "Templates", "Content"]]

        print(self.table)

        final = {}
        for me in tqdm(self.metrics, desc="Running metrics..."):
            final[me] = methods[me](self.table)

        return final


def evaluate(
    logs: list[str],
    ground_templates: list[str] | LogTemplates | str,
    templates: list[str] | LogTemplates | str,
    n_workers: int = 1,
    batch: int = int(3e+6),
    regex: str = r"(?P<Content>.*)",
    metrics: list[str] = list(methods.keys()),
) -> pl.DataFrame:
    print("Running Ground Truth")
    ground_truth = TreeMatcher(_init_temp(ground_templates))(
        logs, get_var=False, n_workers=n_workers, batch=batch, regex=regex
    )["Templates"]

    print("Running Templates")
    predicted = TreeMatcher(_init_temp(templates))(
        logs, get_var=False, n_workers=n_workers, batch=batch, regex=regex
    )["Templates"]

    results = pl.DataFrame()
    results.insert_column(0, ground_truth.rename("GroundTruth"))
    results.insert_column(1, predicted)
    print(results)

    final = {}
    for me in tqdm(metrics, desc="Running metrics..."):
        final[me] = methods[me](results)

    return final
