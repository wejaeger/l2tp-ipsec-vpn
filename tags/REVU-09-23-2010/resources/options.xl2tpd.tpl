# {{FILENAME}} - Options used by PPP when a connection is made by an L2TP daemon
# $Id$

# Manual: PPPD(8)

# Created: {{CREATIONDATE}}
#      by: The {{APPNAME}} application version {{APPVERSION}}
#
# WARNING! All changes made in this file will be lost!

#debug
#dump
#record /var/log/pppd

plugin passprompt.so
ipcp-accept-local
ipcp-accept-remote
idle 72000
ktune
noproxyarp
asyncmap 0
noauth
crtscts
lock
hide-password
modem
noipx

ipparam {{IPPARAM}}

promptprog "{{APPPFILEPATH}}"

{{#PEFUSE_SECTION}}{{REFUSEPROTOCOL}}
{{/PEFUSE_SECTION}}

remotename "{{REMOTENAME}}"
name "{{NAME}}"

cert "{{CERT}}"
ca "{{CA}}"
key "{{KEY}}"

{{USEPEERDNS}}
{{NOBSDCOMP}}
{{NODEFLATE}}
{{NOVJ}}
{{LCPECHOINTERVAL}}
