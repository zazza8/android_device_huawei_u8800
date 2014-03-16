#
# Huawei Property Reader
# Copyright (C) 2014  Rudolf Tammekivi <rtammekivi@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see [http://www.gnu.org/licenses/].
#


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libhwprops
LOCAL_SRC_FILES := hwprops.c
LOCAL_MODULE_TAGS := optional

# Proprietary
LOCAL_SHARED_LIBRARIES += libnv
$(shell mkdir -p $(OUT)/obj/SHARED_LIBRARIES/libnv_intermediates/)
$(shell touch $(OUT)/obj/SHARED_LIBRARIES/libnv_intermediates/export_includes)

# Proprietary
LOCAL_SHARED_LIBRARIES += liboncrpc
$(shell mkdir -p $(OUT)/obj/SHARED_LIBRARIES/liboncrpc_intermediates/)
$(shell touch $(OUT)/obj/SHARED_LIBRARIES/liboncrpc_intermediates/export_includes)

include $(BUILD_SHARED_LIBRARY)
