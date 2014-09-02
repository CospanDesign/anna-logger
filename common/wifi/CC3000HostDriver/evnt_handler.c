/*****************************************************************************
*
*  evnt_handler.c  - CC3000 Host Driver Implementation.
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
//! \addtogroup evnt_handler_api
//! @{
//
//*****************************************************************************

#include "cc3000_common.h"
#include "string.h"
#include "hci.h"
#include "evnt_handler.h"
#include "wlan.h"
#include "socket.h"
#include "netapp.h"
#include "spi.h"


uint32_t socket_active_status = SOCKET_STATUS_INIT_VAL;  

#define FLOW_CONTROL_EVENT_HANDLE_OFFSET		(0)
#define FLOW_CONTROL_EVENT_BLOCK_MODE_OFFSET	(1)
#define FLOW_CONTROL_EVENT_FREE_BUFFS_OFFSET	(2)
#define FLOW_CONTROL_EVENT_SIZE					(4)

#define BSD_RSP_PARAMS_SOCKET_OFFSET		(0)
#define BSD_RSP_PARAMS_STATUS_OFFSET		(4)

#define GET_HOST_BY_NAME_RETVAL_OFFSET	(0)
#define GET_HOST_BY_NAME_ADDR_OFFSET	(4)

#define ACCEPT_SD_OFFSET			(0)
#define ACCEPT_RETURN_STATUS_OFFSET	(4)
#define ACCEPT_ADDRESS__OFFSET		(8)

#define SL_RECEIVE_SD_OFFSET			(0)
#define SL_RECEIVE_NUM_BYTES_OFFSET		(4)
#define SL_RECEIVE__FLAGS__OFFSET		(8)


#define SELECT_STATUS_OFFSET			(0)
#define SELECT_READFD_OFFSET			(4)
#define SELECT_WRITEFD_OFFSET			(8)
#define SELECT_EXFD_OFFSET				(12)

#define NETAPP_IPCONFIG_IP_LENGTH				(4)

#define NETAPP_IPCONFIG_IP_OFFSET				(0)
#define NETAPP_IPCONFIG_SUBNET_OFFSET			(4)
#define NETAPP_IPCONFIG_GW_OFFSET				(8)
#define NETAPP_IPCONFIG_DHCP_OFFSET				(12)
#define NETAPP_IPCONFIG_DNS_OFFSET				(16)
#define NETAPP_IPCONFIG_MAC_OFFSET				(20)
#define NETAPP_IPCONFIG_SSID_OFFSET				(26)

#define NETAPP_IPCONFIG_MAC_LENGTH				(6)
#define NETAPP_IPCONFIG_SSID_LENGTH				(32)


#define NETAPP_PING_PACKETS_SENT_OFFSET			(0)
#define NETAPP_PING_PACKETS_RCVD_OFFSET			(4)
#define NETAPP_PING_MIN_RTT_OFFSET				(8)
#define NETAPP_PING_MAX_RTT_OFFSET				(12)
#define NETAPP_PING_AVG_RTT_OFFSET				(16)

#define GET_SCAN_RESULTS_TABlE_COUNT_OFFSET				(0)
#define GET_SCAN_RESULTS_SCANRESULT_STATUS_OFFSET		(4)
#define GET_SCAN_RESULTS_ISVALID_TO_SSIDLEN_OFFSET		(8)
#define GET_SCAN_RESULTS_FRAME_TIME_OFFSET				(10)
#define GET_SCAN_RESULTS_SSID_MAC_LENGTH				(38)


void hci_unsol_handle_patch_request(int8_t *event_hdr);


//*****************************************************************************
//
// Prototypes for the static functions
//
//*****************************************************************************
static long hci_event_unsol_flowcontrol_handler(int8_t *pEvent);


static void update_socket_active_status(int8_t *resp_params);

//*****************************************************************************
//
//!  ...
//!
//!  \param  string  pointer to the string contains parameters for IPERF
//!
//!  \return none    count of arguments in string
//!
//!  \brief
//
//*****************************************************************************
void hci_unsol_handle_patch_request(int8_t *event_hdr)
{
	int8_t *params = (int8_t *)(event_hdr) + HCI_EVENT_HEADER_SIZE;
	uint32_t ucLength = 0;
	int8_t *patch;
	
	switch (*params)
	{
		case HCI_EVENT_PATCHES_DRV_REQ:

			if (tSLInformation.sDriverPatches)
			{
				patch = tSLInformation.sDriverPatches(&ucLength);
				
				if (patch)
				{
					hci_patch_send(HCI_EVENT_PATCHES_DRV_REQ, tSLInformation.pucTxCommandBuffer, 
						patch, ucLength);
					return;
				}
			}

 			// Send 0 length Patches responce event
 			hci_patch_send(HCI_EVENT_PATCHES_DRV_REQ, tSLInformation.pucTxCommandBuffer, 0, 0);
			break;
			
		case HCI_EVENT_PATCHES_FW_REQ:
			if (tSLInformation.sFWPatches)
			{
				patch = tSLInformation.sFWPatches(&ucLength);

				//
				// Build and send a patch
				//
				if (patch)
				{
					hci_patch_send(HCI_EVENT_PATCHES_FW_REQ, tSLInformation.pucTxCommandBuffer, 
						patch, ucLength);
					return;
				}
			}

			// Send 0 length Patches responce event
 			hci_patch_send(HCI_EVENT_PATCHES_FW_REQ, tSLInformation.pucTxCommandBuffer, 0, 0);
			break;
			
		case HCI_EVENT_PATCHES_BOOTLOAD_REQ:
			if (tSLInformation.sBootLoaderPatches)
			{
				patch = tSLInformation.sBootLoaderPatches(&ucLength);

				if (patch)
				{
					hci_patch_send(HCI_EVENT_PATCHES_BOOTLOAD_REQ, tSLInformation.pucTxCommandBuffer, 
						patch, ucLength);
					return;
				}
			}

			// Send 0 length Patches responce event
			hci_patch_send(HCI_EVENT_PATCHES_BOOTLOAD_REQ, tSLInformation.pucTxCommandBuffer, 0, 0);
			break;
	}
}



//*****************************************************************************
//
//!  Event Handler
//!
//!  @param  buf     incoming data buffer
//!  @param  len     size of data buffer
//!
//!  @return         ESUCCESS if successful, EFAIL if an error occurred
//!
//!  @brief          Parse the incoming events packets and issues corresponding
//!                  event handler from global array of handlers pointers
//
//*****************************************************************************

uint8_t *
hci_event_handler(void *pRetParams, uint8_t *from, uint8_t *fromlen)
{
	uint8_t *pucReceivedData, ucArgsize;
    uint16_t usLength;
	uint8_t *pucReceivedParams;
	uint16_t usReceivedEventOpcode = 0;
	uint32_t retValue32;
	
	while (1)
	{
		if (tSLInformation.usEventOrDataReceived != 0)
		{				
			pucReceivedData = (tSLInformation.pucReceivedData);
			
			if (*pucReceivedData == HCI_TYPE_EVNT)
			{
				//
				// Event Received
				//
				STREAM_TO_UINT16((int8_t *)pucReceivedData, HCI_EVENT_OPCODE_OFFSET,usReceivedEventOpcode);
				pucReceivedParams = pucReceivedData + HCI_EVENT_HEADER_SIZE;
				//
				// In case unsolicited event received - here the handling finished
				//
				if (hci_unsol_event_handler((int8_t *)pucReceivedData) == 0)
				{
					STREAM_TO_UINT8(pucReceivedData, HCI_DATA_LENGTH_OFFSET, usLength);
					switch(usReceivedEventOpcode)
				    {		
				    	case HCI_CMND_READ_BUFFER_SIZE:
						{
							STREAM_TO_UINT8((int8_t *)pucReceivedParams, 0, tSLInformation.usNumberOfFreeBuffers);
							STREAM_TO_UINT16((int8_t *)pucReceivedParams, 1, tSLInformation.usSlBufferLength);
				    	}
						break;

						case HCI_CMND_WLAN_CONFIGURE_PATCH:
						case HCI_NETAPP_DHCP:
                        case HCI_NETAPP_PING_SEND:
                        case HCI_NETAPP_PING_STOP:
						case HCI_NETAPP_ARP_FLUSH:
						case HCI_NETAPP_SET_DEBUG_LEVEL:
						case HCI_NETAPP_SET_TIMERS:
						case HCI_EVNT_NVMEM_READ:
				        case HCI_EVNT_NVMEM_CREATE_ENTRY:
						case HCI_CMND_NVMEM_WRITE_PATCH:
						 case HCI_NETAPP_PING_REPORT:
							STREAM_TO_UINT8(pucReceivedData, HCI_EVENT_STATUS_OFFSET,*(uint8_t *)pRetParams);
							break;
							
						case HCI_CMND_SETSOCKOPT:
						case HCI_CMND_WLAN_CONNECT:
						case HCI_CMND_WLAN_IOCTL_STATUSGET:
						case HCI_EVNT_WLAN_IOCTL_ADD_PROFILE:
						case HCI_CMND_WLAN_IOCTL_DEL_PROFILE:
						case HCI_CMND_WLAN_IOCTL_SET_CONNECTION_POLICY:
						case HCI_CMND_WLAN_IOCTL_SET_SCANPARAM:
                        case HCI_CMND_WLAN_IOCTL_SIMPLE_CONFIG_START:
                        case HCI_CMND_WLAN_IOCTL_SIMPLE_CONFIG_STOP:
                        case HCI_CMND_WLAN_IOCTL_SIMPLE_CONFIG_SET_PREFIX:
						case HCI_CMND_EVENT_MASK:
						case HCI_EVNT_WLAN_DISCONNECT:
						case HCI_EVNT_SOCKET:
						case HCI_EVNT_BIND:
						case HCI_CMND_LISTEN:
			  			case HCI_EVNT_CLOSE_SOCKET:
                                                case HCI_EVNT_CONNECT:
												case HCI_EVNT_NVMEM_WRITE:

							STREAM_TO_UINT32((int8_t *)pucReceivedParams,0,*(uint32_t *)pRetParams);
							break;
						case HCI_EVNT_READ_SP_VERSION:
							STREAM_TO_UINT8(pucReceivedData, HCI_EVENT_STATUS_OFFSET,*(uint8_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 1;
							STREAM_TO_UINT32((int8_t *)pucReceivedParams, 0, retValue32);
							UINT32_TO_STREAM((uint8_t *)pRetParams, retValue32);
							
							break;
				        
                        case HCI_EVNT_BSD_GETHOSTBYNAME:
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,GET_HOST_BY_NAME_RETVAL_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,GET_HOST_BY_NAME_ADDR_OFFSET,*(uint32_t *)pRetParams);
                           
                            break;
				        
				        case HCI_EVNT_ACCEPT:
				        {
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,ACCEPT_SD_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,ACCEPT_RETURN_STATUS_OFFSET,*(uint32_t *)pRetParams);
                            pRetParams = ((int8_t *)pRetParams) + 4; 
							//This argurment returns in network order, therefore the use of memcpy.                                                      
							memcpy((uint8_t *)pRetParams, pucReceivedParams, sizeof(sockaddr));


				            break;
				        }
						
				        case HCI_EVNT_RECV:
				        case HCI_EVNT_RECVFROM:
				        {
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,SL_RECEIVE_SD_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,SL_RECEIVE_NUM_BYTES_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,SL_RECEIVE__FLAGS__OFFSET,*(uint32_t *)pRetParams);
							
                            if(((tBsdReadReturnParams *)pRetParams)->iNumberOfBytes == ERROR_SOCKET_INACTIVE)
                            {
                                set_socket_active_status(((tBsdReadReturnParams *)pRetParams)->iSocketDescriptor,
                                                         SOCKET_STATUS_INACTIVE);
                            }
 				            break;
				        }
				        
				        case HCI_EVNT_SELECT:
				        { 
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,SELECT_STATUS_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,SELECT_READFD_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,SELECT_WRITEFD_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,SELECT_EXFD_OFFSET,*(uint32_t *)pRetParams);

				            break;
				        }

						case HCI_CMND_GETSOCKOPT:
							STREAM_TO_UINT8(pucReceivedData, HCI_EVENT_STATUS_OFFSET,((tBsdGetSockOptReturnParams *)pRetParams)->iStatus);
							//This argurment returns in network order, therefore the use of memcpy.  
							memcpy((uint8_t *)pRetParams, pucReceivedParams, 4);
							break;

                    case HCI_CMND_WLAN_IOCTL_GET_SCAN_RESULTS:

							STREAM_TO_UINT32((int8_t *)pucReceivedParams,GET_SCAN_RESULTS_TABlE_COUNT_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;                                                        
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,GET_SCAN_RESULTS_SCANRESULT_STATUS_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;                                                        

                            STREAM_TO_UINT16((int8_t *)pucReceivedParams,GET_SCAN_RESULTS_ISVALID_TO_SSIDLEN_OFFSET,*(uint32_t *)pRetParams);
                            pRetParams = ((int8_t *)pRetParams) + 2;   

                            STREAM_TO_UINT16((int8_t *)pucReceivedParams,GET_SCAN_RESULTS_FRAME_TIME_OFFSET,*(uint32_t *)pRetParams);
                            pRetParams = ((int8_t *)pRetParams) + 2;  
 
                            memcpy((uint8_t *)pRetParams, (int8_t *)(pucReceivedParams + GET_SCAN_RESULTS_FRAME_TIME_OFFSET + 2), GET_SCAN_RESULTS_SSID_MAC_LENGTH);

                     




					
							break;

						case HCI_CMND_SIMPLE_LINK_START:
							break;

						case HCI_NETAPP_IPCONFIG:
							
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,NETAPP_IPCONFIG_IP_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
                                                        
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,NETAPP_IPCONFIG_SUBNET_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
                                                        
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,NETAPP_IPCONFIG_GW_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
                                                        
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,NETAPP_IPCONFIG_DHCP_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
                                                        
							STREAM_TO_UINT32((int8_t *)pucReceivedParams,NETAPP_IPCONFIG_DNS_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 4;
                                                        
                                                        STREAM_TO_UINT32((int8_t *)pucReceivedParams,NETAPP_IPCONFIG_MAC_OFFSET,*(uint32_t *)pRetParams);
							pRetParams = ((int8_t *)pRetParams) + 6;
                                                        
                                                        STREAM_TO_UINT32((int8_t *)pucReceivedParams,NETAPP_IPCONFIG_SSID_OFFSET,*(uint32_t *)pRetParams);
							
														
					}
				}

				if (usReceivedEventOpcode == tSLInformation.usRxEventOpcode)
				{
					tSLInformation.usRxEventOpcode = 0;
				}
			}
			else
			{
                        
                pucReceivedParams = pucReceivedData;
                STREAM_TO_UINT8((int8_t *)pucReceivedData, HCI_PACKET_ARGSIZE_OFFSET, ucArgsize);
                                
				STREAM_TO_UINT16((int8_t *)pucReceivedData, HCI_PACKET_LENGTH_OFFSET, usLength);
				
				//
				// Data received: note that the only case where from and from length are not null is in 
				// recv from, so fill the args accordingly
				//
				if (from)
				{
					STREAM_TO_UINT32((int8_t *)(pucReceivedData + HCI_DATA_HEADER_SIZE),
												BSD_RECV_FROM_FROMLEN_OFFSET,
												*(uint32_t *)fromlen);
					memcpy(from,
						   (pucReceivedData + HCI_DATA_HEADER_SIZE + BSD_RECV_FROM_FROM_OFFSET) ,
						   *fromlen);
				}

				memcpy(pRetParams, pucReceivedParams + HCI_DATA_HEADER_SIZE + ucArgsize,
							usLength - ucArgsize);
				
				tSLInformation.usRxDataPending = 0;
			}

			
			tSLInformation.usEventOrDataReceived = 0;
                        
                        
			SpiResumeSpi();

			//
			// Since we are going to TX - we need to handle this event after the ResumeSPi since we need interrupts
			// 
			if ((*pucReceivedData == HCI_TYPE_EVNT) && (usReceivedEventOpcode == HCI_EVNT_PATCHES_REQ))
			{
				hci_unsol_handle_patch_request((int8_t *)pucReceivedData);
			}

			if ((tSLInformation.usRxEventOpcode == 0) && (tSLInformation.usRxDataPending == 0))
			{
				return NULL;
			}
                        
                   
		}
	}

}

//*****************************************************************************
//
//!  Unsolicited events handler.
//!
//!  @param  event       event type
//!
//!  @param  arg         pointer to the buffer of probable arguments
//!
//!  @param  len         buffer length
//!
//!  @return             ESUCCESS if successful EAGAIN if in progress
//!                      EERROR if an error occurred
//!
//!  @brief              This function is Network Application events handler.
//
//*****************************************************************************
long
hci_unsol_event_handler(int8_t *event_hdr)
{
    int8_t * data = NULL;
    long event_type;

	STREAM_TO_UINT16(event_hdr, HCI_EVENT_OPCODE_OFFSET,event_type);
	
    if (event_type & HCI_EVNT_UNSOL_BASE)
    {
        switch(event_type)
        {
            case HCI_EVNT_DATA_UNSOL_FREE_BUFF:
            {
                hci_event_unsol_flowcontrol_handler(event_hdr);

                if (tSLInformation.NumberOfReleasedPackets==tSLInformation.NumberOfSentPackets)
                	    {
							if (tSLInformation.InformHostOnTxComplete)
							{
                		 	 	tSLInformation.sWlanCB(HCI_EVENT_CC3000_CAN_SHUT_DOWN, NULL, 0);
							}
                        }

                return 1;
            }
        }
    }

    if(event_type & HCI_EVNT_WLAN_UNSOL_BASE)
    {           
        switch(event_type)
        {
           case HCI_EVNT_WLAN_KEEPALIVE:
           case HCI_EVNT_WLAN_UNSOL_CONNECT:
           case HCI_EVNT_WLAN_UNSOL_DISCONNECT:
           case HCI_EVNT_WLAN_UNSOL_INIT:
           case HCI_EVNT_WLAN_ASYNC_SIMPLE_CONFIG_DONE:
                if( tSLInformation.sWlanCB )
                {
                  tSLInformation.sWlanCB(event_type, 0, 0);
                }
                break;
				
			case HCI_EVNT_WLAN_UNSOL_DHCP:
			{
				//tNetappDhcpParams params;
				
				uint8_t	params[NETAPP_IPCONFIG_MAC_OFFSET + 1];	// extra byte is for the status
				uint8_t *recParams = params;
				data = (int8_t*)(event_hdr) + HCI_EVENT_HEADER_SIZE;
				/*

				STREAM_TO_UINT32(data, NETAPP_IPCONFIG_IP_OFFSET, *(long *)params.aucIP);
				
				STREAM_TO_UINT32(data, NETAPP_IPCONFIG_SUBNET_OFFSET, *(long *)params.aucSubnetMask);
				
				STREAM_TO_UINT32(data, NETAPP_IPCONFIG_GW_OFFSET, *(long *)params.aucDefaultGateway);
				
				STREAM_TO_UINT32(data, NETAPP_IPCONFIG_DHCP_OFFSET, *(long *)params.aucDHCPServer);

				STREAM_TO_UINT32(data, NETAPP_IPCONFIG_DNS_OFFSET, *(long *)params.aucDNSServer);
				*/

				//Read IP address
				STREAM_TO_STREAM(data, recParams, NETAPP_IPCONFIG_IP_LENGTH);
				data += 4;
				//Read subnet
				STREAM_TO_STREAM(data, recParams, NETAPP_IPCONFIG_IP_LENGTH);
				data += 4;
				//Read default GW
				STREAM_TO_STREAM(data, recParams, NETAPP_IPCONFIG_IP_LENGTH);
				data += 4;
				//Read DHCP server
				STREAM_TO_STREAM(data, recParams, NETAPP_IPCONFIG_IP_LENGTH);
				data += 4;
				//Read DNS server
				STREAM_TO_STREAM(data, recParams, NETAPP_IPCONFIG_IP_LENGTH);
				// read the status
				STREAM_TO_UINT8(event_hdr, HCI_EVENT_STATUS_OFFSET, *recParams);				

				if( tSLInformation.sWlanCB )
				{
					tSLInformation.sWlanCB(event_type, (int8_t *)params, sizeof(params));
				}
			}
			break;
				
			case HCI_EVNT_WLAN_ASYNC_PING_REPORT:
			{
				netapp_pingreport_args_t params;
				
				data = (int8_t*)(event_hdr) + HCI_EVENT_HEADER_SIZE;

				STREAM_TO_UINT32(data, NETAPP_PING_PACKETS_SENT_OFFSET, params.packets_sent);
				
				STREAM_TO_UINT32(data, NETAPP_PING_PACKETS_RCVD_OFFSET, params.packets_received);
				
				STREAM_TO_UINT32(data, NETAPP_PING_MIN_RTT_OFFSET, params.min_round_time);
				
				STREAM_TO_UINT32(data, NETAPP_PING_MAX_RTT_OFFSET, params.max_round_time);
				
				STREAM_TO_UINT32(data, NETAPP_PING_AVG_RTT_OFFSET, params.avg_round_time);

				if( tSLInformation.sWlanCB )
				{
					tSLInformation.sWlanCB(event_type, (int8_t *)&params, sizeof(params));
				}
			}
			break;
		    default: /* 'default' case which means "event not supported" */
				return (0);
        }
        return(1);
    }

	if ((event_type == HCI_EVNT_SEND) || (event_type == HCI_EVNT_SENDTO)|| (event_type == HCI_EVNT_WRITE))
	{
		//
		// The only synchronous event that can come from SL device in form of command complete is
		// "Command Complete" on data sent, in case SL device was unable to transmit
		//
		STREAM_TO_UINT8(event_hdr, HCI_EVENT_LENGTH_OFFSET, tSLInformation.slTransmitDataError);
        update_socket_active_status(M_BSD_RESP_PARAMS_OFFSET(event_hdr));

		return (1);
	}
	
    return(0);
}

