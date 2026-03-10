#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
class PermutationCipher {
public:

    string maHoa(string message, int d, vector<int> h) {
        string result = "";
        while (message.length() % d != 0) {
            message += " ";
        }
        for (size_t i = 0; i < message.length(); i += d) {
            string block = message.substr(i, d);
            string encodedBlock = block;
            for (int j = 0; j < d; j++) {
                encodedBlock[j] = block[h[j] - 1];
            }
            result += encodedBlock;
        }
        return result;
    }
    string giaiMa(string cipher, int d, vector<int> h) {
        vector<int> h_inv(d);
        for (int i = 0; i < d; i++) {
            h_inv[h[i] - 1] = i + 1;
        }
        return maHoa(cipher, d, h_inv);
    }
};
int main() {
    PermutationCipher pc;
    string p;
    int d;
    cout << "=== CHUONG TRINH MA HOAN VI BAC D (PERMUTATION CIPHER) ===\n";
    cout << "Nhap plain text (p): ";
    getline(cin, p);
    cout << "Nhap bac d (vi du p = hello, d = 5): ";
    cin >> d;
    vector<int> h(d);
    cout << "Nhap day hoan vi h (vi du d=4, nhap thu tu moi (h): 2 4 1 3): ";
    for (int i = 0; i < d; i++) {
        cin >> h[i];
    }
    string c = pc.maHoa(p, d, h);
    cout << "\n[+] Ban ma sau khi ma hoa: " << c << endl;
    string decoded = pc.giaiMa(c, d, h);
    cout << "[+] Ban ro sau khi giai ma: " << decoded << endl;
    return 0;
}





