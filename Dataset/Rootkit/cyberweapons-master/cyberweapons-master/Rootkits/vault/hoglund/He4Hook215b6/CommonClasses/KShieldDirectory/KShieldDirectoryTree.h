#ifndef __KSHIELDDIRECTORY_TREE_H
 #define __KSHIELDDIRECTORY_TREE_H

#ifndef __TEST_WIN32
 extern "C"
 {
  #include "ntddk.h"
 }
 
 #include "../Include/KNew.h"
 #include "../Include/KTypes.h"
 #include "../KStdLib/krnlstdlib.h"
#else
 #include <windows.h>
#endif //__TEST_WIN32

#include "../KSpinSynchroObject/KSpinSynchroObject.h"
//#include "../KNativeSynchroObject/KNativeSynchroObject.h"
#include "KShieldDirectory.h"

class KShieldDirectoryTree;
//*******************************************************************//

class KShieldDirectoryTree
{
  public:
   explicit
   KShieldDirectoryTree();
   virtual ~KShieldDirectoryTree();

   // �� ���� ���� �-�� pUserContext ������ ���� ������� �� NULL,
   // ���� ���� ��� �� �� �����, ��� ���������� ����� �������� ��������,
   // ������� �� �������� �� �������������.
   // ������:
   //  ��������� "\\Device\\Harddisk0\\Partition0\\i386"
   //  � ������ ����� ������� ��������� ����
   //   Device
   //   Harddisk0
   //   Partition0
   //   i386
   //  ��� ��� �� ���� ����� ����� i386, pUserContext ����� ����� 0,
   //  � ��� i386 - ���� ��� �� ��������.
   BOOLEAN                Add(PWSTR pwszFullPath, PVOID pUserContext);
   BOOLEAN                Remove(PWSTR pwszFullPath, PVOID* ppUserContext);
   KShieldDirectory*      Find(PWSTR pwszFullPath, PVOID* ppUserContext, PVOID* ppParentUserContext = NULL);
   KShieldDirectory*      FindMatch(PWSTR pwszFullPath, PVOID* ppUserContext, PVOID* ppParentUserContext = NULL);
   KShieldDirectory*      FindMatchRest(PWSTR pwszFullPath, PVOID* ppUserContext, PVOID* ppParentUserContext = NULL);

  //protected:
   // ��� �-�� ������� ���������� ����� ���������, ������ ��� ���� ��
   // �������� � ������ ������ KShieldDirectory*, �� ������ ����� � ���������
   // � �������� pRoot ����� ��������� ����� �������� � �������, ���� ���-��
   // ����� ������� ���� ����. ��� ���� ���� �-� �����������: 
   // if (pRoot == NULL) pRoot = m_pRoot;
   KShieldDirectory*      FindFrom(KShieldDirectory* pRoot, PWSTR pwszFullPath, KShieldDirectory** ppParentDir = NULL);
   KShieldDirectory*      FindMatchFrom(KShieldDirectory* pRoot, PWSTR pwszFullPath, KShieldDirectory** ppParentDir = NULL);
   KShieldDirectory*      FindMatchRestFrom(KShieldDirectory* pRoot, PWSTR pwszFullPath, KShieldDirectory** ppParentDir = NULL);

  private:
   KShieldDirectoryTree(const KShieldDirectoryTree&);
   KShieldDirectoryTree& operator=(const KShieldDirectoryTree& right);

  protected:
   KShieldDirectory*      m_pRoot;

   KSpinSynchroObject     m_KSynchroObject;
   //KNativeSynchroObject   m_KSynchroObject;
};


#endif //__KSHIELDDIRECTORY_TREE_H