from detectmateperformance.match_tree import TreeMatcher
from detectmateperformance.types_ import LogTemplates

from detectmateperformance._iparser import IParser

from detectmateperformance.temp_c import aecid_pg_generator

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

    def __str__(self) -> str:
        text = f"<Theta_1: {self.theta_1} Theta_2: {self.theta_2} Theta_3: {self.theta_3}>"

        msg = "\033[46m >>>> Generating templates    \033[0m\n"
        msg += "\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m\n"
        msg += f"\033[46m  \033[0mAECID_PG <{text}>"
        msg += f"\n\033[46m  \033[0m  -> (N. logs in buffer {len(self)})\n"
        return msg + "\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m\n"

    def generate_from_df(self, df: pl.DataFrame) -> TreeMatcher:

        print("\033[46m >>>> Generating templates    \033[0m")
        print("\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m")

        print("\033[46m  \033[0m  ⚙️  Generating and prunning log tree")
        templates = aecid_pg_generator(
            logs=df["Content"].to_list(),
            theta_1=self.theta_1,
            theta_2=self.theta_2,
            theta_3=self.theta_3,
        )

        print("\033[46m  \033[0m  💻  Initializing tree matcher instance")
        tree_matcher = TreeMatcher(LogTemplates(templates))

        print("\033[46m  \033[0m  ✅ Process complete!")
        print("\033[46m" + "".join([" " for _ in range(100)]) + "\033[0m")

        return tree_matcher
