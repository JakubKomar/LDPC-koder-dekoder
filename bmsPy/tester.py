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

np.set_printoptions(threshold=sys.maxsize)


bit_matrix = string_to_binary_row(inStr)
print(bit_matrix)
inStrLen= len(bit_matrix)

n = inStrLen*2 
d_v = inStrLen-1
d_c = inStrLen
snr = 20

H=np.array(
[[1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
[ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1],
[ 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0],
[ 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1],
[ 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0],
[ 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1],
[ 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0],
[ 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1],
[ 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0],
[ 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1],
[ 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0],
[ 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1],
[ 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0],
[ 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1]]
)

G = coding_matrix(H, sparse=False)

y = encode(G, bit_matrix, snr)
codedMassage= (y > 0).astype(int)

inStr ="1111001100011110"
codedMassage=np.array([int(bit) for bit in inStr])
print("Coded message:",codedMassage)

codedMassage = 2 * codedMassage - 1
print("Coded messageRaw:",codedMassage)

d = decode(H, codedMassage, snr)
print("Decoded message:",d)
x = get_message(G, d)

ascii_retezec = ''.join([chr(int(''.join(map(str, x[i:i+8])), 2)) for i in range(0, len(x), 8)])
print("Decoded massege:",ascii_retezec)
