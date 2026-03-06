from setuptools import setup, find_packages

setup(
    name="DetectMatePerformace",
    version="0.1",
    packages=find_packages(),
    description="A library for HPC operations in DetectMate",
    author="Andre Garcia Gomez",
    author_email="andre@example.com",
    install_requires=[
        "polars>=1.38.1",
        "pybind11>=3.0.2",
        "pytest",
        "pytest-cpp",
        "tqdm",
    ],
)
