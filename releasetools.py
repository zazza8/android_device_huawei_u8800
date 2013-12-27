#
# Copyright (C) 2012 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import sha
import re

import common

# Function used to write the boot/recovery image to the corresponding location.
def InstallImage(info, mount_point, location, file):
	info.script.Mount(mount_point)
	args = {'file': file, 'location': location}
	info.script.AppendExtra(
		'package_extract_file("%(file)s", "%(location)s/%(file)s");' % args)

# Function used to copy boot/recovery image to the /cust (/boot) partition.
def InstallCustImage(info, file):
	InstallImage(info, "/boot", "/boot/image", file)

# Function called after flashing the main system is complete.
def FullOTA_InstallEnd(info):
	# Manually remove boot.img direct install done by releasetools.
	# This removal is done before installing cust image.
	edify = info.script
	for cmd in edify.script:
		if "package_extract_file(\"boot.img\"" in cmd:
			edify.script.remove(cmd)
	InstallCustImage(info, "boot.img")
