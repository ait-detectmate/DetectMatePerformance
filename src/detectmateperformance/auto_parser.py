
from detectmateperformance.match_tree import TreeMatcher
from detectmateperformance.types_ import LogTemplates

from detectmateperformance.lib.bind_class import auto_parser
import polars as pl


python_regex: list[str] = [
    r"type=(?P<Type>\w+) msg=audit\((?P<Time>[^:]+):(?P<Serial>\d+)\): (?P<Content>.*)",
    r'(?P<Label>\S+) (?P<Timestamp>\S+) (?P<Date>\S+) (?P<Node>\S+) (?P<Time>\S+) (?P<NodeRepeat>\S+) (?P<Type>\S+) (?P<Component>\S+) (?P<Level>\S+) (?P<Content>.*)',  # noqa: E501
    r'(?P<Month>\S+) (?P<Day>\S+) (?P<Time>\S+) (?P<Value>\S+) (?P<Content>.*)',
    r'(?P<Date>\S+) (?P<Time>\S+) (?P<Pid>\d+) (?P<Level>\S+) (?P<Component>[^:]+): (?P<Content>.*)',
    r'(?P<Date>\S+) (?P<Time>\S+) (?P<IP>\S+) (?P<Content>.*)',
    r'(?P<Month>\S+)\s+(?P<Day>\S+)\s+(?P<Time>\S+)\s+(?P<Node>\S+)\s+(?P<Keyword>\S+?)\[(?P<ID>\d+)\]:\s+(?P<Content>.*)',  # noqa: E501
    r'(?P<Label>\S+) (?P<Timestamp>\S+) (?P<Date>\S+) (?P<User>\S+) (?P<Month>\S+) (?P<Day>\S+) (?P<Time>\S+) (?P<Location>\S+) (?P<Component>\S+): (?P<Content>.*)',  # noqa: E501
]


class AutoParse:
    def __init__(self, num_use: int = 10) -> None:
        self.path = "src/detectmateperformance/_templates/"  # TODO: change so it works for pip
        self.reset()
        self.num_use = num_use

    def __len__(self) -> int:
        return len(self.buffer)

    def add(self, log: str) -> None:
        self.buffer.append(log)

    def reset(self) -> None:
        self.buffer: list[str] = []

    def generate(self) -> tuple[TreeMatcher, bytes]:
        print("\033[46m >>>> Searching templates    \033[0m")
        print("\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m")

        sample = self.buffer[:self.num_use]
        print(f"\033[46m  \033[0m  💻 Sampling {len(sample)}")

        print("\033[46m  \033[0m  🔎 Doing the template search")
        templates, idx = auto_parser(sample, self.path)
        templates = LogTemplates(templates)

        print("\033[46m  \033[0m  💻  Initializing tree matcher instance")
        tree_matcher = TreeMatcher(templates)

        print("\033[46m  \033[0m  ✅ Process complete!")
        print("\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m")

        return tree_matcher, python_regex[idx]

    def __call__(self, logs: list[str] | pl.DataFrame | str) -> TreeMatcher:
        return TreeMatcher(LogTemplates([]))
