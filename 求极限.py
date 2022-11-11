import math
import sympy

x = sympy.Symbol('x')

f = (sympy.cos(sympy.sqrt(x+1))-sympy.cos(sympy.sqrt(x)))

l1 = sympy.limit(f,x,sympy.oo)

print(l1)
