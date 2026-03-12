from functools import lru_cache
from Levenshtein import distance
import polars as pl


def calculate_ts(df: pl.DataFrame) -> float:
    # Calculate Template Set
    ground_n = len(ground_set := set(df["GroundTruth"].unique()))
    parsed_set = set(df["Templates"].unique())

    return float(ground_n - len(ground_set - parsed_set)) / ground_n  


@lru_cache
def calculate_distance(x1, x2):
    n = max(len(x1), len(x2))
    return float(n - distance(x1, x2)) / n


def calculate_ned(df: pl.DataFrame) -> float:
    # Calculate Normalize Edit Distance
    result = df.map_rows(lambda x: calculate_distance(x[0], x[1])).mean() 
    return float(result.to_numpy()[0, 0])


@lru_cache
def are_equal(x1, x2):
    return x1 == x2


def calculate_pa(df: pl.DataFrame) -> float:
    # Calculate Parse Accuracy
    result = df.map_rows(lambda x: are_equal(x[0], x[1])).mean() 
    return float(result.to_numpy()[0, 0])