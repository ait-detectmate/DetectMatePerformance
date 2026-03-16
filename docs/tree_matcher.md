
# Tree Matcher

Match logs with pre-defined templates.

```python
class TreeMatcher:
    def __init__(self, templates: LogTemplates) -> None: pass

    @classmethod
    def from_file(cls, path: str) -> "TreeMatcher": pass

    def match_log(self, log: str, get_var: bool = False) -> ParsedLogs:
        """Match one preprocess log with the templates"""

    def match_batch(
        self, logs: list[str], get_var: bool = False, n_workers: int = 1
    ) -> ParsedLogs:
        """Match a batch preprocess logs to templates"""

    def __call__(
        self,
        logs: list[str] | pl.DataFrame,
        get_var: bool = False,
        n_workers: int = 1,
        batch: int = int(3e+6),
        regex: str = r"(?P<Content>.*)"
    ) -> pl.DataFrame:
        """Apply full pipeline from raw logs to full parsed logs"""
```


## Usage

```python
from detectmateperformance.match_tree import TreeMatcher
from detectmateperformance.types_ import LogTemplates

 logs = load_logs()
tree_matcher = TreeMatcher.from_file(path_temp)
results = tree_matcher(logs, get_var=True, n_workers=3)
```
