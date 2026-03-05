

from DetectMatePerformance.src.match_tree import TreeMatcher

path_temp = "DetectMatePerformance/tests/test_data/audit_templates.txt"
path_logs = "DetectMatePerformance/tests/test_data/audit.log"
regex = r"type=(?P<Type>\w+) msg=audit\((?P<Time>[^:]+):(?P<Serial>\d+)\): (?P<Content>.*)"

def load_logs() -> list[str]:
    logs = []
    with open(path_logs, "r") as f:
        for log in f.readlines():
            logs.append(log)
    return logs

print(tree_matcher := TreeMatcher.from_file(path_temp))

logs = load_logs()
print(f"Num logs: {len(logs)}")


print(tree_matcher(logs, get_var=True, regex=regex))