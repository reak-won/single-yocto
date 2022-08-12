DESCRIPTION = "sec-package"
LICENSE = "CLOSED"

PV = '1'

#vars for package
sec-daemon = "sec-daemon"
sec-daemon-bin = "secd"

inherit cmake systemd
FILESPATH =+ "/home/ubuntu/yocto/source/:"
SRC_URI = "file://sec"
S = "${WORKDIR}/sec"

#secd systemd enabl
DEPENDS ?= ""
DEPENDS =+ " systemd"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "sec-daemon.service"
SYSTEMD_AUTO:ENABLE = "enable"

#bb file vars
#BBCLASSEXTEND = "native"

do_install(){
	install -d ${D}${bindir}
	install -m 0777 ${B}/${sec-daemon}/${sec-daemon-bin} ${D}/${bindir}

	install -d ${D}${systemd_system_unitdir}
	install -m 0666 ${WORKDIR}/${PN}/${sec-daemon}/${sec-daemon}.service ${D}${systemd_system_unitdir}
}

FILES:${PN} += "/lib/systemd/system/sec-daemon.service"

