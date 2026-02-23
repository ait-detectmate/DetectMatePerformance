from DetectMatePerformance.src.message import Logs, LogTemplates


msg = [
    "Hello, world!", "Goodbye, world! goodbye"
]


class TestCaseMessage:
    def test_len(self):
        logs = Logs(msg)
        assert len(logs) == 2

    def test_shape(self):
        logs = Logs(msg)
        assert logs.shape() == (2, 3)

    def test_messages(self):
        logs = Logs(msg)

        assert logs.get_next() == ["Hello", "world"]
        assert logs.get_next() == ["Goodbye", "world", "goodbye"]
        assert logs.get_next() == []


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