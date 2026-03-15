#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<char>> generateMatrixKey(string key) {
    bool used[26] = {false};
    vector<char> matrix;

    transform(key.begin(), key.end(), key.begin(), ::toupper);

    for (char c : key) {
        if (c == ' ') continue;
        if (c == 'J') c = 'I';

        int idx = c - 'A';
        if (!used[idx]) {
            used[idx] = true;
            matrix.push_back(c);
        }
    }

    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue;

        int idx = c - 'A';
        if (!used[idx]) {
            used[idx] = true;
            matrix.push_back(c);
        }
    }

    vector<vector<char>> playfair(5, vector<char>(5));

    for (int i = 0; i < 25; i++) {
        playfair[i/5][i%5] = matrix[i];
    }

    return playfair;
}

string prepareText(string text) {
    transform(text.begin(), text.end(), text.begin(), ::toupper);

    text.erase(remove(text.begin(), text.end(), ' '), text.end());

    replace(text.begin(), text.end(), 'J', 'I');

    string result = "";

    for (int i = 0; i < text.size(); i++) {
        result += text[i];

        if (i + 1 < text.size() && text[i] == text[i+1])
            result += 'X';
    }

    if (result.size() % 2) result += 'X';

    return result;
}
void findPosition(char c,vector<vector<char>> &matrix, int &row, int &col) {
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (matrix[i][j] == c) {
                    row = i; col = j;
                    return;
                }
    }
string encryptPlayfair(string text, string key){
    transform(key.begin(), key.end(), key.begin(), ::toupper);

    key.erase(remove(key.begin(), key.end(), ' '), key.end());

    replace(key.begin(), key.end(), 'J', 'I');

    vector<vector<char>> matrixKey = generateMatrixKey(key);
    text = prepareText(text);
    string result = "";
    int rA, cA, rB, cB;
    for (int i = 0; i < text.size(); i += 2){
        findPosition(text[i],matrixKey, rA, cA);
        findPosition(text[i+1], matrixKey, rB, cB);
        
        if (rA == rB) {
            result += matrixKey[rA][(cA + 1) % 5];
            result += matrixKey[rB][(cB + 1) % 5];    
        }
        else if(cA == cB){
            result += matrixKey[(rA + 1) % 5][(cA)];
            result += matrixKey[(rB + 1) % 5][(cB)];
        }
        else {
            result += matrixKey[rA][cB];
            result += matrixKey[rB][cA];
        }
    }
    return result;

}
string decryptPlayfair(string text, string key){
    transform(key.begin(), key.end(), key.begin(), ::toupper);

    key.erase(remove(key.begin(), key.end(), ' '), key.end());

    replace(key.begin(), key.end(), 'J', 'I');

    vector<vector<char>> matrixKey = generateMatrixKey(key);
    // Không cần chuẩn bị văn bản giải mã vì nó đã được mã hóa theo quy tắc của Playfair
    string result = "";
    int rA, cA, rB, cB;
    for (int i = 0; i < text.size(); i += 2){
        findPosition(text[i],matrixKey, rA, cA);
        findPosition(text[i+1], matrixKey, rB, cB);
        
        if (rA == rB) {
            result += matrixKey[rA][(cA + 5 - 1 )% 5];
            result += matrixKey[rB][(cB + 5 - 1) % 5];    
        }
        else if(cA == cB){
            result += matrixKey[(rA + 5 - 1) % 5][(cA)];
            result += matrixKey[(rB + 5 - 1) % 5][(cB)];
        }
        else {
            result += matrixKey[rA][cB];
            result += matrixKey[rB][cA];
        }
    }
    return result;

}
int main() {
    int choice;
    cout << "--- PLAYFAIR CIPHER TOOL ---" << endl;
    cout << "1. Ma hoa (Encrypt)" << endl;
    cout << "2. Giai ma (Decrypt)" << endl;
    cout << "Chon chuc nang (1/2): ";
    cin >> choice;
    cin.ignore();   
    if (choice == 1) {
        string text;
        string key;
        cout << "Nhap van ban can ma hoa: ";
        getline(cin, text);
        
        cout << "Nhap khoa : "; getline(cin,key);

        cout << "Ma tran khoa: " << endl;
        auto playfairMatrix = generateMatrixKey(key);
        for (const auto &row : playfairMatrix) {
            for (char c : row) {
                cout << c << " ";
            }
            cout << endl;
        }
        cout << "Ket qua: " << encryptPlayfair(text, key) << endl;
    }
    else if (choice == 2) {
        string text;
        string key;
        cout << "Nhap van ban can giai ma: ";
        getline(cin, text);
        
        cout << "Nhap khoa : "; getline(cin,key);

        cout << "Ma tran khoa: " << endl;
        auto playfairMatrix = generateMatrixKey(key);
        for (const auto &row : playfairMatrix) {
            for (char c : row) {
                cout << c << " ";
            }
            cout << endl;
        }
        cout << "Ket qua: " << decryptPlayfair(text, key) << endl;
    } else {
        cout << "Lua chon khong hop le!" << endl;
    }

    return 0;
}