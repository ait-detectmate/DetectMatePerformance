from DetectMatePerformance.src.types_ import  LogTemplates, ParsedLogs, Parsed


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


class TestCaseParsed:
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

    def test_add_elements_with_vars(self):
        parsed = ParsedLogs(LogTemplates(templates), 5, with_vars=True)
        assert parsed.shape() == (5, 0)
        parsed[0] = ("Hello VAR world VAR", [])
        parsed[3] = ("ciaoo bellaaa", ["a", "b", "c"])

        assert ("Hello VAR world VAR", []) ==  parsed[0]
        assert parsed[3] == ("template not found", ["a", "b", "c"])
        
    def test_add_elements(self):
        parsed = ParsedLogs(LogTemplates(templates), 5)
        assert parsed.shape() == (5, 0)

    def test_init_from_class(self) -> None:
        temp = LogTemplates.from_file(path_temp)
        parsed = Parsed(temp.inst, 2)

        assert len(ParsedLogs.from_cls(parsed)) == 2