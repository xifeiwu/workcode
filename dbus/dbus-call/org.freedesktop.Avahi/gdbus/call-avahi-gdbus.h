#include <gio/gio.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    guint length;
    guchar **txt;
}_TxtInfo;
void showTxfInfo(_TxtInfo);
GDBusProxy *proxy_avahi_service;
GDBusProxy *proxy_avahi_service_browser;