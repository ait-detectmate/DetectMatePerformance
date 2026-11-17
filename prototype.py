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


def is_rule_1(tree: Tree, childs: list[str], theta_1: float, n: int) -> bool:
    return theta_1 > max([tree[node][0] / n for node in childs])


def apply_rule_1(tree: Tree, childs: list[str]) -> None:
    tree["<*>"] = tree.pop(childs[0])
    for child in childs[1:]:
        tree["<*>"][0] += tree[child][0]
        merge_left(tree["<*>"][1], tree[child][1])


def is_rule_2(tree: Tree, child: str, theta_2: float, n: int) -> bool:
    return tree[child][0] / n < theta_2


def apply_rule_2(tree: Tree, child: str) -> None:
    if "<*>" not in tree:
        tree["<*>"] = tree.pop(child)
    else:
        tree["<*>"][0] += tree[child][0]
        merge_left(tree["<*>"][1], tree[child][1])
        tree.pop(child)


def rule_3(
    tree: Tree, childs: list[str], theta_1: float, theta_3: float, n: int
) -> bool:
    per = [tree[node][0] / n for node in childs]
    if sum(list(filter(lambda x: x >= theta_1, per))) < theta_3:
        return False

    f_childs = []
    for per_i, child_i in zip(per, childs):
        if per_i < theta_1:
            f_childs.append(child_i)

    if len(f_childs) > 0:
        tree["<*>"] = tree.pop(f_childs[0])
        for child in f_childs[1:]:
            tree["<*>"][0] += tree[child][0]
            merge_left(tree["<*>"][1], tree[child][1])
            tree.pop(child)
    return True


def prune(tree: Tree, theta_1: float = 0.2, theta_2: float = 0.8, theta_3: float = 0.5) -> None:
    childs = list(tree.keys())
    if len(childs) == 0:
        return None

    n = sum([tree[node][0] for node in childs])

    if is_rule_1(tree, childs=childs, theta_1=theta_1, n=n):
        apply_rule_1(tree, childs=childs)
    elif not rule_3(tree, childs=childs, theta_1=theta_1, theta_3=theta_3, n=n):
        for child in childs:
            if is_rule_2(tree, child=child, theta_2=theta_2, n=n):
                apply_rule_2(tree, child=child)

    for child in tree:
        damp = 1 - (tree[child][0] / n)
        prune(
            tree[child][1],
            theta_1=theta_1 * (1 + damp),
            theta_2=theta_2 * (1 + damp),
            theta_3=theta_3 * (1 + damp),
        )
