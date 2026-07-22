from detectmateperformance.lib.bind_class import Templates
from detectmateperformance import drain

import polars as pl


class TestCommonMethods:
    def test_cluster_logs_df(self):
        df = pl.DataFrame({"Content": ["hello world, 12 ciao", "Ciao! ? bella bella"]})

        expected = {'3_Ciao_bella': ['Ciao bella bella'], '4_hello_world': ['hello world <*> ciao']}
        assert drain.cluster_logs_df(df, depth=2) == expected

        expected = {'3_Ciao_bella_bella': ['Ciao bella bella'], '4_hello_world_<*>': ['hello world <*> ciao']}
        assert drain.cluster_logs_df(df, depth=3) == expected

    def test_cluster_logs_df_special_cases(self):
        df = pl.DataFrame({"Content": ["hello world, 12 ciao", "Ciao bella bella"]})

        expected = {'2_Ciao': ['Ciao bella bella'], '2_hello': ['hello world <*> ciao']}
        assert drain.cluster_logs_df(df, depth=1, max_child=2) == expected

        expected = {
            '3_Ciao_bella_bella_None_None_None_None_None_None_None': ['Ciao bella bella'],
            '4_hello_world_<*>_ciao_None_None_None_None_None_None': ['hello world <*> ciao']
        }
        assert drain.cluster_logs_df(df, depth=10) == expected

    def test_bind_method(self):
        assert isinstance(drain.drain_generator(
            [["hello there", "ciaooo bellaa ciaoo"], ["general kenobi"]], 0.4
        ), Templates)


class TestDrain:
    def test_add_reset(self):
        drain_ = drain.Drain()
        assert len(drain_) == 0

        drain_.add("Hello world")
        drain_.add("Hello world")
        drain_.add("Hello planet")
        assert len(drain_) == 3

        drain_.reset()
        assert len(drain_) == 0

    def test_drain(self):
        drain_ = drain.Drain(depth=1, sim=0.3)
        assert len(drain_) == 0

        drain_.add("hello tobias my man")
        drain_.add("hello guillermo my man")
        drain_.add("hello julie my woman")

        tree_match = drain_.generate()
        template = tree_match.match_log("hello tobias my man").get_all_templates()[0]
        assert "hello VAR my VAR" == template