//*****************************************************************************
//
//!  Event Handler for Unsolicited Events
//!
//!  @param None
//!
//!  @return         ESUCCESS if successful, EFAIL if an error occurred
//!
//!  @brief          Parse the incoming events packets and issues corresponding
//!                  event handler from global array of handlers pointers
//
//*****************************************************************************
long
hci_unsolicited_event_handler(void)
{
	uint32_t   res = 0;
	uint8_t *pucReceivedData;
	
	if (tSLInformation.usEventOrDataReceived != 0)
	{
		pucReceivedData = (tSLInformation.pucReceivedData);
			
		if (*pucReceivedData == HCI_TYPE_EVNT)
		{			
			//
			// In case unsolicited event received - here the handling finished
			//
			if (hci_unsol_event_handler((int8_t *)pucReceivedData) == 1)
			{
				//
				// There was an un-solicited event received - we can release the buffer and clean the
				// event received 
				//
				tSLInformation.usEventOrDataReceived = 0;
							
				res = 1;

				SpiResumeSpi();
			}
		}
	}

	return res;
}

void set_socket_active_status(long Sd, long Status)
{
    if(M_IS_VALID_SD(Sd) && M_IS_VALID_STATUS(Status))
    {
        socket_active_status &= ~(1 << Sd);      /* clean socket's mask */
        socket_active_status |= (Status << Sd); /* set new socket's mask */
    }
}


