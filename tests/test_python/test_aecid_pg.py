
from detectmateperformance import aecid_pg


class TestAECID_PG:
    def test_add_reset(self):
        aecid_pg_ = aecid_pg.AECID_PG()
        assert len(aecid_pg_) == 0

        aecid_pg_.add("Hello world")
        aecid_pg_.add("Hello world")
        aecid_pg_.add("Hello planet")
        assert len(aecid_pg_) == 3

        aecid_pg_.reset()
        assert len(aecid_pg_) == 0

    def test_aecid_pg(self):
        aecid_pg_ = aecid_pg.AECID_PG(theta_1=0.5)
        assert len(aecid_pg_) == 0

        aecid_pg_.add("hello tobias my man")
        aecid_pg_.add("hello guillermo my man")
        aecid_pg_.add("hello julie my woman")

        tree_match = aecid_pg_.generate()
        template = tree_match.match_log("hello tobias my boy").get_all_templates()[0]
        assert template == "hello VAR my VAR"

        template = tree_match.match_log("hello alex my man").get_all_templates()[0]
        assert template == "hello VAR my man"

    def test_num_cases(self):
        aecid_pg_ = aecid_pg.AECID_PG(theta_1=0.8)
        aecid_pg_.add("Hello 1ad231b1")
        aecid_pg_.add("Hello 2a2sss1")

        tree_match = aecid_pg_.generate()
        template = tree_match.match_log("Hello world").get_all_templates()[0]
        assert "Hello VAR" == template

    def test_aecid_pg_pipeline(self):
        aecid_pg_ = aecid_pg.AECID_PG()
        regex = r"type=(?P<Type>\w+) msg=audit\((?P<Time>[^:]+):(?P<Serial>\d+)\): (?P<Content>.*)"

        templates = aecid_pg_(logs="tests/test_data/audit.log", regex=regex).get_templates(True)
        assert len(templates) == 10
