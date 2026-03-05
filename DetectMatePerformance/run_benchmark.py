

from DetectMatePerformance.src.match_tree import TreeMatcher

path_temp = "DetectMatePerformance/tests/test_data/audit_templates.txt"
path_logs = "DetectMatePerformance/tests/test_data/audit.log"


def load_logs() -> list[str]:
    logs = []
    with open(path_logs, "r") as f:
        for log in f.readlines():
            logs.append("):".join(log.split("):")[1:]))
    return logs

print(tree_matcher := TreeMatcher.from_file(path_temp))

logs = load_logs()
print(f"Num logs: {len(logs)}")

