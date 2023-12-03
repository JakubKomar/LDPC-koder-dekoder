import numpy as np
from pyldpc import make_ldpc, encode,decode,get_message,coding_matrix
import sys

def string_to_binary_row(input_string):
    binary_row = []
    for char in input_string:
        binary_representation = bin(ord(char))[2:].zfill(8)  # Převod na binární reprezentaci
        binary_row.extend([int(bit) for bit in binary_representation])
    return binary_row

inStr="a"
bit_matrix = string_to_binary_row(inStr)


n = len(bit_matrix)*2
d_c = n//2
d_v = d_c-1
snr = 20


#H, G = make_ldpc(n, d_v, d_c,seed=42)

original_matrix = [
    [1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0],
    [1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1],
    [0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0],
    [0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0],
    [0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0],
    [0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0],
    [0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1],
    [0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0],
    [0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0],
    [0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1],
    [0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0],
    [0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1]
]

# Převedení do formátu NumPy
H = np.array(original_matrix)
G=coding_matrix(H)

print("Parity check matrix:\n",H,sep="")
print("Coding matrix:\n",G,sep="")

#np.set_printoptions(threshold=sys.maxsize)

y = encode(G, bit_matrix, snr)

codedMassage= (y > 0).astype(int)

print("Coded massege:",end="")

for i in codedMassage:
    print(i, end="")
print()


d = decode(H, codedMassage, snr)
x = get_message(G, d)

ascii_retezec = ''.join([chr(int(''.join(map(str, x[i:i+8])), 2)) for i in range(0, len(x), 8)])
print("Decoded massege:",ascii_retezec)
