/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <port.h>
#include <sd_mmc_mem.h>
#include <anna_logger_wifi.h>
#include <cc3000_common.h>
#include <ap.h>
#include <wlan.h>

xQueueHandle * WIFI_queue;

static void fatfs_task(void *params)
{
	char test_file_name[] = "0:sd_mmc_test.txt";
	Ctrl_status status;
	FRESULT res;
	FATFS fs;
	FIL file_object;

	// Wait card present and ready
	
	do {
		status = sd_mmc_test_unit_ready(0);
		if (CTRL_FAIL == status) {
			//dbg_print_str("Card install FAIL\n\r");
			//dbg_print_str("Please unplug and re-plug the card.\n\r");
			while (CTRL_NO_PRESENT != sd_mmc_check(0)) {
				vTaskDelay(10 / portTICK_RATE_MS);
			}
		}
	} while (CTRL_GOOD != status);

	//dbg_print_str("Mount disk (f_mount)...\r\n");
	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
	if (FR_INVALID_DRIVE == res) {
		//dbg_print_str("Failed: ");
		//dbg_print_hexint(res);
		//dbg_print_str("\r\n");
		
		goto main_end_of_test;
	}
	//dbg_print_str("[OK]\r\n");

	//dbg_print_str("Create a file (f_open)...\r\n");
	test_file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';

	res = f_open(&file_object,
	(char const *)test_file_name,
	FA_CREATE_ALWAYS | FA_WRITE);
	if (res != FR_OK) {
		//dbg_print_str("Failed: ");
		//dbg_print_hexint(res);
		//dbg_print_str("\r\n");
		goto main_end_of_test;
	}
	//dbg_print_str("[OK]\r\n");
	//dbg_print_str("Write to test file (f_puts)...\r\n");
	
	if (0 == f_puts("Test SD/MMC stack\n", &file_object)) {
		f_close(&file_object);
		//dbg_print_str("Failed: ");
		//dbg_print_hexint(res);
		//dbg_print_str("\r\n");
		goto main_end_of_test;
	}

	//dbg_print_str("[OK]\r\n");
	f_close(&file_object);
	//dbg_print_str("Test is successful.\n\r");

main_end_of_test:
	//dbg_print_str("Done!.\n\r");
	while (1) {

		//dbg_print_str("Main task loop executing\r\n");
		//dbg_sprint_hexint(&main_string[23], main_counter++);
		//dbg_print_str(main_string);
				
		port_pin_toggle_output_level(ANNA_LED_GRN_PIN);
     	vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

static void wifi_task(void *params)
{
	uint32_t ip;
	uint32_t netmask;
	uint32_t gateway;
	uint32_t dhcp_server;
	uint32_t dns_server;
	
	wifi_init(0, false, "WIFI");
	
	dbg_print_str("Attempt to connect to AP: ");
	dbg_print_str(AP_NAME);
	dbg_print_str("...");
	
	if (!connect_to_ap(AP_NAME, AP_PASSWORD, WLAN_SEC_WPA2, (uint8_t )3)){
		dbg_print_str("Failed!\r\n");
		while (1) {
			port_pin_toggle_output_level(ANNA_LED_ORG_PIN);
			vTaskDelay(100 / portTICK_RATE_MS);
		};
	}
	dbg_print_str("Connected!\r\n");
	
	dbg_print_str("Request DHCP");
	while (!check_dhcp()){
		vTaskDelay(100);
		dbg_print_str(".");
	}
	dbg_print_str("Found!\r\n");
	dbg_print_str("Details:\r\n");
	get_ip_address(&ip, &netmask, &gateway, &dhcp_server, &dns_server);
	dbg_print_str("IP: ");
	dbg_print_str((uint8_t) ip >> 24);
	dbg_print_str(".");
	dbg_print_str((uint8_t) ip >> 16);
	dbg_print_str(".");
	dbg_print_str((uint8_t) ip >> 8);
	dbg_print_str(".");
	dbg_print_str((uint8_t) ip);
	dbg_print_str("\r\n");

	dbg_print_str("Netmask: ");
	dbg_print_str((uint8_t) netmask >> 24);
	dbg_print_str(".");
	dbg_print_str((uint8_t) netmask >> 16);
	dbg_print_str(".");
	dbg_print_str((uint8_t) netmask >> 8);
	dbg_print_str(".");
	dbg_print_str((uint8_t) netmask);
	dbg_print_str("\r\n");
	
	dbg_print_str("Gateway: ");
	dbg_print_str((uint8_t) gateway >> 24);
	dbg_print_str(".");
	dbg_print_str((uint8_t) gateway >> 16);
	dbg_print_str(".");
	dbg_print_str((uint8_t) gateway >> 8);
	dbg_print_str(".");
	dbg_print_str((uint8_t) gateway);
	dbg_print_str("\r\n");
		
	dbg_print_str("DHCP Server: ");
	dbg_print_str((uint8_t) dhcp_server >> 24);
	dbg_print_str(".");
	dbg_print_str((uint8_t) dhcp_server >> 16);
	dbg_print_str(".");
	dbg_print_str((uint8_t) dhcp_server >> 8);
	dbg_print_str(".");
	dbg_print_str((uint8_t) dhcp_server);
	dbg_print_str("\r\n");
	
	dbg_print_str("DSN Server: ");
	dbg_print_str((uint8_t) dns_server >> 24);
	dbg_print_str(".");
	dbg_print_str((uint8_t) dns_server >> 16);
	dbg_print_str(".");
	dbg_print_str((uint8_t) dns_server >> 8);
	dbg_print_str(".");
	dbg_print_str((uint8_t) dns_server);
	dbg_print_str("\r\n");
			
	while (1) {
		port_pin_toggle_output_level(ANNA_LED_ORG_PIN);
		vTaskDelay(1000 / portTICK_RATE_MS);
	};
}

static void adc_task(void * params){
	//dbg_print_str("In Fast LED FLash Task\r\n");
	while (1){
		port_pin_toggle_output_level(ANNA_LED_WIFI_PIN);
		vTaskDelay(100 / portTICK_RATE_MS);		
	}
}

int main (void)
{
	system_init();
	dbg_init();	
	sd_mmc_init();
	
	//Create the queues that will throw around data
	WIFI_queue = xQueueCreate(10, sizeof (wifi_queue_type_t));

	//spi_wifi_init();

	//DBG_PRINT_STR(__func__, "Creating tasks\n");
	
	xTaskCreate(&fatfs_task,
		(const signed char *)"FAT FS task",
		configMINIMAL_STACK_SIZE + 400,
		NULL,
		tskIDLE_PRIORITY + 3,
		NULL);
	
	xTaskCreate(&wifi_task,
		(const signed char *)"WIFI task",
		configMINIMAL_STACK_SIZE + 400,
		NULL,
		tskIDLE_PRIORITY + 2,
		NULL);
		
	xTaskCreate(&adc_task,
		(const signed char *)"Print task",
		configMINIMAL_STACK_SIZE + 100,
		NULL,
		tskIDLE_PRIORITY + 1,
		NULL);		
			
	vTaskStartScheduler();
}
