#include <iostream>
#include <string>
#include <vector>

using namespace std;

string caesar(string text, int key, int mode) {
    string result = "";
 
    key = (key % 26 + 26) % 26;
    if (mode == -1) key = 26 - key; 

    for (char &c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            result += (char)((c - base + key) % 26 + base);
        } else {
            result += c; 
        }
    }
    return result;
}
bool isEnglish(string text) {
    // Chuyển về chữ thường để dễ tìm kiếm
    string lowerText = text;
    for (int i = 0; i < lowerText.length(); i++) {
        lowerText[i] = tolower(lowerText[i]);
    }

    string markers[] = {" the ", " and ", " of ", " to ", " is ", " it ", " with "};
    
    int count = 0;
    for (string s : markers) {
        if (lowerText.find(s) != string::npos) {
            count++;
        }
    }

    // Nếu tìm thấy từ 5 cụm từ trở lên thì xác nhận là tiếng Anh
    return count >= 5;
}
void bruteForce(string cipher) {
    cout << "\n--- KET QUA BRUTE-FORCE ---" << endl;
    for (int k = 0; k < 26; k++) {
        string decoded = caesar(cipher, k, -1);
        
        if (isEnglish(decoded)){
            cout << "[*] Tim thay khoa kha thi: " << k << endl;
            cout << "[*] Ban ro: " << decoded << endl;
            return;
        }
    }
    cout << "Khong tu dong tim thay ban ro phu hop." << endl;
}
int main() {
    int choice;
    cout << "--- CAESAR CIPHER TOOL ---" << endl;
    cout << "1. Ma hoa (Encrypt) / Giai ma (Decrypt)" << endl;
    cout << "2. Brute-force doan van ban" << endl;
    cout << "Chon chuc nang (1/2): ";
    cin >> choice;
    cin.ignore();   
    if (choice == 1) {
        string text;
        int key;
        cout << "Nhap van ban: ";
        getline(cin, text);
        cout << "Nhap khoa (0-25): "; cin >> key;
        cout << "Ma hoa (E) hay Giai ma (D)? ";
        char mode; cin >> mode;

        if (mode == 'E' || mode == 'e') 
            cout << "Ket qua: " << caesar(text, key, 1) << endl;
        else 
            cout << "Ket qua: " << caesar(text, key, -1) << endl;
    } 
    else if (choice == 2) {
        string cipher;
        cout << "Nhap ciphertext can giai: ";
        getline(cin, cipher);
        bruteForce(cipher);
    } else {
        cout << "Lua chon khong hop le!" << endl;
    }

    return 0;
}
