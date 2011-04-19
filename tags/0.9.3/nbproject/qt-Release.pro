# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = L2tpIPsecVpn
VERSION = 0.9.3
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core gui network
SOURCES += src/util/SmartCardInsertWaitTask.cpp src/conf/PppDownScriptWriter.cpp src/main.cpp src/dialogs/AdvancedSettingsDialog.cpp src/conf/IPsecConfWriter.cpp src/conf/IPsecSecretsConfWriter.cpp src/conf/PppUpScriptWriter.cpp src/dialogs/PeerAuthenticationDialog.cpp src/conf/AbstractConfWriter.cpp src/util/NetworkInterface.cpp src/util/VpnControlDaemonClient.cpp src/dialogs/ConnectionInformationDialog.cpp src/models/IPsecCertificateListModel.cpp src/pkcs11/SmartCardInfo.cpp src/settings/Preferences.cpp src/PasswordCallback.cpp src/dialogs/RouteSettingsDialog.cpp src/conf/PPPConfWriter.cpp src/conf/L2tpConfWriter.cpp src/util/NetworkInterfaceMonitor.cpp src/conf/PppDnsConfWriter.cpp src/ConnectionManager.cpp src/settings/Settings.cpp src/VPNControlTask.cpp src/conf/OpensslConfWriter.cpp src/dialogs/ConnectionSettingsDialog.cpp src/L2tpIPsecVpnApplication.cpp src/ConnectionEditor.cpp src/pkcs11/Pkcs11.cpp src/models/SmartCardObjectListModel.cpp src/util/EncSecrets.cpp src/conf/ConfWriter.cpp src/models/PppRoutesModel.cpp src/util/CertificateInfo.cpp src/localpeer/LockedFile.cpp src/conf/GetIPsecInfoScriptWriter.cpp src/localpeer/LocalPeer.cpp src/util/SmartCardState.cpp src/conf/RSysLogConfWriter.cpp src/dialogs/SmartCardObjectsDialog.cpp src/dialogs/EapSettingsDialog.cpp src/settings/ConnectionSettings.cpp src/ConnectionState.cpp src/dialogs/IpSettingsDialog.cpp src/util/SecretsChecker.cpp src/pkcs11/Pkcs11Attribute.cpp src/dialogs/ConnectionEditorDialog.cpp src/util/GlobalFunctions.cpp src/models/ConnectionsModel.cpp
HEADERS += src/util/EncSecrets.h src/util/NetworkInterfaceMonitor.h src/util/SmartCardState.h src/models/SmartCardObjectListModel.h src/util/VpnControlDaemonClient.h src/localpeer/LocalPeer.h src/ConnectionManager.h src/util/ErrorEx.h src/dialogs/RouteSettingsDialog.h src/dialogs/AdvancedSettingsDialog.h src/dialogs/IpSettingsDialog.h src/ConnectionEditor.h src/conf/PppDownScriptWriter.h src/conf/ConfWriter.h src/conf/AbstractConfWriter.h src/PasswordCallback.h src/util/SmartCardInsertWaitTask.h src/util/SecretsChecker.h src/conf/IPsecSecretsConfWriter.h src/VPNControlTask.h src/settings/Settings.h src/models/PppRoutesModel.h src/conf/OpensslConfWriter.h src/settings/ConnectionSettings.h src/L2tpIPsecVpnApplication.h src/dialogs/SmartCardObjectsDialog.h src/pkcs11/Pkcs11Attribute.h src/pkcs11/Pkcs11.h src/dialogs/ConnectionEditorDialog.h src/conf/IPsecConfWriter.h src/models/ConnectionsModel.h src/settings/Preferences.h src/conf/L2tpConfWriter.h src/dialogs/PeerAuthenticationDialog.h src/dialogs/ConnectionSettingsDialog.h src/conf/GetIPsecInfoScriptWriter.h src/pkcs11/SmartCardInfo.h src/util/CertificateInfo.h src/conf/PppUpScriptWriter.h src/dialogs/EapSettingsDialog.h src/conf/RSysLogConfWriter.h src/ConnectionState.h src/conf/PppDnsConfWriter.h src/models/IPsecCertificateListModel.h src/util/NetworkInterface.h src/localpeer/LockedFile.h src/conf/PPPConfWriter.h src/dialogs/ConnectionInformationDialog.h src/util/GlobalFunctions.h
FORMS += src/dialogs/PeerAuthenticationDialog.ui src/dialogs/SmartCardObjectsDialog.ui src/dialogs/ConnectionSettingsDialog.ui src/dialogs/ConnectionEditorDialog.ui src/dialogs/RouteSettingsDialog.ui src/dialogs/EapSettingsDialog.ui src/dialogs/AdvancedSettingsDialog.ui src/dialogs/IpSettingsDialog.ui src/dialogs/ConnectionInformationDialog.ui
RESOURCES += resources/L2tpIPsecVpn.qrc
TRANSLATIONS +=
OBJECTS_DIR = build/Release/GNU-Linux-x86
MOC_DIR = src/generated
RCC_DIR = src/generated
UI_DIR = src/generated
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += src 
LIBS += 
LIBS+=-lopensc

LIBS+=-lctemplate
target.path = /usr/bin
INSTALLS += target
data.path = /usr/share/L2tpIPsecVpn
data.files = resources/logo.png
INSTALLS += data