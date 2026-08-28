
from detectmateperformance.pipeline_op import preprocessing


class TestCasePreprocessingPipeline:
    def test_regex_statement(self):
        logs = ["A B C 123", "D E F 345"]
        df = preprocessing(
            logs, regex=r"(?P<Label>\S+) (?P<Timestamp>\S+) (?P<Date>\S+) (?P<Content>.*)"
        )

        assert df["Label"].to_list() == ["A", "D"]
        assert df["Timestamp"].to_list() == ["B", "E"]
        assert df["Date"].to_list() == ["C", "F"]
        assert df["Content"].to_list() == ["123", "345"]

    def test_read_log_file(self):
        df = preprocessing("tests/test_data/audit.log")
        assert len(df) == 2316

    def test_read_multiple_log_files(self):
        df = preprocessing(
            ["tests/test_data/sys.log1", "tests/test_data/sys.log2"]
        )
        print(df)
        assert len(df) == 4
