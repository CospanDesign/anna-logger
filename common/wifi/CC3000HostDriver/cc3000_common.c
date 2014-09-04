/*****************************************************************************
*
*  cc3000_common.c.c  - CC3000 Host Driver Implementation.
*  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/
//*****************************************************************************
//
//! \addtogroup common_api
//! @{
//
//*****************************************************************************

#include "cc3000_common.h"
#include "socket.h"
#include "wlan.h"
#include "evnt_handler.h"

//*****************************************************************************
//
//! void __error__(int8_t *pcFilename, uint32_t ulLine)
//!
//!  \param  pcFilename - file name, where error occurred
//!  \param  ulLine     - line number, where error occurred
//!
//!  \return none
//!
//!  \brief stub function for ASSERT macro
//
//*****************************************************************************
void
__error__(int8_t *pcFilename, uint32_t ulLine)
{
    //TODO full up function
}


void 
SimpleLinkWaitEvent(uint16_t usOpcode, void *pRetParams)
{
	//
	// In the blocking implementation the control to caller will be returned only after the 
	// end of current transaction
	//
	tSLInformation.usRxEventOpcode = usOpcode;
	hci_event_handler(pRetParams, 0, 0);
}


void 
SimpleLinkWaitData(uint8_t *pBuf, uint8_t *from, uint8_t *fromlen)
{
	//
	// In the blocking implementation the control to caller will be returned only after the 
	// end of current transaction, i.e. only after data will be received
	//
	tSLInformation.usRxDataPending = 1;
	hci_event_handler(pBuf, from, fromlen);
}


//This function is used for copying 32 bit to stream while converting to little endian format.
uint8_t* UINT32_TO_STREAM_f (uint8_t *p, uint32_t u32)
{
	*(p)++ = (uint8_t)(u32);
	*(p)++ = (uint8_t)((u32) >> 8);
	*(p)++ = (uint8_t)((u32) >> 16);
	*(p)++ = (uint8_t)((u32) >> 24);
	return p;
}
//This function is used for copying 16 bit to stream while converting to little endian format.
uint8_t* uint16_t_TO_STREAM_f (uint8_t *p, uint16_t u16)
{
	*(p)++ = (uint8_t)(u16);
	*(p)++ = (uint8_t)((u16) >> 8);
	return p;
}


//This function is used for copying received stream to 16 bit in little endian format.
uint16_t STREAM_TO_uint16_t_f(int8_t* p, uint16_t offset)
{
        return (uint16_t)((uint16_t)((uint16_t)(*(p + offset + 1)) << 8) + (uint16_t)(*(p + offset)));
}


//This function is used for copying received stream to 32 bit in little endian format.
uint32_t STREAM_TO_UINT32_f(int8_t* p, uint16_t offset)
{
        return (uint32_t)((uint32_t)((uint32_t)(*(p + offset + 3)) << 24) + (uint32_t)((uint32_t)(*(p + offset + 2)) << 16) + (uint32_t)((uint32_t)(*(p + offset + 1)) << 8) + (uint32_t)(*(p + offset)));
}



//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
