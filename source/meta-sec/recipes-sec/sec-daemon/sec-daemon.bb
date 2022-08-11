DESCRIPTION = "sec-daemon"
LICENSE = "CLOSED"

PV = '1'
#PR = 'r0'

inherit cmake 
FILESPATH =+ "/home/ubuntu/yocto/source/sec:"
SRC_URI = "file://${PN}"
S = "${WORKDIR}/${PN}"

#bb file vars
IMAGE_LOCATION = "usr/bin"
BIN_NAME = "secd"
#BBCLASSEXTEND = "native"

do_install(){
	install -d ${D}${bindir}
	install -m 0777 ${B}/${BIN_NAME} ${D}/${bindir}
}
