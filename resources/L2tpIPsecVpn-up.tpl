#!/bin/sh -e
#
# {{FILENAME}}
# $Id$
#
# This script is called by /etc/ppp/ip-up after pppd has established the link
# to add routes for given connections.
#
# Created: {{CREATIONDATE}}
#      by: The {{APPNAME}} application version {{APPVERSION}}
#
# WARNING! All changes made in this file will be lost!
#
#

PPP_IFACE="$IFNAME"
PPP_TTY="$DEVICE"
PPP_SPEED="$SPEED"
PPP_LOCAL="$IPLOCAL"
PPP_REMOTE="$IPREMOTE"
PPP_IPPARAM="$6"

if test -f /var/run/L2tpIPsecVpnControlDaemon/connectionName.info; then
   PPP_IPPARAM={{OBJECTNAME}}-$(cat /var/run/L2tpIPsecVpnControlDaemon/connectionName.info)
fi

echo "{{FILENAME}} called with PPP_IPPARAM=$PPP_IPPARAM"

# exit if PPP_IPPARAM is not set or does not start with "{{OBJECTNAME}}"
if [ ! `echo $PPP_IPPARAM | grep "{{OBJECTNAME}}"` ]; then
  exit 0
fi

. {{GETIPSECINFOLIB}}

# remember remote PPP address for given connection name
echo "${PPP_IPPARAM}" | cut -d'-' -f2 > /var/run/L2tpIPsecVpnControlDaemon/${PPP_REMOTE}
chmod go+r /var/run/L2tpIPsecVpnControlDaemon/${PPP_REMOTE}

case $PPP_IPPARAM in
{{#CONN_SECTION}}  "{{IPPARAM}}" )
    # direct tunneled packets to the tunnel server
    /sbin/route add -host {{GATEWAY}} gw ${DFLT_GWY} dev ${EXT_INTF}{{#ROUTE_SECTION}}
    /sbin/route add -net {{IPADDRESS}} netmask {{IPNETMASK}} dev ${PPP_IFACE}{{/ROUTE_SECTION}}{{#DEFAULT_GATEWAY_SECTION}}
    /sbin/route add -net 0.0.0.0 dev ${PPP_IFACE}
    /sbin/route add -net 0.0.0.0 gw ${DFLT_GWY} metric 100 dev ${EXT_INTF}
    /sbin/route del -net 0.0.0.0 gw ${DFLT_GWY} metric 0 dev ${EXT_INTF} || true {{/DEFAULT_GATEWAY_SECTION}}{{#NOROUTE_SECTION}}
    /sbin/route add -net {{IPADDRESS}} netmask {{IPNETMASK}} gw ${DFLT_GWY} dev ${EXT_INTF}{{/NOROUTE_SECTION}}
	 ;;

{{/CONN_SECTION}}
esac
