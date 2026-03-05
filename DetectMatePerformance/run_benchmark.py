

from DetectMatePerformance.src.match_tree import TreeMatcher

datasets = [
    {
        "name": "AIT - Audit",
        "path_temp": "DetectMatePerformance/tests/test_data/audit_templates.txt",
        "path_logs": "DetectMatePerformance/tests/test_data/audit.log",
        "regex": r"type=(?P<Type>\w+) msg=audit\((?P<Time>[^:]+):(?P<Serial>\d+)\): (?P<Content>.*)"
    }
]

def load_logs(path_logs) -> list[str]:
    with open(path_logs, "r") as f:
        return f.readlines()



for dataset in datasets:
    logs = load_logs(dataset["path_logs"])
    print(tree_matcher := TreeMatcher.from_file(dataset["path_temp"]))
    print(f"Num logs: {len(logs)} from {dataset['name']}")
    print(tree_matcher(logs, get_var=True, regex=dataset["regex"]))
    print("---------------------------------------------------\n")