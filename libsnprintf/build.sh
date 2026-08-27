#!/bin/bash
# This is a buildpkg build.sh script
# build.sh helper functions
. ${BUILDPKG_SCRIPTS}/build.sh.functions
# ###########################################################
# Check the following 4 variables before running the script
topdir=libsnprintf
version=2.2.1
version_major="${version%.*}"

pkgver=1

source[0]=https://ftp.deu.edu.tr/pub/Solaris/sunfreeware/SOURCES/${topdir}-${version}.tar.gz
# If there are no patches, simply comment this

# Source function library
. ${BUILDPKG_SCRIPTS}/buildpkg.functions

# -mcpu=v7
# Global settings
export CFLAGS="-std=gnu99 -I$prefix/include -mcpu=v7"

# Fix: Ensure -lgcc_s and runtime search paths are passed to LDFLAGS 
# so libsnprintf.so gets built with an explicit DT_NEEDED tag for libgcc_s
export LDFLAGS="-L$prefix/lib -R$prefix/lib -lgcc_s"
export LIBS="-L$prefix/lib -R$prefix/lib -lgcc_s"

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
    doc ChangeLog NEWS README INSTALL LICENSE.txt
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
