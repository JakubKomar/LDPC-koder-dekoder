import numpy as np

from pyldpc import make_ldpc, encode, decode, get_message


inStr =  input()

n = len(inStr)*2
d_v = len(inStr)-1
d_c = len(inStr)
snr = 20
H, G = make_ldpc(n, d_v, d_c, systematic=True, sparse=True)
k = G.shape[1]
v = np.random.randint(2, size=k)
y = encode(G, v, snr)
d = decode(H, y, snr)
x = get_message(G, d)
assert abs(x - v).sum() == 0

print(x)