from DetectMatePerformance.src.match_tree import TreeMatcher
from DetectMatePerformance.src.types_ import LogTemplates


path_temp = "DetectMatePerformance/tests/test_data/audit_templates.txt"
path_logs = "DetectMatePerformance/tests/test_data/audit.log"


def load_logs() -> list[str]:
    with open(path_logs, "r") as f:
        return f.readlines()


class TestCaseTreeMatcher:
    def test_init(self):
        assert len(TreeMatcher.from_file(path_temp)) == 9

        templates = LogTemplates.from_file(path_temp)
        assert len(TreeMatcher(templates)) == 9 

    def test_match_log(self):
        tree_matcher = TreeMatcher(LogTemplates(["Hello`=<*> kenobi"]))
        log = "Hello`=there general kenobi"

        assert tree_matcher.match_log(log, False)[0] == "Hello VAR kenobi"
 
    def test_match_batch(self):
        tree_matcher = TreeMatcher(LogTemplates(["Hello`=<*> kenobi"]))
        logs = ["Hello`=there general kenobi", "roger roger"]
        results = tree_matcher.match_batch(logs, False)

        assert results[0] == "Hello VAR kenobi"
        assert results[1] == "template not found"