from detectmateperformance import match_tree
from detectmateperformance import autoparser
from detectmateperformance import aecid_pg
from detectmateperformance import brain
from detectmateperformance import drain

import pytest


regex = r"type=(?P<Type>\w+) msg=audit\((?P<Time>[^:]+):(?P<Serial>\d+)\): (?P<Content>.*)"
path_temp = "tests/test_data/audit_templates.txt"
logs_path = "tests/test_data/audit.log"
max_time = 6


class TestPerformance:
    @pytest.mark.ignored
    @pytest.mark.timeout(max_time)
    def test_drain_pipeline(self):
        for _ in range(10):
            drain_ = drain.Drain(sim=0.3, max_child=1000, depth=2)
            drain_(logs=logs_path, regex=regex)

    @pytest.mark.ignored
    @pytest.mark.timeout(max_time)
    def test_brain_pipeline(self):
        for _ in range(10):
            drain_ = brain.Brain(weight=0.5)
            drain_(logs=logs_path, regex=regex)

    @pytest.mark.ignored
    @pytest.mark.timeout(max_time)
    def test_aecid_pg_pipeline(self):
        for _ in range(10):
            aecid_pg_ = aecid_pg.AECID_PG(
                theta_1=0.1, theta_2=0.6, theta_3=0.6
            )
            aecid_pg_(logs=logs_path, regex=regex)

    @pytest.mark.ignored
    @pytest.mark.timeout(max_time)
    def test_autoparser_pipeline(self):
        for _ in range(10):
            auto_parser = autoparser.AutoParse(num_use=10)
            auto_parser(logs=logs_path)

    @pytest.mark.ignored
    @pytest.mark.timeout(max_time)
    def test_matchtree_pipeline(self):
        for _ in range(10):
            tree_macther = match_tree.TreeMatcher.from_file(path_temp)
            tree_macther(logs=logs_path, get_var=True, n_workers=5)
