import csv
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

def read_data_from_csv(file, n: int) -> [int, float]:
    if file is None:
        exit("dupa")

    x = np.arange(0, n, 1, dtype=int)
    y = np.zeros(n, dtype=float)

    file = open(file, mode="r", newline='')

    csv_reader = csv.reader(file, delimiter=",")
    i: int = 0
    for data in csv_reader:
        for value in data:
            if value == "":
                continue
            y[i] = float(value)
            i += 1

    file.close()
    return [x, y]


N = 512

working_path = Path(".")
build_path = working_path.joinpath("build-manual")
file_name = build_path.joinpath("signal_file.csv")

data = read_data_from_csv(file_name, N)

plt.figure(figsize=(10, 10))
plt.plot(data[0], data[1])
plt.xlabel("samples")
plt.ylabel("values")
plt.title("signal")

plt.show()
