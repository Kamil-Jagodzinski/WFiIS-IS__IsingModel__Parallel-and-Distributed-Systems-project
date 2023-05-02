import numpy as np
import matplotlib.pyplot as plt

def plot_spin_map(file_path, save_path):
    with open(file_path) as f:
        lines = f.readlines()

    config = np.zeros((len(lines), len(lines[0].split())), dtype=int)

    for i, line in enumerate(lines):
        spin_row = line.split()
        config[i] = spin_row

    cmap = plt.cm.RdBu
    plt.imshow(config, cmap=cmap)
    plt.colorbar()
    plt.savefig(save_path)
    plt.show()

file_path = "spins.txt"
save_path = "out.png"

plot_spin_map(file_path, save_path)
