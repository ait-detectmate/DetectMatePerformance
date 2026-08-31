
from detectmateperformance.match_tree import TreeMatcher
from detectmateperformance.types_ import LogTemplates

from detectmateperformance.lib.bind_class import auto_parser
import polars as pl


class AutoParse:
    def __init__(self, num_use: int = 10) -> None:
        self.path = "tests/test_data/"  # TODO: change so it works for pip
        self.reset()
        self.num_use = num_use

    def __len__(self) -> int:
        return len(self.buffer)

    def add(self, log: str) -> None:
        self.buffer.append(log)

    def reset(self) -> None:
        self.buffer: list[str] = []

    def generate(self) -> TreeMatcher:
        print("\033[46m >>>> Searching templates    \033[0m")
        print("\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m")

        print("\033[46m  \033[0m  🔎 Doing the template search")
        templates = LogTemplates(auto_parser(self.buffer[:self.num_use], self.path))

        print("\033[46m  \033[0m  💻  Initializing tree matcher instance")
        tree_matcher = TreeMatcher(templates)

        print("\033[46m  \033[0m  ✅ Process complete!")
        print("\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m")

        return tree_matcher

    def __call__(self, logs: list[str] | pl.DataFrame | str) -> TreeMatcher:
        return TreeMatcher(LogTemplates([]))
