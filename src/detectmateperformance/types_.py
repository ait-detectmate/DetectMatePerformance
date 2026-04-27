from detectmateperformance.lib.bind_class import Templates, Parsed, ParsedElement

from typing import Any


def _load_file(path: str) -> list[str]:
    with open(path, "r") as f:
        return [line.strip() for line in f if line.strip()]


class LogTemplates:
    def __init__(self, templates: list[str]):
        self.inst = Templates(list(
            map(lambda x: x.replace("<*>", "VAR"), templates)
        ))

    def __len__(self) -> int:
        return self.inst.size()  # type: ignore

    def shape(self) -> tuple[int, int]:
        return self.inst.shape()  # type: ignore

    def get_next(self) -> list[str]:
        return self.inst.get_next_template()  # type: ignore

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, LogTemplates):
            return False

        if self.shape() != other.shape():
            return False

        while (x := self.get_next()) != []:
            if x != other.get_next():
                return False

        return True

    def __str__(self) -> str:
        return f"LogTemplates(shape={self.shape()})"

    @classmethod
    def from_file(cls, path: str) -> "LogTemplates":
        return cls(_load_file(path))


class ParsedLogElement:
    def __init__(self, parsed_ele: ParsedElement) -> None:
        self.event_id: int = parsed_ele.event_id
        self.log_template: str = parsed_ele.log_template
        self.variables: list[str] = parsed_ele.variables

    def to_dict(self) -> dict[str, Any]:
        return {
            "EventID": self.event_id,
            "Template": self.log_template,
            "ParamList": self.variables,
        }


class ParsedLogs:
    def __init__(
        self, templates: LogTemplates, n: int, with_vars: bool = False
    ) -> None:
        self.inst = Parsed(templates.inst, n)
        self.with_vars = with_vars

    @classmethod
    def from_cls(cls, inst: Parsed, with_vars: bool = False) -> "ParsedLogs":
        cls_ints = cls(LogTemplates([]), n=1, with_vars=with_vars)
        cls_ints.inst = inst
        return cls_ints

    def __len__(self) -> int:
        return self.inst.size()  # type: ignore

    def shape(self) -> tuple[int, int]:
        return self.inst.shape()  # type: ignore

    def __str__(self) -> str:
        return f"ParsedLogs(shape={self.shape()}, vars={self.with_vars})"

    def __getitem__(self, idx: int) -> dict[str, Any]:
        if self.with_vars:
            result = ParsedLogElement(self.inst.get_elem_with_var(idx))
        else:
            result = ParsedLogElement(self.inst.get_elem(idx))
        return result.to_dict()

    def __setitem__(self, idx: int, values: str | tuple[str, str]) -> str:
        if self.with_vars:
            return self.inst.set_elem_with_var(idx, values[0], values[1])  # type: ignore
        return self.inst.set_elem(idx, values)  # type: ignore

    def get_all_events_ids(self) -> list[int]:
        return self.inst.get_all_ids()  # type: ignore

    def get_all_templates(self) -> list[str]:
        return self.inst.get_all_elem()  # type: ignore

    def get_all_vars(self) -> list[list[str]] | None:
        if self.with_vars:
            return self.inst.get_all_var()  # type: ignore
        return None
