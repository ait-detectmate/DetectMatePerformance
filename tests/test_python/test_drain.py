
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

    def test_cluster_logs_stream(self):
        logs = ["hello world, 12 ciao", "Ciao! ? bella bella"]

        assert drain.cluster_logs_stream(logs[0], depth=2) == {'4_hello_world': ['hello world <*> ciao']}
        assert drain.cluster_logs_stream(logs[1], depth=2) == {'3_Ciao_bella': ['Ciao bella bella']}

        assert drain.cluster_logs_stream(logs[0], depth=3) == {'4_hello_world_<*>': ['hello world <*> ciao']}
        assert drain.cluster_logs_stream(logs[1], depth=3) == {'3_Ciao_bella_bella': ['Ciao bella bella']}

    def test_cluster_logs_stream_special_cases(self):
        logs = ["hello world, 12 ciao", "Ciao! ? bella bella"]

        assert drain.cluster_logs_stream(logs[0], depth=1, max_child=2) == {
            '2_hello': ['hello world <*> ciao']
        }
        assert drain.cluster_logs_stream(logs[1], depth=1, max_child=2) == {'2_Ciao': ['Ciao bella bella']}

        expected = {'4_hello_world_<*>_ciao_None_None_None_None_None_None': ['hello world <*> ciao']}
        assert drain.cluster_logs_stream(logs[0], depth=10) == expected

        expected = {'3_Ciao_bella_bella_None_None_None_None_None_None_None': ['Ciao bella bella']}
        assert drain.cluster_logs_stream(logs[1], depth=10) == expected
