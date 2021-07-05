

It provides a system tray icon in the notification area from which a non privileged user can establish and bring down L2TP over IPsec VPN connections.

There is also a 'Edit connections' menu item. In order to bring up the editor dialog, a non privileged user must authenticate as root.

From there the user can add, remove and edit vpn connections.

Editing allows configuring various options for IPsec, L2TP and PPP.

Among others, the user can configure eg. the gateway, the use of either PSK or a certificate for authentication, various L2TP options as redial- timeout and attempts and of course all important PPP options. It also allows you to configure PPP for tunnel splitting because you can add routes as you want.

When applying your settings, all necessary configuration files are written accordingly (ipsec.conf, xl2tp.conf, options.xl2tpd, opensc.conf up and down scripts ...).

It relies on Openswan and xl2tp packages as the underlying protocol handlers.
Features

    Pure L2tp or L2tp over IPSEC VPN connetions
    Supports PSK or certificate IPSEC authentication
    Supports PAP, CHAP, MS-Chap, MS-Chap2, EAP authentication protocol
    Supports certificates for EAP authentication on local machine or on SmartCard
    Monitoring connection status

