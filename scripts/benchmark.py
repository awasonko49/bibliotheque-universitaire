import csv
import math
import random
import statistics
import time
from pathlib import Path

"""Benchmark Python des structures de la bibliothèque."""

BASE = Path(__file__).resolve().parent.parent
CSV_FILE = BASE / "data" / "benchmark.csv"
OUT_DIR = BASE / "graphs"
OUT_DIR.mkdir(exist_ok=True)


def generate_data(n, profile):
    if profile == "uniforme":
        return [random.randint(1900, 2025) for _ in range(n)]
    if profile == "gaussienne":
        mu = 2010
        sigma = 18
        return [max(1900, min(2025, int(random.gauss(mu, sigma)))) for _ in range(n)]
    return list(range(1900, 1900 + n))


def benchmark_insert_static(data):
    from src.tableau_statique import ts_creer, ts_inserer, ts_detruire


def run_once():
    pass
