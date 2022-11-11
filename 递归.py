def m(i):
    if i == 0:
          return 0

    else:
      return i / (2 * i + 1) + m(i - 1)
print(m(1))
print(m(2))
print(m(0))
