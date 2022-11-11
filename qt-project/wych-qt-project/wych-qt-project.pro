TEMPLATE = subdirs

SUBDIRS += \
    libhv \
    libuv \
    qwebdavlib \
    project


RC_ICONS = ./favicon.ico

QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator'uiAccess='false'"
