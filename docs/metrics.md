# Metrics

Parse metrics use to benchmark parsers:

* `Parsed Accuracy (PA)`
* `Normalize Edit Distance (NED)`
* `Template Set (TS)`
* `Group Accuracy (GA)`
* `Found Ratio (FR)`

## Evaluation

Evaluation method to compare ground truth templates against parsed templates using a test dataset of logs.

```python
def evaluate(
    logs: list[str],
    ground_templates: list[str] | LogTemplates | str,
    templates: list[str] | LogTemplates | str,
    n_workers: int = 1,
    batch: int = int(3e+6),
    regex: str = r"(?P<Content>.*)",
    metrics: list[str] = list(methods.keys()),
) -> pl.DataFrame:
    pass
```

### Usage

```python
from detectmateperformance.metrics import evaluate


evaluation = evaluate(
    logs=load_file(dataset["path_logs"]),
    ground_templates=dataset["path_temp"],
    templates=dataset["path_temp"],
    regex=dataset["regex"]
)

for k in evaluation:
    assert evaluation[k] == 1.0
```

## Multiple evaluation

Evaluation class to allow multiple parsed templates to be compare against the same grounth truth (without the need of recalculate the preprocessing or the ground truth).

```python
from detectmateperformance.metrics import MultipleEvaluation


m_evaluation = MultipleEvaluation(
    logs=load_file(dataset["path_logs"]),
    ground_templates=dataset["path_temp"],
    regex=dataset["regex"],
)

valuation = m_evaluation(dataset["path_temp"])
for k in evaluation:
    assert evaluation[k] == 1.0

evaluation = m_evaluation(dataset["path_temp_incomplete"])
for k in evaluation:
    assert evaluation[k] != 1.0
```
