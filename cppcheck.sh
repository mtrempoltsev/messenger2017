#!/bin/bash

### Cppcheck is a static analysis tool for C/C++ code.
###
### Links:
###   http://cppcheck.sourceforge.net/
###   https://github.com/danmar/cppcheck/
###
### Installation:
###
###   Windows:
###   - Download installer from http://cppcheck.sourceforge.net/
###
###   Linux:
###
###   - Cppcheck can be installed from package managers:
###     $ sudo apt-get install cppcheck
###     for Debian-based or
###     $ sudo yum install cppcheck
###     for Fedora
###
###   - For latest version compile it from sources:
###     1) Download or ´git clone´ latest release from https://github.com/danmar/cppcheck/releases/latest
###     2) Build and install according to readme.md
###
### Usage:
###   $ ./cppcheck.sh
###
### Help:
###   $ cppcheck --help

### Main cfg parameters

# Global parameters
SCRIPT_NAME=$(basename $0)
SCRIPT_DIR=$(readlink -f $(dirname $0))
JOBS_COUNT=20

SCRIPT_STDOUT_LOG=/dev/null

### User-friendly making

# Color
LGREEN='\033[1;32m'
# Mode
NORMAL='\033[0m'
BOLD='\033[1m'

### Script

# Dont check third-party libs
EXCLUDE_LIBS="-i ./3rd-party"

echo -e "${BOLD}${LGREEN}"
echo "Running cppcheck for project..."
echo -e "${NORMAL}"

time cppcheck -j $JOBS_COUNT --enable=warning,performance,portability,style --inconclusive --force --max-configs=30 $EXCLUDE_LIBS . > $SCRIPT_STDOUT_LOG

echo -e "${BOLD}${LGREEN}"
echo "Finished!"
echo -e "${NORMAL}"
