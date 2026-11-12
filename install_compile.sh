#!/usr/bin/env bash
set -euo pipefail

# Usage: bash install_compile.sh [12 | 3.12 | 3.12.8]
# Run from the project directory containing compile.sh.
# With no argument, uv selects a managed Python 3 interpreter.

if (( $# > 1 )); then
    echo "Usage: $0 [12 | 3.12 | 3.12.8]" >&2
    exit 2
fi

PYTHON_VERSION="${1:-3}"
if [[ "$PYTHON_VERSION" =~ ^[0-9]+$ && "$PYTHON_VERSION" != 3 ]]; then
    PYTHON_VERSION="3.$PYTHON_VERSION"
fi
if [[ ! "$PYTHON_VERSION" =~ ^3(\.[0-9]+){0,2}$ ]]; then
    echo "Invalid Python version: $PYTHON_VERSION" >&2
    exit 2
fi
if [[ ! -f compile.sh ]]; then
    echo "Run this script from the project directory containing compile.sh." >&2
    exit 1
fi

apt-get update
apt-get install -y --no-install-recommends ca-certificates curl git cmake build-essential

# Make uv available immediately, without requiring a new shell.
if ! command -v uv >/dev/null 2>&1; then
    UV_BIN_DIR="${HOME}/.local/bin"
    curl -LsSf https://astral.sh/uv/install.sh | env UV_INSTALL_DIR="$UV_BIN_DIR" UV_NO_MODIFY_PATH=1 sh
    export PATH="$UV_BIN_DIR:$PATH"
fi

# Fail if the requested version is unavailable; do not fall back to system Python.
uv python install "$PYTHON_VERSION"
if [[ -e .venv || -L .venv ]]; then
    echo "Removing existing .venv"
    rm -rf -- .venv
fi

# Seed pip for compile.sh scripts that use pip or python -m pip.
uv venv --managed-python --python "$PYTHON_VERSION" --seed .venv
source .venv/bin/activate
python --version

sh compile.sh
