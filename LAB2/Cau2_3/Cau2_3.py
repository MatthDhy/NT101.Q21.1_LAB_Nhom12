from Crypto.Cipher import DES

def bits(data: bytes) -> str:
    return bin(int.from_bytes(data, 'big'))[2:].zfill(len(data) * 8)

def hamming(b1: bytes, b2: bytes) -> int:
    return sum(c1 != c2 for c1, c2 in zip(bits(b1), bits(b2)))

def avalanche_test():
    key = b'87654321'
    p1 = b'STAYHOME'
    p2 = b'STAYHOMA'

    c1 = DES.new(key, DES.MODE_ECB).encrypt(p1)
    c2 = DES.new(key, DES.MODE_ECB).encrypt(p2)

    diff = hamming(c1, c2)
    print(f"Key: {key.decode()}")
    print(f"  CipherText1: {c1.hex()}")
    print(f"  CipherText2: {c2.hex()}")
    print(f"  Bits khác nhau: {diff}/64 ({diff/64*100:.1f}%)\n")

avalanche_test()