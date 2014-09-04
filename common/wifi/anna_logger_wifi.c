


#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <asf.h>

#include <anna_logger_wifi.h>
#include <anna_logger_wifi_spi.h>

#include <cc3000_common.h>
#include <wlan.h>
#include <hci.h>
#include <socket.h>
#include <security.h>
#include <evnt_handler.h>


#define DELAY(delay_timeout)  vTaskDelay(delay_timeout/portTICK_RATE_MS)
#define PRINT(output_string)  dbg_print_str(__func__ ); dbg_print_str(": "); dbg_print_str(output_string);
#define PRINTLN(output_string)  dbg_print_str(__func__ ); dbg_print_str(": "); dbg_print_str(output_string); dbg_print_str("\r\n");

#define CC3000_SUCCESS							(0)
#define CHECK_SUCCESS(func,Notify,errorCode)  {if ((func) != CC3000_SUCCESS) { PRINTLN(Notify); return errorCode;}}

#define SL_SIMPLE_CONFIG_PREFIX_LENGTH			(3)
#define WLAN_SMART_CONFIG_START_PARAMS_LEN		(4)

uint8_t profileArray[SMART_CONFIG_PROFILE_SIZE];

#define MAXSSID									(32)
#define MAXLENGTHKEY							(32)  /* Cleared for 32 bytes by TI engineering 29/08/13 */

#define MAX_SOCKETS								32  // can change this
bool closed_sockets[MAX_SOCKETS] = {false, false, false, false};


static const uint8_t IS_SMART_CONFIG_FINISHED = 0x01;
static const uint8_t IS_CONNECTED = 0x02;
static const uint8_t HAS_DHCP = 0x04;
static const uint8_t OK_TO_SHUT_DOWN = 0x08;
static const uint8_t INITIALIZED = 0x10;

volatile uint16_t cc3000_flags;

volatile uint16_t ul_socket;
char _cc3000_prefix[] = {'T', 'T', 'T'};

uint8_t ping_report_num;
netapp_pingreport_args_t ping_report;
ResultStruct_t SSIDScanResultBuff;


