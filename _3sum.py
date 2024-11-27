def _3sum(a):
    """no of triplets in `a` which sum to 0"""
    a.sort()
    count = 0
    n = len(a)
    for i in range(n):
        stop = n - 1
        start = i + 1
        while start < stop:
            if a[start] + a[stop] < -a[i]:
                start += 1
            elif a[start] + a[stop] > -a[i]:
                stop -= 1
            else:
                count += 1
                break
    return count


a = [1, 2, -3, 5, 6, 7, -13, -11]
print(_3sum(a))
