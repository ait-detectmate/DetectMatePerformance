
from detectmateperformance.autoparser import AutoParse, apache_regex

import polars as pl


def load_logs(path: str = "tests/test_data/audit.log") -> list[str]:
    with open(path) as f:
        return f.readlines()


regex_audit = r"type=(?P<Type>\w+) msg=audit\((?P<Time>[^:]+):(?P<Serial>\d+)\): (?P<Content>.*)"


class TestAutoParse:
    def test_basic(self):
        auto_parser = AutoParse(10)
        for log in load_logs():
            auto_parser.add(log)
        tree_matcher, regex = auto_parser.generate()

        assert regex_audit == regex

        parsed_logs = tree_matcher(
            logs="tests/test_data/audit.log",
            regex=regex
        )
        assert parsed_logs["Type"].to_list()[0] == "USER_ACCT"
        assert parsed_logs.filter(pl.col("Templates") == "template not found").shape[0] == 0

    def test_apache(self):
        auto_parser = AutoParse(10)
        for log in load_logs("tests/test_data/apache.log"):
            auto_parser.add(log)
        _, regex = auto_parser.generate()

        assert apache_regex == regex

    def test_specific(self):
        auto_parser = AutoParse(10)
        for log in load_logs("tests/test_data/apache.log"):
            auto_parser.add(log)
        _, regex = auto_parser.generate("Apache")

        assert apache_regex == regex

        auto_parser = AutoParse(10)
        for log in load_logs():
            auto_parser.add(log)
        _, regex = auto_parser.generate("BGL")

        assert regex_audit != regex

    def test_call(self):
        auto_parser = AutoParse(10)
        tree_matcher, regex = auto_parser("tests/test_data/audit.log")

        df = pl.DataFrame({"Content": load_logs()})
        auto_parser = AutoParse(10)
        tree_matcher2, regex2 = auto_parser(df)

        assert tree_matcher.get_templates(False) == tree_matcher2.get_templates(False)
        assert regex == regex2

        assert regex_audit == regex

        parsed_logs = tree_matcher(
            logs="tests/test_data/audit.log",
            regex=regex
        )
        assert parsed_logs["Type"].to_list()[0] == "USER_ACCT"
        assert parsed_logs.filter(pl.col("Templates") == "template not found").shape[0] == 0

    def test_call_fix_type(self):
        auto_parser = AutoParse(10)
        tree_matcher, regex = auto_parser("tests/test_data/audit.log")

        df = pl.DataFrame({"Content": load_logs()})
        auto_parser = AutoParse(10)
        tree_matcher2, regex2 = auto_parser(df, "BGL")

        assert tree_matcher.get_templates(False) != tree_matcher2.get_templates(False)
        assert regex != regex2

        assert regex_audit == regex
