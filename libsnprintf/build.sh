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

# Global settings
export COMPATIBILITY="-DSNPRINTF_LONGLONG_SUPPORT -DSOLARIS_COMPATIBLE -DSOLARIS_BUG_COMPATIBLE"
export CFLAGS="-std=gnu99 -I$prefix/include -mcpu=v7 $COMPATIBILITY"

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
    clean stage
    setdir source
    
    # 1. Run the default install into the staging directory
    ${__make} DESTDIR=${stagedir} install

    # 2. Create the compat directory inside the staged include folder
    ${__mkdir} -p ${stagedir}${prefix}/include/compat

    # 3. Move the header into the compat folder
    # (Renaming it to snprintf_compat.h to perfectly match your Git config.mak)
    ${__mv} ${stagedir}${prefix}/include/snprintf.h ${stagedir}${prefix}/include/compat/snprintf_compat.h

    # 4. Tell the framework we already ran 'make install' and to just package it
    custom_install=1
    generic_install DESTDIR
    
    # 5. Install the documentation
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
