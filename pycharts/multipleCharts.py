import numpy as np
import matplotlib.pyplot as plt

def save_colormaps(file_path, save_dir, net_size):
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

    # Save each colormap as a separate image
    for i in range(num_grids):
        plt.imshow(config[i], cmap=cmap)
        plt.colorbar()
        save_path = f"{save_dir}/colormap_{i}.png"
        plt.savefig(save_path)
        plt.clf()  # Clear the figure

file_path = "spins.txt"
save_dir = "../colormaps"
net_size = 100

save_colormaps(file_path, save_dir, net_size)
