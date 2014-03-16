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

/* Must be initialized before requesting information. */
void hwprops_init(void);
void hwprops_exit(void);

/* Returns byte array of the MAC address. */
int hwprops_get_wlan_mac(uint8_t *buffer);
/* Returns byte array of the MAC address. */
int hwprops_get_bt_mac(uint8_t *buffer);
