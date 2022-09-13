DESCRIPTION = "sec-package"
LICENSE = "CLOSED"

PV = '1'

#vars for package
sec-daemon = "sec-daemon"
sec-core = "sec-core"
sec-daemon-bin = "secd"
sec-api = "sec-api"
libsec-api = "libsecapi.so"
sec-test = "sec-test"

inherit cmake systemd
FILESPATH =+ "file://home/ubuntu/yocto/source/:"
#FILESPATH =+ "${MY_WORKSPACE}"
SRC_URI = "file://sec"
S = "${WORKDIR}/sec"

#secd systemd enable
DEPENDS ?= ""
DEPENDS =+ " systemd openssl"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "sec-daemon.service sec-core.service"
SYSTEMD_AUTO:ENABLE = "enable"

#bb file vars
#BBCLASSEXTEND = "native"

#non-version so file support
INSANE_SKIP:${PN} = "ldflags"
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"
SOLIBS = ".so"
FILES_SOLIBSDEV = ""

do_install(){
	#daemon install
	install -d ${D}${bindir}
	install -m 0777 ${B}/${sec-daemon}/${sec-daemon-bin} ${D}/${bindir}

	#core install
	install -d ${D}${bindir}
	install -m 0777 ${B}/${sec-core}/${sec-core} ${D}/${bindir}
	
	#daemon service install
	install -d ${D}${systemd_system_unitdir}
	install -m 0666 ${WORKDIR}/${PN}/${sec-daemon}/${sec-daemon}.service ${D}${systemd_system_unitdir}

	#core service install
	install -d ${D}${systemd_system_unitdir}
	install -m 0666 ${WORKDIR}/${PN}/${sec-core}/${sec-core}.service ${D}${systemd_system_unitdir}

	#install api so
	install -d ${D}${libdir}
	install -m 0777 ${B}/${sec-api}/${libsec-api} ${D}${libdir}/${libsec-api} #.1.0.0

	#install test
	install -m 0777 ${B}/${sec-test}/${sec-test} ${D}/${bindir}

}
#INSANE:SKIP:${PN} += "dev-so"

FILES:${PN} += "${systemd_system_unitdir}"
FILES:${PN} += "/usr/lib64"
