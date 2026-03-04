import sys
sys.path.append("./build/")

from message_class import MatchTree

from DetectMatePerformance.src.types_ import LogTemplates, ParsedLogs, Parsed


class TreeMatcher:
    def __init__(self, templates: LogTemplates) -> None:
        self.templates = templates
        self.inst = MatchTree(self.templates.inst)

    def __wrap(self, inst: Parsed, get_var: bool) -> ParsedLogs:
        return ParsedLogs.from_cls(inst, with_vars=get_var)
    
    @classmethod
    def from_file(cls, path: str) -> "TreeMatcher":
        return cls(LogTemplates.from_file(path))

    def __len__(self) -> int:
        return len(self.templates)
        
    def match_log(self, log: str, get_var: bool = False) -> ParsedLogs:
        return self.__wrap(self.inst.match_string(log), get_var=get_var)
    
    def match_batch(
        self, logs: list[str], get_var: bool = False, n_workers: int = 1
    ) -> ParsedLogs:
        return self.__wrap(self.inst.match_batch(logs, n_workers), get_var=get_var)