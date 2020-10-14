#ifndef __CAENVMELIB_H
#define __CAENVMELIB_H

#include <stdio.h>
// Rev. 2.5
#ifdef LINUX
        #include <stdint.h>
#endif
#include <stdlib.h>
#include <malloc.h>
#include "CAENVMEoslib.h"
#include "CAENVMEtypes.h"

#ifdef WIN32
        typedef INT8   int8_t;
        typedef UINT8  uint8_t;
        typedef INT16  int16_t;
        typedef UINT16 uint16_t;
        typedef INT32  int32_t;
        typedef UINT32 uint32_t;
        typedef INT64  int64_t;
        typedef UINT64 uint64_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/*
        CAENVME_BLTReadCycle
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle    : The handle that identifies the device.
                [in]  Address   : The VME bus address.
                [out] Buffer    : The data read from the VME bus.
                [in]  Size      : The size of the transfer in bytes.
                [in]  AM        : The address modifier (see CVAddressModifier enum).
                [in]  DW        : The data width.(see CVDataWidth enum).
                [out] count     : The number of bytes transferred.
        -----------------------------------------------------------------------------
        Returns:
                An error code about the execution of the function.
        -----------------------------------------------------------------------------
        Description:
                The function performs a VME block transfer read cycle. It can be used to
                perform MBLT transfers using 64 bit data width.
*/
CAENVME_API
CAENVME_BLTReadCycle(int32_t Handle, uint32_t Address, void *Buffer,
                     int Size, CVAddressModifier AM, CVDataWidth DW, int *count);


/*
        CAENVME_BLTReadAsync
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle    : The handle that identifies the device.
                [in]  Address   : The VME bus address.
                [out] Buffer    : The data read from the VME bus.
                [in]  Size      : The size of the transfer in bytes.
                [in]  AM        : The address modifier (see CVAddressModifier enum).
                [in]  DW        : The data width.(see CVDataWidth enum).
        -----------------------------------------------------------------------------
        Returns:
                An error code about the execution of the function.
        -----------------------------------------------------------------------------
        Description:
                The function starts a VME block transfer read cycle. It can be used
                to perform MBLT transfers using 64 bit data width.
                This function cannot be used with the V1718 USB bridge.
                This function can be used only on Linux platforms.
                Take care to call the CAENVME_BLTReadWait function before any other
                call to a CAENVMElib function with the same handle.
*/
CAENVME_API
CAENVME_BLTReadAsync(int32_t Handle, uint32_t Address, void *Buffer,
                     int Size, CVAddressModifier AM, CVDataWidth DW);


/*
        CAENVME_BLTReadWait
        -----------------------------------------------------------------------------
        Parameters:
                [in]  Handle    : The handle that identifies the device.
                [out] Count     : The number of bytes transferred.
        -----------------------------------------------------------------------------
        Returns:
                An error code about the execution of the function.
        -----------------------------------------------------------------------------
        Description:
                The function wait for the completion of a VME block transfer read
                cycle started with the CAENVME_BLTReadAsync function call.
                This function cannot be used with the V1718 USB bridge.
                This function can be used only on Linux platforms.
*/
CAENVME_API
CAENVME_BLTReadWait(int32_t Handle, int *Count);

#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __CAENVMELIB_H

