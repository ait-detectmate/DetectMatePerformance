from DetectMatePerformance.src.message_class import Messages, Templates


class LogTemplates:
    def __init__(self, templates: list[str]):
        self.templates = Templates(
            map(lambda x: x.replace("<*>", "VAR"), templates)
        )

    def __str__(self) -> str:
        return f"LogTemplates(shape={self.shape()})"

    def __len__(self) -> int:
        return self.templates.size()

    def shape(self) -> tuple[int, int]:
        return self.templates.shape()
    
    def get_next(self) -> list[str]:
        return self.templates.get_next_message()


class Logs:
    def __init__(self, logs: list[str]):
        self.messages = Messages(logs)

    def __str__(self) -> str:
        return f"Logs(shape={self.shape()})"

    def __len__(self) -> int:
        return self.messages.size()

    def shape(self) -> tuple[int, int]:
        return self.messages.shape()
    
    def get_next(self) -> list[str]:
        return self.messages.get_next_message()