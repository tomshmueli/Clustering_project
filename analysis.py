import sys
from sklearn import metrics as sk
import numpy as np
import pandas as pd

from kmeans import kmean_flow
from symnmf import symnmf

def main():
    """
    Receive user arguments, perform clustering using `symnmf` and `kmeans`,
    compute silhouette scores, and print them.
    """
    argc = len(sys.argv)
    if argc != 3:
        return None

    k = int(sys.argv[1])
    filename = sys.argv[2]
    dp = pd.read_csv(filename, header=None).values.tolist()
    H = symnmf(dp, k)

    if dp is None or k is None or filename is None or H is None:
        print("An Error Has Occurred")
        return None

    symnmf_clusters = np.argmax(H, axis=1)
    kmeans_clusters = kmean_flow(k, filename)

    print(f"nmf: {sk.silhouette_score(dp, symnmf_clusters):.4f}")
    print(f"kmeans: {sk.silhouette_score(dp, kmeans_clusters):.4f}")


if __name__ == "__main__":
    main()
