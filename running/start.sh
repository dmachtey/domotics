# Filename: start.sh
# 
# Description: 
# Author: Damian Machtey
# Maintainer: 
# 
# Created: 2015-06-22 Mon
# 
# Last-Updated: Sat Jul 23 12:45:41 2016 (-0500)
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


#cd pru2
./gpio_config.sh
./test-pwm&
# disown

# 
# start.sh ends here
