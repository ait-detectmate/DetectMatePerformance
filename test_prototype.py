# mypy: ignore-errors
import prototype as op


class TestTreeOp:
    def test_add_log(self) -> None:
        tree = op.init_tree()
        log = "hello there"

        op.add_log(tree, log.split(" "))

        expected = {
            "hello": [1, {"there": [1, {}]}]
        }
        assert tree == expected

    def test_add_logs(self) -> None:
        tree = op.init_tree()
        expected = {
            "hello": [2, {"there": [1, {}], "ciao": [1, {}]}]
        }
        logs = [lo.split(" ") for lo in ["hello there", "hello ciao"]]

        op.add_logs(tree, logs)
        assert tree == expected

    def test_merge_left(self) -> None:
        tree_1 = {
            "hello": [2, {"there": [1, {}], "ciao": [1, {}]}]
        }
        tree_2 = {
            "ciao": [1, {}],
            "hello": [1, {
                "ciao": [2, {}],
                "a": [1, {}]
            }]
        }

        expected = {
            "ciao": [1, {}],
            "hello": [3, {
                "there": [1, {}],
                "ciao": [3, {}],
                "a": [1, {}]
            }]
        }

        op.merge_left(tree_1, tree_2)
        assert tree_1 == expected

    def test_prune_rule_1(self) -> None:
        tree = {
            "ciao": [4, {}],
            "bye": [1, {"b": [2, {}]}],
            "hello": [1, {
                "ciao": [20, {}],
                "a": [1, {}]
            }]
        }
        op.prune(tree, theta=0.5)

        expected = {
            "ciao": [4, {}],
            "<*>": [2, {
                "ciao": [20, {}],
                "<*>": [3, {}]
            }]
        }
        assert tree == expected
