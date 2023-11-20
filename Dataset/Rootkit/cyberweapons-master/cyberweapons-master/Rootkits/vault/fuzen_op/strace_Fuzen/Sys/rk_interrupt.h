
#ifndef __RK_INTERRUPT_H__
#define __RK_INTERRUPT_H__


#define DWORD unsigned long
#define WORD unsigned short
#define BOOL unsigned long
#define BYTE unsigned char

// Length of process name (rounded up to next DWORD)
#define PROCNAMELEN     20
// Maximum length of NT process name
#define NT_PROCNAMELEN  16

#pragma pack(1)
typedef struct ServiceDescriptorEntry {
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTableBase; //Used only in checked build
	unsigned int NumberOfServices;
	unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;
#pragma pack()

__declspec(dllimport)  ServiceDescriptorTableEntry_t KeServiceDescriptorTable;
//#define SYSTEMSERVICE(_function)  KeServiceDescriptorTable.ServiceTableBase[ *(PULONG)((PUCHAR)_function+1)]

#define FUNC_NAME_LEN 30
char *name_array = NULL;

const WCHAR deviceLinkBuffer[]  = L"\\DosDevices\\strace";
const WCHAR deviceNameBuffer[]  = L"\\Device\\strace";
DEVICE_OBJECT g_StraceTrapDevice;

#define MAKELONG(a, b) ((LONG) (((WORD) (a)) | ((DWORD) ((WORD) (b))) << 16)) 

ULONG gProcessNameOffset = 0;
ULONG gProcessID = 0;
ULONG gSyscallLimit = 0;


/**********************************************************************************
 * Interrupt Descriptor Table
 **********************************************************************************/
#pragma pack(1)
typedef struct
{
	WORD LowOffset;
	WORD selector;
	BYTE unused_lo;
	unsigned char unused_hi:5; /* stored TYPE ? */
	unsigned char DPL:2; 
	unsigned char P:1; /* present */
	WORD HiOffset;
} IDTENTRY;

/* sidt returns idt in this format */
typedef struct
{
	WORD IDTLimit;
	WORD LowIDTbase;
	WORD HiIDTbase;
} IDTINFO;

/* from undoc nt */
typedef struct
{
    unsigned short  limit_0_15;
    unsigned short  base_0_15;
    unsigned char   base_16_23;

    unsigned char    accessed    : 1;
    unsigned char    readable    : 1;
    unsigned char    conforming  : 1;
    unsigned char    code_data   : 1;
    unsigned char    app_system  : 1;
    unsigned char    dpl         : 2;
    unsigned char    present     : 1;

    unsigned char    limit_16_19 : 4;
    unsigned char    unused      : 1;
    unsigned char    always_0    : 1;
    unsigned char    seg_16_32   : 1;
    unsigned char    granularity : 1;

    unsigned char   base_24_31;
} CODE_SEG_DESCRIPTOR;

/* from undoc nt */
typedef struct
{
    unsigned short  offset_0_15;
    unsigned short  selector;

    unsigned char    param_count : 4;
    unsigned char    some_bits   : 4;

    unsigned char    type        : 4;
    unsigned char    app_system  : 1;
    unsigned char    dpl         : 2;
    unsigned char    present     : 1;

    unsigned short  offset_16_31;
} CALLGATE_DESCRIPTOR;

#pragma pack()


#define MAX_IDT_ENTRIES 0xFF

#define NT_SYSTEM_SERVICE_INT 0x2e

/* prototypes */
int		HookInterrupts();
int		UnhookInterrupts();
void PrintDbg(ULONG);
void PrintNtCreateFile();
void PrintNtCreateDirectoryObject();
void PrintNtCreateEvent();
void PrintNtCreateEventPair();
void PrintObjectAttributes(int);

ULONG getPID();
NTSTATUS
sTraceDeviceControl(
    IN PFILE_OBJECT FileObject, 
    IN BOOLEAN Wait,
    IN PVOID InputBuffer, 
    IN ULONG InputBufferLength, 
    OUT PVOID OutputBuffer, 
    IN ULONG OutputBufferLength, 
    IN ULONG IoControlCode, 
    OUT PIO_STATUS_BLOCK IoStatus, 
    IN PDEVICE_OBJECT DeviceObject 
    ); 

#endif