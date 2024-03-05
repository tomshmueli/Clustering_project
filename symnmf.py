import sys
import numpy as np
import pandas as pd
import symnmfAPI

np.random.seed(0)


def symnmf(dp, k):
    """
    Compute the Laplacian W matrix using `symnmfAPI.norm` and return updated H as list.
    """
    normalized_similarity_W = symnmfAPI.norm(dp)
    if normalized_similarity_W is None:
        print("An Error Has Occurred")
        return None

    n = len(dp)
    mean_W = np.mean(np.array(normalized_similarity_W))
    H = np.random.uniform(0, 2 * ((mean_W / k) ** 0.5), size=(n, k))

    return symnmfAPI.symnmf(normalized_similarity_W, H.tolist())


def main():
    """
    Main logic: parse user input, call the correct method, and print the result.
    """
    argc = len(sys.argv)
    if argc != 4:
        print("An Error Has Occurred")
        return
    k = int(sys.argv[1])
    goal = sys.argv[2]
    filename = sys.argv[3]
    if k is None or goal is None or filename is None:
        print("An Error Has Occurred")
        return None
    dp = pd.read_csv(filename, header=None).values.tolist()

    if dp is None or goal is None or k is None:
        print("An Error Has Occurred")
        return

    result_matrix = None

    if goal == "sym":
        result_matrix = symnmfAPI.sym(dp)
    if goal == "ddg":
        result_matrix = symnmfAPI.ddg(dp)
    if goal == "norm":
        result_matrix = symnmfAPI.norm(dp)
    if goal == "symnmf":
        result_matrix = symnmf(dp, k)

    if result_matrix is None:
        print("An Error Has Occurred")
        return

    # print result_matrix
    for row in result_matrix:
        print(",".join(f"{coord:.4f}" for coord in row))


if __name__ == "__main__":
    main()
