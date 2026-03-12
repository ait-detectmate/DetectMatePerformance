from setuptools import setup, find_packages
import tomllib


def gather_dependencies(toml_path="pyproject.toml"):
    with open(toml_path, "rb") as f:
        data = tomllib.load(f)

    # Try Poetry first
    poetry_deps = data.get("tool", {}).get("poetry", {}).get("dependencies", {})
    if poetry_deps:
        return [f"{dep}{version}" for dep, version in poetry_deps.items()]

    # Fall back to PEP 621
    project_deps = data.get("project", {}).get("dependencies", [])
    return project_deps


setup(
    name="DetectMatePerformace",
    version="0.1",
    packages=find_packages(),
    description="A library for HPC operations in DetectMate",
    author="Andre Garcia Gomez",
    author_email="andre@example.com",
    install_requires=gather_dependencies(),
)
