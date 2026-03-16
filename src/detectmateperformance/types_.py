from detectmateperformance._core.bind_class import Templates, Parsed


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

    def __getitem__(self, idx: int) -> str | tuple[str, list[str]]:
        if self.with_vars:
            return self.inst.get_elem_with_var(idx)  # type: ignore
        return self.inst.get_elem(idx)  # type: ignore

    def __setitem__(self, idx: int, values: str | tuple[str, list[str]]) -> str:
        if self.with_vars:
            return self.inst.set_elem_with_var(idx, values[0], values[1])  # type: ignore
        return self.inst.set_elem(idx, values)  # type: ignore

    def get_all_templates(self) -> list[str]:
        return self.inst.get_all_elem()  # type: ignore

    def get_all_vars(self) -> list[list[str]] | None:
        if self.with_vars:
            return self.inst.get_all_var()  # type: ignore
        return None
