# DetectMate Performance

`DetectMate Performance` is a library that performs high-performance operations in DetectMate. While the `DetectMate Library` runs everything in a streaming manner, `DetectMate Performance` allows you to process data in batches as well.

It combines C++ functionality with [`Polars`](https://pola.rs/) DataFrames.


## Installation

[`CMake`](https://cmake.org/) allows to build the C++ code:

```bash
sudo apt-get install cmake
```

Intall python dependencies:

```bash
pip install uv
uv sync
prek install
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