//*****************************************************************************
//
//!  ...
//!
//!  \param  string  pointer to the string contains parameters for IPERF
//!
//!  \return none    count of arguments in string
//!
//!  \brief
//
//*****************************************************************************
long
hci_event_unsol_flowcontrol_handler(int8_t *pEvent)
{
    
    long temp, value;
     uint16_t i;
     uint16_t  pusNumberOfHandles=0;
    int8_t *pReadPayload;
    
  

    STREAM_TO_UINT16((int8_t *)pEvent,HCI_EVENT_HEADER_SIZE,pusNumberOfHandles);

  
    pReadPayload = ((int8_t *)pEvent +
                   HCI_EVENT_HEADER_SIZE + sizeof(pusNumberOfHandles));
   
    temp = 0;
    
    for(i = 0; i < pusNumberOfHandles ; i++)
    {
        STREAM_TO_UINT16(pReadPayload, FLOW_CONTROL_EVENT_FREE_BUFFS_OFFSET, value);
		temp += value;
        pReadPayload += FLOW_CONTROL_EVENT_SIZE;  
    }
        
    tSLInformation.usNumberOfFreeBuffers += temp;
    tSLInformation.NumberOfReleasedPackets += temp;

    return(ESUCCESS);
}



long
get_socket_active_status(long Sd)
{
    if(M_IS_VALID_SD(Sd))
    {
        return (socket_active_status & (1 << Sd)) ? SOCKET_STATUS_INACTIVE : SOCKET_STATUS_ACTIVE;
    }
    return SOCKET_STATUS_INACTIVE;
}

void
update_socket_active_status(int8_t *resp_params)
{
	long status, sd;

	STREAM_TO_UINT32(resp_params, BSD_RSP_PARAMS_SOCKET_OFFSET,sd);
	STREAM_TO_UINT32(resp_params, BSD_RSP_PARAMS_STATUS_OFFSET,status);
	
    if(ERROR_SOCKET_INACTIVE == status)
    {
        set_socket_active_status(sd, SOCKET_STATUS_INACTIVE);
    }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
