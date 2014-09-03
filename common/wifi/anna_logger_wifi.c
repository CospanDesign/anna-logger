
#include <anna_logger_wifi_spi.h>
#include <cc3000_common.h>
#include <wlan.h>


static const uint8_t IS_SMART_CONFIG_FINISHED = 0x01;
static const uint8_t IS_CONNECTED = 0x02;
static const uint8_t HAS_DHCP = 0x04;
static const uint8_t OK_TO_SHUT_DOWN = 0x08;

uint8_t cc3000_flags;

static void anna_logger_wifi_callback(long event_type, char * data, uint8_t length){
	if (event_type == HCI_EVNT_WLAN_ASYNC_SIMPLE_CONFIG_DONE){
		cc3000_flags |= IS_SMART_CONFIG_FINISHED);
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
		pingReportnum++;
		memcpy(&pingReport, data, length);
	}
	if (event_type == HCI_EVNT_BSD_TCP_CLOSE_WAIT) {
		uint8_t socketnum;
		socketnum = data[0];
		//PRINT_F("TCP Close wait #"); printDec(socketnum);
		if (socketnum < MAX_SOCKETS)
		closed_sockets[socketnum] = true;
	}
}

void wifi_init(void){
	cc3000_flags = 0x00;
	
	
}