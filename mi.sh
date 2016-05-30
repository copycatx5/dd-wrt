#!/bin/sh

#DD-WRT's toolchains
GCC_DIR=/mnt/blue1t/OpenWrt-Toolchain-bcm53xx_gcc-5.3.0_musl-1.1.14_eabi.Linux-x86_64/toolchain-arm_cortex-a9_gcc-5.3.0_musl-1.1.14_eabi

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


[ -e $GCC_DIR/bin/arm-linux-gcc ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-gcc $GCC_DIR/bin/arm-linux-gcc
[ -e $GCC_DIR/bin/arm-linux-ar ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-ar $GCC_DIR/bin/arm-linux-ar
[ -e $GCC_DIR/bin/arm-linux-ranlib ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-ranlib $GCC_DIR/bin/arm-linux-ranlib
[ -e $GCC_DIR/bin/arm-linux-ld ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-ld $GCC_DIR/bin/arm-linux-ld
[ -e $GCC_DIR/bin/arm-linux-nm ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-nm $GCC_DIR/bin/arm-linux-nm
[ -e $GCC_DIR/bin/arm-linux-g++ ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-g++ $GCC_DIR/bin/arm-linux-g++
[ -e $GCC_DIR/bin/arm-linux-strip ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-strip $GCC_DIR/bin/arm-linux-strip
[ -e $GCC_DIR/bin/arm-linux-size ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-size $GCC_DIR/bin/arm-linux-size
[ -e $GCC_DIR/bin/arm-linux-objcopy ] || ln -sf $GCC_DIR/bin/arm-openwrt-linux-objcopy $GCC_DIR/bin/arm-linux-objcopy


export PATH=$GCC_DIR/bin:$PATH

export CROSS_COMPILE=arm-openwrt-linux-
export ARCH=arm
export STAGING_DIR=~/dd-wrt-staging

