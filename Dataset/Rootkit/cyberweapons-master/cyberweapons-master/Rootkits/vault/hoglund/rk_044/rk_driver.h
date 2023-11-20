
#ifndef __NTROOT_DRIVERH__
#define __NTROOT_DRIVERH__

#include "ntddk.h"
#include "stdarg.h"
#include "stdio.h"
#include "ndis.h"
#include "ntddpack.h"
#include "ntddkbd.h"
#include "ntiologc.h"

/*
 * Defines and such
 * --------------------------------------------------------
 */
/* For the definitions in Winioctl.h */
/* #undef DEVICE_TYPE */
typedef UCHAR  BYTE;
typedef USHORT WORD;
typedef ULONG  DWORD;
typedef LONGLONG  DWORDLONG;
typedef PVOID SID;
#include "winioctl.h"

#define DWORD unsigned __int32
#define WORD unsigned __int16
#define BYTE unsigned __int8
#define BOOL __int32
typedef PVOID POBJECT;

#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#define MAKELONG(a, b) ((LONG) (((WORD) (a)) | ((DWORD) ((WORD) (b))) << 16)) 

#define MAX_REQUESTS    4
#define MAX_PATH_LENGTH 256
// Length of process name (rounded up to next DWORD)
#define PROCNAMELEN     20

// Maximum length of NT process name
#define NT_PROCNAMELEN  16

#undef	ASSERT
#define ASSERT(_c) \
	if(!(_c)) { \
		DbgPrint("Assert failed in file %s, line %d.\n", __FILE__, __LINE__); \
	}

/**********************************************************************************
 * System Structures - reversed from various sources
 **********************************************************************************/

typedef struct
{
	WORD NtSDTfCount;
	DWORD fAddress[ANYSIZE_ARRAY];
} NTSDT;

typedef struct
{
	WORD CallNumber;
	WORD ProcessId;
	LARGE_INTEGER Time;
	DWORD status;	// execute status
} NTS_TRACE_ENTRY;

typedef struct
{
	WORD EntriesCount;
	NTS_TRACE_ENTRY trace[ANYSIZE_ARRAY];
} NTS_TRACE;

typedef struct
{
	WORD EntriesCount;
	WORD ProcessIdArray[ANYSIZE_ARRAY];
} PROCESS_FILTER;

/* ________________________________________________
 . Timer
 . ________________________________________________ */
typedef enum _TIMER_INFO_CLASS {
	TimerBasicInfo
} TIMER_INFO_CLASS;

typedef struct TimerInfo_t {
	LARGE_INTEGER DueTime;
	CCHAR TimerState;
	CCHAR Unused[3];
	ULONG TimerType;
} TIMER_INFO, *PTIMER_INFO;


/* ________________________________________________
 . Event Objects
 . ________________________________________________ */
typedef enum _EVENT_INFO_CLASS {
	EventBasicInfo
} EVENT_INFO_CLASS;

typedef struct EventInfo_t {
	EVENT_TYPE EventType;
	LONG EventState;
} EVENT_INFO, *PEVENT_INFO;

/* ________________________________________________
 . Mutexes
 . ________________________________________________ */
typedef enum _MUTANT_INFO_CLASS {
	MutantBasicInfo
} MUTANT_INFO_CLASS;

typedef struct MutantInfo_t {
	LONG MutantState;
	BOOLEAN bOwnedByCallingThread;
	BOOLEAN bAbandoned;
	USHORT Unused;
} MUTANT_INFO, *PMUTANT_INFO;

typedef enum _SEMAPHORE_INFO_CLASS 
{
	SemaphoreBasicInfo	/* ntddk */
} SEMAPHORE_INFO_CLASS;

typedef struct SemaphoreInfo_t 
{
	ULONG CurrentCount;
	ULONG MaxCount;
} SEMAPHORE_INFO, *PSEMAPHORE_INFO;


/**********************************************************************************
 * System Call Prototypes
 **********************************************************************************/

NTSYSAPI
NTSTATUS
NTAPI ObQueryNameString( POBJECT Object, PUNICODE_STRING Name, ULONG MaximumLength, PULONG ActualLength );


/* _______________________________________
 . System Time
 . _______________________________________ */

NTSYSAPI 
NTSTATUS 
NTAPI 
RtlLocalTimeToSystemTime(PLARGE_INTEGER LocalTime, 
						 PLARGE_INTEGER SystemTime
);

NTSYSAPI 
NTSTATUS 
NTAPI 
RtlSystemTimeToLocalTime(PLARGE_INTEGER SystemTime, 
						 PLARGE_INTEGER LocalTime
);


