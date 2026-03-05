import sys
sys.path.append("./build/")

from message_class import MatchTree

from DetectMatePerformance.src.types_ import LogTemplates, ParsedLogs, Parsed

import polars as pl


class TreeMatcher:
    def __init__(self, templates: LogTemplates) -> None:
        self.templates = templates
        self.inst = MatchTree(self.templates.inst)

    def __wrap(self, inst: Parsed, get_var: bool) -> ParsedLogs:
        return ParsedLogs.from_cls(inst, with_vars=get_var)
    
    @classmethod
    def from_file(cls, path: str) -> "TreeMatcher":
        return cls(LogTemplates.from_file(path))

    def __str__(self)-> str:
        return f"TreeMatcher(n_templates={len(self)})"

    def __len__(self) -> int:
        return len(self.templates)
        
    def match_log(self, log: str, get_var: bool = False) -> ParsedLogs:
        if get_var:
            result = self.inst.match_string_with_var(log)
        else:
            result = self.inst.match_string(log)
        return self.__wrap(result, get_var=get_var)
    
    def match_batch(
        self, logs: list[str], get_var: bool = False, n_workers: int = 1
    ) -> ParsedLogs:
        if get_var:
            result = self.inst.match_batch_with_var(logs, n_workers)
        else:
            result = self.inst.match_batch(logs, n_workers)
        return self.__wrap(result, get_var=get_var)
    
    def __call__(
        self, logs: list[str] | str, get_var: bool = False, n_workers: int = 1
    ) -> pl.DataFrame:
        pass