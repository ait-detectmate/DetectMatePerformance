"""One-shot golden generator.

Run ONLY on the unmodified origin/development matcher. NEVER regenerate
after the capture change -- the whole point is freezing the pre-change
template assignment (fix/verbatim-wildcard-capture must not alter it).
"""
import argparse
import json
import os

OUT_PATH = "tests/test_data/audit_golden_event_ids.json"


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--force",
        action="store_true",
        help="overwrite an existing golden file (DANGEROUS: it destroys the frozen baseline)",
    )
    args = parser.parse_args()

    if os.path.exists(OUT_PATH) and not args.force:
        print(f"refusing to overwrite existing golden {OUT_PATH} (pass --force to override)")
        raise SystemExit(1)

    # imported late so the overwrite guard runs before the matcher is loaded
    from detectmateperformance.match_tree import TreeMatcher

    with open("tests/test_data/audit.log") as f:
        logs = ["):".join(line.split("):")[1:]) for line in f.readlines()]

    res = TreeMatcher.from_file(
        "tests/test_data/audit_templates.txt"
    ).match_batch(logs, True, n_workers=1)
    ids = res.get_all_events_ids()
    with open(OUT_PATH, "w") as f:
        json.dump(ids, f)
    print(f"wrote {len(ids)} golden event ids")


if __name__ == "__main__":
    main()
