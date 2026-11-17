# mypy: ignore-errors
type Tree = dict[str, list[int | Tree]]


def init_tree() -> Tree:
    return {}


def add_log(tree: Tree, log: list[str]) -> None:
    if len(log) == 0:
        return None

    head, tail = log[0], log[1:]
    if head in tree:
        tree[head][0] += 1
    else:
        tree[head] = [1, {}]

    add_log(tree[head][1], tail)


def add_logs(tree: Tree, logs: list[list[str]]) -> None:
    for log in logs:
        add_log(tree, log=log)


def merge_left(tree_left: Tree, tree_right: Tree) -> None:
    childs_left = tree_left.keys()
    for child_right in tree_right:
        if child_right not in childs_left:
            tree_left[child_right] = tree_right[child_right]
        else:
            tree_left[child_right][0] += tree_right[child_right][0]
            merge_left(tree_left[child_right][1], tree_right[child_right][1])


def prune(tree: Tree, theta_1: float = 0.2, theta_2: float = 0.4) -> None:
    childs = list(tree.keys())
    if len(childs) == 0:
        return None
    n = sum([tree[node][0] for node in childs])

    if theta_1 > max([tree[node][0] / n for node in childs]):
        # Rule 1
        tree["<*>"] = tree.pop(childs[0])
        for child in childs[1:]:
            merge_left(tree["<*>"][1], tree[child][1])

    else:
        # Rule 2
        for child in childs:
            if tree[child][0] / n < theta_2:
                if "<*>" not in tree:
                    tree["<*>"] = tree.pop(child)
                else:
                    tree["<*>"][0] += tree[child][0]
                    merge_left(tree["<*>"][1], tree[child][1])
                    tree.pop(child)

    for child in tree:
        prune(tree[child][1], theta_2=theta_2)
