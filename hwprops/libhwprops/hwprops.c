/*
 * Huawei Property Reader
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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "nv_private.h"
#include <oncrpc_private.h>

void hwprops_init(void)
{
	oncrpc_init();
	oncrpc_task_start();
}

void hwprops_exit(void)
{
	oncrpc_task_stop();
	oncrpc_deinit();
}

#define MAC_SIZE 6
int hwprops_get_wlan_mac(uint8_t *buffer)
{
	nv_item_type value;
	int i;

	memset(&value, 0, sizeof(value));

	if (nv_cmd_remote(NV_READ_F, NV_WLAN_MAC_ADDRESS_I, &value)) {
		hwprops_exit();
		return -1;
	}

	/* Convert endianness. */
	for (i = 0; i < MAC_SIZE; i++)
		buffer[(MAC_SIZE - 1) - i] = value.mac_address[i];

	return 0;
}

int hwprops_get_bt_mac(uint8_t *buffer)
{
	nv_item_type value;
	int i;

	memset(&value, 0, sizeof(value));

	if (nv_cmd_remote(NV_READ_F, NV_BD_ADDR_I, &value)) {
		hwprops_exit();
		return -1;
	}

	/* Convert endianness. */
	for (i = 0; i < MAC_SIZE; i++)
		buffer[(MAC_SIZE - 1) - i] = value.mac_address[i];

	return 0;
}