/* _______________________________________________
 . Event Objects
 . _______________________________________________ */

NTSYSAPI
NTSTATUS
NTAPI
NtCreateEvent(
	OUT PHANDLE hEvent,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN EVENT_TYPE EventType,
	IN BOOLEAN bInitialState
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateEvent(
	OUT PHANDLE hEvent,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN EVENT_TYPE EventType,
	IN BOOLEAN bInitialState
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenEvent(
	OUT PHANDLE hEvent,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenEvent(
	OUT PHANDLE hEvent,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtClearEvent(
	IN HANDLE hEvent
);

NTSYSAPI
NTSTATUS
NTAPI
ZwClearEvent(
	IN HANDLE hEvent
);

NTSYSAPI
NTSTATUS
NTAPI
NtPulseEvent(
	IN HANDLE hEvent,
	OUT OPTIONAL PULONG PreviousState
);

NTSYSAPI
NTSTATUS
NTAPI
ZwPulseEvent(
	IN HANDLE hEvent,
	OUT OPTIONAL PULONG PreviousState
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetEvent(
	IN HANDLE hEvent,
	OUT OPTIONAL PULONG PreviousState
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetEvent(
	IN HANDLE hEvent,
	OUT OPTIONAL PULONG PreviousState
);

NTSYSAPI
NTSTATUS
NTAPI
NtResetEvent(
	IN HANDLE hEvent,
	OUT OPTIONAL PULONG PreviousState
);

NTSYSAPI
NTSTATUS
NTAPI
ZwResetEvent(
	IN HANDLE hEvent,
	OUT OPTIONAL PULONG PreviousState
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryEvent(
	IN HANDLE hEvent,
	IN EVENT_INFO_CLASS InfoClass,
	OUT PVOID EventInfoBuffer,
	IN ULONG EventInfoBufferSize,
	OUT PULONG BytesCopied
);

NTSTATUS
NTAPI
ZwQueryEvent(
	IN HANDLE hEvent,
	IN EVENT_INFO_CLASS InfoClass,
	OUT PVOID EventInfoBuffer,
	IN ULONG EventInfoBufferSize,
	OUT PULONG BytesCopied
);

NTSYSAPI
NTSTATUS
NTAPI
NtCreateEventPair(
	OUT PHANDLE hEventPair,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateEventPair(
	OUT PHANDLE hEventPair,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenEventPair(
	OUT PHANDLE hEventPair,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenEventPair(
	OUT PHANDLE hEventPair,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetLowWaitHighEventPair(
	IN HANDLE hEventPair
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetLowWaitHighEventPair(
	IN HANDLE hEventPair
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetHighWaitLowEventPair(
	IN HANDLE hEventPair
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetHighWaitLowEventPair(
	IN HANDLE hEventPair
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetHighEventPair(
	IN HANDLE hEventPair
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetHighEventPair(
	IN HANDLE hEventPair
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetLowEventPair(
	IN HANDLE hEventPair
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetLowEventPair(
	IN HANDLE hEventPair
);

NTSYSAPI
NTSTATUS
NTAPI
NtWaitHighEventPair(
	IN HANDLE hEventPair
);

NTSYSAPI
NTSTATUS
NTAPI
ZwWaitHighEventPair(
	IN HANDLE hEventPair
);

NTSYSAPI
NTSTATUS
NTAPI
NtWaitLowEventPair(
	IN HANDLE hEventPair
);

NTSYSAPI
NTSTATUS
NTAPI
ZwWaitLowEventPair(
	IN HANDLE hEventPair
);

/* ______________________________________________
 . Mutants are mutexes - sync objects
 . ______________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI
NtCreateMutant(
	OUT PHANDLE hMutex,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN BOOLEAN bOwnMutant
);

NTSTATUS
NTAPI
ZwCreateMutant(
	OUT PHANDLE hMutex,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN BOOLEAN bOwnMutant
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenMutant(
	OUT PHANDLE hMutex,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSTATUS
NTAPI
ZwOpenMutant(
	OUT PHANDLE hMutex,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryMutant(
	IN HANDLE hMutant,
	IN MUTANT_INFO_CLASS InfoClass,
	OUT PVOID MutantInfoBuffer,
	IN ULONG MutantInfoBufferSize,
	OUT PULONG BytesCopied
);

NTSTATUS
NTAPI
ZwQueryMutant(
	IN HANDLE hMutant,
	IN MUTANT_INFO_CLASS InfoClass,
	OUT PVOID MutantInfoBuffer,
	IN ULONG MutantInfoBufferSize,
	OUT PULONG BytesCopied
);

NTSYSAPI
NTSTATUS
NTAPI
NtReleaseMutant(
	IN HANDLE hMutant,
	OUT OPTIONAL PULONG bWasSignalled
);

NTSTATUS
NTAPI
ZwReleaseMutant(
	IN HANDLE hMutant,
	OUT OPTIONAL PULONG bWasSignalled
);


NTSYSAPI
NTSTATUS
NTAPI
NtCreateSemaphore(
	OUT PHANDLE hSemaphore,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN ULONG InitialCount,
	IN ULONG MaximumCount
);


NTSTATUS
NTAPI
ZwCreateSemaphore(
	OUT PHANDLE hSemaphore,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN ULONG InitialCount,
	IN ULONG MaximumCount
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenSemaphore(
	OUT PHANDLE hSemaphore,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSTATUS
NTAPI
ZwOpenSemaphore(
	OUT PHANDLE hSemaphore,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtQuerySemaphore(
	IN HANDLE hSemaphore,
	IN SEMAPHORE_INFO_CLASS SemaphoreInfoClass,
	OUT PVOID Buffer,
	IN ULONG BufferSize,
	OUT PULONG BytesReturned
);

NTSTATUS
NTAPI
ZwQuerySemaphore(
	IN HANDLE hSemaphore,
	IN SEMAPHORE_INFO_CLASS SemaphoreInfoClass,
	OUT PVOID Buffer,
	IN ULONG BufferSize,
	OUT PULONG BytesReturned
);

NTSYSAPI
NTSTATUS
NTAPI
NtReleaseSemaphore(
	IN HANDLE hSemaphore,
	IN ULONG ReleaseCount,
	OUT PULONG PreviousCount
);

NTSTATUS
NTAPI
ZwReleaseSemaphore(
	IN HANDLE hSemaphore,
	IN ULONG ReleaseCount,
	OUT PULONG PreviousCount
);


NTSYSAPI
NTSTATUS
NTAPI
NtWaitForSingleObject(
	IN HANDLE hObject,
	IN BOOLEAN bAlertable,
	IN PLARGE_INTEGER Timeout
);

NTSYSAPI
NTSTATUS
NTAPI
ZwWaitForSingleObject(
	IN HANDLE hObject,
	IN BOOLEAN bAlertable,
	IN PLARGE_INTEGER Timeout
);

NTSYSAPI
NTSTATUS
NTAPI
NtSignalAndWaitForSingleObject(
	IN HANDLE hSignalObject,
	IN HANDLE hWaitObject,
	IN BOOLEAN bAlertable,
	IN PLARGE_INTEGER Timeout
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSignalAndWaitForSingleObject(
	IN HANDLE hSignalObject,
	IN HANDLE hWaitObject,
	IN BOOLEAN bAlertable,
	IN PLARGE_INTEGER Timeout
);


NTSYSAPI
NTSTATUS
NTAPI
NtWaitForMultipleObjects(
	IN ULONG nWaitObjectHandles,
	IN PHANDLE WaitObjectHandlesArray,
	IN WAIT_TYPE WaitType,
	IN BOOLEAN bAlertable,
	IN PLARGE_INTEGER Timeout
);

NTSYSAPI
NTSTATUS
NTAPI
ZwWaitForMultipleObjects(
	IN ULONG nWaitObjectHandles,
	IN PHANDLE WaitObjectHandlesArray,
	IN WAIT_TYPE WaitType,
	IN BOOLEAN bAlertable,
	IN PLARGE_INTEGER Timeout
);

/* ______________________________________________
 . Timer
 . ______________________________________________ */

NTSYSAPI
NTSTATUS
NTAPI
NtCreateTimer(
	OUT PHANDLE phTimer,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN TIMER_TYPE TimerType
);

NTSTATUS
NTAPI
ZwCreateTimer(
	OUT PHANDLE phTimer,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN TIMER_TYPE TimerType
);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenTimer(
	OUT PHANDLE phTimer,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSTATUS
NTAPI
ZwOpenTimer(
	OUT PHANDLE phTimer,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryTimer(
	IN HANDLE hTimer,
	IN TIMER_INFO_CLASS InfoClass,
	OUT PVOID TimerInfoBuffer,
	IN ULONG TimerInfoBufferSize,
	OUT PULONG BytesCopied
);

NTSTATUS
NTAPI
ZwQueryTimer(
	IN HANDLE hTimer,
	IN TIMER_INFO_CLASS InfoClass,
	OUT PVOID TimerInfoBuffer,
	IN ULONG TimerInfoBufferSize,
	OUT PULONG BytesCopied
);


typedef VOID
(NTAPI *PTIMERAPCROUTINE)(
   PVOID lpArgToCompletionRoutine,
   ULONG dwTimerLowValue,
   ULONG dwTimerHighValue);


NTSYSAPI
NTSTATUS
NTAPI
NtSetTimer(
	IN HANDLE hTimer,
	IN PLARGE_INTEGER pDueTime,
	IN PTIMERAPCROUTINE pfnCompletionRoutine OPTIONAL,
	IN ULONG pfnCompletionRoutineArg,
	IN BOOLEAN bResume,
	IN LONG Period,
	OUT PBOOLEAN bTimerState
);


NTSTATUS
NTAPI
ZwSetTimer(
	IN HANDLE hTimer,
	IN PLARGE_INTEGER pDueTime,
	IN PTIMERAPCROUTINE pfnCompletionRoutine OPTIONAL,
	IN ULONG pfnCompletionRoutineArg,
	IN BOOLEAN bResume,
	IN LONG Period,
	OUT PBOOLEAN bTimerState
);

NTSYSAPI
NTSTATUS
NTAPI
NtCancelTimer(
	IN HANDLE hTimer,
	OUT PBOOLEAN pbState
);


NTSTATUS
NTAPI
ZwCancelTimer(
	IN HANDLE hTimer,
	OUT PBOOLEAN pbState
);

NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
	IN ULONG bAlertable,
	IN PLARGE_INTEGER pDuration
);

NTSTATUS
NTAPI
ZwDelayExecution(
	IN ULONG bAlertable,
	IN PLARGE_INTEGER pDuration
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryTimerResolution(
	OUT PULONG MaxResolution,
	OUT PULONG MinResolution,
	OUT PULONG SystemResolution
);

NTSTATUS
NTAPI
ZwQueryTimerResolution(
	OUT PULONG MaxResolution,
	OUT PULONG MinResolution,
	OUT PULONG SystemResolution
);

NTSYSAPI
NTSTATUS
NTAPI
NtSetTimerResolution(
	IN ULONG NewResolution,
	IN BOOLEAN bSet,
	OUT PULONG pResolutionSet
);

NTSTATUS
NTAPI
ZwSetTimerResolution(
	IN ULONG NewResolution,
	IN BOOLEAN bSet,
	OUT PULONG pResolutionSet
);

/* _____________________________________________
 . NT Performance Timers
 . -alter behavior to hide system activity such
 . as CPU usage.  hide l0phtcrack
 . _____________________________________________ */

NTSYSAPI
NTSTATUS
NTAPI
NtQueryPerformanceCounter(
	OUT PLARGE_INTEGER pPerformanceCount,
	OUT PLARGE_INTEGER pFrequency
);

NTSTATUS
NTAPI
ZwQueryPerformanceCounter(
	OUT PLARGE_INTEGER pPerformanceCount,
	OUT PLARGE_INTEGER pFrequency
);

NTSYSAPI
NTSTATUS
NTAPI
NtQuerySystemTime(
	OUT PLARGE_INTEGER pSystemTime
);

NTSTATUS
NTAPI
ZwQuerySystemTime(
	OUT PLARGE_INTEGER pSystemTime
);


NTSYSAPI
NTSTATUS
NTAPI
NtSetSystemTime(
	IN PLARGE_INTEGER pSystemTime,
	OUT PLARGE_INTEGER pOldsystemTime OPTIONAL
);

NTSTATUS
NTAPI
ZwSetSystemTime(
	IN PLARGE_INTEGER pSystemTime,
	OUT PLARGE_INTEGER pOldsystemTime OPTIONAL
);

NTSYSAPI
ULONG
NTAPI
NtGetTickCount(
);

ULONG
NTAPI
ZwGetTickCount(
);






/* LUID */
NTSYSAPI
NTSTATUS
NTAPI
NtAllocateLocallyUniqueId(
	OUT PLUID pLuid
);

NTSYSAPI
NTSTATUS
NTAPI
ZwAllocateLocallyUniqueId(
	OUT PLUID pLuid
);

/* display data on boot-up screen */
NTSYSAPI
NTSTATUS
NTAPI
NtDisplayString(
	IN PUNICODE_STRING pString
);

NTSYSAPI
NTSTATUS
NTAPI
ZwDisplayString(
	IN PUNICODE_STRING pString
);


/* __________________________________________________________________________
 . Internationalization
 . __________________________________________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI
NtQueryDefaultUILanguage(
	OUT PUSHORT DefaultUILanguage
);

typedef 
NTSTATUS 
(NTAPI *PFNNTQUERYDEFAULTUILANGUAGE)(	
	OUT PUSHORT DefaultUILanguage
);


NTSYSAPI
NTSTATUS
NTAPI
ZwQueryDefaultUILanguage(
	OUT PUSHORT DefaultUILanguage
);

NTSYSAPI
NTSTATUS
NTAPI
NtQueryInstallUILanguage(
	OUT PUSHORT InstallUILanguage
);

typedef 
NTSTATUS 
(NTAPI *PFNNTQUERYINSTALLUILANGUAGE)(	
	OUT PUSHORT InstallUILanguage
);


NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInstallUILanguage(
	OUT PUSHORT InstallUILanguage
);


NTSYSAPI
NTSTATUS
NTAPI
NtSetDefaultUILanguage(
	IN USHORT DefaultUILanguage
);

typedef 
NTSTATUS 
(NTAPI *PFNNTSETDEFAULTUILANGUAGE)(	
	IN USHORT DefaultUILanguage
);

NTSYSAPI
NTSTATUS
NTAPI
ZwSetDefaultUILanguage(
	IN USHORT DefaultUILanguage
);

/* _______________________________________________________________________
 . Error Handling
 . _______________________________________________________________________ */
NTSYSAPI
NTSTATUS
NTAPI
NtRaiseHardError(
	NTSTATUS NtStatus,
	ULONG nParameters,
	ULONG ParametersMask,
	PVOID *ParameterList,
	ULONG Unknown1,
	PULONG Unknown2
);

NTSYSAPI
NTSTATUS
NTAPI
ZwRaiseHardError(
	NTSTATUS NtStatus,
	ULONG nParameters,
	ULONG ParametersMask,
	PVOID *ParameterList,
	ULONG Unknown1,
	PULONG Unknown2
);


/**********************************************************************************
 * Display strings to the boot-up-screen.  Kinda cool.  Can only use during boot-up,
 * else you will BSOD.
 **********************************************************************************/
NTSYSAPI
NTSTATUS
NTAPI ZwDisplayString( PUNICODE_STRING Text );

/**********************************************************************************
 * Extra shit.
 **********************************************************************************/

/*
 * Driver Related Types
 * --------------------------------------------------------
 */
typedef struct _INTERNAL_REQUEST {
    LIST_ENTRY     ListElement;
    PIRP           Irp;
    NDIS_REQUEST   Request;
} INTERNAL_REQUEST, *PINTERNAL_REQUEST;

/* this can be whatever we want, hail the void pointer! */
typedef struct _DEVICE_EXTENSION {
    PDEVICE_OBJECT DeviceObject;
    NDIS_HANDLE    NdisProtocolHandle;
    NDIS_HANDLE	   AdapterObject;
	UINT           Medium;
	NDIS_STRING    AdapterName;
    PWSTR          BindString;
    PWSTR          ExportString;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

typedef struct _OPEN_INSTANCE {
    PDEVICE_EXTENSION   DeviceExtension;
    NDIS_HANDLE			AdapterHandle;  /* returned from ndisOpenAdapter */
    
	NDIS_HANDLE         mPacketPoolH;    
    NDIS_HANDLE			mBufferPoolH;

	NDIS_STATUS         mStatus; /* for async status */
	UINT                mMedium;
	NDIS_EVENT			Event;
	NDIS_STATUS			Status;
} OPEN_INSTANCE, *POPEN_INSTANCE;

typedef struct _PACKET_RESERVED {
    LIST_ENTRY     ListElement;
    PIRP           Irp;
	PVOID		   pBuffer; /* used for buffers built in kernel mode */
	ULONG		   bufferLen;
	PVOID		   pHeaderBufferP;
	ULONG		   pHeaderBufferLen;
    PMDL           pMdl;
}  PACKET_RESERVED, *PPACKET_RESERVED;

/* 
 * Prototypes
 * ---------------------------------------------------------------------
 */
VOID		OnUnload(IN PDRIVER_OBJECT DriverObject );
VOID        testCreateProcess(void); /* only testing, do not use */
NTSTATUS    OnStubDispatch( IN PDEVICE_OBJECT theDeviceObjectP, IN PIRP theIrpP );

/*
 * Global symbols
 */
extern KIRQL gIrqL;
extern POPEN_INSTANCE gOpenInstance;
extern KSPIN_LOCK	GlobalArraySpinLock;

extern PDEVICE_OBJECT	   gKbdHookDevice; /* hook keyboard class driver */
extern PDEVICE_OBJECT      kbdDevice;

extern PDEVICE_OBJECT		gUserDevice; 
extern PDRIVER_OBJECT		gDriverObject;

extern KEVENT		command_signal_event;
extern KEVENT		exec_signal_event;
extern KSPIN_LOCK		WorkItemSpinLock;

#endif
