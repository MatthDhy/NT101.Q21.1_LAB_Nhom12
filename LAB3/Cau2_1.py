import math
import base64

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

def extended_gcd(a, b):
    if a == 0:
        return b, 0, 1
    d, x1, y1 = extended_gcd(b % a, a)
    x = y1 - (b // a) * x1
    y = x1
    return d, x, y

def mod_inverse(e, phi):
    d, x, y = extended_gcd(e, phi)
    if d != 1:
        raise ValueError("Số nghịch đảo không tồn tại")
    return x % phi

def generate_keys(p, q, e):
    n = p * q
    phi = (p - 1) * (q - 1)
    if gcd(e, phi) != 1:
        raise ValueError("e và phi(n) không nguyên tố cùng nhau")
    d = mod_inverse(e, phi)
    return (e, n), (d, n)

# 1. Thực thi tính toán cho khóa công khai PU và khóa riêng PR
p1, q1, e1 = 11, 17, 7
pu1, pr1 = generate_keys(p1, q1, e1)
print(f"Public Key (PU1): {pu1}")
print(f"Private Key (PR1): {pr1}\n")

p2, q2, e2 = 20079993872842322116151219, 676717145751736242170789, 17
pu2, pr2 = generate_keys(p2, q2, e2)
print(f"Public Key (PU2): {pu2}")
print(f"Private Key (PR2): {pr2}\n")

p3, q3, e3 = 0xF7E75FDC469067FFDC4E847C51F452DF, 0xE85CED54AF57E53E092113E62F436F4F, 0x0D88C3
pu3, pr3 = generate_keys(p3, q3, e3)
print(f"Public Key (PU3): ({hex(pu3[0])}, {hex(pu3[1])})")
print(f"Private Key (PR3): ({hex(pr3[0])}, {hex(pr3[1])})\n")

# 2. Thí nghiệm với M = 5
M = 5
e, n = pu1
d, n = pr1

# Confidentiality: Mã hóa bằng PU, Giải mã bằng PR
c_conf = pow(M, e, n)
m_conf = pow(c_conf, d, n)
print(f"\n[Confidentiality] Ciphertext: {c_conf}, Decrypted: {m_conf}")

# Authentication: Ký bằng PR, Xác minh bằng PU
c_auth = pow(M, d, n)
m_auth = pow(c_auth, e, n)
print(f"[Authentication] Signature: {c_auth}, Verified: {m_auth}")

# 3. Mã hóa chuỗi văn bản và xuất Base64
def encrypt_string(text, public_key):
    e, n = public_key
    # Chuyển chuỗi thành số nguyên (byte-wise)
    encrypted_blocks = [pow(ord(char), e, n) for char in text]
    # Chuyển danh sách số thành chuỗi byte rồi encode Base64
    bytes_data = b"".join([block.to_bytes((n.bit_length() + 7) // 8, 'big') for block in encrypted_blocks])
    return base64.b64encode(bytes_data).decode()

message = "The University of Information Technology"
for pu in [pu1, pu2, pu3]:
    b64_cipher = encrypt_string(message, pu)
    print(f"\nMessage: {message}")
    print(f"Base64 Ciphertext: {b64_cipher}")


