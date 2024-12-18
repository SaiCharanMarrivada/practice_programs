from z3 import *

address1 = BitVec("address1", 32)
address2 = BitVec("address2", 32)
mask = BitVec("mask", 32)

e1 = (address1 & mask) == (address2 & mask)
e2 = (address1 ^ address2) & mask == 0
solve(e1 != e2)
