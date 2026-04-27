
# Types

DetectMatePerformance use standard type classes in all its methods.

## LogTemplates

Log templates is a class that manage all the log templates in the code.

```python
class LogTemplates:
    def __init__(self, templates: list[str]): pass

    def __len__(self) -> int: pass

    def shape(self) -> tuple[int, int]: pass

    def get_next(self) -> list[str]: pass

    def __eq__(self, other: object) -> bool: pass

    def __str__(self) -> str: pass

    @classmethod
    def from_file(cls, path: str) -> "LogTemplates": pass
```

Usage example:

```python
from detectmateperformance.types_ import LogTemplates

log_temp = LogTemplates.from_file(path_temp)
```

## ParsedLogs

Class that contains all the parsed logs.

```python
class ParsedLogs:
    def __init__(
        self, templates: LogTemplates, n: int, with_vars: bool = False
    ) -> None:
        pass

    @classmethod
    def from_cls(
        cls, inst: Parsed, with_vars: bool = False
    ) -> "ParsedLogs":
        pass

    def __len__(self) -> int: pass

    def shape(self) -> tuple[int, int]: pass

    def __str__(self) -> str: pass

    def __getitem__(self, idx: int) -> str | tuple[str, str]: pass

    def __setitem__(self, idx: int, values: str | tuple[str, str]) -> str:
        pass

    def get_all_events_ids(self) -> list[int]: pass

    def get_all_templates(self) -> list[str]: pass

    def get_all_vars(self) -> list[str] | None: pass
```

Usage example without variables:

```python
from detectmateperformance.types_ import ParsedLogs

parsed = ParsedLogs(LogTemplates(templates), 5)
assert parsed.shape() == (5, 0)

parsed[0] = "Hello VAR world VAR"
parsed[3] = "ciaoo bellaaa"

assert parsed[0] == "Hello VAR world VAR"
assert parsed[3] == "template not found"
```

Usage example with variables:

```python
from detectmateperformance.types_ import ParsedLogs

parsed = ParsedLogs(LogTemplates(templates), 5, with_vars=True)
assert parsed.shape() == (5, 0)

parsed[0] = ("Hello VAR world VAR", "")
parsed[3] = ("ciaoo bellaaa", "a b c")

assert parsed[0] == ("Hello VAR world VAR", "")
assert parsed[3] == ("template not found", "a b c")
```
