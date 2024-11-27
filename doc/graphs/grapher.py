import matplotlib.pyplot as plt

# In the future I would like to add dynamic value loading

# State pruning effectivity graph
depth = [5, 6, 7, 8, 9, 10]
minmax = [152243, 1441355, 16082973, 151144099, 1582409313, 14455391126]
alpha = [4455, 11188, 53307, 122945, 585697, 1554766]
nega = [4852, 9092, 59045, 136329, 620559, 1246934]
plt.plot(depth, minmax, label="Minimax")
plt.plot(depth, alpha, label="AlphaBeta")
plt.plot(depth, nega, label="Negascout")
plt.ylim(0, 2000000)
plt.xlabel("Search depth")
plt.ylabel("Searched states")
plt.title("Efficiency of state pruning (without transposition tables)")
plt.legend()
plt.savefig("pruning.png")
plt.clf()

# Transposition table efficiency graph
x = ["nega w/ table", "alpha w/ table", "nega w/o table", "alpha w/o table"]
y = [774643, 1171044, 1246934, 1554766]
plt.bar(x, y, color=["green", "orange", "green", "orange"])
plt.xlabel("Search algorithm")
plt.ylabel("Searched states")
plt.title("Efficiency of transposition tables")
plt.savefig("transposition.png")
plt.clf()

# Move order efficiency graph
x = ["nega w/ opt", "alpha w/ opt", "nega w/ default", "alpha w/ default"]
y = [774643, 1171044, 4746446, 10441421]
plt.bar(x, y, color=["green", "orange", "green", "orange"])
plt.xlabel("Search algorithm")
plt.ylabel("Searched states")
plt.title("Efficiency of move order (with transposition tables)")
plt.savefig("move_order.png")
plt.clf()

# SIMD performance graph
x = ["-AVX2-", "-NOSIMD-", "_AVX2_", "_NOSIMD_", "__AVX2__", "__NOSIMD__", "NOSIMD"]
y = [1.1, 3.0, 1.35, 3.8, 1.7, 5.0, 3.0]
colors = ["darkred", "darkred", "blue", "blue", "darkblue", "darkblue", "orange"]

cpu_labels = [
    "AMD r7-5700x3d",
    "INTEL i5-1340p",
    "INTEL xeon e5-2699v3",
    "QUALCOMM Snapdragon 8-gen2"
]
cpu_colors = ["darkred", "blue", "darkblue", "orange"]

plt.bar(x, y, color=colors)

# Add a concise legend for the CPUs
legend_handles = [plt.Line2D([0], [0], color=color, lw=4, label=label) for color, label in zip(cpu_colors, cpu_labels)]
plt.legend(handles=legend_handles, bbox_to_anchor=(0.45, 1.0), loc="upper right", ncol=1, fontsize=9)

# Labels and title
plt.xlabel("Instruction Set")
plt.ylabel("Runtime (s)")
plt.title("Runtime of engine playing one game against itself")

# Adjust layout
plt.tight_layout()
plt.savefig("simd.png")
plt.clf()


x = ["-single-", "-parallel-", "_single_", "_parallel_", "__single__", "__parallel__", "single", "parallel"]
y = [1.1, 1.1, 1.35, 1.2, 1.7, 2.8, 3.0, 3.1]
colors = ["darkred", "darkred", "blue", "blue", "darkblue", "darkblue", "orange", "orange"]
cpu_labels = [
    "AMD r7-5700x3d AVX2",
    "INTEL i5-1340p AVX2",
    "INTEL xeon e5-2699v3 AVX2",
    "QUALCOMM Snapdragon 8-gen2"
]
cpu_colors = ["darkred", "blue", "darkblue", "orange"]
# Plot the bars
plt.bar(x, y, color=colors)

# Add a concise legend for the CPUs
legend_handles = [plt.Line2D([0], [0], color=color, lw=4, label=label) for color, label in zip(cpu_colors, cpu_labels)]
plt.legend(handles=legend_handles, bbox_to_anchor=(0.5, 1.0), loc="upper right", ncol=1, fontsize=9)

# Labels and title
plt.ylabel("Runtime (s)")
plt.title("Runtime of engine playing one game against itself (1 vs 2 cores)")

# Adjust layout
plt.tight_layout()
plt.savefig("parallel.png")
plt.clf()
