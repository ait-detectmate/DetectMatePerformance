# DetectMatePerformance

## C++ conventions

- Use C++17. For new code, use `PascalCase` for types,
  `lowerCamelCase` for functions, and `snake_case` for filenames.
- Put native code in `src/detectmateperformance/_core/` and
  GoogleTest cases in `tests/test_c/`.
- Function arguments and tabs follow python pep8 standards
- Do not initialize variables inside loops, do it outside and then use the variable inside the loop
- When porting Python to C++, preserve behavior, default arguments,
  and existing test cases. Keep Python callers working through pybind11.
- Keep the diff focused on the requested change.
- The C++ should be fast and memory efficient. If you have suggestions let me know.

## Python

- Python tests use pytest in `tests/test_python/`.

## Build and test

- Set up Python dependencies with `uv sync`.
- Python code must follow pep8 and pass mypy rules
- With `.venv` active, rebuild the Python extension with `sh compile_manual.sh`.
- Run Python and C++ tests with `uv run pytest -s`.
