import numpy as np

class KMeans():

    def __init__(self, k=3):
        self.k = k

    def _distance(self, x, y):
        return np.sqrt(np.sum((x -  y) ** 2))

    def fit(self, X):
        # 随机选取K个中心
        center = np.zeros((self.k, X.shape[1]))
        for i in range(self.k):
            index = np.random.randint(X.shape[0])
            center[i] = X[index]
        clusterChanged = True
        cluster = np.zeros((X.shape[0]))
        while clusterChanged:
            clusterChanged = False
            # 聚类
            for i in range(X.shape[0]):
                min_dist = 10000000
                min_index = 0
                for j in range(self.k):
                    dist = self._distance(X[i], center[j])
                    if dist < min_dist:
                        min_dist = dist
                        min_index = j
                if min_index != cluster[i]:
                    cluster[i] = min_index
                    clusterChanged = True
            # 更新中心
            for j in range(self.k):
                center[j] = np.mean(X[cluster == j], axis=0)
        return center, cluster

        

