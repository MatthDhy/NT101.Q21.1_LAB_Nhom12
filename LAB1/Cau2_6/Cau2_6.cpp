/*
 * Task 2.6 - Phá mã Vigenère Cipher (C++)
 * =========================================
 * Phương pháp:
 *   1. Chuẩn hóa ciphertext
 *   2. Kasiski Examination  → gợi ý độ dài khóa
 *   3. Index of Coincidence → xác nhận độ dài khóa tốt nhất
 *   4. Frequency Analysis (Chi-squared) → tìm từng ký tự khóa
 *   5. Giải mã và hiển thị kết quả
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <climits>
#include <set>

using namespace std;

// ──────────────────────────────────────────────────
// Tần suất chuẩn tiếng Anh (%)
// ──────────────────────────────────────────────────
const double ENGLISH_FREQ[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094,
    6.966, 0.153, 0.772, 4.025, 2.406,  6.749, 7.507, 1.929,
    0.095, 5.987, 6.327, 9.056, 2.758,  0.978, 2.360, 0.150,
    1.974, 0.074
};

// ──────────────────────────────────────────────────
// Bước 1: Chuẩn hóa - chỉ giữ chữ thường
// ──────────────────────────────────────────────────
string normalize(const string& text) {
    string result;
    for (char c : text) {
        if (isalpha(c))
            result += tolower(c);
    }
    return result;
}

// ──────────────────────────────────────────────────
// Bước 2: Kasiski Examination
// Tìm trigram lặp lại → thống kê thừa số khoảng cách
// ──────────────────────────────────────────────────
vector<int> kasiskiExamination(const string& ctext, int ngramSize = 3) {
    map<string, vector<int>> positions;

    for (int i = 0; i <= (int)ctext.size() - ngramSize; ++i) {
        string ngram = ctext.substr(i, ngramSize);
        positions[ngram].push_back(i);
    }

    // Tính khoảng cách giữa các lần lặp
    map<int, int> factorCount;
    for (auto& [ngram, pos] : positions) {
        if (pos.size() >= 2) {
            for (int j = 1; j < (int)pos.size(); ++j) {
                int dist = pos[j] - pos[j - 1];
                for (int f = 2; f <= min(20, dist); ++f) {
                    if (dist % f == 0)
                        factorCount[f]++;
                }
            }
        }
    }

    // Sắp xếp thừa số theo tần suất giảm dần
    vector<pair<int,int>> sorted(factorCount.begin(), factorCount.end());
    sort(sorted.begin(), sorted.end(),
         [](auto& a, auto& b){ return a.second > b.second; });

    vector<int> result;
    for (auto& [f, cnt] : sorted)
        result.push_back(f);

    if (result.empty()) {
        for (int i = 2; i <= 12; ++i) result.push_back(i);
    }
    return result;
}

// ──────────────────────────────────────────────────
// Bước 3: Index of Coincidence
// Tiếng Anh ≈ 0.065, ngẫu nhiên ≈ 0.038
// ──────────────────────────────────────────────────
double indexOfCoincidence(const string& text) {
    if (text.size() < 2) return 0.0;
    int counts[26] = {};
    for (char c : text) counts[c - 'a']++;
    int n = text.size();
    double num = 0;
    for (int i = 0; i < 26; ++i)
        num += counts[i] * (counts[i] - 1);
    return num / ((double)n * (n - 1));
}

int findKeyLength(const string& ctext, int maxLen = 20) {
    cout << "\n[*] Phan tich do dai khoa:\n";
    cout << setw(8) << "Do dai"
         << setw(18) << "IoC trung binh"
         << "   Danh gia\n";
    cout << string(50, '-') << "\n";

    vector<int> kasiskiHints = kasiskiExamination(ctext);
    set<int> topHints(kasiskiHints.begin(),
                      kasiskiHints.begin() + min(5, (int)kasiskiHints.size()));

    double bestIoC = -1;
    int bestLen = 6;

    for (int keyLen = 2; keyLen <= maxLen; ++keyLen) {
        // Tách ciphertext thành keyLen nhóm, tính IoC trung bình
        double avgIoC = 0;
        for (int i = 0; i < keyLen; ++i) {
            string group;
            for (int j = i; j < (int)ctext.size(); j += keyLen)
                group += ctext[j];
            avgIoC += indexOfCoincidence(group);
        }
        avgIoC /= keyLen;

        string hint = topHints.count(keyLen) ? " <- Kasiski goi y" : "";
        cout << setw(8) << keyLen
             << setw(18) << fixed << setprecision(4) << avgIoC
             << hint << "\n";

        if (avgIoC > bestIoC) {
            bestIoC = avgIoC;
            bestLen = keyLen;
        }
    }

    cout << "\n[+] Do dai khoa tot nhat: " << bestLen
         << " (IoC = " << fixed << setprecision(4) << bestIoC << ")\n";
    return bestLen;
}

// ──────────────────────────────────────────────────
// Bước 4: Frequency Analysis → Chi-squared
// ──────────────────────────────────────────────────
double chiSquared(const string& text) {
    if (text.empty()) return 1e9;
    int counts[26] = {};
    int n = text.size();
    for (char c : text) counts[c - 'a']++;

    double score = 0;
    for (int i = 0; i < 26; ++i) {
        double observed = (double)counts[i] / n;
        double expected = ENGLISH_FREQ[i] / 100.0;
        score += (observed - expected) * (observed - expected) / expected;
    }
    return score;
}

string findKey(const string& ctext, int keyLength) {
    string key = "";

    for (int i = 0; i < keyLength; ++i) {
        // Lấy nhóm ký tự tại vị trí i, i+keyLen, i+2*keyLen, ...
        string group;
        for (int j = i; j < (int)ctext.size(); j += keyLength)
            group += ctext[j];

        int bestShift = 0;
        double bestScore = 1e18;

        for (int shift = 0; shift < 26; ++shift) {
            // Dịch ngược group với shift này
            string decrypted;
            for (char c : group)
                decrypted += (char)(((c - 'a') - shift + 26) % 26 + 'a');

            double score = chiSquared(decrypted);
            if (score < bestScore) {
                bestScore = score;
                bestShift = shift;
            }
        }
        key += (char)(bestShift + 'a');
    }
    return key;
}

// ──────────────────────────────────────────────────
// Bước 5: Giải mã Vigenère
// ──────────────────────────────────────────────────
string vigenereDecrypt(const string& cipherRaw, const string& key) {
    string result;
    int keyLen = key.size();
    int keyIdx = 0;

    for (char c : cipherRaw) {
        if (isalpha(c)) {
            int shift = key[keyIdx % keyLen] - 'a';
            char dec = (char)(((tolower(c) - 'a') - shift + 26) % 26 + 'a');
            result += dec;
            keyIdx++;
        } else {
            result += c;  // Giữ nguyên ký tự không phải chữ cái
        }
    }
    return result;
}

string vigenereEncrypt(const string& plainRaw, const string& key) {
    string result;
    int keyLen = key.size();
    int keyIdx = 0;

    for (char c : plainRaw) {
        if (isalpha(c)) {
            int shift = key[keyIdx % keyLen] - 'a';
            char enc = (char)(((tolower(c) - 'a') + shift) % 26 + 'a');
            result += enc;
            keyIdx++;
        } else {
            result += c;
        }
    }
    return result;
}

// ──────────────────────────────────────────────────
// Hàm chính: phá mã
// ──────────────────────────────────────────────────
void breakVigenere(const string& ciphertextRaw, int maxKeyLen = 20) {
    cout << string(60, '=') << "\n";
    cout << "       VIGENERE CIPHER BREAKER - Task 2.6 \n";
    cout << string(60, '=') << "\n";

    // Bước 1: Chuẩn hóa
    string ctext = normalize(ciphertextRaw);
    cout << "\n[*] Ciphertext da chuan hoa (" << ctext.size() << " ky tu):\n";
    cout << "    " << ctext.substr(0, 80) << "...\n";

    // Bước 2+3: Tìm độ dài khóa
    int keyLength = findKeyLength(ctext, maxKeyLen);

    // Bước 4: Tìm khóa
    string key = findKey(ctext, keyLength);
    cout << "\n[+] Khoa tim duoc: '" << key
         << "' (do dai = " << keyLength << ")\n";

    // Bước 5: Giải mã (giữ nguyên khoảng trắng và dấu câu)
    string plaintext = vigenereDecrypt(ciphertextRaw, key);
    cout << "\n[+] Ban ro:\n\n" << plaintext << "\n";

    // Kiểm tra IoC bản rõ
    string plainNorm = normalize(plaintext);
    double ioc = indexOfCoincidence(plainNorm);
    cout << "\n" << string(60, '=') << "\n";
    cout << "[*] IoC cua ban ro: " << fixed << setprecision(4) << ioc
         << "  (tieng Anh chuan ~ 0.0650)\n";
    cout << string(60, '=') << "\n";
}

// ──────────────────────────────────────────────────
// Entry point
// ──────────────────────────────────────────────────
int main() {
    cout << "===== VIGENERE CIPHER BREAKER =====\n";
    cout << "Chon che do:\n";
    cout << "  1. Doc ciphertext tu FILE\n";
    cout << "  2. Nhap ciphertext thu cong\n";
    cout << "Lua chon (1/2): ";

    int choice;
    cin >> choice;
    cin.ignore();  // bỏ '\n' còn lại trong buffer

    string ciphertext;

    if (choice == 1) {
        cout << "Nhap duong dan file: ";
        string filepath;
        getline(cin, filepath);

        ifstream fin(filepath);
        if (!fin) {
            cerr << "[!] Khong mo duoc file: " << filepath << "\n";
            return 1;
        }
        stringstream ss;
        ss << fin.rdbuf();
        ciphertext = ss.str();
        cout << "[+] Doc file thanh cong!\n";

    } else {
        cout << "Nhap ciphertext (nhap dong trong de ket thuc):\n";
        string line, prevLine = "x";
        bool firstLine = true;
        while (getline(cin, line)) {
            if (firstLine) { firstLine = false; }  // bỏ qua dòng rỗng đầu tiên do cin.ignore
            if (line.empty() && prevLine.empty()) break;
            ciphertext += line + "\n";
            prevLine = line;
        }
    }

    if (normalize(ciphertext).size() < 10) {
        cerr << "[!] Ciphertext qua ngan, khong the phan tich.\n";
        return 1;
    }

    breakVigenere(ciphertext);
    return 0;
}