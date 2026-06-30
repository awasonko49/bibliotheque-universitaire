import csv
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

BASE = Path(__file__).resolve().parent.parent
CSV_FILE = BASE / "data" / "benchmark.csv"
OUT_DIR = BASE / "graphs"
OUT_DIR.mkdir(exist_ok=True)


def load_data():
    data = {"insertion": {}, "recherche": {}, "tri": {}}
    with open(CSV_FILE, newline="", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            op = row["operation"]
            profile = row["profile"]
            structure = row["structure"]
            n = int(row["n"])
            mean = float(row["mean_ms"])
            std = float(row["std_ms"])
            data.setdefault(op, {}).setdefault(profile, {}).setdefault(structure, []).append((n, mean, std))
    return data


def average_points(points):
    xs = sorted({n for n, _, _ in points})
    ys = []
    errs = []
    for n in xs:
        vals = [m for (nn, m, _) in points if nn == n]
        stds = [s for (nn, _, s) in points if nn == n]
        ys.append(sum(vals) / len(vals))
        errs.append(sum(stds) / len(stds))
    return xs, ys, errs


def plot_structure_comparison(op, title):
    data = load_data()
    fig, ax = plt.subplots(figsize=(9, 5.5))

    ordered_structures = ("statique", "dynamique", "liste_chainee")
    structure_labels = {
        "statique": "Tableau statique",
        "dynamique": "Tableau dynamique",
        "liste_chainee": "Liste chaînée",
    }
    style_map = {
        "statique": {"marker": "o", "linestyle": "-", "color": "#1f77b4"},
        "dynamique": {"marker": "s", "linestyle": "--", "color": "#ff7f0e"},
        "liste_chainee": {"marker": "^", "linestyle": ":", "color": "#2ca02c"},
    }

    for structure in ordered_structures:
        all_points = []
        for profile in ("uniforme", "gaussienne", "tri"):
            all_points.extend(data.get(op, {}).get(profile, {}).get(structure, []))

        if not all_points:
            continue

        xs, ys, errs = average_points(all_points)
        style = style_map.get(structure, {})
        ax.errorbar(
            xs,
            ys,
            yerr=errs,
            marker=style.get("marker", "o"),
            linestyle=style.get("linestyle", "-"),
            color=style.get("color"),
            linewidth=1.8,
            label=structure_labels.get(structure, structure),
            capsize=3,
            elinewidth=1.0,
        )

    ax.set_title(f"Comparaison des structures — {title}")
    ax.set_xlabel("Nombre d’ouvrages n")
    ax.set_ylabel("Temps moyen (ms)")
    ax.grid(True, alpha=0.3)
    ax.legend(loc="best", fontsize=9)
    fig.tight_layout()
    fig.savefig(OUT_DIR / f"{op}_comparison.png", dpi=150)
    plt.close(fig)


def plot_profile_comparison(op, title):
    data = load_data()
    for profile in ("uniforme", "gaussienne", "tri"):
        fig, ax = plt.subplots(figsize=(9, 5.5))

        ordered_structures = ("statique", "dynamique", "liste_chainee")
        structure_labels = {
            "statique": "Tableau statique",
            "dynamique": "Tableau dynamique",
            "liste_chainee": "Liste chaînée",
        }
        style_map = {
            "statique": {"marker": "o", "linestyle": "-", "color": "#1f77b4"},
            "dynamique": {"marker": "s", "linestyle": "--", "color": "#ff7f0e"},
            "liste_chainee": {"marker": "^", "linestyle": ":", "color": "#2ca02c"},
        }

        for structure in ordered_structures:
            points = data.get(op, {}).get(profile, {}).get(structure, [])
            if not points:
                continue
            xs, ys, errs = average_points(points)
            style = style_map.get(structure, {})
            ax.errorbar(
                xs,
                ys,
                yerr=errs,
                marker=style.get("marker", "o"),
                linestyle=style.get("linestyle", "-"),
                color=style.get("color"),
                linewidth=1.8,
                label=structure_labels.get(structure, structure),
                capsize=3,
                elinewidth=1.0,
            )

        ax.set_title(f"{title} — profil {profile}")
        ax.set_xlabel("Nombre d’ouvrages n")
        ax.set_ylabel("Temps moyen (ms)")
        ax.grid(True, alpha=0.3)
        ax.legend(loc="best", fontsize=9)
        fig.tight_layout()
        fig.savefig(OUT_DIR / f"{op}_{profile}.png", dpi=150)
        plt.close(fig)


def plot_structure_comparison_logscale(op, title):
    data = load_data()
    fig, ax = plt.subplots(figsize=(9, 5.5))

    ordered_structures = ("statique", "dynamique", "liste_chainee")
    structure_labels = {
        "statique": "Tableau statique",
        "dynamique": "Tableau dynamique",
        "liste_chainee": "Liste chaînée",
    }
    style_map = {
        "statique": {"marker": "o", "linestyle": "-", "color": "#1f77b4"},
        "dynamique": {"marker": "s", "linestyle": "--", "color": "#ff7f0e"},
        "liste_chainee": {"marker": "^", "linestyle": ":", "color": "#2ca02c"},
    }

    for structure in ordered_structures:
        all_points = []
        for profile in ("uniforme", "gaussienne", "tri"):
            all_points.extend(data.get(op, {}).get(profile, {}).get(structure, []))

        if not all_points:
            continue

        xs, ys, errs = average_points(all_points)
        style = style_map.get(structure, {})
        ax.errorbar(
            xs,
            ys,
            yerr=errs,
            marker=style.get("marker", "o"),
            linestyle=style.get("linestyle", "-"),
            color=style.get("color"),
            linewidth=1.8,
            label=structure_labels.get(structure, structure),
            capsize=3,
            elinewidth=1.0,
        )

    ax.set_title(f"Comparaison des structures (échelle logarithmique) — {title}")
    ax.set_xlabel("Nombre d’ouvrages n")
    ax.set_xscale("log")
    ax.set_xticks([100, 1000, 10000, 100000])
    ax.xaxis.set_major_formatter(ticker.ScalarFormatter())
    ax.set_ylabel("Temps moyen (ms)")
    ax.grid(True, alpha=0.3)
    ax.legend(loc="best", fontsize=9)
    fig.tight_layout()
    fig.savefig(OUT_DIR / f"{op}_comparison_log.png", dpi=150)
    plt.close(fig)


def plot_fixed_n_distribution(op, title):
    data = load_data()
    ns = sorted({n for profile_data in data.get(op, {}).values() for structure_data in profile_data.values() for n, _, _ in structure_data})
    profiles = ("uniforme", "gaussienne", "tri")
    ordered_structures = ("statique", "dynamique", "liste_chainee")
    structure_labels = {
        "statique": "Tableau statique",
        "dynamique": "Tableau dynamique",
        "liste_chainee": "Liste chaînée",
    }
    style_map = {
        "statique": {"marker": "o", "linestyle": "-", "color": "#1f77b4"},
        "dynamique": {"marker": "s", "linestyle": "--", "color": "#ff7f0e"},
        "liste_chainee": {"marker": "^", "linestyle": ":", "color": "#2ca02c"},
    }

    for n in ns:
        fig, ax = plt.subplots(figsize=(9, 5.5))
        x = list(range(len(profiles)))

        for structure in ordered_structures:
            means = []
            for profile in profiles:
                points = data.get(op, {}).get(profile, {}).get(structure, [])
                mean = next((m for nn, m, _ in points if nn == n), float('nan'))
                means.append(mean)

            style = style_map.get(structure, {})
            ax.plot(
                x,
                means,
                marker=style.get("marker", "o"),
                linestyle=style.get("linestyle", "-"),
                color=style.get("color"),
                linewidth=1.8,
                label=structure_labels.get(structure, structure),
            )

        ax.set_title(f"Profil de distribution fixe (n={n}) — {title}")
        ax.set_xlabel("Profil de données")
        ax.set_xticks(x)
        ax.set_xticklabels(["uniforme", "gaussienne", "tri"])
        ax.set_ylabel("Temps moyen (ms)")
        ax.grid(True, alpha=0.3)
        ax.legend(loc="best", fontsize=9)
        fig.tight_layout()
        fig.savefig(OUT_DIR / f"{op}_fixed_n_{n}.png", dpi=150)
        plt.close(fig)


def plot_empirical_vs_theory(op, title):
    data = load_data()
    fig, ax = plt.subplots(figsize=(9, 5.5))
    ordered_structures = ("statique", "dynamique", "liste_chainee")
    structure_labels = {
        "statique": "Tableau statique",
        "dynamique": "Tableau dynamique",
        "liste_chainee": "Liste chaînée",
    }
    style_map = {
        "statique": {"marker": "o", "linestyle": "-", "color": "#1f77b4"},
        "dynamique": {"marker": "s", "linestyle": "--", "color": "#ff7f0e"},
        "liste_chainee": {"marker": "^", "linestyle": ":", "color": "#2ca02c"},
    }

    if op == "tri":
        exponent = 2
        theory_label = "$n^2$"
        theory_func = lambda x: x ** 2
    else:
        exponent = 1
        theory_label = "$n$"
        theory_func = lambda x: x

    for structure in ordered_structures:
        points = []
        for profile in data.get(op, {}).keys():
            points.extend(data.get(op, {}).get(profile, {}).get(structure, []))
        if not points:
            continue

        xs, ys, _ = average_points(points)
        style = style_map.get(structure, {})
        ax.plot(
            xs,
            ys,
            marker=style.get("marker", "o"),
            linestyle=style.get("linestyle", "-"),
            color=style.get("color"),
            linewidth=1.8,
            label=structure_labels.get(structure, structure),
        )

    if xs:
        scale = ys[0] / theory_func(xs[0]) if theory_func(xs[0]) != 0 else 1
        theory_values = [scale * theory_func(x) for x in xs]
        ax.plot(
            xs,
            theory_values,
            linestyle="--",
            color="#7f7f7f",
            linewidth=2,
            label=f"Référence {theory_label}",
        )

    ax.set_title(f"Courbes empiriques vs théorie — {title}")
    ax.set_xlabel("Nombre d’ouvrages n")
    ax.set_ylabel("Temps moyen (ms)")
    ax.grid(True, alpha=0.3)
    ax.legend(loc="best", fontsize=9)
    fig.tight_layout()
    fig.savefig(OUT_DIR / f"{op}_theory.png", dpi=150)
    plt.close(fig)


def plot_slowdown_histogram(op, title):
    data = load_data()
    profiles = ("uniforme", "gaussienne", "tri")
    ordered_structures = ("statique", "dynamique", "liste_chainee")
    structure_labels = {
        "statique": "Tableau statique",
        "dynamique": "Tableau dynamique",
        "liste_chainee": "Liste chaînée",
    }
    style_map = {
        "statique": {"marker": "o", "linestyle": "-", "color": "#1f77b4"},
        "dynamique": {"marker": "s", "linestyle": "--", "color": "#ff7f0e"},
        "liste_chainee": {"marker": "^", "linestyle": ":", "color": "#2ca02c"},
    }

    fig, axes = plt.subplots(1, len(profiles), figsize=(16, 4.8), sharey=True)

    for ax, profile in zip(axes, profiles):
        ns = sorted({n for structure_data in data.get(op, {}).get(profile, {}).values() for n, _, _ in structure_data})
        for structure in ordered_structures:
            ratios = []
            for n in ns:
                points = data.get(op, {}).get(profile, {}).get(structure, [])
                mean = next((m for nn, m, _ in points if nn == n), None)
                baseline = min(
                    (next((m for nn, m, _ in data.get(op, {}).get(profile, {}).get(other, []) if nn == n), float('inf'))
                     for other in ordered_structures),
                    default=1,
                )
                ratios.append(mean / baseline if mean is not None and baseline != 0 else float('nan'))

            style = style_map.get(structure, {})
            ax.plot(
                ns,
                ratios,
                marker=style.get("marker", "o"),
                linestyle=style.get("linestyle", "-"),
                color=style.get("color"),
                linewidth=1.8,
                label=structure_labels.get(structure, structure),
            )

        ax.set_title(f"{profile}")
        ax.set_xlabel("Nombre d’ouvrages n")
        ax.set_xscale("log")
        ax.xaxis.set_major_formatter(ticker.ScalarFormatter())
        ax.set_xticks([100, 1000, 10000, 100000])
        ax.grid(True, alpha=0.3)
        if ax is axes[0]:
            ax.set_ylabel("Ralentissement relatif")

    fig.suptitle(f"Histogramme des ralentissements relatifs — {title}")
    handles, labels = axes[-1].get_legend_handles_labels()
    fig.legend(handles, labels, loc="upper center", ncol=3, fontsize=9)
    fig.tight_layout(rect=[0, 0, 1, 0.95])
    fig.savefig(OUT_DIR / f"{op}_slowdown.png", dpi=150)
    plt.close(fig)


if __name__ == "__main__":
    for op, title in [
        ("insertion", "Insertion"),
        ("recherche", "Recherche"),
        ("tri", "Tri"),
    ]:
        plot_structure_comparison(op, title)
        plot_structure_comparison_logscale(op, title)
        plot_profile_comparison(op, title)
        plot_empirical_vs_theory(op, title)
        plot_slowdown_histogram(op, title)
        plot_fixed_n_distribution(op, title)
    print("Courbes generees dans", OUT_DIR)
