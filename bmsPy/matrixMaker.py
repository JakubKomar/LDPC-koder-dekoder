import numpy as np
from pyldpc import make_ldpc, encode
import sys

H, G = make_ldpc(16, 7, 8,seed=42,systematic=False,sparse=False)
np.set_printoptions(threshold=sys.maxsize)
print(H)
print("")
print(G)