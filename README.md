
# DetectMate Performance

Library to run high performance operations in DetectMate. It combines C++ functionality with [`Polars`](https://pola.rs/) DataFrames.


## Installation

[`CMake`](https://cmake.org/) allows to build the C++ code:

```bash
sudo apt-get install cmake
```

Intall python dependencies:

```bash
pip install uv
uv sync
```

Compile the code, make sure that all unit tests are green:

```bash
sh compile.sh
```

To run the C++ and Python unit tests:

```bash
pytest
```

### To do a local instalation in a different venv

```bash
uv pip install --no-cache-dir <directory_detectmateperformance>
```

## Code example


```python
import detectmateperformance as matep


dataset = {
    "name": "AIT - Audit",
    "path_temp": "data/audit_templates.txt",
    "path_logs": "data/audit.log",
    "regex": r"type=(?P<Type>\w+) msg=audit\((?P<Time>[^:]+):(?P<Serial>\d+)\): (?P<Content>.*)"
}


def load_file(path_logs) -> list[str]:
    with open(path_logs, "r") as f:
        return f.readlines()


results = matep.metrics.evaluate(
    logs=load_file(dataset["path_logs"]),
    ground_templates=dataset["path_temp"],
    templates=dataset["path_temp"],
    regex=dataset["regex"]
)

print(results)  # {'PA': 1.0, 'NED': 1.0, 'TS': 1.0, 'GA': 1.0, 'FR': 1.0}

```
