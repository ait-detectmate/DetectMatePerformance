
# Drain

This parsed implementation was based from this [publication](https://ieeexplore.ieee.org/document/8029742).

```python
class Drain:
    def __init__(
        self, depth: int = 2, max_child: int = 10, sim: float = 0.5
    ) -> None:
        pass

    def __len__(self) -> int:
       """Buffer size"""

    def add(self, log: str) -> None:
        """Add log to buffer"""

    def reset(self) -> None:
        """Reset train buffer"""

    def generate(self) -> TreeMatcher:
        """Generate Tree matcher"""
```


## Usage

```python
from detectmateperformanc.drain import Drain


rain_ = drain.Drain(depth=1, sim=0.3)

drain_.add("hello tobias my man")
drain_.add("hello guillermo my man")
drain_.add("hello julie my woman")

tree_match = drain_.generate()
```
