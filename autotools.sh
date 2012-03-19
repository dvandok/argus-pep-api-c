#!/bin/sh
##
# Copyright (c) Members of the EGEE Collaboration. 2004-2010.
# See http://www.eu-egee.org/partners for details on the copyright holders. 
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##
# $Id: autotools.sh 2385 2011-07-25 13:17:52Z vtschopp $
##
#set -x

die() {
    echo "$@"
    exit
}

echo "Bootstrapping autotools..."

echo "aclocal..."
aclocal -I project || die "aclocal failed"
echo "libtoolize..."
libtoolize --force || die "libtoolize failed"
echo "autoheader..."
autoheader || die "autoheader failed"
echo "automake..."
automake --foreign --add-missing --copy || die "automake failed"
echo "autoconf..."
autoconf || die "autoconf failed"

echo "Done."
