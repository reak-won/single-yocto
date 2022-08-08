#!/bin/bash

BUILDDIR = ./build

export TEMPLATECONF=${PWD}/source/meta-conf
source poky/oe-init-build-env ${BUILDDIR}
