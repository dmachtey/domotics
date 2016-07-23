# Filename: gpio_config.sh
# 
# Description: 
# Author: Damian Machtey
# Maintainer: 
# 
# Created: 2015-06-22 Mon
# 
# Last-Updated: Sat Jul 23 12:43:28 2016 (-0500)
#           By: Damian Machtey
#     Update #: 3

# Change Log:
# 
# 
# Copyright (C) 2016 Damian Machtey
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
# 
# 

# Code:


#!/bin/bash

# By Damian Machtey
# 2015-07-02 Thu

SLOTS=/sys/devices/bone_capemgr.9/slots
PINS=/sys/kernel/debug/pinctrl/44e10800.pinmux/pins
GPOIO_DIR=/sys/class/gpio

echo half-in-half-out > $SLOTS

OUTPUT_PIN=(38 39 34 35 66 67 69 68 45 44 23 26 47 46 27 65 22 63 62 37 36 33 32 61 86 88 87 89 10 11 9 81 8)
INPUT_PIN=(80 78 79 76 77 74 75 72 73 70 71 30 60 31 50 48 51 5 4 3 2 49 15 117 14 115 113 111 112 110 20 7)

for i in ${OUTPUT_PIN[*]}; do
    echo $i > $GPOIO_DIR/export
    echo "out" > $GPOIO_DIR/gpio$i/direction
done


for i in ${INPUT_PIN[*]}; do
    echo $i > $GPOIO_DIR/export
    echo "in" > $GPOIO_DIR/gpio$i/direction
done

mosquitto -c /etc/mosquitto/mosquitto.conf &  # for mosquitto broker
/etc/init.d/openhab start

# 
# gpio_config.sh ends here
