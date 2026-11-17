# mypy: ignore-errors
import detectmateperformance.temp_c as op


class TestTreeOp:
    def test_add_log(self) -> None:
        tree = op.init_tree()
        log = "hello there"

        op.add_log(tree, log.split(" "))

        expected = op.Tree(children={
            "hello": [1, op.Tree(children={
                "there": [1, op.Tree(children={}, is_end_state=True)]
            })]
        })
        assert tree == expected

    def test_add_logs(self) -> None:
        tree = op.init_tree()
        expected = op.Tree(children={
            "hello": [2, op.Tree(children={
                "there": [1, op.Tree(children={}, is_end_state=True)],
                "ciao": [1, op.Tree(children={}, is_end_state=True)]})
            ]
        })
        logs = [lo.split(" ") for lo in ["hello there", "hello ciao"]]

        op.add_logs(tree, logs)
        assert tree == expected

    def test_merge_left(self) -> None:
        tree_1 = op.Tree(children={
            "hello": [2, op.Tree(children={
                "there": [1, op.Tree(children={}, is_end_state=True)],
                "ciao": [1, op.Tree(children={}, is_end_state=True)]})
            ]
        })
        tree_2 = op.Tree(children={
            "ciao": [1, op.Tree(children={}, is_end_state=True)],
            "hello": [1, op.Tree(children={
                "ciao": [2, op.Tree(children={}, is_end_state=True)],
                "a": [1, op.Tree(children={}, is_end_state=True)]
            })]
        })

        expected = op.Tree(children={
            "ciao": [1, op.Tree(children={}, is_end_state=True)],
            "hello": [3, op.Tree(children={
                "there": [1, op.Tree(children={}, is_end_state=True)],
                "ciao": [3, op.Tree(children={}, is_end_state=True)],
                "a": [1, op.Tree(children={}, is_end_state=True)]
            })]
        })

        op.merge_left(tree_1, tree_2)
        assert tree_1 == expected

    def test_prune_with_rule_1(self) -> None:
        tree = op.Tree(children={
            "hello": [4, op.Tree(children={
                "a": [2, op.Tree(children={}, is_end_state=True)],
                "b": [2, op.Tree(children={}, is_end_state=True)],
                "c": [2, op.Tree(children={}, is_end_state=True)],
                "d": [2, op.Tree(children={"ciao": [
                    1, op.Tree(children={}, is_end_state=True)]})
                ],
                "e": [1, op.Tree(children={}, is_end_state=True)],
            })]
        })
        op.prune(tree, theta_1=0.2, theta_3=1.)

        expected = op.Tree(children={
            "hello": [4, op.Tree(children={
                "<*>": [9, op.Tree(children={
                    "ciao": [1, op.Tree(children={}, is_end_state=True)]}, is_end_state=True)
                ]
            })]
        })
        assert tree == expected

    def test_prune_with_rule_2(self) -> None:
        tree = op.Tree(children={
            "ciao": [40, op.Tree(children={}, is_end_state=True)],
            "bye": [1, op.Tree(children={"b": [2, op.Tree(children={}, is_end_state=True)]})],
            "hello": [1, op.Tree(children={
                "ciao": [20, op.Tree(children={}, is_end_state=True)],
                "a": [1, op.Tree(children={}, is_end_state=True)]
            })]
        })
        op.prune(tree, theta_1=0.2, theta_2=0.5, theta_3=1.0)

        expected = op.Tree(children={
            "ciao": [40, op.Tree(children={}, is_end_state=True)],
            "<*>": [2, op.Tree(children={
                "<*>": [23, op.Tree(children={}, is_end_state=True)],
            })]
        })
        assert tree == expected

    def test_prune_with_rule_3(self) -> None:
        tree = op.Tree(children={
            "ciao": [3, op.Tree(children={}, is_end_state=True)],
            "bye": [2, op.Tree(children={}, is_end_state=True)],
            "hello": [1, op.Tree(children={}, is_end_state=True)],
            "hello1": [1, op.Tree(children={}, is_end_state=True)],
            "hello2": [1, op.Tree(children={}, is_end_state=True)],
            "hello3": [1, op.Tree(children={}, is_end_state=True)],
            "hello4": [1, op.Tree(children={}, is_end_state=True)],
        })
        op.prune(tree, theta_1=0.2, theta_2=0.8, theta_3=0.4)

        expected = op.Tree(children={
            'ciao': [3, op.Tree(children={}, is_end_state=True)],
            'bye': [2, op.Tree(children={}, is_end_state=True)],
            '<*>': [5, op.Tree(children={}, is_end_state=True)]
        })
        assert tree == expected
