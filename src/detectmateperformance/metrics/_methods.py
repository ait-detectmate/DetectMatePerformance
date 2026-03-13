from functools import lru_cache
from Levenshtein import distance
import polars as pl
import numpy as np


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


def calculate_ga(df: pl.DataFrame) -> float:
    # Calculate Group Accuracy
    df = df.with_row_index("index")
    grouped_t = df.group_by("Templates").agg(pl.col("index").alias("a"))["a"].to_list()
    grouped_t = set([tuple(i) for i in grouped_t])
    
    grouped_g = df.group_by("GroundTruth").agg(pl.col("index").alias("a"))["a"].to_list()
    grouped_g = set([tuple(i) for i in grouped_g])

    final = np.sum([len(s) for s in grouped_g - grouped_t])


    return float((len(df) - final) / len(df))


def calculate_fr(df: pl.DataFrame) -> float:
    # Percentage of found ratio
    return float((df["Templates"] != "template not found").sum() / len(df))