#include <ntifs.h>
#include <ntddk.h>
#include <string.h>
#include "sioctl.h"
#include "mem.h"


DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD SioctlUnloadDriver;


NTSTATUS NTAPI MmCopyVirtualMemory
(
	PEPROCESS SourceProcess,
	PVOID SourceAddress,
	PEPROCESS TargetProcess,
	PVOID TargetAddress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode,
	PSIZE_T ReturnSize
);


NTSTATUS SioctlCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	PAGED_CODE();

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

VOID SioctlUnloadDriver(_In_ PDRIVER_OBJECT DriverObject)
{
	PDEVICE_OBJECT deviceObject = DriverObject->DeviceObject;
	UNICODE_STRING uniWin32NameString;

	PAGED_CODE();

	// Create counted string version of our Win32 device name.
	RtlInitUnicodeString(&uniWin32NameString, DOS_DEVICE_NAME);

	// Delete the link from our device name to a name in the Win32 namespace.
	IoDeleteSymbolicLink(&uniWin32NameString);

	if (deviceObject != NULL)
	{
		IoDeleteDevice(deviceObject);
	}
}

NTSTATUS SioctlDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PIO_STACK_LOCATION irpSp;
	NTSTATUS ntStatus = STATUS_SUCCESS;
	KERNEL_READ_REQUEST* kernelReadRequest = NULL;
	PEPROCESS process = NULL;

	UNREFERENCED_PARAMETER(DeviceObject);

	PAGED_CODE();

	irpSp = IoGetCurrentIrpStackLocation(Irp);

	DbgPrint("Received IOCTL: %x\n", irpSp->Parameters.DeviceIoControl.IoControlCode);

	// Determine which I/O control code was specified.
	switch (irpSp->Parameters.DeviceIoControl.IoControlCode)
	{
		case IOCTL_READ_REQUEST:
			kernelReadRequest = (KERNEL_READ_REQUEST*)Irp->AssociatedIrp.SystemBuffer;

			if (kernelReadRequest == NULL || 
				kernelReadRequest->size == 0 || 
				kernelReadRequest->address == 0 || 
				kernelReadRequest->buffer == NULL) 
			{
				DbgPrint("Invalid parameters\n");
				ntStatus = STATUS_INVALID_PARAMETER;
				break;
			}

			ntStatus = PsLookupProcessByProcessId((HANDLE)kernelReadRequest->pid, &process);
			if (!NT_SUCCESS(ntStatus))
			{
				DbgPrint("Failed to lookup process: %x\n", ntStatus);
				break;
			}

			PEPROCESS targetProcess = PsGetCurrentProcess();

			if (!targetProcess)
			{
				DbgPrint("Failed to get target process\n");
				ObDereferenceObject(process);
				ntStatus = STATUS_UNSUCCESSFUL;
				break;
			}

			SIZE_T bytesCopied = 0;
			ntStatus = MmCopyVirtualMemory(
				process,
				(PVOID)kernelReadRequest->address,
				targetProcess,
				kernelReadRequest->buffer,
				kernelReadRequest->size,
				KernelMode,
				&bytesCopied
			);

			if (!NT_SUCCESS(ntStatus))
			{
				DbgPrint("Failed to copy memory: %x\n", ntStatus);
				ntStatus = STATUS_UNSUCCESSFUL;
			}

			ObDereferenceObject(process);
			break;
		default:
			// The specified I/O control code is unrecognized by this driver.
			DbgPrint("Unrecognized IOCTL: %x\n", irpSp->Parameters.DeviceIoControl.IoControlCode);
			ntStatus = STATUS_INVALID_DEVICE_REQUEST;
			break;
	}

	Irp->IoStatus.Status = ntStatus;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return ntStatus;
}

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	NTSTATUS ntStatus;
	UNICODE_STRING ntUnicodeString;     // NT Device Name "\Device\SIOCTL"
	UNICODE_STRING ntWin32NameString;   // Win32 Name "\DosDevices\IoctlTest"
	PDEVICE_OBJECT deviceObject = NULL; // ptr to device object

	UNREFERENCED_PARAMETER(RegistryPath);

	RtlInitUnicodeString(&ntUnicodeString, NT_DEVICE_NAME);

	ntStatus = IoCreateDevice(
		DriverObject,            // Our Driver Object
		0,                       // We don't use a device extension
		&ntUnicodeString,        // Device name "\Device\SIOCTL"
		FILE_DEVICE_UNKNOWN,     // Device type
		FILE_DEVICE_SECURE_OPEN, // Device characteristics
		FALSE,                   // Not an exclusive device
		&deviceObject);          // Returned ptr to Device Object

	if (!NT_SUCCESS(ntStatus))
	{
		//SIOCTL_KDPRINT(("Couldn't create the device object\n"));
		return ntStatus;
	}

	// Initialize the driver object with this driver's entry points.
	DriverObject->MajorFunction[IRP_MJ_CREATE] = SioctlCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = SioctlCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = SioctlDeviceControl;
	DriverObject->DriverUnload = SioctlUnloadDriver;

	// Initialize a Unicode String containing the Win32 name for our device.
	RtlInitUnicodeString(&ntWin32NameString, DOS_DEVICE_NAME);

	// Create a symbolic link between our device name  and the Win32 name
	ntStatus = IoCreateSymbolicLink(&ntWin32NameString, &ntUnicodeString);

	if (!NT_SUCCESS(ntStatus))
	{
		// Delete everything that this routine has allocated.
		// SIOCTL_KDPRINT(("Couldn't create symbolic link\n"));
		IoDeleteDevice(deviceObject);
	}

	return ntStatus;
}
