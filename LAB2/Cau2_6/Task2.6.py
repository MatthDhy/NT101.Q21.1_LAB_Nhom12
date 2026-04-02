import random

"""Tính (a^x) mod p bằng thuật toán bình phương và nhân"""
def power_mod(a, x, p):
    return pow(a, x, p) 

"""Kiểm tra số nguyên tố lớn bằng Miller-Rabin"""
def is_prime_miller_rabin(n, k=10):
    if n < 2: return False
    if n == 2 or n == 3: return True
    if n % 2 == 0: return False
    d = n - 1
    s = 0
    while d % 2 == 0:
        d //= 2
        s += 1
    for _ in range(k):
        a = random.randint(2, n - 2)
        x = pow(a, d, n)
        if x == 1 or x == n - 1: continue
        for _ in range(s - 1):
            x = pow(x, 2, n)
            if x == n - 1: break
        else: return False
    return True

"""Tìm GCD bằng thuật toán Euclid"""
def gcd_euclid(a, b):
    while b:
        a, b = b, a % b
    return a


def main():
    print("=== Mersenne Prime - Số nguyên tố lớn ===")
    
    # 1. Kiểm tra số nguyên tố
    print("\n[1] KIỂM TRA SỐ NGUYÊN TỐ")
    try:
        n_check = int(input("Nhập một số nguyên: "))
        if is_prime_miller_rabin(n_check):
            print(f"-> {n_check} lÀ số nguyên tố.")
        else:
            print(f"-> {n_check} không là số nguyên tố.")
    except ValueError:
        print("Không hợp lệ, nhập lại")

    # 2. Tìm GCD
    print("\n[2] GCD")
    try:
        num1 = int(input("Nhập số thứ nhất: "))
        num2 = int(input("Nhập số thứ hai: "))
        print(f"-> GCD({num1}, {num2}) = {gcd_euclid(num1, num2)}")
    except ValueError:
        print("Hãy nhập số nguyên: ")

    # 3. Tính Lũy thừa Module
    print("\n[3] TÍNH LŨY THỪA MODULE (a^x mod p)")
    try:
        base = int(input("Nhập cơ số (a): "))
        exp = int(input("Nhập số mũ (x): "))
        mod = int(input("Nhập số chia (p): "))
        print(f"-> {base}^{exp} mod {mod} = {power_mod(base, exp, mod)}")
    except ValueError:
        print("Hãy nhập các số nguyên!")
    except ZeroDivisionError:
        print("Số chia phải khác 0")

if __name__ == "__main__":
    main()






