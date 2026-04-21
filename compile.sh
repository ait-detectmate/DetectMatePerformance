
# Update python dependencies
$HOME/.local/bin/uv sync

# Remove old build
rm -rf build
mkdir build

# Run cmake and build
cmake -B build
cd build
make
cd ..

# Move compiled C++ code
mkdir lib
cp build/bind_class* src/detectmateperformance/lib/
chmod 775 src/detectmateperformance/lib/*

# Install package
$HOME/.local/bin/uv pip uninstall detectmateperformance
$HOME/.local/bin/uv pip install --no-cache-dir .

# Run tests
$HOME/.local/bin/uv run pytest
