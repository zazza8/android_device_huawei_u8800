#!/bin/bash

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

# This script checks if there is a device to pull from, downloads the files to
# their corresponding directories in $BASE. After downloading it gives the
# correct permissions to libraries and binaries.

# Assign vendor names used in directories.
VENDOR=huawei
DEVICE=u8800
VENDOR_BASE=../../../vendor/$VENDOR/$DEVICE/proprietary
BASE=$VENDOR_BASE/pulled
PULL_BASE=system

# Global array used for pulling files.
filelist=()

function getFileList
{
	filelist=()
	while read file
	do
		# Skip the line if it is a comment or empty line.
		if [[ ${file:0:1} == "#" ]] || [[ $file == "" ]]; then
			continue
		# If we have - in front of the file, remove it.
		elif [[ ${file:0:1} == "-" ]]; then
			file=${file:1}
		fi
		filelist+=($file)
	done < proprietary-files.txt
}

function pullAdb
{
	# Start ADB server.
	adb start-server

	# List devices through ADB.
	devices=$(adb devices)

	# Back up IFS.
	OLD_IFS="$IFS"

	# Give newline char as splitter.
	IFS=$'\n'
	devicefound=false
	for line in $devices; do
		# First line is not showing any devices
		if [[ "$line" =~ "List of devices attached" ]]
			then
			continue
		# If we found a device, exit the loop.
		elif [[ "$line" =~ "device" ]]
			then
			devicefound=$line
		fi
	done

	# Revert back to original IFS.
	IFS="$OLD_IFS"

	# If no device found, let's wait for one.
	if [[ "$devicefound" == false ]]; then
		echo "Waiting for device..."
		adb wait-for-device
	fi
	echo "Found "$devicefound

	getFileList

	for file in ${filelist[@]}; do
		echo -en "\e[00;34mPulling $file "

		# Find the directory name.
		dir=$(dirname $file)
		if [ ! -d $BASE/$dir ]; then
			mkdir -p $BASE/$dir
		fi
		adb pull $PULL_BASE/$file $BASE/$file
		echo -en "\e[00m"
	done
}

function pullZip
{
	getFileList

	for file in ${filelist[@]}; do
		# Find the directory name.
		dir=$(dirname $file)
		if [ ! -d $BASE/$dir ]; then
			mkdir -p $BASE/$dir
		fi
		echo -e "\e[00;34mPulling $file\e[00m"
		unzip -j $PARAMETER $PULL_BASE/$file -d $BASE/$dir > /dev/null
	done
}

# Remove old files (if exists)
rm -rf $BASE/*

PARAMETER=$1
# Check if file is specified.
if [[ -f $PARAMETER ]] && [[ ${PARAMETER##*.} == "zip" ]]
then
	echo "Pulling from zip"
	pullZip
else
	echo "Pulling from device"
	pullAdb
fi

# Give correct permissions.
echo "Setting correct permissions..."

# Give newline char as splitter.
IFS=$'\n'

for files in $(find $VENDOR_BASE -type f); do
	echo -e "\e[00;32mSetting permission 644 on "$(basename $files)"\e[00m"
	chmod 644 $files
done

for binary in $(find $VENDOR_BASE/*/bin -type f); do
	echo -e "\e[00;32mSetting permission 755 on "$(basename $binary)"\e[00m"
	chmod 755 $binary
done

# Revert back to original IFS.
IFS="$OLD_IFS"

# Start the makefiles script which adds the makefiles for Android.
echo "Executing setup-makefiles.sh"
./setup-makefiles.sh