static void anna_logger_wifi_callback(int16_t event_type, int8_t * data, uint8_t length ){
	if (event_type == HCI_EVNT_WLAN_ASYNC_SIMPLE_CONFIG_DONE){
		cc3000_flags |= IS_SMART_CONFIG_FINISHED;
	}
	if (event_type == HCI_EVNT_WLAN_UNSOL_CONNECT){
		cc3000_flags |= IS_CONNECTED;
	}
	if (event_type == HCI_EVNT_WLAN_UNSOL_DISCONNECT){
		cc3000_flags &= ~(IS_CONNECTED | HAS_DHCP);
	}
	if (event_type == HCI_EVNT_WLAN_UNSOL_DHCP){
		cc3000_flags |= HAS_DHCP;
	}
	if (event_type == HCI_EVENT_CC3000_CAN_SHUT_DOWN){
		cc3000_flags |= OK_TO_SHUT_DOWN;
	}
	if (event_type == HCI_EVNT_WLAN_ASYNC_PING_REPORT){
		ping_report_num++;
		memcpy(&ping_report, data, length);
	}
	if (event_type == HCI_EVNT_BSD_TCP_CLOSE_WAIT) {
		uint8_t socketnum;
		socketnum = data[0];
		//PRINT_F("TCP Close wait #"); printDec(socketnum);
		if (socketnum < MAX_SOCKETS)
		closed_sockets[socketnum] = true;
	}
}
bool wifi_init(uint8_t patch_request, bool use_smart_config_data, const char *device_name){
  //Clear the status flags
	cc3000_flags = 0x00;
  //setup SPI
  spi_wifi_init();
  
  //Setup the wireless stack with all the function pointers
  PRINT("Initializing\n")
  wlan_init(  anna_logger_wifi_callback,
              sendWLFWPatch,
              sendDrivePatch,
              sendBootLoaderPatch,
              &ReadWlanInterruptPin,
              &WlanInterruptEnable,
              &WlanInterruptDisable,
              &WriteWlanPin);
		  
	
/*			  
extern void wlan_init(		
				tWlanCB	 				sWlanCB,
				tFWPatches				sFWPatches,
				tDriverPatches			sDriverPatches,
				
				tBootLoaderPatches		sBootLoaderPatches,
				tWlanReadInteruptPin	sReadWlanInterruptPin,
				tWlanInterruptEnable	sWlanInterruptEnable,
				tWlanInterruptDisable	sWlanInterruptDisable,
				tWriteWlanPin			sWriteWlanPin);

typedef int8_t *(*tFWPatches)(uint32_t *usLength);
typedef int8_t *(*tDriverPatches)(uint32_t *usLength);
typedef int8_t *(*tBootLoaderPatches)(uint32_t *usLength);
typedef void (*tWlanCB)(int16_t event_type, int8_t * data, uint8_t length );
typedef int16_t (*tWlanReadInteruptPin)(void);
typedef void (*tWlanInterruptEnable)(void);
typedef void (*tWlanInterruptDisable)(void);
typedef void (*tWriteWlanPin)(uint8_t val);

uint8_t * sendDrivePatch(uint32_t *length);
uint8_t * sendBootLoaderPatch(uint32_t *length);
uint8_t * sendWLFWPatch(uint32_t *length);

extern void SpiClose(void);
extern void SpiOpen(gcSpiHandleRx pfRxHandler);
extern int16_t SpiWrite(uint8_t *data_out, uint16_t length);
extern int16_t SpiRead(uint8_t *data_in, uint16_t length);
extern void SpiPauseSpi(void);
extern void SpiResumeSpi(void);
extern int16_t ReadWlanInterruptPin(void);
extern void WlanInterruptEnable(void);
extern void WlanInterruptDisable(void);
extern void WriteWlanPin(uint8_t val);



*/			  

  //Start the wireless stack
  wlan_start(patch_request);
  PRINT("Initializing\n")

  //Check if we need to erase the previously stored connection details
  if (!use_smart_config_data){
    wlan_ioctl_set_connection_policy(0, 0, 0);
    wlan_ioctl_del_profile(255);
  }
  else {
    //Autoconnect - device tries to connect to any AP it detects during scanning:
    // wlan_ioctl_set_connection_policy(1, 0, 0);
    
    //Fast Connect - device tries to reconnect to the last AP connected to:
    // wlan_ioctl_set_connection_policy(0, 1, 0);

    //User profiles - cc3000 tries to connect to an AP from the internal profile
    wlan_ioctl_set_connection_policy(0, 0, 1);
  }

  CHECK_SUCCESS(
		wlan_set_event_mask(
        HCI_EVNT_WLAN_UNSOL_INIT  |
        //HCI_EVNT_WLAN_ASYNC_PING_REPORT | //we want ping reports
        //HCI_EVNT_BSD_TCP_CLOSE_WAIT |
        //HCI_EVNT_WLAN_TX_COMPLETE),
        HCI_EVNT_WLAN_KEEPALIVE),
        "WLAN_ Set Event Mask Fail",
        false);

  cc3000_flags |= INITIALIZED;

  //Wait for re-connection if we're using SmartConfig data
  if (use_smart_config_data){
    uint32_t timeout = 0;
    while (!(cc3000_flags & IS_CONNECTED)){
//XXX: Check the wlan line??
      cc3k_int_poll();
      if (timeout > WLAN_CONNECT_TIMEOUT){
        //XXX: Failed to connect
        PRINT("Failed to connect (timeout)\n");
        return false;
      }
      //wait for 10 ms
      timeout += 10;
      DELAY(10);
    }
    DELAY(1000);
    if (cc3000_flags & HAS_DHCP){
      mdnsAdvertiser(1, (char *) device_name, strlen(device_name));
    } 
  }
  return true;
}
void reboot(bool patch_request){
  if ((cc3000_flags & INITIALIZED) == 0){
    //We're not even initialized, return
    return;
  }
  wlan_stop();
  DELAY(5000);
  wlan_start(patch_request);
}
void stop(void){
  wlan_stop();
}
bool disconnect(void){
  if ((cc3000_flags & INITIALIZED) == 0){
    //We're not even initialized, return
    return false;
  }
  long retval = wlan_disconnect();
  return retval != 0 ? false : true;
}
bool delete_profiles(void){
  if ((cc3000_flags & INITIALIZED) == 0){
    //We're not even initialized, return
    return false;
  }
  CHECK_SUCCESS(wlan_ioctl_set_connection_policy(0, 0, 0),
                "deleteProfiles connection failure",
                false);
  CHECK_SUCCESS(wlan_ioctl_del_profile(255),
                "Failed deleting profiles",
                false);
  return true;
}
uint32_t ip_2_uint32(uint8_t a, uint8_t b, uint8_t c, uint8_t d){
  uint32_t ip = a;
  ip <<= 8;
  ip |= b;
  ip <<= 8;
  ip |= c;
  ip <<= 8;
  ip |= d;

  return ip;
}

