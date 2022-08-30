#!/bin/bash

BDIR=${PWD}/build

export TEMPLATECONF=${PWD}/source/meta-conf
source poky/oe-init-build-env ${BDIR}
