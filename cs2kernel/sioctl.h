#pragma once
//
// Device type           -- in the "User Defined" range."
//
#define SIOCTL_TYPE 40000
//
// The IOCTL function codes from 0x800 to 0xFFF are for customer use.
//
#define DRIVER_FUNC_INSTALL 0x01
#define DRIVER_FUNC_REMOVE 0x02

#define DRIVER_NAME "SIoctl"
#define NT_DEVICE_NAME L"\\Device\\SIOCTL"
#define DOS_DEVICE_NAME L"\\DosDevices\\IoctlTest"

#define IOCTL_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0701, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_WRITE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0702, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _KERNEL_READ_REQUEST {
	ULONG pid;
	ULONG64 address;
	PVOID buffer;
	SIZE_T size;
} KERNEL_READ_REQUEST;