//Network
bool get_mac_address(uint8_t address[6]){
  if ((cc3000_flags & INITIALIZED) == 0){
    //We're not even initialized, return
    return false;
  }
  CHECK_SUCCESS(nvmem_read(NVMEM_MAC_FILEID, 6, 0, address),
                "Failed reading MAC address!",
                false);

  return true;

}
bool set_mac_address(uint8_t address[6]){
  if ((cc3000_flags & INITIALIZED) == 0){
    //We're not even initialized, return
    return false;
  }
  if (address[0] == 0){
    return false;
  }
  CHECK_SUCCESS(netapp_config_mac_adrress(address),
                "Failed setting MAC address!",
                false);

  wlan_stop();
  DELAY(200);
  wlan_start(0);
  return true;
}
bool set_static_ip_address(uint32_t ip, uint32_t subnet_mask, uint32_t default_gateway, uint32_t dns_server){
  ip = (ip >> 24) | ((ip >> 8) & 0x0000FF00L) | ((ip << 8) & 0x00FF0000L) | (ip << 24);
  subnet_mask = (subnet_mask >> 24) | ((subnet_mask >> 8) & 0x0000FF00L) | ((subnet_mask << 8) & 0x00FF0000L) | (subnet_mask << 24);
  default_gateway = (default_gateway >> 24) | ((default_gateway >> 8) & 0x0000FF00L) | ((default_gateway << 8) & 0x00FF0000L) | (default_gateway << 24);
  dns_server = (dns_server >> 24) | ((dns_server >> 8) & 0x0000FF00L) | ((dns_server << 8) & 0x00FF0000L) | (dns_server << 24);
  if (netapp_dhcp(&ip, &subnet_mask, &default_gateway, &dns_server) != 0) {
    return false;
  }
  wlan_stop();
  DELAY(200);
  wlan_start(0);
  return true;
}
bool set_dhcp(){
  return set_static_ip_address(0, 0, 0, 0);

}

