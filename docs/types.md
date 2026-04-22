
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
