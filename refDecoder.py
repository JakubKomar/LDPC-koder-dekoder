import numpy as np

from pyldpc import make_ldpc, decode, get_message

inStr =  input()

codedMassage=np.array([int(bit) for bit in inStr])

inputConverzion= 2 * codedMassage - 1
n = len(inputConverzion)
d_c = n//2
d_v = d_c-1
snr = 20

H, G = make_ldpc(n, d_v, d_c,seed=42)

d = decode(H, inputConverzion, snr)
x = get_message(G, d)

ascii_retezec = ''.join([chr(int(''.join(map(str, x[i:i+8])), 2)) for i in range(0, len(x), 8)])

# Vytiskněte řetězec
print(ascii_retezec)