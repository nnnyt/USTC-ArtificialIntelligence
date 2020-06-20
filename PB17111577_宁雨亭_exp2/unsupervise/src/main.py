import numpy as np
import csv
import matplotlib.pyplot as plt
from PCA import PCA
from KMeans import KMeans


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


def evaluate(cluster, y):
    # 兰德系数
    a = b = c = d = 0
    for i in range(len(y)):
        for j in range(len(y)):
            if i == j:
                continue
            if y[i] == y[j]:
                if cluster[i] == cluster[j]:
                    a += 1
                else:
                    b += 1
            else:
                if cluster[i] == cluster[j]:
                    c += 1
                else:
                    d += 1
    r = (a + d) / (a + b + c + d)
    return r


if __name__ == '__main__': 
    dataset_dir = '../input/wine.data'
    x, y = load_data(dataset_dir)
    model = PCA()
    pca = model.fit(x)
    print('dim: ', pca.shape[1])
    kmodel = KMeans()
    cluster, s = kmodel.fit(pca)
    print('r: ', evaluate(cluster, y))
    