//Wireless
bool connect_to_ap(const char *ssid, const char *key, uint8_t secmode, uint8_t attempts){
  if ((cc3000_flags & INITIALIZED) == 0){
    //We're not even initialized, return
    return false;
  }
  int16_t timer = 0;
  bool retry_forever = (attempts = 0);

  do {
    //see if this is the last retry
    if (!retry_forever){
      //we will not retry forever
      if (attempts == 0){
        return check_connected();
      } 
      attempts -= 1;
    }
    cc3k_int_poll();
    
    //MEME: not sure why this is absolutely required by the cc3k freaks if you don't, maybe there is an internal boot delay?
    scan_ssids(4000);
    //Wait for results
    DELAY(4500);
    scan_ssids(0);

    //Attempt to connect to the specified access point
    if ((secmode == 0) || strlen(key) == 0){
      //connect to an unsecured network
      if (!connect_open(ssid)){
//XXX: Failed to connect, need a way to indicate this to the user
      }
      continue;
    }
    else {
      //XXX: NOTE: Secure connections are not availbalbe in 'Tiny' mode!
#ifndef CC3000_TINY_DRIVER
      if (! connect_to_secure(ssid, key, secmode)){
//XXX: Failed to connect, need a way to indicate this to the user
      }
      continue;
#endif
    }
    timer = WLAN_CONNECT_TIMEOUT;
    while ((timer > 0) && !check_connected()){
      cc3k_int_poll();
      DELAY(10);
      timer -= 10;
    }
    if (timer <= 0){
//XXX: Timed out... need a way to indicate to the user
    }
  } while(!check_connected());

  return true;
}
bool connect_to_secure(const char *ssid, const char *key, int32_t secmode){
  if ((cc3000_flags & INITIALIZED) == 0){
    return false;
  }
  if ( (secmode < 0) || secmode > 3){
    PRINT("Security mode must be between 0 and 3\n");
    return false;
  }
  if (strlen(ssid) > MAXSSID){
    PRINT("SSID length must be < 32\n");
    return false;
  }
  if (strlen(key) > MAXLENGTHKEY){
    PRINT("Key length must be < 32");
    return false;
  }

  CHECK_SUCCESS(wlan_ioctl_set_connection_policy(0, 0, 0),
                "Failed setting the connection policy",
                false);
  DELAY(500);
  CHECK_SUCCESS(wlan_connect(	secmode,
								(int8_t *)ssid, 
								(int16_t) strlen(ssid),
								NULL,
								(uint8_t *)key, 
								(int16_t) strlen(key)),
                "SSID Connection Failed",
                false);

  //Wait for the HCI_EVNT_WLAN_UNSOL_CONNECT in the callback
  return true;
}
bool connect_open(const char *ssid){
  if ((cc3000_flags & INITIALIZED) == 0){
    return false;
  }
  #ifndef CC3000_TINY_DRIVER
  CHECK_SUCCESS(wlan_ioctl_set_connection_policy(0, 0, 0),
                "Failed to set connection policy",
                false);
  DELAY(500);
  CHECK_SUCCESS(wlan_connect(WLAN_SEC_UNSEC,
                             (char *) ssid,
							 strlen(ssid),
                             0,
							 NULL,
							 0),
                "SSID connection failed",
                false);
  #else
  wlan_connect(ssid, strlen(ssid));
  #endif
  return true;
}
bool check_connected(void){
  return ((cc3000_flags & IS_CONNECTED) == 0);
}
bool check_dhcp(void){
  return ((cc3000_flags & HAS_DHCP) == 0);
}
bool get_ip_address(uint32_t *retip, uint32_t *netmask, uint32_t *gateway, uint32_t *dhcp_serv, uint32_t *dns_serv){
  if ((cc3000_flags & INITIALIZED) == 0){
    return false;
  }
  if ((cc3000_flags & IS_CONNECTED) == 0){
    return false;
  }
  if ((cc3000_flags & HAS_DHCP) == 0){
    return false;
  }
  tNetappIpconfigRetArgs ipconfig;
  netapp_ipconfig(&ipconfig);

  // If byte 1 is 0 we don't have a valid address
  if (ipconfig.aucIP[3] == 0){
    return false;
  }

  memcpy(retip, ipconfig.aucIP, 4);
  memcpy(netmask, ipconfig.aucSubnetMask, 4);
  memcpy(gateway, ipconfig.aucDefaultGateway, 4);
  memcpy(dhcp_serv, ipconfig.aucDHCPServer, 4);
  memcpy(dns_serv, ipconfig.aucDNSServer, 4);

  return true;
}

