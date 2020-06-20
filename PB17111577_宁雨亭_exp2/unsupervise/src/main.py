import numpy as np
import csv
import matplotlib.pyplot as plt
from PCA import PCA


def load_data(filename):
    data = [[] for _ in range(13)]
    with open(filename, 'r') as f:
        lines = csv.reader(f, delimiter=',')
        for line in lines:
            for i in range(13):
                data[i].append(float(line[i]))
    data  = np.array(data).T
    np.random.shuffle(data)
    labels = data[:,0]
    x = data[:, 1:]
    # print(labels.shape)
    # print(x.shape)
    return x, labels

if __name__ == '__main__': 
    dataset_dir = '../input/wine.data'
    x, y = load_data(dataset_dir)
    model = PCA()
    pca = model.fit(x)
    print(pca.shape)
