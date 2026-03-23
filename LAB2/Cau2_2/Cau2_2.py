from Crypto.Cipher import AES
key = b'1234567890123456'
iv=b'abcdefghijklmnop'
plaintext = b"UIT_LAB_UIT_LAB_UIT_LAB_UIT_LAB_" # 32 bytes (2 blocks)

# 1. AES-ECB
cipher_ecb = AES.new(key, AES.MODE_ECB)
ct_ecb = cipher_ecb.encrypt(plaintext)

# 2. AES-CBC
cipher_cbc = AES.new(key, AES.MODE_CBC, iv)
ct_cbc = cipher_cbc.encrypt(plaintext)

# 3. AES-CFB
cipher_cfb = AES.new(key, AES.MODE_CFB, iv,segment_size=128)
ct_cfb = cipher_cfb.encrypt(plaintext)

# 4. AES-OFB
cipher_ofb = AES.new(key,AES.MODE_OFB, iv)
ct_ofb = cipher_ofb.encrypt(plaintext)

# 5. AES-CTR
from Crypto.Util import Counter
cipher_ctr = AES.new(key, AES.MODE_CTR, counter=Counter.new(128))
ct_ctr = cipher_ctr.encrypt(plaintext)


print(f"ECB: {ct_ecb.hex()}")
print(f"CBC: {ct_cbc.hex()}")
print(f"CFB: {ct_cfb.hex()}")
print(f"OFB: {ct_ofb.hex()}")
print(f"CTR: {ct_ctr.hex()}")