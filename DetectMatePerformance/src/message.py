import sys
sys.path.append("./build/")

from message_class import Templates


def _load_file(path: str) -> list[str]:
    with open(path, "r") as f:
        return [line.strip() for line in f if line.strip()]


class LogTemplates:
    def __init__(self, templates: list[str]):
        self.inst = Templates(list(
            map(lambda x: x.replace("<*>", "VAR"), templates)
        ))

    def __len__(self) -> int:
        return self.inst.size()

    def shape(self) -> tuple[int, int]:
        return self.inst.shape()

    def get_next(self) -> list[str]:
        return self.inst.get_next_template()
    
    def __eq__(self, other: "LogTemplates") -> bool:
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


     


