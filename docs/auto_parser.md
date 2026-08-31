
# Auto Parser

Check all the templates in the dataset and select the most fitting one.

```python
class AutoParse:
    def __init__(self, num_use: int = 10) -> None:
        pass

    def __len__(self) -> int:
       """Buffer size"""

    def add(self, log: str) -> None:
        """Add log to buffer"""

    def reset(self) -> None:
        """Reset train buffer"""

    def generate(self) -> tuple[TreeMatcher, str]:
        """Generate Tree matcher and a regex pattern"""

    def __call__(self, logs: list[str] | pl.DataFrame | str) -> tuple[TreeMatcher, str]:
        """Generate Tree matcher and a regex pattern from df"""
```


## Usage

```python
from detectmateperformance.auto_parser import AutoParse


auto_parser = AutoParse(10)
tree_matcher, regex = auto_parser("tests/test_data/audit.log")
```
