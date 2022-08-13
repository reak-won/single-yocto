DESCRIPTION = "sec-package"
LICENSE = "CLOSED"

PV = '1'

#vars for package
sec-daemon = "sec-daemon"
sec-daemon-bin = "secd"
sec-api = "sec-api"
libsec-api = "libsecapi.so"

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
	#daemon install
	install -d ${D}${bindir}
	install -m 0777 ${B}/${sec-daemon}/${sec-daemon-bin} ${D}/${bindir}
	
	#daemon service install
	install -d ${D}${systemd_system_unitdir}
	install -m 0666 ${WORKDIR}/${PN}/${sec-daemon}/${sec-daemon}.service ${D}${systemd_system_unitdir}

	#install api so
	install -d ${D}${libdir}
	install -m 0666 ${B}/${sec-api}/${libsec-api} ${D}${libdir}/${libsec-api}.1.0.0
}
#INSANE:SKIP:${PN} += "dev-so"

#FILES:${PN} += "/lib/systemd/system/sec-daemon.service"
FILES:${PN} += "${systemd_system_unitdir}"
#FILES:${PN} += "/lib"
FILES:${PN} += "/usr/lib64"

