from detectmateperformance.match_tree import TreeMatcher
from detectmateperformance.types_ import LogTemplates

from detectmateperformance.pipeline_op import preprocessing

import polars as pl


def clean_strings(df: pl.DataFrame) -> pl.DataFrame:
    return df.with_columns(
        # Pad text so boundary-sensitive patterns match at both ends.
        pl.concat_str([pl.lit(" "), pl.col("Content"), pl.lit(" ")])
        # Replace listed punctuation and whitespace with spaces.
        .str.replace_all(r'[!"#$%&\'()*+,:;<=>?@\[\\\]^`{|}~\s]', " ")
        # Remove periods adjacent to whitespace.
        .str.replace_all(r'\.\s|\s\.', " ")
        # Remove hyphens adjacent to whitespace.
        .str.replace_all(r'-\s|\s-', " ")
        # Replace IPv4-shaped strings.
        .str.replace_all(r'\b(?:\d{1,3}\.){3}\d{1,3}\b', "VAR")
        # Replace path-like strings; remove unreachable Windows-path
        # matching and redundant backslash exclusions.
        .str.replace_all(r'(?:/[^-\s]*|\.\.[^-]*|\./[^-]*)\b', "VAR")
        # Replace negative integers, ranges, and pairs of integers.
        # Word boundaries replace the original Python lookarounds.
        .str.replace_all(
            r'(?:\B-\d+|\b\d+-\d+|\b\d+\s\d+)\b',
            "VAR",
        )
        # Replace individual integers surrounded by spaces.
        .str.replace_all(r' \d+ ', " VAR ")
        # Collapse repeated whitespace, then trim both ends.
        .str.replace_all(r'\s+', " ")
        .str.strip_chars()
        .alias("Content")
    )


class IParser:
    def __init__(self) -> None:
        self.reset()

    def __len__(self) -> int:
        return len(self.buffer)

    def add(self, log: str) -> None:
        self.buffer.append(log)

    def reset(self) -> None:
        self.buffer: list[str] = []

    def generate_from_df(self, df: pl.DataFrame) -> TreeMatcher:
        return TreeMatcher(LogTemplates([]))

    def generate(self) -> TreeMatcher:
        return self.generate_from_df(
            clean_strings(pl.DataFrame({"Content": self.buffer}))
        )

    def __call__(
        self, logs: list[str] | pl.DataFrame | str, regex: str = r"(?P<Content>.*)"
    ) -> TreeMatcher:
        if not isinstance(logs, pl.DataFrame):
            logs = preprocessing(logs=logs, regex=regex)
        logs = clean_strings(logs[["Content"]])

        return self.generate_from_df(logs)
