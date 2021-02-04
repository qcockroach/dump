#!/usr/bin/env bash
set -e

BASEDIR="$PWD"

#build options defaults
BUILD_INIT=0
BUILD_PREP_SDK=0
BUILD_SCENARIO="debug"
BUILD_SM_UPDATE=1
BUILD_PARALLEL=""
BUILD_TOBUILD=1
BUILD_MENUCONFIG=0
BUILD_CLEAN=0

# build script usage prompt
bs_usage() {
    echo -e "Syntax: $0 [op1] [op2] ..."
    echo -e "\t where opN can be:"
    echo -e "\t prep_sdk - to check if current platform fits to build, may install some packages (asks for sudo)"
    echo -e "\t init - to make initial build"
    echo -e "\t debug - to make debug build"
    echo -e "\t release - to make release build"
    echo -e "\t nosub - do not update submodules (use current)"
    echo -e "\t parallel - make parallel build using NCPU+1 threads"
    echo -e "\t nobuild - not to call build at the end of all preparations"
    echo -e "\t menuconfig - runs OpenWRT menuconfig to setup chosen build scenario"
    echo -e "\t clean - removes precompiled data from previosus build"
}

# parse input arguments
for arg in "$@"
do
    case $arg in
        prep_sdk)
            BUILD_PREP_SDK=1
            BUILD_INIT=1
            shift
            ;;
        init)
            BUILD_INIT=1
            shift
            ;;
        debug)
            BUILD_SCENARIO="debug"
            shift
            ;;
        release)
            BUILD_SCENARIO="release"
            shift
            ;;
        nosub)
            BUILD_SM_UPDATE=0
            shift
            ;;
        parallel)
            BUILD_PARALLEL="-j$(($(nproc)+1))"
            shift
            ;;
        nobuild)
            BUILD_TOBUILD=0
            shift
            ;;
        menuconfig)
            BUILD_MENUCONFIG=1
            shift
            ;;
        clean)
            BUILD_CLEAN=1
            shift
            ;;
        *)
            bs_usage
            exit 1
            ;;
esac
done

CONFIG_PATH=$BASEDIR/extras/config/v2.$BUILD_SCENARIO

if [ $BUILD_MENUCONFIG = 1 ]; then
    cp $CONFIG_PATH .config
    make menuconfig
    cp .config $CONFIG_PATH
fi

# prepare SDK for the first build
if [ $BUILD_PREP_SDK = 1 ]; then
    echo "Installing packages... "
    sudo apt update && sudo apt -y upgrade && sudo apt -y dist-upgrade && sudo apt -y autoremove
    sudo apt-get install -y	gcc g++ binutils patch bzip2 flex make gettext coccinelle \
                            pkg-config unzip zlib1g-dev libc6-dev subversion libncurses5-dev gawk \
                            sharutils curl libxml-parser-perl ocaml-nox ocaml-nox ocaml ocaml-findlib \
                            libpcre3-dev binutils-gold python-yaml u-boot-tools device-tree-compiler \
                            mtd-utils libssl-dev
    echo "Installing packages done!"
fi

if [ $BUILD_SM_UPDATE = 1 ]; then
    echo "Updating submodules... "
    git submodule update --init --recursive
    echo "Submodules are updated"
fi

if [ $BUILD_INIT = 1 ]; then
    echo "Updating feeds... "
    ./scripts/feeds update -a  
    echo "Installing feeds... "
    ./scripts/feeds install -a -f
fi

cp $BASEDIR/extras/config/v2.$BUILD_SCENARIO .config

# QCA SDK hacs from official tutorial
echo "CONFIG_TOOLCHAINOPTS=y" >> .config
echo '# CONFIG_GCC_USE_VERSION_4_8_LINARO is not set' >> .config
echo "CONFIG_GCC_USE_VERSION_5=y" >> .config
echo 'CONFIG_GCC_VERSION="5.2.0"' >> .config
echo "CONFIG_GCC_VERSION_5=y" >> .config

make defconfig

for pkg_num in 2 9;do sed 's/CONFIG_PACKAGE_qca-wifi-fw-hw'${pkg_num}'-10.4-asic=y/# CONFIG_PACKAGE_qca-wifi-fw-hw'${pkg_num}'-10.4-asic is not set/g' -i .config;done
sed '/CONFIG_PACKAGE_kmod-wil6210=m/d' -i .config
sed '/CONFIG_PACKAGE_qca-fst-manager=m/d' -i .config
sed '/CONFIG_PACKAGE_wigig-firmware=m/d' -i .config
sed '/CONFIG_PACKAGE_qca-wigig-tools=m/d' -i .config
sed '/CONFIG_PACKAGE_qca-wigig-debug-tools=m/d' -i .config
sed '/CONFIG_PACKAGE_kmod-cfg80211/d' -i .config

if [ $BUILD_CLEAN = 1 ]; then
    make clean
fi

if [ $BUILD_TOBUILD = 1 ]; then
    echo "Building FW ..."
    make $BUILD_PARALLEL
    ./img-v2.sh
    echo "Building FW done!"
fi

echo "$0 finished"
