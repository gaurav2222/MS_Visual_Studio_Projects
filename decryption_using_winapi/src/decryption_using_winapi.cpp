#include <Windows.h>
#include <wincrypt.h>
#include <iostream>
#pragma comment(lib, "crypt32.lib")
using namespace std;

#define BLOCK_LEN 128

int wmain(int argc, wchar_t* argv[])
{
    const wchar_t* filename = L"dapp_encrypted.exe"; //input encrypted file
    const wchar_t* filename2 = L"dapp_decrypted_or_dapp.exe"; //output decrypted file

    wchar_t default_key[] = L"type_your_password";//Use the same password that is used to encrypt the file
    wchar_t* key_str = default_key;

    printf("DECRYPTING...\n");
    size_t len = lstrlenW(key_str);

    //printf("Key: %S\n", key_str);
    printf("Input File: %S\n", filename);
    printf("Output File: %S\n", filename2);
    printf("--------\n");

    DWORD dwStatus = 0;
    BOOL bResult = FALSE;
    wchar_t info[] = L"Microsoft Enhanced RSA and AES Cryptographic Provider";
    HCRYPTPROV hProv;
    if (!CryptAcquireContextW(&hProv, NULL, info, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        dwStatus = GetLastError();
        printf("Function - CryptAcquireContext Failed: %d\n", dwStatus);
        CryptReleaseContext(hProv, 0);
        system("pause");
        return (-1);

        return dwStatus;
    }
    HCRYPTHASH hHash;
    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        dwStatus = GetLastError();
        printf("Function - CryptCreateHash Failed: %d\n", dwStatus);
        CryptReleaseContext(hProv, 0);
        system("pause");
        return (-1);
    }

    if (!CryptHashData(hHash, (BYTE*)key_str, len, 0)) {
        DWORD err = GetLastError();
        printf("Function - CryptHashData Failed : %s\n", err);
        system("pause");
        return (-1);
    }
    printf("Function - CryptHashData Success!\n");

    HCRYPTKEY hKey;
    if (!CryptDeriveKey(hProv, CALG_AES_128, hHash, 0, &hKey)) {
        dwStatus = GetLastError();
        printf("Function - CryptDeriveKey failed: %d\n", dwStatus);
        CryptReleaseContext(hProv, 0);
        system("pause");
        return (-1);
    }
    printf("Function - CryptDeriveKey Success!\n");

    const size_t chunk_size = BLOCK_LEN;
    BYTE chunk[chunk_size];
    DWORD read = 0;
    DWORD written = 0;

    HANDLE hInpFile = CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    HANDLE hOutFile = CreateFileW(filename2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hInpFile == NULL) {
        printf("Cannot open input file!\n");
        system("pause");
        return (-1);
    }
    if (hOutFile == NULL) {
        printf("Cannot open output file!\n");
        system("pause");
        return (-1);
    }

    //bool fEOF = FALSE;
    //do
    //{
    //    if (!ReadFile(hInpFile, chunk, chunk_size, &read, NULL)) {
    //        printf("Error in reading file %d\n", GetLastError());
    //    }
    //    if (read < chunk_size)
    //    {
    //        fEOF = TRUE;
    //    }
    //    DWORD ciphertextLen = BLOCK_LEN;
    //    if (!CryptDecrypt(hKey, NULL, fEOF, 0, chunk, &read)) {
    //        printf("Failed!\n");
    //        break;
    //    }
    //    if (!WriteFile(hOutFile, chunk, ciphertextLen, &written, NULL)) {
    //        printf("writing failed!\n");
    //        break;
    //    }
    //    //memset(chunk, 0, chunk_size);
    //} while (!fEOF);

    BOOL Final_Block = FALSE;
    while (bResult = ReadFile(hInpFile, chunk, chunk_size, &read, NULL)) {
        if (0 == read) {
            break;
        }
        DWORD ciphertextLen = BLOCK_LEN;
        if (read < chunk_size) {
            Final_Block = TRUE;
            ciphertextLen = read;
            memset(chunk, 0, read);
        }
        if (!CryptDecrypt(hKey, NULL, Final_Block, 0, chunk, &ciphertextLen) && (read == BLOCK_LEN || read <= 0)) {
            printf("Failed!\n");
            break;
        }
        if (!WriteFile(hOutFile, chunk, ciphertextLen, &written, NULL)) {
            printf("Writing Failed!\n");
            break;
        }
        memset(chunk, 0, chunk_size);
    }

    CryptReleaseContext(hProv, 0);
    CryptDestroyKey(hKey);
    CryptDestroyHash(hHash);

    CloseHandle(hInpFile);
    CloseHandle(hOutFile);
    printf("Function - CryptDecrypt Success!\n");
    printf("Decryption is Successful !\n");
    system("pause");
    return 0;
}