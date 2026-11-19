from detectmateperformance.match_tree import TreeMatcher, CleanString
from detectmateperformance.types_ import LogTemplates

import polars as pl

path_temp = "tests/test_data/audit_templates.txt"
path_logs = "tests/test_data/audit.log"


def load_logs() -> list[str]:
    logs = []
    with open(path_logs, "r") as f:
        for log in f.readlines():
            logs.append("):".join(log.split("):")[1:]))
    return logs


class TestCaseTreeMatcher:
    def test_clean_strings(self):
        clean_string = CleanString()
        values = [
            "Hello, world",
            "Hello. world",
            "Hello. world 129.131.12.12",
            "Receiving block blk_-1608999687919862906",
            "Receiving block /home/linux/home/windows",
            "pid=123",
        ]

        expected = [
            "Hello world",
            "Hello world",
            "Hello world 129.131.12.12",
            "Receiving block blk_-1608999687919862906",
            "Receiving block /home/linux/home/windows",
            "pid 123",
        ]

        for value, exp in zip(values, expected):
            assert clean_string(value) == exp

    def test_init(self):
        assert len(TreeMatcher.from_file(path_temp)) == 9

        templates = LogTemplates.from_file(path_temp)
        assert len(TreeMatcher(templates)) == 9

    def test_match_log(self):
        tree_matcher = TreeMatcher(LogTemplates(["Hello`=<*> kenobi"]))
        log = "Hello`=there general kenobi"

        assert tree_matcher.match_log(log, False)[0]["Template"] == "Hello <*> kenobi"

    def test_match_log_with_var(self):
        tree_matcher = TreeMatcher(LogTemplates(["Hello`=<*> kenobi"]))
        log = "Hello`=there general kenobi"

        result = tree_matcher.match_log(log, True)
        assert result[0]["Template"] == "Hello <*> kenobi"
        assert result[0]["ParamList"] == ["there", "general"]

    def test_match_batch(self):
        tree_matcher = TreeMatcher(LogTemplates(["Hello`=<*> kenobi"]))
        logs = ["Hello`=there general kenobi", "roger roger"]
        results = tree_matcher.match_batch(logs, False)

        assert results[0]["Template"] == "Hello <*> kenobi"
        assert results[1]["Template"] == "template not found"

    def test_match_batch_with_var(self):
        tree_matcher = TreeMatcher(LogTemplates(["Hello`=<*> kenobi"]))
        logs = ["Hello`=there general kenobi", "roger roger"]
        results = tree_matcher.match_batch(logs, True)

        assert results[0]["Template"] == "Hello <*> kenobi"
        assert results[1]["Template"] == "template not found"
        assert results[0]["ParamList"] == ["there", "general"]
        assert results[1]["ParamList"] == []

    def test_big_batch(self):
        logs = load_logs()
        tree_matcher = TreeMatcher.from_file(path_temp)
        results = tree_matcher.match_batch(logs, False, n_workers=3)

        for i in range(len(results)):
            assert "template not found" != results[i]["Template"]

    def test_big_batch_with_var(self):
        logs = load_logs()
        tree_matcher = TreeMatcher.from_file(path_temp)
        results = tree_matcher.match_batch(logs, True, n_workers=3)

        for i in range(len(results)):
            assert "template not found" != results[i]["Template"]

    def test_call(self):
        logs = load_logs()
        tree_matcher = TreeMatcher.from_file(path_temp)
        results = tree_matcher(path_logs, True, n_workers=3)
        print(results)

        assert isinstance(results, pl.DataFrame)
        assert len(logs) == len(results)


class TestCaseHardones:
    def test_case_1(self):
        temp = LogTemplates([
            "<*> floating point alignment exceptions",
            "floating pt ex mode <*> enable......<*>",
            "floating point instr. enabled.....<*>",
        ])
        tree_macther = TreeMatcher(temp)
        logs = [
            "8 floating point alignment exceptions",
            "8 9 hi floating point alignment exceptions"
        ]

        results = tree_macther(logs, get_var=False)

        assert len(results["Templates"].unique()) == 1
        assert results["Templates"].unique()[0] == "<*> floating point alignment exceptions"

    def test_case_2(self):
        temp = LogTemplates([
            "Exception in receiveBlock for block <*> java.io.IOException: Broken pipe",
            "VAR:Exception writing block <*> to mirror <*>",
        ])
        log = "10.250.14.196:50010:Exception writing block blk_-6994808880344424033 to mirror 10.251.107.50:50010"  # noqa: E501

        tree_macther = TreeMatcher(temp)
        results = tree_macther.match_log(log, get_var=False).get_all_templates()
        assert results[0] == "VAR Exception writing block VAR to mirror VAR"

    def test_case_3(self):
        temp = LogTemplates([
            "rts: kernel terminated for reason VAR",
            "rts: kernel terminated for reason VAR: bad message header: invalid cpu, type=VAR, cpu=VAR, index=VAR, total=VAR",  # noqa: E501
        ])
        log = "rts: kernel terminated for reason VAR"

        tree_macther = TreeMatcher(temp)
        results = tree_macther.match_log(log, get_var=False).get_all_templates()
        assert results[0] == "rts kernel terminated for reason VAR"

    def test_case_4(self):
        temp = LogTemplates([
            "data TLB error interrupt data address space................0",
            "data TLB error interrupt",
        ])
        log = "data TLB error interrupt"

        tree_macther = TreeMatcher(temp)
        results = tree_macther.match_log(log, get_var=False).get_all_templates()
        assert results[0] == "data TLB error interrupt"

    def test_case_5(self):
        temp = LogTemplates([
            "VAR Killing process VAR (kill) with signal SIGKILL.",
        ])
        log = "user@1002.service: Killing process 29 (kill) with signal SIGKILL."

        tree_macther = TreeMatcher(temp)
        results = tree_macther.match_log(log, get_var=False).get_all_templates()
        assert results[0] == "VAR Killing process VAR kill with signal SIGKILL"
