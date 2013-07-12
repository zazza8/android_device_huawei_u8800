/*
 * Copyright (c) 2013, Rudolf Tammekivi.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <cutils/properties.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "oncrpc.h"
#include "nv.h"

#define MAC_SIZE 6
#define WIFI_MAC_PROPERTY "persist.wifi.mac"
#define DRIVER_ARG_PROP_NAME "wlan.driver.arg"

static void start_rpc_client(bool enable);
static bool is_modem_available(void);
static void handle_wlan_mac(void);

static void get_wlan_mac(unsigned char *buffer);

int main()
{
	start_rpc_client(true);

	if (!is_modem_available()) {
		printf("Modem is not available\n");
		goto exit;
	}

	handle_wlan_mac();

exit:
	start_rpc_client(false);
	return 0;
}

static bool is_modem_available(void)
{
	return nv_null() ? true : false;
}

static void start_rpc_client(bool enable)
{
	if (enable) {
		oncrpc_init();
		oncrpc_task_start();
	} else {
		oncrpc_task_stop();
		oncrpc_deinit();
	}
}

static void handle_wlan_mac(void)
{
	unsigned char wlanmac[MAC_SIZE];
	char wlan_property[PROPERTY_VALUE_MAX];
	char wlan_arg_property[PROPERTY_VALUE_MAX];

	/* Only write the property if it is not set. This allows the user
	 * to override it manually. */
	if (!property_get(WIFI_MAC_PROPERTY, wlan_property, NULL)) {
		get_wlan_mac(wlanmac);

		sprintf(wlan_property, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",
			wlanmac[0], wlanmac[1], wlanmac[2],
			wlanmac[3], wlanmac[4], wlanmac[5]);

		property_set(WIFI_MAC_PROPERTY, wlan_property);
	}

	sprintf(wlan_arg_property, "mac_param=%s", wlan_property);
	property_set(DRIVER_ARG_PROP_NAME, wlan_arg_property);
}

static void get_wlan_mac(unsigned char *buffer)
{
	nv_item_type nv_item;
	int i;

	/* Read the WLAN MAC NV Item */
	nv_cmd_remote(NV_READ_F, NV_WLAN_MAC_ADDRESS_I, &nv_item);

	/* Convert endianness (reverse the order). */
	for (i = 0; i < MAC_SIZE; i++)
		buffer[(MAC_SIZE - 1) - i] = nv_item.wlan_mac_address[i];
}