//CC3000
bool check_smart_config_finished(void){
  return ((cc3000_flags & IS_SMART_CONFIG_FINISHED) > 0);
}
bool connect_tcp(anna_logger_wifi_client_t * client, uint32_t dest_ip, uint16_t dest_port){
  sockaddr    sockAddress;
  int32_t     tcp_socket;

  PRINT("Creating TCP socket\n")

  tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (-1 == tcp_socket){
    PRINT("Failed to open socket\n");
  }
  return false;

  //clear the previous closed event
  closed_sockets[tcp_socket] = false;

  //Try to open the socket

  memset(&sockAddress, 0x00, sizeof(sockAddress));

  sockAddress.sa_family = AF_INET;
  sockAddress.sa_data[0] = (dest_port & 0xFF00) >> 8;  //Set the port number
  sockAddress.sa_data[1] = (dest_port & 0x00FF);
  sockAddress.sa_data[2] = dest_ip >> 24;
  sockAddress.sa_data[3] = dest_ip >> 16;
  sockAddress.sa_data[4] = dest_ip >> 8;
  sockAddress.sa_data[5] = dest_ip;

  PRINT("Connected!");

  if (!setup_anna_logger_wifi_client(client, tcp_socket, &sockAddress, sizeof(sockAddress))){
    PRINT("Connection Error\n");
    closesocket(tcp_socket);
    return false;
  }

  return true;
}
bool connect_udp(anna_logger_wifi_client_t * client, uint32_t dest_ip, uint16_t dest_port){
  sockaddr socketAddress;
  int32_t udp_socket;

  //create the socket(s)
  //socket = SOCK_STREAM, SOCK_DGRAM, or SOCK_RAW
  //protocol = IPROTO_TCP, IPROTO_UDP, or IPROTO_RAW
  PRINT ("Creating UDP Socket\n");

  udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (-1 == udp_socket){
    PRINT("Failed to open socket\n");
    return false;
  }

  memset(&socketAddress, 0x00, sizeof(socketAddress));
  socketAddress.sa_family = AF_INET;
  socketAddress.sa_data[0] = (dest_port & 0xFF00) >> 8;
  socketAddress.sa_data[1] = (dest_port & 0x00FF);
  socketAddress.sa_data[2] = dest_ip >> 24;
  socketAddress.sa_data[3] = dest_ip >> 16;
  socketAddress.sa_data[4] = dest_ip >> 8;
  socketAddress.sa_data[5] = dest_ip;

  PRINT("Connected!\n");

  if (!setup_anna_logger_wifi_client(client, udp_socket, &socketAddress, sizeof(socketAddress))){
    PRINT("Connection error\n");
  }
  return true;
}
bool setup_anna_logger_wifi_client(anna_logger_wifi_client_t * client, int32_t tcp_socket, sockaddr * addr, uint32_t addr_size){
	return false;
}
bool setup_anna_logger_wifi_server(anna_logger_wifi_client_t * client, int32_t tcp_socket, sockaddr * addr, uint32_t addr_size){
	return false;
}
int32_t wlan_smart_config_start(uint32_t aes_flag){
	int32_t ret;
	uint8_t *ptr;
	uint8_t *args;
	
	ret = EFAIL;
	ptr = tSLInformation.pucTxCommandBuffer;
	args = (uint8_t *)(ptr + HEADERS_SIZE_CMD);
	
	//Fill in the HCI Packet Structure
	args = UINT32_TO_STREAM(args, aes_flag);
	
	hci_command_send(HCI_CMND_WLAN_IOCTL_SIMPLE_CONFIG_START,
					 ptr,
					 WLAN_SMART_CONFIG_START_PARAMS_LEN);
	SimpleLinkWaitEvent(HCI_CMND_WLAN_IOCTL_SIMPLE_CONFIG_START, &ret);
}
#ifndef CC3000_TINY_DRIVER
//If we have enough space for these functions
bool get_firmware_version(uint8_t *major, uint8_t *minor){
  uint8_t fwp_return[2];

  if ((cc3000_flags & INITIALIZED) == 0) return false;
  
  CHECK_SUCCESS(nvmem_read_sp_version(fwp_return),
                "Unable to read the firmware version",
                false);

  *major = fwp_return[0];
  *minor = fwp_return[1];
  return true;
}
status_t get_status(void){
  if ((cc3000_flags & INITIALIZED) == 0) return false;
  long results = wlan_ioctl_statusget();
  switch (results){
    case 1: 
      return STATUS_SCANNING;
    case 2:
      return STATUS_CONNECTING;
    case 3:
      return STATUS_CONNECTED;
    case 4:
    default:
      return STATUS_DISCONNECTED;
      break;
  }
}
bool start_ssid_scan(uint32_t *index){
  if ((cc3000_flags & INITIALIZED) == 0) return false;
  if (!scan_ssids(4000)){
    return false;
  }
  DELAY(4500);

  CHECK_SUCCESS(wlan_ioctl_get_scan_results(0, (uint8_t *) &SSIDScanResultBuff),
                "SSID scan failed!",
                false);

  *index = SSIDScanResultBuff.num_networks;
  return true;
}
void stop_ssid_scan(void){
  scan_ssids(0);
}
uint8_t get_next_ssid(uint8_t *rssi, uint8_t *secmode, char * ssidname){
  uint8_t valid = (SSIDScanResultBuff.rssiByte & (~0xFE));
  
  *rssi = (SSIDScanResultBuff.rssiByte >> 1);
  *secmode = (SSIDScanResultBuff.Sec_ssidLen & (~0xFC));
  uint8_t ssid_len = (SSIDScanResultBuff.Sec_ssidLen >> 2);
  strncpy(ssidname, (char *)SSIDScanResultBuff.ssid_name, ssid_len);
  ssidname[ssid_len] = 0;
  CHECK_SUCCESS(wlan_ioctl_get_scan_results(0, (uint8_t *) &SSIDScanResultBuff),
                "Problem with the SSID scan result",
                false);
  return valid;
}
bool list_ssid_results(void){
	return false;
}
bool start_smart_config(const char *device_name, const char * smart_config_key){
  bool enable_aes = smart_config_key != NULL;
  uint32_t timeout = 0;

  //Clear everythign except for the initialization flag
  cc3000_flags &= INITIALIZED;
  if ((cc3000_flags & INITIALIZED) == 0) return false;

  CHECK_SUCCESS(wlan_ioctl_set_connection_policy(WIFI_DISABLE, WIFI_DISABLE, WIFI_DISABLE),
                "Failed setting the connection policy",
                false);

  CHECK_SUCCESS(wlan_ioctl_del_profile(255),
                "Failed deleting existing profiles",
                false);

  PRINT("Disconnecting...");
  while (cc3000_flags & IS_CONNECTED){
    cc3k_int_poll();
    CHECK_SUCCESS(wlan_disconnect(),
                  "Failed to disconnect from AP",
                  false);
    DELAY(10);
    hci_unsolicited_event_handler();
  }
  PRINT("Finished\n");

  wlan_stop();
  DELAY(1000);
  wlan_start(0);


  //Create new entry for AES encryption key
  CHECK_SUCCESS(nvmem_create_entry(NVMEM_AES128_KEY_FILEID, 16),
                "Failed to create NVMEM entry",
                false);

  if (enable_aes){
    CHECK_SUCCESS(aes_write_key((uint8_t *) (smart_config_key)),
                  "Faield writing AES key",
                  false);
  }
  PRINT("Set Prefix...");
  CHECK_SUCCESS(wlan_smart_config_set_prefix((char *)&_cc3000_prefix),
                "Failed setting the SmartConfig prefix",
                false);
  PRINT("Success\n");

  PRINT("Starting smart config...");
  CHECK_SUCCESS(wlan_smart_config_start(enable_aes),
                "Failed starting smart config",
                 false);
  PRINT("Success\n");

  PRINT("Starting smart config");
  while ((cc3000_flags & IS_SMART_CONFIG_FINISHED) == 0){
    cc3k_int_poll();
    timeout += 10;
    if (timeout > 60000){ //wait about 60 seconds
      PRINT("Timed out!\n");
      return false;
    }
    DELAY(10);
    PRINT(".");
  }
  PRINT("Success!\n");
  PRINT("Got smart config data\n");

  
  if (enable_aes){
    PRINT("Enable AES... checking for wlan process...");
    CHECK_SUCCESS(wlan_smart_config_process(),
                  "wlan_smart_config_process Failed",
                  false);
    PRINT("Success");
  }

  CHECK_SUCCESS(wlan_ioctl_set_connection_policy(WIFI_DISABLE, WIFI_DISABLE, WIFI_ENABLE),
                "Failed setting connection policy",
                false);

  PRINT ("Reset the CC3000\n");
  wlan_stop();
  DELAY(1000);
  wlan_start(0);

  CHECK_SUCCESS(wlan_set_event_mask(HCI_EVNT_WLAN_KEEPALIVE |
                                    //HCI_EVNT_WLAN_ASYNC_PING_REPORT |
                                    //HCI_EVNT_WLAN_TX_COMPLETE |
                                    HCI_EVNT_WLAN_UNSOL_INIT),
                "Failed setting the event mask",
                false);

  //Wait for connection
  timeout = 0;
  while ((cc3000_flags & IS_CONNECTED) == 0){
    cc3k_int_poll();
    if (timeout > WLAN_CONNECT_TIMEOUT){ // ~20s
      PRINT("Timed out waiting for connect\n");
      return false;
    }
    timeout += 10;
    DELAY(10);
  }

  DELAY(1000);
  if (cc3000_flags & HAS_DHCP){
    mdnsAdvertiser(1, (char *) device_name, strlen(device_name));
  }
  return true;
}

