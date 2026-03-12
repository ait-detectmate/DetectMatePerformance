

from src.detectmateperformance.match_tree import TreeMatcher

datasets = [
    {
        "name": "AIT - Audit",
        "path_temp": "detectmateperformance/tests/test_data/audit_templates.txt",
        "path_logs": "detectmateperformance/tests/test_data/audit.log",
        "regex": r"type=(?P<Type>\w+) msg=audit\((?P<Time>[^:]+):(?P<Serial>\d+)\): (?P<Content>.*)"
    },
    {
        "name": "HDFS",
        "path_temp": "data/hdfs_templates.txt",
        "path_logs": "data/HDFS.log",
        "regex": r'(?P<Date>\S+) (?P<Time>\S+) (?P<Pid>\d+) (?P<Level>\S+) (?P<Component>[^:]+): (?P<Content>.*)' 
    },
    {
        "name": "BGL",
        "path_temp": "data/bgl_templates.txt",
        "path_logs": "data/BGL.log",
        "regex": r'(?P<Label>\S+) (?P<Timestamp>\S+) (?P<Date>\S+) (?P<Node>\S+) (?P<Time>\S+) (?P<NodeRepeat>\S+) (?P<Type>\S+) (?P<Component>\S+) (?P<Level>\S+) (?P<Content>.*)'
    },
]


def load_logs(path_logs) -> list[str]:
    with open(path_logs, "r") as f:
        return f.readlines()


for dataset in datasets:
    logs = load_logs(dataset["path_logs"])
    print(tree_matcher := TreeMatcher.from_file(dataset["path_temp"]))
    print(f"Num logs: {len(logs)} from {dataset['name']}")
    print(tree_matcher(logs, get_var=False, regex=dataset["regex"], n_workers=10, batch=int(3e+6)))
    print("---------------------------------------------------\n")
