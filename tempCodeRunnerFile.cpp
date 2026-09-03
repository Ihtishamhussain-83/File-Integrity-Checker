#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>

using namespace std;

string calculateSHA256(const string& filePath) {
    ifstream file(filePath, ios::binary);

    if (!file.is_open()) {
        return "";
    }

    EVP_MD_CTX* context = EVP_MD_CTX_new();

    if (context == nullptr) {
        return "";
    }

    EVP_DigestInit_ex(context, EVP_sha256(), nullptr);

    char buffer[4096];

    while (file.read(buffer, sizeof(buffer))) {
        EVP_DigestUpdate(context, buffer, file.gcount());
    }

    EVP_DigestUpdate(context, buffer, file.gcount());

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength = 0;

    EVP_DigestFinal_ex(context, hash, &hashLength);

    EVP_MD_CTX_free(context);
    file.close();

    stringstream ss;

    for (unsigned int i = 0; i < hashLength; i++) {
        ss << hex << setw(2) << setfill('0')
           << static_cast<int>(hash[i]);
    }

    return ss.str();
}

int main() {

    int choice;
    string filePath;

    while (true) {

        cout << "\n==============================\n";
        cout << "     FILE INTEGRITY CHECKER\n";
        cout << "==============================\n";
        cout << "1. Calculate SHA-256 Hash\n";
        cout << "2. Exit\n";
        cout << "------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        cin.ignore();

        if (choice == 1) {

            cout << "\nEnter file path: ";
            getline(cin, filePath);

            string hash = calculateSHA256(filePath);

            if (hash.empty()) {
                cout << "\nError: Could not open the file.\n";
            }
            else {
                cout << "\nSHA-256 Hash:\n";
                cout << hash << "\n";
            }

        }
        else if (choice == 2) {

            cout << "\nExiting program...\n";
            break;

        }
        else {

            cout << "\nInvalid choice. Please try again.\n";
        }
    }
    

    return 0;
}