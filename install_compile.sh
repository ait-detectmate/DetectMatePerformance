
#!/usr/bin/env bash
set -euo pipefail

# Simple installer: pass a suffix (e.g. 12 or 13) to install python3.X
# Usage: ./install_compile.sh [12]

SUFFIX="${1:-}"

apt-get update
apt-get install -y --no-install-recommends curl cmake build-essential

if [ -n "$SUFFIX" ]; then
	PYPKG="python3.$SUFFIX"
	PYDEV="python3.$SUFFIX-dev"
	echo "Installing ${PYPKG} and ${PYDEV}"
	apt-get install -y --no-install-recommends ${PYPKG} ${PYDEV} || (
		echo "Failed to install ${PYPKG}. Falling back to system python3." && apt-get install -y --no-install-recommends python3 python3-dev
	)
else
	echo "No suffix provided. Installing system python3 and headers."
	apt-get install -y --no-install-recommends python3 python3-dev
fi

curl -LsSf https://astral.sh/uv/install.sh | sh

sh compile.sh
