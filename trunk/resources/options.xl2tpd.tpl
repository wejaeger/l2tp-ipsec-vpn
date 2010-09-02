# {{FILENAME}} - Options used by PPP when a connection is made by an L2TP daemon
# $Id$

# Manual: PPPD(8)

# Created: {{CREATIONDATE}}
#      by: The {{APPNAME}} application version {{APPVERSION}}
#
# WARNING! All changes made in this file will be lost!

plugin passprompt.so
promptprog "{{APPPFILEATH}}"

ipparam {{IPPARAM}}
ipcp-accept-local
ipcp-accept-remote

{{#PEFUSE_SECTION}}{{REFUSEPROTOCOL}}
{{/PEFUSE_SECTION}}
crtscts
idle 72000
lock
mtu 1492
mru 1492

remotename "{{REMOTENAME}}"
name "{{NAME}}"

ktune
noproxyarp
{{USEPEERDNS}}

#debug
#dump
#record /var/log/pppd

cert "{{CERT}}"
ca "{{CA}}"
key "{{KEY}}"
