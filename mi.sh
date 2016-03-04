#!/bin/sh

#GCC_DIR=~/OpenWrt-Toolchain-bcm53xx-for-arm_cortex-a9-gcc-4.8-linaro_uClibc-0.9.33.2_eabi/toolchain-arm_cortex-a9_gcc-4.8-linaro_uClibc-0.9.33.2_eabi
#musl toolchain
#GCC_DIR=~/openwrt/staging_dir/toolchain-arm_cortex-a9_gcc-4.9.0_musl-1.1.4_eabi

#DD-WRT's toolchains
GCC_DIR=~/toolchains-dd-wrt/toolchain-arm_cortex-a9_gcc-4.9-linaro_musl-1.1.2_eabi
#GCC_DIR=~/openwrt/staging_dir/toolchain-arm_cortex-a9_gcc-4.9.0_musl-1.1.4_eabi

[ -e $GCC_DIR/bin/arm-linux-uclibc-gcc ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-gcc $GCC_DIR/bin/arm-linux-uclibc-gcc
[ -e $GCC_DIR/bin/arm-linux-uclibc-ar ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-ar $GCC_DIR/bin/arm-linux-uclibc-ar
[ -e $GCC_DIR/bin/arm-linux-uclibc-ranlib ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-ranlib $GCC_DIR/bin/arm-linux-uclibc-ranlib
[ -e $GCC_DIR/bin/arm-linux-uclibc-ld ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-ld $GCC_DIR/bin/arm-linux-uclibc-ld
[ -e $GCC_DIR/bin/arm-linux-uclibc-nm ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-nm $GCC_DIR/bin/arm-linux-uclibc-nm
[ -e $GCC_DIR/bin/arm-linux-nm ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-nm $GCC_DIR/bin/arm-linux-nm
[ -e $GCC_DIR/bin/arm-linux-uclibc-g++ ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-g++ $GCC_DIR/bin/arm-linux-uclibc-g++
[ -e $GCC_DIR/bin/arm-linux-uclibc-strip ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-strip $GCC_DIR/bin/arm-linux-uclibc-strip
[ -e $GCC_DIR/bin/arm-linux-uclibc-size ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-size $GCC_DIR/bin/arm-linux-uclibc-size
[ -e $GCC_DIR/bin/arm-linux-uclibc-objcopy ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-objcopy $GCC_DIR/bin/arm-linux-uclibc-objcopy

export PATH=$GCC_DIR/bin:$PATH

export CROSS_COMPILE=arm-openwrt-linux-
export ARCH=arm
export STAGING_DIR=~/dd-wrt-staging

