#!/bin/bash

BDIR=${PWD}/build
MY_WORKSPACE=${PWD}/source

export TEMPLATECONF=${PWD}/source/meta-conf
source poky/oe-init-build-env ${BDIR}
