TEMPLATE = lib
TARGET = shellshare

CONFIG += plugin share-ui-plugin

HEADERS = shareuiplugin.h shareuimethod.h
SOURCES = shareuiplugin.cpp shareuimethod.cpp

icon.path = /usr/share/themes/blanco/meegotouch/images/shellshare/
icon.files = icon-m-shellshare.png
INSTALLS += icon

CONFIG += meegotouch
