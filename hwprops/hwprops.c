/*
 * Hardware Property Writer
 * Copyright (C) 2014  Rudolf Tammekivi <rtammekivi@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see [http://www.gnu.org/licenses/].
 */

#define LOG_TAG "hwprops"

#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <private/android_filesystem_config.h>

#include <cutils/log.h>
#include <cutils/properties.h>

#include <libhwprops/hwprops.h>

static void usage(void)
{
	printf("Usage:\n");
	printf("\t-wlanmac: Assigns ${wlan.driver.arg} with correct"
		"parameter.\n");
	printf("\t-btmac: Writes the MAC address to ${ro.bt.bdaddr_path}.\n");

	printf("Custom Addresses (/data/misc/hwprops)\n");
	printf("\t-wlanmac: if saved, overrides NV.\n");
	printf("\t-btmac: if saved, overrides NV.\n");
}

#define CUSTOM_PROPS_DIR "/data/misc/hwprops/"
static int get_custom_wlan_mac(uint8_t *wlanmac)
{
	char buffer[18];

	int fd = open(CUSTOM_PROPS_DIR "wlanmac", O_RDONLY);
	if (fd < 0) {
		return -1;
	}

	read(fd, buffer, 18);

	if (sscanf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X",
		(unsigned int *)&wlanmac[0], (unsigned int *)&wlanmac[1],
		(unsigned int *)&wlanmac[2], (unsigned int *)&wlanmac[3],
		(unsigned int *)&wlanmac[4], (unsigned int *)&wlanmac[5]) != 6) {
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}

static int get_custom_bt_mac(uint8_t *btmac)
{
	char buffer[18];

	int fd = open(CUSTOM_PROPS_DIR "btmac", O_RDONLY);
	if (fd < 0) {
		return -1;
	}

	read(fd, buffer, 18);

	if (sscanf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X",
		(unsigned int *)&btmac[0], (unsigned int *)&btmac[1],
		(unsigned int *)&btmac[2], (unsigned int *)&btmac[3],
		(unsigned int *)&btmac[4], (unsigned int *)&btmac[5]) != 6) {
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}

static void set_wlan_mac(void)
{
	int ret;
	uint8_t wlanmac[6];
	char wlan_addr[19];
	char wlan_arg_prop[PROPERTY_VALUE_MAX];

	ret = get_custom_wlan_mac(wlanmac);
	if (!ret) {
		ALOGD("Using custom wlan mac\n");
	} else {
		ALOGD("Using NV wlan mac\n");
		if (hwprops_get_wlan_mac(wlanmac))
			return;
	}

	sprintf(wlan_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
		wlanmac[0], wlanmac[1], wlanmac[2],
		wlanmac[3], wlanmac[4], wlanmac[5]);

	sprintf(wlan_arg_prop, "mac_param=%s", wlan_addr);
	property_set("wlan.driver.arg", wlan_arg_prop);
}

static void set_bt_mac(void)
{
	int ret;
	uint8_t btmac[6];
	char bt_addr[PROPERTY_VALUE_MAX];
	char bt_path_prop[PROPERTY_VALUE_MAX];
	int fd;
	int size;

	ret = get_custom_bt_mac(btmac);
	if (!ret) {
		ALOGD("Using custom bt mac\n");
	} else {
		ALOGD("Using NV bt mac\n");
		if (hwprops_get_bt_mac(btmac))
			return;
	}

	size = sprintf(bt_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
		btmac[0], btmac[1], btmac[2],
		btmac[3], btmac[4], btmac[5]);

	/* Bluetooth MAC address is stored in a file. If the file path is set,
	 * write the MAC address. */
	if (property_get("ro.bt.bdaddr_path", bt_path_prop, NULL)) {
		fd = open(bt_path_prop, O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
		if (fd < 0) {
			printf("Failed to open %s\n", bt_path_prop);
			return;
		}

		write(fd, bt_addr, size);

		/* Set same chown permissions as bt_config.xml */
		fchown(fd, AID_BLUETOOTH, AID_NET_BT_STACK);

		close(fd);
	}

}

int main(int argc, char *argv[])
{
	int i;

	hwprops_init();

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-wlanmac"))
			set_wlan_mac();
		else if (!strcmp(argv[i], "-btmac"))
			set_bt_mac();
		else {
			usage();
			hwprops_exit();
			return 0;
		}
	}

	if (argc == 1) {
		set_wlan_mac();
		set_bt_mac();
	}

	hwprops_exit();
	return 0;
}
