from DetectMatePerformance.src.message import  LogTemplates, ParsedLogs


path_temp = "DetectMatePerformance/tests/test_data/audit_templates.txt"

msg = ["Hello, world!", "Goodbye, world! goodbye"]

templates = [
    "Hello <*>, world=<*>:<*> <*>", "Goodbye <*>, world=<*>:<*> <*>"
]


class TestCaseTemplates:
    def test_len(self):
        log_temp = LogTemplates(templates)
        assert len(log_temp) == 2

    def test_shape(self):
        log_temp = LogTemplates(templates)
        assert log_temp.shape() == (2, 4)

    def test_templates(self):
        log_temp = LogTemplates(templates)

        assert log_temp.get_next() == ["Hello", "VAR", "world", "VAR"]
        assert log_temp.get_next() == ["Goodbye", "VAR", "world", "VAR"]
        assert log_temp.get_next() == []

    def test_from_file(self):
        log_temp = LogTemplates.from_file(path_temp)
        assert log_temp.shape() == (9, 50)

    def test_eq(self):
        assert LogTemplates(msg) == LogTemplates(msg)
        assert LogTemplates(msg) != LogTemplates(["hi"])


class TestCaseParsed():
    def test_len(self):
        parsed = ParsedLogs(LogTemplates.from_file(path_temp), 5)
        assert len(parsed) == 5

    def test_add_elements(self):
        parsed = ParsedLogs(LogTemplates(templates), 5)
        assert parsed.shape() == (5, 0)

        parsed[0] = "Hello VAR world VAR"
        parsed[3] = "ciaoo bellaaa"

        assert parsed[0] == "Hello VAR world VAR" 
        assert parsed[3] == "template not found"
