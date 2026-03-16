
from detectmateperformance.metrics import evaluate, MultipleEvaluation
import detectmateperformance.metrics._methods as m


import polars as pl


class TestCaseMetrics:
    def test_template_set(self):
        best = pl.DataFrame({
            "GroundTruth": ["t1", "t2"],
            "Templates": ["t2", "t1"]
        })
        worst = pl.DataFrame({
            "GroundTruth": ["t1", "t2"],
            "Templates": ["t3", "t3"]
        })
        average = pl.DataFrame({
            "GroundTruth": ["t1", "t2"],
            "Templates": ["t3", "t1"]
        })

        assert m.calculate_ts(best) == 1.0
        assert m.calculate_ts(average) == 0.5
        assert m.calculate_ts(worst) == 0.0

    def test_parsing_accuracy(self):
        best = pl.DataFrame({
            "GroundTruth": ["t1", "t2"],
            "Templates": ["t1", "t2"]
        })
        worst = pl.DataFrame({
            "GroundTruth": ["t1", "t2"],
            "Templates": ["t2", "t1"]
        })
        average = pl.DataFrame({
            "GroundTruth": ["t1", "t2"],
            "Templates": ["t1", "t1"]
        })

        assert m.calculate_pa(best) == 1.0
        assert m.calculate_pa(average) == 0.5
        assert m.calculate_pa(worst) == 0.0

    def test_parsing_ned(self):
        best = pl.DataFrame({
            "GroundTruth": ["t1", "t2"],
            "Templates": ["t1", "t2"]
        })
        worst = pl.DataFrame({
            "GroundTruth": ["t1", "t2"],
            "Templates": ["2a", "1a"]
        })
        average = pl.DataFrame({
            "GroundTruth": ["t2", "t2"],
            "Templates": ["t1", "t1"]
        })

        assert m.calculate_ned(best) == 1.0
        assert m.calculate_ned(average) == 0.5
        assert m.calculate_ned(worst) == 0.0

    def test_parsing_ga(self):
        best = pl.DataFrame({
            "GroundTruth": ["t1", "t2", "t2"],
            "Templates": ["t4", "t5", "t5"]
        })
        worst = pl.DataFrame({
            "GroundTruth": ["t1", "t2", "t2"],
            "Templates": ["t2", "t2", "t2"]
        })
        average = pl.DataFrame({
            "GroundTruth": ["t1", "t2", "t2"],
            "Templates": ["t1", "t2", "t3"]
        })

        assert m.calculate_ga(best) == 1.0
        assert m.calculate_ga(average) == 0.3333333333333333
        assert m.calculate_ga(worst) == 0.0

    def test_parsing_fr(self):
        best = pl.DataFrame({
            "GroundTruth": ["t1", "t2", "t2"],
            "Templates": ["t4", "t5", "t5"]
        })
        worst = pl.DataFrame({
            "GroundTruth": ["t1", "t2", "t2"],
            "Templates": ["template not found", "template not found", "template not found"]
        })
        average = pl.DataFrame({
            "GroundTruth": ["t1", "t2"],
            "Templates": ["t1", "template not found"]
        })

        assert m.calculate_fr(best) == 1.0
        assert m.calculate_fr(average) == 0.5
        assert m.calculate_fr(worst) == 0.0


def load_file(path_logs) -> list[str]:
    with open(path_logs, "r") as f:
        return f.readlines()


dataset = {
    "name": "AIT - Audit",
    "path_temp": "tests/test_data/audit_templates.txt",
    "path_temp_incomplete": "tests/test_data/incomplete_templates.txt",
    "path_logs": "tests/test_data/audit.log",
    "regex": r"type=(?P<Type>\w+) msg=audit\((?P<Time>[^:]+):(?P<Serial>\d+)\): (?P<Content>.*)"
}


class TestCaseRunMetrics:
    def test_evaluate(self):
        evaluation = evaluate(
            logs=load_file(dataset["path_logs"]),
            ground_templates=dataset["path_temp"],
            templates=dataset["path_temp"],
            regex=dataset["regex"]
        )

        for k in evaluation:
            assert evaluation[k] == 1.0

        evaluation = evaluate(
            logs=load_file(dataset["path_logs"]),
            ground_templates=dataset["path_temp"],
            templates=dataset["path_temp_incomplete"],
            regex=dataset["regex"]
        )

        for k in evaluation:
            assert evaluation[k] != 1.0

    def test_multiple_evaluation(self):
        m_evaluation = MultipleEvaluation(
            logs=load_file(dataset["path_logs"]),
            ground_templates=dataset["path_temp"],
            regex=dataset["regex"],
        )

        evaluation = m_evaluation(dataset["path_temp"])
        for k in evaluation:
            assert evaluation[k] == 1.0

        evaluation = m_evaluation(dataset["path_temp_incomplete"])
        for k in evaluation:
            assert evaluation[k] != 1.0
