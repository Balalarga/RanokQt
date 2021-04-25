from math import sqrt

def w1(x, z):
    return 4-x*x-z
    
def w2(y, z):
    return 4-y*y-z

def w3(a, b):
    return a + b + sqrt( a*a + b*b )

def w6(z):
    return 0.00001 - z*z

def w(a, b):
    return a + b - sqrt( a*a + b*b )

def getValue(point):
    return w(w3(w2(point[1], point[2]),w1(point[0], point[2])), w6(point[2]))



points = [
    (5,   5  , 5),
    (5,   2.5, 5),
    (5,   5,   2.5),
    (5,   2.5, 2.5),
    (2.5, 5,   5),
    (0  , 5,   5),
]

for i in points:
    print("{} = {}".format(i, getValue(i)))