# AECID-PG

`AECID_PG` learns log templates by counting token sequences in a tree and
merging selected branches into the wildcard `<*>`. The parser follows the
tree-based method described by Wurzenberger et al. (2019). [AECID-PG paper](https://ieeexplore.ieee.org/document/8717887)

```python
class AECID_PG:
    def __init__(
        self,
        theta_1: float = 0.2,
        theta_2: float = 0.8,
        theta_3: float = 0.5,
    ) -> None:
        """Set the tree pruning thresholds."""

    def __len__(self) -> int:
        """Return the number of buffered logs."""

    def add(self, log: str) -> None:
        """Add a log line to the buffer."""

    def reset(self) -> None:
        """Clear the log buffer."""

    def generate(self) -> TreeMatcher:
        """Generate a matcher from the buffered logs."""

    def generate_from_df(self, df: pl.DataFrame) -> TreeMatcher:
        """Generate a matcher from the DataFrame's string Content column."""

    def __call__(
        self,
        logs: list[str] | pl.DataFrame | str,
        regex: str = r"(?P<Content>.*)",
    ) -> TreeMatcher:
        """Generate a matcher directly from log lines, paths, or a DataFrame."""
```

At each node, `theta_1` is the minimum frequency for a branch to count as
common. If no branch meets that threshold, the parser merges all branches. If
the common branches have a combined frequency of at least `theta_3`, the
parser merges the less frequent branches. Otherwise, it merges branches below
`theta_2`.

## Usage

```python
from detectmateperformance.aecid_pg import AECID_PG

parser = AECID_PG(theta_1=0.5)
parser.add("user alice logged in")
parser.add("user bob logged in")
parser.add("user carol logged in")

matcher = parser.generate()
print(matcher.get_templates(do_print=False))
# ['user <*> logged in']
```

Use the returned matcher to match new log lines. [TreeMatcher API](tree_matcher.md)
