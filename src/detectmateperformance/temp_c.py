# mypy: ignore-errors
from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class Tree:
    """A prefix-tree state, including whether a log ends at this state."""

    children: dict[str, Tree] = field(default_factory=dict)
    count: dict[str, int] = field(default_factory=dict)
    is_end_state: bool = False


def init_tree() -> Tree:
    return Tree()


def add_log(tree: Tree, log: list[str]) -> None:
    if len(log) == 0:
        tree.is_end_state = True
        return None

    head, tail = log[0], log[1:]
    if head in tree.children:
        tree.count[head] += 1
    else:
        tree.children[head] = init_tree()
        tree.count[head] = 1

    add_log(tree.children[head], tail)


def add_logs(tree: Tree, logs: list[list[str]]) -> None:
    for log in logs:
        add_log(tree, log=log)


def merge_left(tree_left: Tree, tree_right: Tree) -> None:
    tree_left.is_end_state |= tree_right.is_end_state
    childs_left = tree_left.children.keys()
    for child_right in tree_right.children:
        if child_right not in childs_left:
            tree_left.children[child_right] = tree_right.children[child_right]
            tree_left.count[child_right] = tree_right.count[child_right]
        else:
            tree_left.count[child_right] += tree_right.count[child_right]
            merge_left(
                tree_left.children[child_right], tree_right.children[child_right]
            )


def is_rule_1(tree: Tree, childs: list[str], theta_1: float, n: int) -> bool:
    return theta_1 > max([tree.count[node] / n for node in childs])


def apply_rule_1(tree: Tree, childs: list[str]) -> None:
    tree.children["<*>"] = tree.children.pop(childs[0])
    tree.count["<*>"] = tree.count.pop(childs[0])
    for child in childs[1:]:
        tree.count["<*>"] += tree.count[child]
        merge_left(tree.children["<*>"], tree.children[child])
        tree.children.pop(child)
        tree.count.pop(child)


def is_rule_2(tree: Tree, child: str, theta_2: float, n: int) -> bool:
    return tree.count[child] / n < theta_2


def apply_rule_2(tree: Tree, child: str) -> None:
    if "<*>" not in tree.children:
        tree.children["<*>"] = tree.children.pop(child)
        tree.count["<*>"] = tree.count.pop(child)
    else:
        tree.count["<*>"] += tree.count[child]
        merge_left(tree.children["<*>"], tree.children[child])
        tree.children.pop(child)
        tree.count.pop(child)


def rule_3(
    tree: Tree, childs: list[str], theta_1: float, theta_3: float, n: int
) -> bool:
    per = [tree.count[node] / n for node in childs]
    if sum(list(filter(lambda x: x >= theta_1, per))) < theta_3:
        return False

    f_childs = []
    for per_i, child_i in zip(per, childs):
        if per_i < theta_1:
            f_childs.append(child_i)

    if len(f_childs) > 0:
        tree.children["<*>"] = tree.children.pop(f_childs[0])
        tree.count["<*>"] = tree.count.pop(f_childs[0])
        for child in f_childs[1:]:
            tree.count["<*>"] += tree.count[child]
            merge_left(tree.children["<*>"], tree.children[child])
            tree.children.pop(child)
            tree.count.pop(child)
    return True


def prune(
    tree: Tree, theta_1: float = 0.2, theta_2: float = 0.8, theta_3: float = 0.5
) -> None:
    childs = list(tree.children.keys())
    if len(childs) == 0:
        return None

    n = sum([tree.count[node] for node in childs])

    if is_rule_1(tree, childs=childs, theta_1=theta_1, n=n):
        apply_rule_1(tree, childs=childs)
    elif not rule_3(tree, childs=childs, theta_1=theta_1, theta_3=theta_3, n=n):
        for child in childs:
            if is_rule_2(tree, child=child, theta_2=theta_2, n=n):
                apply_rule_2(tree, child=child)

    for child in tree.children:
        damp = 1 - (tree.count[child] / n)
        prune(
            tree.children[child],
            theta_1=theta_1 * (1 + damp),
            theta_2=theta_2 * (1 + damp),
            theta_3=theta_3 * (1 + damp),
        )


def get_templates(tree: Tree) -> list[str]:
    templates = []

    def visit(node: Tree, words: list[str]) -> None:
        if node.is_end_state:
            templates.append(" ".join(words))

        for token, child in node.children.items():
            visit(child, words + [token])

    visit(tree, [])
    return templates
