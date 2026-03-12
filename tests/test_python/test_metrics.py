
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