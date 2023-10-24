import csv
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

working_path = Path(".")
build_path = working_path.joinpath("build-manual")

N = 512
x = np.arange(0, N, 1, dtype=int)
y = np.zeros(N, dtype=float)

file_name = "signal_file.csv"
file = open(build_path.joinpath(file_name), mode="r", newline='')

csv_reader = csv.reader(file, delimiter=",")
i: int = 0
for data in csv_reader:
    for value in data:
        if value == "":
            continue
        y[i] = float(value)
        i += 1

plt.figure(figsize=(10, 10))
plt.plot(x, y)
plt.xlabel("samples")
plt.ylabel("values")
plt.title("signal")

file.close()

plt.show()
