#ifndef __MEMORY_MANAGER_H
 #define __MEMORY_MANAGER_H
//
// ������ ��� ����� ������������ �� IRQL <= DISPATCH_LEVEL,
// ���� __KHEAP_LOCK_MUTEX �� ���������, ����� IRQL == PASSIVE_LEVEL
//

//#define __KHEAP_WIN32
#ifndef __KHEAP_WIN32
// #define __KHEAP_LOCK_MUTEX
#endif //__KHEAP_WIN32

#ifndef __KHEAP_WIN32
extern "C"
{
 #include "ntddk.h"
}

 #include "stdio.h"
#else
 #include <windows.h>
 #include <stdio.h>
#endif //!__KHEAP_WIN32

//
// ��� KernelMode ������
//
#ifdef __KHEAP_WIN32
 #define MmIsAddressValid(Array) (!(Array==NULL))
 #define KSPIN_LOCK               CRITICAL_SECTION*
 #define KIRQL                    DWORD
 #define ULONG                    DWORD
 #define BOOLEAN                  BOOL
#endif //__KHEAP_WIN32
//
//
//

#define BLOCK_STATE_FREE          0x00000000
#define BLOCK_STATE_IN_USE        0x00000001
#define BLOCK_STATE_KHEAP         0x80000000 // ���� �������� ���������� ����

#pragma pack(push)
#pragma pack(1)
typedef struct tag_MEMORY_BLOCK
{
  struct tag_MEMORY_BLOCK*  m_pNextBlock; // ��������� ���������� � ���� ���� 
  struct tag_MEMORY_BLOCK*  m_pBaseBlock; // ��������� ���� ��� ���������� ���� (��� BLOCK_STATE_KHEAP)
  ULONG       m_dwStates;
  ULONG       m_dwSize;                   // ������ ����� m_pArray
  #ifndef __KHEAP_LOCK_MUTEX
  KSPIN_LOCK  m_Lock;                     // ������������ ��� BLOCK_STATE_KHEAP
  //KIRQL       m_OldIrql;                // �.�. KIRQL = UCHAR, �� ��� ������������ ��
  ULONG       m_OldIrql_Align;            // 8-�� �������� ������� ������� ��� ULONG (������������ ��� BLOCK_STATE_KHEAP)
  #else
  PKMUTEX     m_Lock;
  ULONG       m_Align;                    // ������������ �� 8 ����.
  #endif //__KHEAP_LOCK_MUTEX
  CHAR        m_pArray[1];
} MEMORY_BLOCK, *PMEMORY_BLOCK;
#pragma pack(pop)

#define SIZEOF_MEMORY_BLOCK      (sizeof(MEMORY_BLOCK))
#define SIZEOF_MEMORY_BLOCK_REAL (SIZEOF_MEMORY_BLOCK-sizeof(CHAR))

typedef PMEMORY_BLOCK KHEAP;

KHEAP    KHeapCreate(ULONG dwSize);
BOOLEAN  KHeapDestroy(KHEAP hKHeap);

BOOLEAN  CreateDefaultHeap(ULONG dwSize);
VOID     DestroyDefaultHeap(VOID);

PVOID    AllocatePoolFromKHeap(KHEAP hKHeap, ULONG dwSize);
VOID     FreePoolToKHeap(KHEAP hKHeap, PVOID pArray);
PVOID    _AllocatePoolFromKHeap(KHEAP hKHeap, ULONG dwSize); // Optimize for min(fragmentation_heap)
ULONG    KHeapGetSizeSystemMemory(KHEAP hKHeap);
ULONG    KHeapGetSizeUsageMemory(KHEAP hKHeap);
KHEAP    KGetDefaultHeap(VOID);
#endif //__MEMORY_MANAGER_H
