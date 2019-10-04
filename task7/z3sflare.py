#sudo pip install z3-solver

from z3 import *

x0 = BitVec('x0', 32)
x1 = BitVec('x1', 32)
x2 = BitVec('x2', 32)
x3 = BitVec('x3', 32)
x4 = BitVec('x4', 32)

x5 = BitVec('x5', 32)
x6 = BitVec('x6', 32)
x7 = BitVec('x7', 32)
x8 = BitVec('x8', 32)
x9 = BitVec('x9', 32)

x10 = BitVec('x10', 32)
x11 = BitVec('x11', 32)
x12 = BitVec('x12', 32)
x13 = BitVec('x13', 32)
x14 = BitVec('x14', 32)

x15 = BitVec('x15', 32)

s = Solver()
s.add(x2 ^ x3 ^ x4 ^ x8 ^ x11 ^ x14 == 115)
s.add(x0 ^ x1 ^ x8 ^ x11 ^ x13 ^ x14 == 29)
s.add(x0 ^ x1 ^ x2 ^ x4 ^ x5 ^ x8 ^ x9 ^ x10 ^ x13 ^ x14 ^ x15 == 32)
s.add(x5 ^ x6 ^ x8 ^ x9 ^ x10 ^ x12 ^ x15 == 68)
s.add(x1 ^ x6 ^ x7 ^ x8 ^ x12 ^ x13 ^ x14 ^ x15 == 106)
s.add(x0 ^ x4 ^ x7 ^ x8 ^ x9 ^ x10 ^ x12 ^ x13 ^ x14 ^ x15 == 108)
s.add(x1 ^ x3 ^ x7 ^ x9 ^ x10 ^ x11 ^ x12 ^ x13 ^ x15 == 89)
s.add(x0 ^ x1 ^ x2 ^ x3 ^ x4 ^ x8 ^ x10 ^ x11 ^ x14 == 76)
s.add(x1 ^ x2 ^ x3 ^ x5 ^ x9 ^ x10 ^ x11 ^ x12 == 21)
s.add(x6 ^ x7 ^ x8 ^ x10 ^ x11 ^ x12 ^ x15 == 71)
s.add(x0 ^ x3 ^ x4 ^ x7 ^ x8 ^ x10 ^ x11 ^ x12 ^ x13 ^ x14 ^ x15 == 78)
s.add(x0 ^ x2 ^ x4 ^ x6 ^ x13 == 51)
s.add(x0 ^ x3 ^ x6 ^ x7 ^ x10 ^ x12 ^ x15 == 75)
s.add(x2 ^ x3 ^ x4 ^ x5 ^ x6 ^ x7 ^ x11 ^ x12 ^ x13 ^ x14 == 1)
s.add(x1 ^ x2 ^ x3 ^ x5 ^ x7 ^ x11 ^ x13 ^ x14 ^ x15 == 55)
s.add(x1 ^ x3 ^ x5 ^ x9 ^ x10 ^ x11 ^ x13 ^ x15 == 102)

if s.check():
	print(s.model())
	