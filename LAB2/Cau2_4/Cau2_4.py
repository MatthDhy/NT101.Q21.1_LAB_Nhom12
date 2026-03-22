from Crypto.Cipher import AES
import os

key = os.urandom(16)
iv  = os.urandom(16)

# Tạo 1000 byte dữ liệu ngẫu nhiên, làm tròn lên bội số 16
plaintext = os.urandom(1000)
padded = plaintext + b'\x00' * (16 - len(plaintext) % 16)  # 1008 bytes

def flip_bit(data: bytes, byte_idx: int) -> bytes:
    ba = bytearray(data)
    ba[byte_idx] ^= 0x01
    return bytes(ba)

def test_mode(mode_name, mode_const, key, iv, plaintext):
    # Mã hóa
    if mode_name == "ECB":
        cipher = AES.new(key, mode_const)
    else:
        cipher = AES.new(key, mode_const, iv=iv)
    ciphertext = cipher.encrypt(plaintext)

    # Làm hỏng byte thứ 26 (index 25)
    corrupted = flip_bit(ciphertext, 25)

    # Giải mã bản mã bị hỏng
    if mode_name == "ECB":
        cipher2 = AES.new(key, mode_const)
    else:
        cipher2 = AES.new(key, mode_const, iv=iv)
    decrypted = cipher2.decrypt(corrupted)

    # So sánh từng block 16 bytes
    blocks_damaged = []
    for i in range(0, len(plaintext), 16):
        if plaintext[i:i+16] != decrypted[i:i+16]:
            blocks_damaged.append(i // 16 + 1)

    print(f"{mode_name}: block bị hỏng = {blocks_damaged}")

test_mode("ECB", AES.MODE_ECB, key, iv, padded)
test_mode("CBC", AES.MODE_CBC, key, iv, padded)
test_mode("CFB", AES.MODE_CFB, key, iv, padded)
test_mode("OFB", AES.MODE_OFB, key, iv, padded)