import numpy as np
from pyldpc import make_ldpc, encode
import sys
def string_to_binary_row(input_string):
    binary_row = []
    for char in input_string:
        binary_representation = bin(ord(char))[2:].zfill(8)  # Převod na binární reprezentaci
        binary_row.extend([int(bit) for bit in binary_representation])
    return binary_row

inStr =  input()


bit_matrix = string_to_binary_row(inStr)
print(bit_matrix)
inStrLen= len(bit_matrix)

n = inStrLen*2 
d_v = inStrLen-1
d_c = inStrLen
snr = 20

H, G = make_ldpc(n, d_v, d_c,seed=42)
np.set_printoptions(threshold=sys.maxsize)
print("H\n",H)
print("G\n",G)
y = encode(G, bit_matrix, snr)

codedMassage= (y > 0).astype(int)

for i in codedMassage:
    print(i, end="")
print()
