from DetectMatePerformance.src.message_class import Messages, Templates


def _load_file(path: str) -> list[str]:
    with open(path, "r") as f:
        return [line.strip() for line in f if line.strip()]


class _BasicMessage:
    def __init__(self, messages: list[str], inst: object):
        self.inst = inst(messages)

    def __len__(self) -> int:
        return self.inst.size()

    def shape(self) -> tuple[int, int]:
        return self.inst.shape()

    def get_next(self) -> list[str]:
        return self.inst.get_next_message()
    
    def __eq__(self, other: "_BasicMessage") -> bool:
        if self.shape() != other.shape():
            return False
        
        while (x := self.get_next()) != []:
            if x != other.get_next():
                return False
            
        return True


class LogTemplates(_BasicMessage):
    def __init__(self, templates: list[str]):
        super().__init__(
            map(lambda x: x.replace("<*>", "VAR"), templates),
            Templates
        )

    def __str__(self) -> str:
        return f"LogTemplates(shape={self.shape()})"
    
    @classmethod
    def from_file(cls, path: str) -> "LogTemplates":
        return cls(_load_file(path))


class Logs(_BasicMessage):
    def __init__(self, logs: list[str]):
        super().__init__(logs, Messages)

    def __str__(self) -> str:
        return f"Logs(shape={self.shape()})"

    @classmethod
    def from_file(cls, path: str) -> "Logs":
        return cls(_load_file(path))