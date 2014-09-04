#ifndef __ANNA_LOGGER_WIFI__
#define __ANNA_LOGGER_WIFI__


#include "anna_logger_wifi_spi.h"
#include "CC3000HostDriver/cc3000_common.h"
#include <wlan.h>
#include <netapp.h>
#include <stdbool.h>

void wifi_init(void);

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

//CC33000
bool check_smart_config_finished(void);

void * connect_tcp(uint32_t dest_ip, uint16_t dest_port);
void * connect_udp(uint32_t dest_ip, uint16_t dest_port);


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
