import pytest


def pytest_addoption(parser):
    # These flags opt into test groups that are skipped by default.
    parser.addoption(
        "--run-ignored",
        action="store_true",
        default=False
    )


def pytest_configure(config):
    config.addinivalue_line("markers", "ignored: skipped unless --run-ignored is passed")


def pytest_collection_modifyitems(config, items):
    """Skip ignored test groups unless their corresponding flag is passed."""
    for item in items:
        if "ignored" in item.keywords and not config.getoption("--run-ignored"):
            item.add_marker(pytest.mark.skip(reason="Skipped: requires --run-ignored flag"))
