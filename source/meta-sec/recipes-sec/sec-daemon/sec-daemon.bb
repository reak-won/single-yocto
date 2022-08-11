DESCRIPTION = "sec-daemon"
LICENSE = "CLOSED"

PV = '1'
#PR = 'r0'

inherit cmake systemd
FILESPATH =+ "/home/ubuntu/yocto/source/sec:"
SRC_URI = "file://${PN}"
S = "${WORKDIR}/${PN}"

#secd systemd enabl
DEPENDS ?= ""
DEPENDS =+ " systemd"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "sec-daemon.service"
SYSTEMD_AUTO:ENABLE = "enable"

#bb file vars
IMAGE_LOCATION = "usr/bin"
BIN_NAME = "secd"
#BBCLASSEXTEND = "native"

do_install(){
	install -d ${D}${bindir}
	install -m 0777 ${B}/${BIN_NAME} ${D}/${bindir}

	install -d ${D}${systemd_system_unitdir}
	install -m 0666 ${WORKDIR}/${PN}/${PN}.service ${D}${systemd_system_unitdir}
}

FILES:${PN} += "/lib/systemd/system/sec-daemon.service"