bool get_ip_config(tNetappIpconfigRetArgs *ipConfig){
  if ((cc3000_flags & INITIALIZED) == 0){
    return false;
  }
  if ((cc3000_flags & IS_CONNECTED) == 0){
    return false;
  }
  if ((cc3000_flags & HAS_DHCP) == 0){
    return false;
  }
  netapp_ipconfig(ipConfig);
  return true;
}
uint16_t ping (uint32_t ip, uint8_t attempts, uint16_t timeout, uint8_t size){
  if ((cc3000_flags & INITIALIZED) == 0){
    return false;
  }
  if ((cc3000_flags & IS_CONNECTED) == 0){
    return false;
  }
  if ((cc3000_flags & HAS_DHCP) == 0){
    return false;
  }

  uint32_t rev_ip = (ip >> 24) | ((ip >> 8) & 0xFF00) | ((ip & 0xFF00) << 8) | (ip << 24);
  ping_report_num = 0;
  ping_report.packets_received = 0;

  //perform a ping request
  netapp_ping_send(&rev_ip, attempts, size, timeout);
  //wait 2 x the length of the timeout * attempts
  DELAY(timeout * attempts * 2);

  if (ping_report_num){
    return ping_report.packets_received;
  }
  return 0;
}
uint16_t get_host_by_name(char * hostname, uint32_t *ip){
  if ((cc3000_flags & INITIALIZED) == 0){
    return false;
  }
  if ((cc3000_flags & IS_CONNECTED) == 0){
    return false;
  }
  if ((cc3000_flags & HAS_DHCP) == 0){
    return false;
  }
  int16_t r = gethostbyname(hostname, strlen(hostname), ip);
  return r;
}
bool scan_ssids(uint32_t time){
  const uint32_t interval_time[16] = { 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
                                       2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000};

  //Check if the stack is initialized
  if ((cc3000_flags & INITIALIZED) == 0){
    //We're not initialized so don't even scan
    return false;
  }

  //If the time is 0 then this will just abort
  CHECK_SUCCESS(
    wlan_ioctl_set_scan_params(time, 20, 100, 5, 0x1FFF, -120, 0, 300, (uint32_t *) &interval_time),
    "Failed setting params for SSID scan",
    false);

  return true;
}

anna_logger_wifi_client_t * new_anna_logger_wifi_client(){
  anna_logger_wifi_client_t * alwc = NULL;
  alwc  = (anna_logger_wifi_client_t *) malloc(sizeof(anna_logger_wifi_client_t));
  if (alwc == NULL){
    PRINT("Error allocating space for anna_logger_wifi_client\n");
    return NULL;
  }
  //Initialize variables

  return alwc;
}
void delete_anna_logger_wifi_client(anna_logger_wifi_client_t * alwc){
  //close things up
  free(alwc);
}

#endif