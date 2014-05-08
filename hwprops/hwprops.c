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

#include <cutils/log.h>
#include <cutils/properties.h>

#define HWPROPS_DIR		"/sys/hwprops/"
#define HWPROPS_WLANMAC		HWPROPS_DIR "wlanmac"
#define HWPROPS_MAC		HWPROPS_DIR "btmac"

static void usage(void)
{
	printf("Usage:\n");
	printf("\t-wlanmac: Writes the MAC address to %s.\n", HWPROPS_WLANMAC);
	printf("\t-btmac: Writes the MAC address to ${ro.bt.bdaddr_path}.\n");

	printf("Custom Addresses (/data/misc/hwprops)\n");
	printf("\t-wlanmac: WLAN MAC address.\n");
	printf("\t-btmac: BT MAC address.\n");
}

#define CUSTOM_PROPS_DIR "/data/misc/hwprops/"

static int get_custom_bt_mac(uint8_t *btmac)
{
	char buffer[18];

	int fd = open(CUSTOM_PROPS_DIR "btmac", O_RDONLY);
	if (fd < 0) {
		return -1;
	}

	read(fd, buffer, 18);

	if (sscanf(buffer, "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX",
		&btmac[0], &btmac[1], &btmac[2],
		&btmac[3], &btmac[4], &btmac[5]) != 6) {
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}

static int get_custom_wlan_mac(uint8_t *wlanmac)
{
	char buffer[18];

	int fd = open(CUSTOM_PROPS_DIR "wlanmac", O_RDONLY);
	if (fd < 0) {
		return -1;
	}

	read(fd, buffer, 18);

	if (sscanf(buffer, "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX",
		&wlanmac[0], &wlanmac[1], &wlanmac[2],
		&wlanmac[3], &wlanmac[4], &wlanmac[5]) != 6) {
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
	int fd;
	int size;

	ret = get_custom_wlan_mac(wlanmac);
	if (ret) {
		printf("WLAN MAC not stored/invalid.\n");
		return;
	}

	size = sprintf(wlan_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
		wlanmac[0], wlanmac[1], wlanmac[2],
		wlanmac[3], wlanmac[4], wlanmac[5]);

	fd = open("/sys/hwprops/wlanmac", O_WRONLY);
	if (fd < 0)
		return;

	printf("Writing custom WLAN MAC address.\n");

	write(fd, wlan_addr, size);
	close(fd);
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
	if (ret) {
		printf("BT MAC not stored/invalid.\n");
		return;
	}

	size = sprintf(bt_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
		btmac[0], btmac[1], btmac[2],
		btmac[3], btmac[4], btmac[5]);

	/* Bluetooth MAC address is stored in a file. If the file path is set,
	 * write the MAC address. */
	if (property_get("ro.bt.bdaddr_path", bt_path_prop, NULL)) {
		fd = open(bt_path_prop, O_WRONLY);
		if (fd < 0)
			return;
		printf("Writing custom BT MAC address.\n");

		write(fd, bt_addr, size);
		close(fd);
	}

}

int main(int argc, char *argv[])
{
	int i;

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-wlanmac"))
			set_wlan_mac();
		else if (!strcmp(argv[i], "-btmac"))
			set_bt_mac();
		else {
			usage();
			return 0;
		}
	}

	if (argc == 1) {
		set_wlan_mac();
		set_bt_mac();
	}

	return 0;
}
