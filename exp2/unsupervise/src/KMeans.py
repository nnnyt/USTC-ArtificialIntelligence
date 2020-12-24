import numpy as np


class KMeans():

    def __init__(self, k=3):
        self.k = k

    def _distance(self, x, y):
        return np.sqrt(np.sum((x -  y) ** 2))
    
    def _evaluate(self, x, center, cluster, k):
        # 找到每个簇最近的簇
        # 计算轮廓系数
        s = []
        for i in range(len(cluster)):
            dist = [[] for _ in range(k)]
            for j in range(len(cluster)):
                if j == i:
                    continue
                dist[cluster[j]].append(self._distance(x[i], x[j]))
            a_i = np.mean(dist[cluster[i]])
            b_i = min([np.mean(dist[j]) for j in range(k) if j != cluster[i]])
            s.append((b_i - a_i) / max(b_i, a_i))
        s = np.mean(np.array(s))
        print('s: ', s)
        return s

    def fit(self, X):
        # 随机选取K个中心
        center = np.zeros((self.k, X.shape[1]))
        for i in range(self.k):
            index = np.random.randint(X.shape[0])
            center[i] = X[index]
        clusterChanged = True
        cluster = np.zeros((X.shape[0]), dtype='int32')
        # 迭代直到中心收敛
        while clusterChanged:
            clusterChanged = False
            # 聚类
            for i in range(X.shape[0]):
                # 对每一个样本计算距离最近的簇
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
        # 计算轮廓系数
        s = self._evaluate(X, center, cluster, self.k)
        return cluster, s

        

