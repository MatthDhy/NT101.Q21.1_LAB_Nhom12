#include <bits/stdc++.h>
using namespace std;
const string ENGLISH_FREQ = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
struct CharFreq {
    char character;
    int count;
    double percentage;
};

bool compareFreq(CharFreq a, CharFreq b) {
    return a.count > b.count;
}

class MonoAlphaSolver {
private:
    string ciphertext;
    vector<CharFreq> freqTable;
    map<char, char> keyMap;
public:
    void napBanMa(string input, bool isFile = false) {
        if (isFile) {
            ifstream file(input);
            string line;
            ciphertext = "";
            while (getline(file, line)) ciphertext += line + "\n";
            file.close();
        } else {
            ciphertext = input;
        }
    }
    string layVanBanGoc() {
        return ciphertext;
    }
    void phanTichTanSuat() {
        map<char, int> counts;
        int totalAlpha = 0;
        for (char c : ciphertext) {
            if (isalpha((unsigned char)c)) {
                counts[toupper((unsigned char)c)]++;
                totalAlpha++;
            }
        }
        freqTable.clear();
        for (auto const& [ch, count] : counts) {
            freqTable.push_back({ch, count, (double)count / totalAlpha * 100});
        }
        sort(freqTable.begin(), freqTable.end(), compareFreq);
    }
    void taoKhoaBanDau() {
        for (size_t i = 0; i < freqTable.size() && i < ENGLISH_FREQ.length(); ++i) {
            keyMap[freqTable[i].character] = ENGLISH_FREQ[i];
        }
    }
    void inBangAnhXa() {
        cout << "\n--- Bang anh xa hien tai (Cipher -> Plain) ---\n";
        int count = 0;
        for (auto const& [cipher, plain] : keyMap) {
            cout << cipher << " -> " << plain << "   ";
            if (++count % 6 == 0) cout << endl;
        }
        cout << "\n----------------------------------------------\n";
    }
    void inBangTanSuat() {
        cout << "\n--- Thong ke tan suat trong Ciphertext ---\n";
        cout << "Char | Count | Percentage\n";
        for (auto f : freqTable) {
            cout << f.character << "    | " << setw(5) << f.count 
                 << " | " << fixed << setprecision(2) << f.percentage << "%\n";
        }
    }
    string giaiMa() {
        string plaintext = ciphertext;
        for (size_t i = 0; i < plaintext.length(); ++i) {
            if (isalpha((unsigned char)plaintext[i])) { 
                char original = toupper((unsigned char)plaintext[i]);
                if (keyMap.count(original)) {
                    char decrypted = keyMap[original];
                    if (islower((unsigned char)plaintext[i])) plaintext[i] = tolower(decrypted);
                    else plaintext[i] = decrypted;
                }
            }
        }
        return plaintext;
    }
    void dieuChinhThuCong(char cipherChar, char plainChar) {
        keyMap[toupper((unsigned char)cipherChar)] = toupper((unsigned char)plainChar);
    }
};

int main() {
    MonoAlphaSolver solver;
    int choice;
    string path;
    cout << "=== CHUONG TRINH GIAI MA MONO-ALPHABETIC ===\n";
    cout << "1. Nhap tu file\n2. Nhap truc tiep\nChoose: ";
    cin >> choice;
    cin.ignore();
    if (choice == 1) {
        cout << "Nhap duong dan file: ";
        getline(cin, path);
        solver.napBanMa(path, true);
    } else {
        cout << "Nhap ciphertext: ";
        string ct;
        getline(cin, ct);
        solver.napBanMa(ct);
    }
    solver.phanTichTanSuat();
    solver.taoKhoaBanDau();
    solver.inBangTanSuat();
    solver.inBangAnhXa();
    cout << endl;
    cout << endl;
    cout << "-----------------------Van Ban Goc------------------" << endl;
    cout << solver.layVanBanGoc() << endl;
    cout << endl;
    cout << "\n--- Ban ro du kien (Dua tren tan suat don le) ---\n";
    cout << solver.giaiMa() << endl;
    char c, p;
    while (true) {
        cout << "\nNhap cap ky tu de tinh chinh (vi du: 'v i'). Nhap '0 0' de thoat: ";
        if (!(cin >> c >> p) || c == '0') break;

        solver.dieuChinhThuCong(c, p);
        solver.inBangAnhXa();
        cout << endl;

        cout << "\n========= VAN BAN GOC (CIPHERTEXT) =========" << endl;
        cout << solver.layVanBanGoc() << endl;
        cout << endl;
        cout << "--------- VAN BAN SAU TINH CHINH -----------" << endl;
        cout << solver.giaiMa() << endl;
        cout << "============================================" << endl;
    }
    return 0;
}



