from detectmateperformance.match_tree import TreeMatcher
from detectmateperformance.types_ import LogTemplates

import polars as pl

path_temp = "tests/test_data/audit_templates.txt"
path_logs = "tests/test_data/audit.log"


def load_logs() -> list[str]:
    logs = []
    with open(path_logs, "r") as f:
        for log in f.readlines():
            logs.append("):".join(log.split("):")[1:]))
    return logs


class TestCaseTreeMatcher:
    def test_init(self):
        assert len(TreeMatcher.from_file(path_temp)) == 9

        templates = LogTemplates.from_file(path_temp)
        assert len(TreeMatcher(templates)) == 9 

    def test_match_log(self):
        tree_matcher = TreeMatcher(LogTemplates(["Hello`=<*> kenobi"]))
        log = "Hello`=there general kenobi"

        assert tree_matcher.match_log(log, False)[0] == "Hello VAR kenobi"

    def test_match_log_with_var(self):
        tree_matcher = TreeMatcher(LogTemplates(["Hello`=<*> kenobi"]))
        log = "Hello`=there general kenobi"

        result = tree_matcher.match_log(log, True)
        assert result[0] == ("Hello VAR kenobi", ["there", "general"])
 
    def test_match_batch(self):
        tree_matcher = TreeMatcher(LogTemplates(["Hello`=<*> kenobi"]))
        logs = ["Hello`=there general kenobi", "roger roger"]
        results = tree_matcher.match_batch(logs, False)

        assert results[0] == "Hello VAR kenobi"
        assert results[1] == "template not found"

    def test_match_batch_with_var(self):
        tree_matcher = TreeMatcher(LogTemplates(["Hello`=<*> kenobi"]))
        logs = ["Hello`=there general kenobi", "roger roger"]
        results = tree_matcher.match_batch(logs, True)

        assert results[0] == ("Hello VAR kenobi", ["there", "general"])
        assert results[1] == ("template not found", [])

    def test_big_batch(self):
        logs = load_logs()
        tree_matcher = TreeMatcher.from_file(path_temp)
        results = tree_matcher.match_batch(logs, False, n_workers=3)

        for i in range(len(results)):
            assert "template not found" != results[i]

    def test_big_batch_with_var(self):
        logs = load_logs()
        tree_matcher = TreeMatcher.from_file(path_temp)
        results = tree_matcher.match_batch(logs, True, n_workers=3)

        for i in range(len(results)):
            assert "template not found" != results[i][0]

    def test_call(self):
        logs = load_logs()
        tree_matcher = TreeMatcher.from_file(path_temp)
        results = tree_matcher(logs, True, n_workers=3)

        assert isinstance(results, pl.DataFrame)
        assert len(logs) == len(results)
