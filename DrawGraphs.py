import csv
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
import argparse

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

parser = argparse.ArgumentParser()
parser.add_argument('file_name', type=str)
args = parser.parse_args()
file_name = args.file_name

working_path = Path(".")
build_path = working_path.joinpath("build-manual")

file_names = [
    "signal_file.csv",
    "windowed_signal_file.csv",
    "hanning_file.csv",
    "bartlett_file.csv",
    "magnitude_file.csv"
]

titles = {
    "signal_file.csv": "signal",
    "windowed_signal_file.csv": "windowed signal",
    "hanning_file.csv": "hanning window",
    "bartlett_file.csv": "bartlett window",
    "magnitude_file.csv": "magnitude"
}

if not (file_name in file_names):
    exit("Invalid file name")

path = build_path.joinpath(file_name)
data = read_data_from_csv(path, N)
plt.figure(1, figsize=(10, 10))
plt.plot(data[0], data[1])
plt.xlabel("samples")
plt.ylabel("values")
plt.title(titles[file_name])


plt.show()
