import numpy as np

a1=[1]
a = np.linspace(1,1,100)
         

for i in range (1,101):
       for t in a:
         a1.append(t)
         a2 = np.diag(a1)
         print(a2)
