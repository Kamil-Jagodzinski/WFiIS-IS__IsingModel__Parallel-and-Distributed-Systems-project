import numpy as np
import matplotlib.pyplot as plt

def plot_spin_map(file_path, save_path, net_size):
    with open(file_path) as f:
        lines = f.readlines()

    # Remove newline characters and split the lines
    lines = [line.strip() for line in lines]

    # Calculate the number of grids in the file
    num_grids = len(lines) // net_size

    # Create an empty configuration array
    config = np.zeros((num_grids, net_size, net_size), dtype=int)

    for i in range(num_grids):
        for j in range(net_size):
            spin_row = lines[i * net_size + j].split()
            config[i, j] = spin_row

    cmap = plt.cm.RdBu
    plt.imshow(config[-1], cmap=cmap)
    plt.colorbar()
    plt.savefig(save_path)
    plt.show()

file_path = "spins.txt"
save_path = "out.png"
net_size = 100

plot_spin_map(file_path, save_path, net_size)
