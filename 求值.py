import math
from math import pi
from math import e
for k in range(1,101):
               f= lambda k : (1+math.log(k,e))/2*pi
               s=0
               s=s+f(k)
m=s+e**3
print(m)
