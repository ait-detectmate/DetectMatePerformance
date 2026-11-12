#!/bin/sh
# Stop immediately if configuration, compilation, copying, or installation fails.
set -eu

cd "$(dirname "$0")"
: "${VIRTUAL_ENV:?Run install_compile.sh first, or activate the intended Python venv.}"
PYTHON="$VIRTUAL_ENV/bin/python"
EXT_SUFFIX=$("$PYTHON" -c 'import sysconfig; print(sysconfig.get_config_var("EXT_SUFFIX"))')
MODULE="bind_class$EXT_SUFFIX"
DEST="src/detectmateperformance/lib"

"$PYTHON" --version
c++ --version

# Delete the old artifact for this Python/architecture before starting.
# Preserve extensions built for other Python versions or architectures.
mkdir -p "$DEST"
rm -f -- "$DEST/$MODULE"
rm -rf -- build

cmake -S . -B build \
    -DPYBIND11_FINDPYTHON=ON \
    -DPython_EXECUTABLE="$PYTHON"
cmake --build build

# Require the exact extension for the selected interpreter, not a wildcard match.
test -f "build/$MODULE"
"$PYTHON" - "build/$MODULE" <<'PYTHON_CHECK'
import importlib.util
import pathlib
import sys
path = pathlib.Path(sys.argv[1]).resolve()
spec = importlib.util.spec_from_file_location("bind_class", path)
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
print(f"Successfully loaded freshly compiled extension: {path}")
PYTHON_CHECK

cp "build/$MODULE" "$DEST/$MODULE"
chmod 775 "$DEST/$MODULE"
echo "Copied freshly compiled extension to $DEST/$MODULE"

# Install using the same interpreter that CMake used.
uv pip install --python "$PYTHON" --no-cache-dir .
