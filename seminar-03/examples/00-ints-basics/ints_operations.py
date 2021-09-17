from ints_format import explain
from ints_format import normalize


def explain_add(a, b):
    print("%d + %d = %d" % (a, b, normalize(a + b)))
    print("    (%s) + (%s) = (%s)" % (explain(a), explain(b), explain(normalize(a + b))))


def explain_mult(a, b):
    print("%d * %d = %d" % (a, b, normalize(a * b)))
    print("    (%s) * (%s) = (%s)" % (explain(a), explain(b), explain(normalize((a * b)))))



if __name__ == '__main__':
    explain_add(2, 1)
    explain_add(2, -1)
    explain_add(7, 7)
    # explain_mult(2, 3)
    # explain_mult(-2, -3)
    # explain_mult(-1, -1)
