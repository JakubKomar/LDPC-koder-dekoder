import numpy as np
from pyldpc import make_ldpc, encode,coding_matrix,decode,get_message
import sys

def string_to_binary_row(input_string):
    binary_row = []
    for char in input_string:
        binary_representation = bin(ord(char))[2:].zfill(8)  # Převod na binární reprezentaci
        binary_row.extend([int(bit) for bit in binary_representation])
    return binary_row

inStr =  "a"


bit_matrix = string_to_binary_row(inStr)
print(bit_matrix)
inStrLen= len(bit_matrix)

n = inStrLen*2 
d_v = inStrLen-1
d_c = inStrLen
snr = 20

H, G = make_ldpc(n, d_v, d_c,seed=584,systematic=False)

H=[[1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
 [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1],
 [0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0],
 [1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1],
 [1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0],
 [0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1],
 [1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0],
 [0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1],
 [1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0],
 [0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1],
 [1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0],
 [0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1],
 [1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0],
 [0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1]]
H=np.array(H)

G = coding_matrix(H, sparse=False)
np.set_printoptions(threshold=sys.maxsize)
print("H\n",H)
print("G\n",G)

y = encode(G, bit_matrix, snr)

d = decode(H, y, snr)
x = get_message(G, d)
print("Decoded message:",x)