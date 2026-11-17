# mypy: ignore-errors
import detectmateperformance.temp_c as op


class TestTreeOp:
    def test_add_log(self) -> None:
        tree = op.init_tree()
        log = "hello there"
        op.add_log(tree, log.split(" "))
        expected = op.Tree(
            children={
                "hello": op.Tree(
                    children={
                        "there": op.Tree(children={}, is_end_state=True, count={})
                    },
                    count={"there": 1},
                )
            },
            count={"hello": 1},
        )
        assert tree == expected

    def test_add_logs(self) -> None:
        tree = op.init_tree()
        expected = op.Tree(
            children={
                "hello": op.Tree(
                    children={
                        "there": op.Tree(children={}, is_end_state=True, count={}),
                        "ciao": op.Tree(children={}, is_end_state=True, count={}),
                    },
                    count={"there": 1, "ciao": 1},
                )
            },
            count={"hello": 2},
        )
        logs = ["hello there", "hello ciao"]
        op.add_logs(tree, logs)
        assert tree == expected

    def test_merge_left(self) -> None:
        tree_1 = op.Tree(
            children={
                "hello": op.Tree(
                    children={
                        "there": op.Tree(children={}, is_end_state=True, count={}),
                        "ciao": op.Tree(children={}, is_end_state=True, count={}),
                    },
                    count={"there": 1, "ciao": 1},
                )
            },
            count={"hello": 2},
        )
        tree_2 = op.Tree(
            children={
                "ciao": op.Tree(children={}, is_end_state=True, count={}),
                "hello": op.Tree(
                    children={
                        "ciao": op.Tree(children={}, is_end_state=True, count={}),
                        "a": op.Tree(children={}, is_end_state=True, count={}),
                    },
                    count={"ciao": 2, "a": 1},
                ),
            },
            count={"ciao": 1, "hello": 1},
        )
        expected = op.Tree(
            children={
                "ciao": op.Tree(children={}, is_end_state=True, count={}),
                "hello": op.Tree(
                    children={
                        "there": op.Tree(children={}, is_end_state=True, count={}),
                        "ciao": op.Tree(children={}, is_end_state=True, count={}),
                        "a": op.Tree(children={}, is_end_state=True, count={}),
                    },
                    count={"there": 1, "ciao": 3, "a": 1},
                ),
            },
            count={"ciao": 1, "hello": 3},
        )
        op.merge_left(tree_1, tree_2)
        assert tree_1 == expected

    def test_prune_with_rule_1(self) -> None:
        tree = op.Tree(
            children={
                "hello": op.Tree(
                    children={
                        "a": op.Tree(children={}, is_end_state=True, count={}),
                        "b": op.Tree(children={}, is_end_state=True, count={}),
                        "c": op.Tree(children={}, is_end_state=True, count={}),
                        "d": op.Tree(
                            children={
                                "ciao": op.Tree(
                                    children={}, is_end_state=True, count={}
                                )
                            },
                            count={"ciao": 1},
                        ),
                        "e": op.Tree(children={}, is_end_state=True, count={}),
                    },
                    count={"a": 2, "b": 2, "c": 2, "d": 2, "e": 1},
                )
            },
            count={"hello": 4},
        )
        op.prune(tree, theta_1=0.2, theta_3=1.0)
        expected = op.Tree(
            children={
                "hello": op.Tree(
                    children={
                        "<*>": op.Tree(
                            children={
                                "ciao": op.Tree(
                                    children={}, is_end_state=True, count={}
                                )
                            },
                            is_end_state=True,
                            count={"ciao": 1},
                        )
                    },
                    count={"<*>": 9},
                )
            },
            count={"hello": 4},
        )
        assert tree == expected

    def test_prune_with_rule_2(self) -> None:
        tree = op.Tree(
            children={
                "ciao": op.Tree(children={}, is_end_state=True, count={}),
                "bye": op.Tree(
                    children={"b": op.Tree(children={}, is_end_state=True, count={})},
                    count={"b": 2},
                ),
                "hello": op.Tree(
                    children={
                        "ciao": op.Tree(children={}, is_end_state=True, count={}),
                        "a": op.Tree(children={}, is_end_state=True, count={}),
                    },
                    count={"ciao": 20, "a": 1},
                ),
            },
            count={"ciao": 40, "bye": 1, "hello": 1},
        )
        op.prune(tree, theta_1=0.2, theta_2=0.5, theta_3=1.0)
        expected = op.Tree(
            children={
                "ciao": op.Tree(children={}, is_end_state=True, count={}),
                "<*>": op.Tree(
                    children={"<*>": op.Tree(children={}, is_end_state=True, count={})},
                    count={"<*>": 23},
                ),
            },
            count={"ciao": 40, "<*>": 2},
        )
        assert tree == expected

    def test_prune_with_rule_3(self) -> None:
        tree = op.Tree(
            children={
                "ciao": op.Tree(children={}, is_end_state=True, count={}),
                "bye": op.Tree(children={}, is_end_state=True, count={}),
                "hello": op.Tree(children={}, is_end_state=True, count={}),
                "hello1": op.Tree(children={}, is_end_state=True, count={}),
                "hello2": op.Tree(children={}, is_end_state=True, count={}),
                "hello3": op.Tree(children={}, is_end_state=True, count={}),
                "hello4": op.Tree(children={}, is_end_state=True, count={}),
            },
            count={
                "ciao": 3,
                "bye": 2,
                "hello": 1,
                "hello1": 1,
                "hello2": 1,
                "hello3": 1,
                "hello4": 1,
            },
        )
        op.prune(tree, theta_1=0.2, theta_2=0.8, theta_3=0.4)
        expected = op.Tree(
            children={
                "ciao": op.Tree(children={}, is_end_state=True, count={}),
                "bye": op.Tree(children={}, is_end_state=True, count={}),
                "<*>": op.Tree(children={}, is_end_state=True, count={}),
            },
            count={"ciao": 3, "bye": 2, "<*>": 5},
        )
        assert tree == expected

    def test_get_templates(self) -> None:
        tree = op.Tree(
            children={
                "ciao": op.Tree(children={}, is_end_state=True, count={}),
                "bye": op.Tree(
                    children={"b": op.Tree(children={}, is_end_state=True, count={})},
                    count={"b": 2},
                ),
                "hello": op.Tree(
                    children={
                        "ciao": op.Tree(children={}, is_end_state=True, count={}),
                        "a": op.Tree(children={}, is_end_state=True, count={}),
                    },
                    count={"ciao": 20, "a": 1},
                    is_end_state=True,
                ),
            },
            count={"ciao": 40, "bye": 1, "hello": 1},
        )

        templates = set(['ciao', 'bye b', 'hello', 'hello ciao', 'hello a'])
        assert templates == set(op.get_templates(tree))
