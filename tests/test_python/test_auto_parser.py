
from detectmateperformance.auto_parser import AutoParse


def load_logs():
    with open("tests/test_data/audit.log") as f:
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
