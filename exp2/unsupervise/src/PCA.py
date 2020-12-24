import numpy as np


class PCA:
    
    def __init__(self, threshold=0.99):
        self.threshold = threshold
    
    def fit(self, X):
        # X 为(m, n)维矩阵
        # X转置成(n, m)维
        X_T = X.T
        # 将每一行进行零均值化，即减去每一行的均值
        mean_X = X_T - np.mean(X_T, axis=1, keepdims=True)
        # 计算协方差
        cov_X = np.dot(mean_X, mean_X.T)
        # 计算特征值、特征向量
        eig_val, eig_vec = np.linalg.eig(cov_X)
        # 按照特征值从大到小排序
        val_index = np.argsort(eig_val)[::-1]
        # 根据threshold选择特征向量
        total_val = np.sum(eig_val)
        temp = 0
        for i in range(len(eig_vec)):
            if temp / total_val >= self.threshold:
                break
            temp += eig_val[val_index[i]]
        vec_m = eig_vec[val_index[:i]]
        vec_m = vec_m.T
        # 得到降维后矩阵
        return np.dot(X, vec_m)
