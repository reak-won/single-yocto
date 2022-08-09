DESCRIPTION = "my_test"
LICENSE = "CLOSED"

PV = '1'

inherit cmake 
FILESPATH =+ "/home/ubuntu/yocto/source:"
SRC_URI = "file://${PN}"
S = "${WORKDIR}/${PN}"


#BBCLASSEXTEND = "native"

do_install(){
}
