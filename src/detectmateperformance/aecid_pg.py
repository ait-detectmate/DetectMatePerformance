from detectmateperformance.match_tree import TreeMatcher
from detectmateperformance.types_ import LogTemplates

from detectmateperformance._iparser import IParser

import polars as pl


class AECID_PG(IParser):
    def __init__(
        self,
        theta_1: float = 0.2,
        theta_2: float = 0.8,
        theta_3: float = 0.5,
    ) -> None:

        self.theta_1 = theta_1
        self.theta_2 = theta_2
        self.theta_3 = theta_3

        super().__init__()

    def generate_from_df(self, df: pl.DataFrame) -> TreeMatcher:
        return TreeMatcher(LogTemplates([]))
