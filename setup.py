from setuptools import setup, find_packages
import tomllib
import os


def gather_dependencies(toml_path: str = "pyproject.toml") -> list[str]:
    with open(toml_path, "rb") as f:
        data = tomllib.load(f)

    # Try Poetry first
    poetry_deps = data.get("tool", {}).get("poetry", {}).get("dependencies", {})
    if poetry_deps:
        return [f"{dep}{version}" for dep, version in poetry_deps.items()]

    # Fall back to PEP 621
    project_deps: list[str] = data.get("project", {}).get("dependencies", [])
    return project_deps


def add_lib_path(path: str = "lib/") -> list[str]:
    arch = os.uname().machine
    files = [
        f"{path}/{f}" for f in os.listdir(path)
        if os.path.isfile(os.path.join(path, f)) and arch in f
    ]
    return files


setup(
    name="detectmateperformace",
    version="0.1",
    package_dir={"": "src"},
    packages=find_packages(where="src"),
    description="A library for HPC operations in DetectMate",
    author="Andre Garcia Gomez",
    author_email="andre@example.com",
    install_requires=gather_dependencies(),
)
