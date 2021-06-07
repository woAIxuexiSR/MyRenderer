import math
import matplotlib.pyplot as plt
import numpy as np
from sklearn.mixture import GaussianMixture as GMM
from scipy.stats import multivariate_normal

def gaussian(mu, theta, x):
    return 1.0 / math.sqrt(2 * math.pi) / theta * math.exp(-(x - mu) * (x - mu) / 2 / theta / theta)

def trainModel(data):
    num_iter = 100

    n, d = data.shape

    mu1 = data.min(axis=0)      # 2
    mu2 = data.max(axis=0)      # 2
    sigma1 = np.identity(d)     # 2x2
    sigma2 = np.identity(d)     # 2x2
    pi1 = 0.5
    pi2 = 0.5

    for _ in range(num_iter):
        # E step
        norm1 = multivariate_normal(mu1, sigma1)
        norm2 = multivariate_normal(mu2, sigma2)

        tau1 = pi1 * norm1.pdf(data)    # n
        tau2 = pi2 * norm2.pdf(data)    # n

        gamma1 = tau1 / (tau1 + tau2)   # n
        gamma2 = tau2 / (tau1 + tau2)   # n
        
        # M step
        mu1 = np.dot(gamma1, data) / np.sum(gamma1)
        mu2 = np.dot(gamma2, data) / np.sum(gamma2)

        sigma1 = np.dot(gamma1 * (data - mu1).T, data - mu1) / np.sum(gamma1)
        sigma2 = np.dot(gamma2 * (data - mu2).T, data - mu2) / np.sum(gamma2)

        pi1 = np.sum(gamma1) / n
        pi2 = np.sum(gamma2) / n

    return mu1, mu2, sigma1, sigma2, pi1, pi2




model = GMM(n_components=2, max_iter=100)

x1 = np.random.multivariate_normal(np.array([0, 1]), np.array([[1.3, 0.7], [0.7, 1.5]]), 1000)
x2 = np.random.multivariate_normal(np.array([-1, 0]), np.array([[1.3, -0.7], [-0.7, 1.5]]), 1000)

x = np.concatenate([x1, x2])

x = np.array([[1.1, 2], [1.2, 1.9], [1, 3.1], [1.1, 3.3]])
model.fit(x)
print(model.means_)
print(model.covariances_)

mu1, mu2, sigma1, sigma2, pi1, pi2 = trainModel(x)
print(mu1)
print(mu2)
print(sigma1)
print(sigma2)
print(pi1)
print(pi2)


# x = np.random.multivariate_normal(np.array([0, 1]), np.array([[0.3, 0.7], [0.7, 0.5]]), 1000)

# plt.scatter(x[:, 0], x[:, 1])
# plt.show()