#ifndef __ANNA_LOGGER_WIFI__
#define __ANNA_LOGGER_WIFI__


#include <stdbool.h>
#include "anna_logger_wifi_spi.h"
#include "CC3000HostDriver/cc3000_common.h"
#include <socket.h>
#include <wlan.h>
#include <netapp.h>

#define WIFI_ENABLE 1
#define WIFI_DISABLE 0
#define WIFI_STATUS_CONNECTED 1


typedef struct Result_Struct{
	uint32_t  num_networks;
	uint32_t  scan_status;
	uint8_t   rssiByte;
	uint8_t   Sec_ssidLen;
	uint16_t  time;
	uint8_t   ssid_name[32];
	uint8_t   bssid[6];
} ResultStruct_t;   /**!ResultStruct_t : data struct to store SSID scan results */

/* Enum for wlan_ioctl_statusget results */
typedef enum {
	STATUS_DISCONNECTED = 0,
	STATUS_SCANNING     = 1,
	STATUS_CONNECTING   = 2,
	STATUS_CONNECTED    = 3
} status_t;

#define WLAN_CONNECT_TIMEOUT 10000
//#define RXBUFFERSIZE 64
//#define TXBUFFERSIZE 32

typedef struct _anna_logger_wifi_client_t anna_logger_wifi_client_t;
typedef struct _anna_logger_wifi_server_t anna_logger_wifi_server_t;

struct _anna_logger_wifi_client_t {
};
struct _anna_logger_wifi_server_t {
};


bool wifi_init(uint8_t patch_request, bool use_smart_config_data, const char *device_name);

bool setup_anna_logger_wifi_client(anna_logger_wifi_client_t * client, int32_t tcp_socket, sockaddr * addr, uint32_t socketaddr_size);
bool setup_anna_logger_wifi_server(anna_logger_wifi_client_t * client, int32_t tcp_socket, sockaddr * addr, uint32_t socketaddr_size);

void reboot(bool patch_request);
void stop(void);
bool disconnect(void);
bool delete_profiles(void);
uint32_t ip_2_uint32(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

//Network
bool get_mac_address(uint8_t address[6]);
bool set_mac_address(uint8_t address[6]);
bool set_static_ip_address(uint32_t ip, uint32_t subnet_mask, uint32_t default_gateway, uint32_t dns_server);
bool set_dhcp();

//Wireless
bool connect_to_ap(const char *ssid, const char *key, uint8_t secmode, uint8_t attempts);
bool connect_to_secure(const char *ssid, const char *key, int32_t sec_mode);
bool connect_open(const char *ssid);
bool check_connected(void);
bool check_dhcp(void);
bool get_ip_address(uint32_t *retip, uint32_t *netmask, uint32_t *gateway, uint32_t *dhcp_serv, uint32_t *dns_serv);



bool connect_tcp(anna_logger_wifi_client_t * client, uint32_t dest_ip, uint16_t dest_port);
bool connect_udp(anna_logger_wifi_client_t * client, uint32_t dest_ip, uint16_t dest_port);


#ifndef CC3000_TINY_DRIVER
//If we have enough space for these functions
bool get_firmware_version(uint8_t *major, uint8_t *minor);
status_t get_status(void);
bool start_ssid_scan(uint32_t *index);
void stop_ssid_scan(void);
uint8_t get_next_ssid(uint8_t *rssi, uint8_t *sec_mode, char * ssidname);

bool list_ssid_results(void);
bool start_smart_config(const char *_device_name, const char * smart_config_key);
bool get_ip_config(tNetappIpconfigRetArgs *ipConfig);

uint16_t ping (uint32_t ip, uint8_t attepts, uint16_t timeout, uint8_t size);
uint16_t get_host_by_name(char * hostname, uint32_t *ip);

bool scan_ssids(uint32_t time);

#endif


#endif
