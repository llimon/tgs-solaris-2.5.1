#!/bin/bash
# This is a buildpkg build.sh script
# build.sh helper functions
. ${BUILDPKG_SCRIPTS}/build.sh.functions
#
###########################################################
# Check the following 4 variables before running the script
topdir=snprintf
version=2.2
version_major="${version%.*}"

pkgver=1

# https://ftp.deu.edu.tr/pub/Solaris/sunfreeware/SOURCES/snprintf-2.2.tar.gz
source[0]=https://ftp.deu.edu.tr/pub/Solaris/sunfreeware/SOURCES/${topdir}-${version}.tar.gz
# If there are no patches, simply comment this
patch[0]=autoconf-fix.patch

# Source function library
. ${BUILDPKG_SCRIPTS}/buildpkg.functions

# Redefin package global attributes

# Global settings
export CFLAGS="-std=gnu99" 
export LDFLAGS="-lgcc_s"
topsrcdir="${topdir}_${version}/with_autoconf"
#configure_args+=()

reg prep
prep()
{	
	 #get_files
    generic_prep
}

reg build
build()
{
    generic_build
}

reg check
check()
{
    generic_check
}

reg install
install()
{
    DESTDIR=${stagedir}
    generic_install DESTDIR
    doc README INSTALL LICENSE.txt
}

reg pack
pack()
{
    generic_pack
}

reg distclean
distclean()
{
    clean distclean
}

###################################################
# No need to look below here
###################################################
build_sh $*
