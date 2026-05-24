import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import os

df = pd.read_csv("/Users/mac/Desktop/c++ unik/set9_a1/cmake-build-debug/results.csv")
os.makedirs("plots", exist_ok=True)

array_types = {
    "random": "Случайные строки",
    "reverse_sorted": "Обратно отсортированные строки",
    "nearly_sorted": "Почти отсортированные строки",
    "common_prefix": "Строки с общим префиксом"
}

style = {
    "QuickSort ": {"color": "crimson",      "ls": "-",  "marker": "o"},
    "MergeSort ": {"color": "steelblue",    "ls": "-",  "marker": "s"},
    "Ternary String QSort": {"color": "darkorange",   "ls": "--", "marker": "^"},
    "String MergeSort ": {"color": "forestgreen",  "ls": "--", "marker": "D"},
    "MSD Radix Sort": {"color": "mediumpurple", "ls": ":",  "marker": "v"},
    "MSD Radix + TStrQSort": {"color": "teal", "ls": ":",  "marker": "P"},
}

algorithms = df["algorithm"].unique().tolist()


def plot_metric(metric, ylabel, title, filename):
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    axes = axes.flatten()

    for ax, (atype, label) in zip(axes, array_types.items()):
        sub = df[df["array_type"] == atype]
        for alg in algorithms:
            s = sub[sub["algorithm"] == alg].sort_values("n")
            st = style.get(alg, {})
            ax.plot(s["n"], s[metric], label=alg,
                    color=st.get("color"), linestyle=st.get("ls", "-"),
                    marker=st.get("marker", "o"), markersize=4, linewidth=1.6)
        ax.set_title(label, fontsize=12, fontweight="bold")
        ax.set_xlabel("размер массива")
        ax.set_ylabel(ylabel)
        ax.xaxis.set_major_locator(mticker.MultipleLocator(500))
        ax.grid(True, linestyle="--", alpha=0.4)
        ax.legend(fontsize=7.5)

    fig.suptitle(title, fontsize=14, fontweight="bold", y=1.01)
    plt.tight_layout()
    plt.savefig(f"plots/{filename}.png", dpi=150, bbox_inches="tight")
    plt.close()


def plot_comparison(atype, label):
    sub = df[df["array_type"] == atype]
    algs = list(style.keys())

    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    for ax, (metric, ylabel) in zip(axes, [("time_ms", "время (мс)"),
                                           ("char_cmps", "посимвольных сравнений")]):
        for alg in algs:
            s = sub[sub["algorithm"] == alg].sort_values("n")
            st = style.get(alg, {})
            ax.plot(s["n"], s[metric], label=alg,
                    color=st.get("color"), linestyle=st.get("ls", "-"),
                    marker=st.get("marker", "o"), markersize=4, linewidth=1.6)
        ax.set_xlabel("размер массива")
        ax.set_ylabel(ylabel)
        ax.grid(True, linestyle="--", alpha=0.4)
        ax.legend(fontsize=8)

    fig.suptitle(f"Стандартные vs адаптированные ({label})", fontsize=13, fontweight="bold")
    plt.tight_layout()
    plt.savefig(f"plots/cmp_{atype}.png", dpi=150, bbox_inches="tight")
    plt.close()


plot_metric("time_ms", "время (мс)", "Время работы алгоритмов", "time")
plot_metric("char_cmps", "посимвольных сравнений", "число посимвольных сравнений", "cmps")

for atype, label in array_types.items():
    plot_comparison(atype, label)