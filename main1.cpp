#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>

using namespace std;

string calculateSHA256(const string &filePath)
{
    ifstream file(filePath, ios::binary);

    if (!file.is_open())
    {
        return "";
    }

    EVP_MD_CTX *context = EVP_MD_CTX_new();

    if (context == nullptr)
    {
        return "";
    }

    EVP_DigestInit_ex(context, EVP_sha256(), nullptr);

    char buffer[4096];
    
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
    {
        EVP_DigestUpdate(context, buffer, file.gcount());
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength = 0;

    EVP_DigestFinal_ex(context, hash, &hashLength);

    EVP_MD_CTX_free(context);
    file.close();

    stringstream ss;

    for (unsigned int i = 0; i < hashLength; i++)
    {
        ss << hex << setw(2) << setfill('0')
           << static_cast<int>(hash[i]);
    }

    return ss.str();
}

int main()
{

    int choice;
    string filePath;

    while (true)
    {

        cout << "\n==============================\n";
        cout << "     FILE INTEGRITY CHECKER\n";
        cout << "==============================\n";
        cout << "1. Calculate SHA-256 Hash\n";
        cout << "2. Save File Hash\n";
        cout << "3. Load Saved Hash\n";
        cout << "4. Verify File Integrity\n";
        cout << "5. Exit\n";
        cout << "------------------------------\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nInvalid input. Please enter a number from 1 to 5.\n";
            continue;
        }

        cin.ignore(1000, '\n');

        if (choice < 1 || choice > 5)
        {
            cout << "\nInvalid choice. Please select 1 to 5.\n";
            continue;
        }

        if (choice == 1)
        {

            cout << "\nEnter file path: ";
            getline(cin, filePath);

            if (filePath.empty())
            {
                cout << "\nError: File path cannot be empty.\n";
                continue;
            }

            string hash = calculateSHA256(filePath);

            if (hash.empty())
            {
                cout << "\nError: Could not open the file.\n";
            }
            else
            {
                cout << "\nSHA-256 Hash:\n";
                cout << hash << "\n";
            }
        }

        else if (choice == 2)
        {

            cout << "\nEnter file path: ";
            getline(cin, filePath);

            if (filePath.empty())
            {
                cout << "\nError: File path cannot be empty.\n";
                continue;
            }

            string hash = calculateSHA256(filePath);

            if (hash.empty())
            {
                cout << "\nError: Could not open the file.\n";
            }
            else
            {
                ofstream hashFile("saved_hash.txt");

                if (hashFile.is_open())
                {
                    hashFile << hash;
                    hashFile.close();

                    cout << "\nHash saved successfully!\n";
                }
                else
                {
                    cout << "\nError: Could not save hash.\n";
                }
            }
        }

        else if (choice == 3)
        {
            ifstream hashFile("saved_hash.txt");

            if (hashFile.is_open())
            {
                string savedHash;

                getline(hashFile, savedHash);
                hashFile.close();

                cout << "\nSaved SHA-256 Hash:\n";
                cout << savedHash << "\n";
            }
            else
            {
                cout << "\nError: No saved hash found.\n";
            }
        }

        else if (choice == 4)
        {
            cout << "\nEnter file path: ";
            getline(cin, filePath);

            if (filePath.empty())
            {
                cout << "\nError: File path cannot be empty.\n";
                continue;
            }

            string currentHash = calculateSHA256(filePath);

            if (currentHash.empty())
            {
                cout << "\nError: Could not open the file.\n";
            }
            else
            {
                ifstream hashFile("saved_hash.txt");

                if (!hashFile.is_open())
                {
                    cout << "\nError: No saved hash found.\n";
                }
                else
                {
                    string savedHash;
                    getline(hashFile, savedHash);
                    hashFile.close();

                    if (currentHash == savedHash)
                    {
                        cout << "\nFile Integrity: OK\n";
                        cout << "The file has not changed.\n";
                    }
                    else
                    {
                        cout << "\nFile Integrity: FAILED\n";
                        cout << "The file has been changed.\n";
                    }
                }
            }
        }
        else if (choice == 5)
        {
            cout << "\nExiting program...\n";
            break;
        }
    }

    return 0;
}