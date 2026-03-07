#include <iostream>
#include <cctype>
#include <string>

using namespace std;

string chuanHoaKhoa(string key);
string maHoaVigenere(string key, string text);
string giaiMaVigenere(string key, string ctext);

int main()
{
    string key, text, ctext;
    int type;

    cout << "Chao mung den voi lop hoc mat ma Vigenere\n";
    cout << "Ban muon giai ma hay ma hoa vui long chon(1-ma hoa, 2- giai ma, 3- ca hai):";
    cin >> type;
    cout << "Vui long nhap khoa cua ban: ";
    cin.ignore();
    getline(cin, key);
    key = chuanHoaKhoa(key);

    switch(type)
    {
        case 1:
        {
            cout << "\n--- MA HOA ---\n";
            cout << "Nhap plaintext: ";
            getline(cin, text);

            string cipher = maHoaVigenere(key, text);
            cout << "\nCiphertext: " << cipher << endl;
            break;
        }

        case 2:
        {
            cout << "\n--- GIAI MA ---\n";
            cout << "Nhap ciphertext: ";
            getline(cin, text);

            string plain = giaiMaVigenere(key, text);
            cout << "\nPlaintext: " << plain << endl;
            break;
        }

        case 3:
        {
            cout << "\n--- MA HOA ---\n";
            cout << "Nhap plaintext: ";
            getline(cin, text);

            string cipher = maHoaVigenere(key, text);
            cout << "\nCiphertext: " << cipher << endl;

            cout << "\n--- GIAI MA ---\n";
            cout << "Nhap ciphertext: ";
            getline(cin, text);

            string plain = giaiMaVigenere(key, text);
            cout << "\nPlaintext: " << plain << endl;
            break;
        }

        default:
            cout << "Lua chon khong hop le.\n";
    }

    return 0;
}   

string maHoaVigenere(string key, string text)
{
    string cipher = "";
    int j = 0;
    for(int i = 0; i < text.length(); i++)
    {   
        if(isalpha(text[i]))
        {   
            char base = isupper(text[i]) ? 'A' : 'a';

            int p = text[i] - base;
            int k = toupper(key[j % key.length()]) - 'A';

            char c = (p + k) % 26 + base;
            cipher += c;

            j++;
        }
        else
        {
            cipher += text[i];
        }
    }

    return cipher;
}

string giaiMaVigenere(string key, string text)
{
    string plain = "";
    int j = 0;
    for(int i = 0; i < text.length(); i++)
    {
        if(isalpha(text[i]))
        {
            char base = isupper(text[i]) ? 'A' : 'a';

            int c = text[i] - base;
            int k = toupper(key[j % key.length()]) - 'A';

            char p = (c - k + 26) % 26 + base;
            plain += p;

            j++;
        }
        else
        {
            plain += text[i];
        }
    }
    return plain;
}

string chuanHoaKhoa(string key)
{
    string cleanKey = "";
    for(char c : key)
    {
        if(isalpha(c))
            cleanKey += toupper(c);
    }
    return cleanKey;
}