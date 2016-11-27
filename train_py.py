TARGET = 10
EPSILON = 0.000001

class Binary(object):
    def works(self, a, b):
        raise NotImplementedError()
    def doIt(self, a, b):
        raise NotImplementedError()

class Plus(Binary):
    def works(self, a, b):
        return True
    def doIt(self, a, b):
        return a + b

class Minus(Binary):
    def works(self, a, b):
        return True
    def doIt(self, a, b):
        return a - b

class Times(Binary):
    def works(self, a, b):
        return True
    def doIt(self, a, b):
        return a * b

class Divide(Binary):
    def works(self, a, b):
        return abs(b) > EPSILON
    def doIt(self, a, b):
        return a / b

class Power(Binary):
    def works(self, a, b):
        return True
    def doIt(self, a, b):
        return a ** b

class Reverse(Binary):
    def __init__(self, other):
        self.other = other
    def works(self, a, b):
        return self.other.works(b, a)
    def doIt(self, a, b):
        return self.other.doIt(b, a)

def get_binaries(exp):
    binaries = [Plus(), Minus(), Reverse(Minus()), Times(), Divide(), Reverse(Divide())]
    if exp:
        binaries.append(Power())
        binaries.append(Reverse(Power()))
    return binaries

def _check(dem, binaries):
    if len(dem) == 1:
        return dem[0] == TARGET
    for iidx, i in enumerate(dem):
        for jidx, j in enumerate(dem[iidx+1:]):
            for o in binaries:
                if not o.works(i, j):
                    continue
                dem[iidx] = dem[0]
                dem[iidx + jidx + 1] = o.doIt(i, j)
                if _check(dem[1:], binaries):
                    return True
                dem[iidx] = i
                dem[iidx + jidx + 1] = j
    return False

def go(st, exp):
    return _check(list(map(int, st)), get_binaries(exp))
