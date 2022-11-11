import random
 
def redPacket1(money=100, number=15):
    ls = []
    for n in range(number - 1):
        lucky_money = random.uniform(0.01, money / (number - n) * 2)
        lucky_money = round(lucky_money, 2)
        ls.append(lucky_money)
        money = money - lucky_money
    ls.append(round(money, 2))
    return ls
 
a = redPacket1()
print(a)
print(sum(a))
 

