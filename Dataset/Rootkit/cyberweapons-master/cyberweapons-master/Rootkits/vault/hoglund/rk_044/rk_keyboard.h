
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

/* Keyboard capture STUPH */

// Define the various device type values.  Note that values used by Microsoft
// Corporation are in the range 0-32767, and 32768-65535 are reserved for use
// by customers.
#define FILE_DEVICE_CTRL2CAP	0x00008300

//
// Define scan codes of interest here. For scan code tables, see 
// "The Undocumented PC", by Frank Van Gullwe, Addison-Wesley 1994.
//

#define KEY_UP         1
#define KEY_DOWN       0

#define LCONTROL       ((USHORT)0x1D)
#define CAPS_LOCK      ((USHORT)0x3A)


NTSTATUS cmdHookKeyboard(IN PDRIVER_OBJECT DriverObject);
NTSTATUS OnKbdReadComplete( IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp,
				IN PVOID Context );

extern PDEVICE_OBJECT gKbdTopOfStack;
extern PDEVICE_OBJECT gKbdHookDevice;

#endif
