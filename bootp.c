#include <syslog.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include "bootp.h"

void setup_bootp_packet(const char *servername, const char *filename, bootp_packet *bpp) {
    memset(bpp, 0, sizeof(*bpp));
    bpp->opcode = 2;
    bpp->hw = 1;
    bpp->hwlength = 6;
    bpp->hopcount = 255; 
                         
    bpp->xid = htonl(0);
    bpp->secs = htons(0);
    bpp->flags = htons(0);
    
    memcpy(&bpp->ciaddr, &your_ip, 4);
    memcpy(&bpp->hwaddr, &BBB_hwaddr, 6);
    memcpy(&bpp->yiaddr, &your_ip, 4);
    memcpy(&bpp->server_ip, &BBB_ip, 4);
    memcpy(&bpp->bootp_gw_ip, &gateway, 4);
    
    strncpy((char *)&bpp->servername, servername, sizeof(bpp->servername));
    strncpy((char *)&bpp->bootfile, filename, sizeof(bpp->bootfile));

    memcpy(&bpp->vendor, vendor, 11);
    syslog(LOG_DEBUG, "%ld", sizeof(*bpp));
}

void debug_bootp_packet(bootp_packet *breq, int breqlen) {
    char vendor[100];
    struct in_addr address;
    u_int8_t *vndptr;
    
    memset(&address, 0, sizeof(address));

    syslog (LOG_DEBUG, "opcode: %i", breq->opcode);
    syslog (LOG_DEBUG, "hw: %i", breq->hw);
    syslog (LOG_DEBUG, "hwlength: %i", breq->hwlength);
    syslog (LOG_DEBUG, "hopcount: %i", breq->hopcount);
    syslog (LOG_DEBUG, "xid: 0x%08x", breq->xid);
    syslog (LOG_DEBUG, "secs: %i", breq->secs);
    syslog (LOG_DEBUG, "flags: 0x%04x", breq->flags);

    memcpy(&address.s_addr, breq->ciaddr, 4);
    syslog (LOG_DEBUG, "ciaddr: %s", inet_ntoa (address));

    memcpy(&address.s_addr, breq->yiaddr, 4);
    syslog (LOG_DEBUG, "yiaddr: %s", inet_ntoa (address));
    
    memcpy(&address.s_addr, breq->server_ip, 4);
    syslog (LOG_DEBUG, "server_ip: %s", inet_ntoa (address));
    
    memcpy(&address.s_addr, breq->bootp_gw_ip, 4);
    syslog (LOG_DEBUG, "bootp_gw_ip: %s", inet_ntoa (address));
    
    syslog (LOG_DEBUG, "hwaddr: %02X:%02X:%02X:%02X:%02X:%02X", 
            breq->hwaddr[0], breq->hwaddr[1], breq->hwaddr[2],
            breq->hwaddr[3], breq->hwaddr[4], breq->hwaddr[5]);
    
    syslog (LOG_DEBUG, "servername: %s", breq->servername);
    syslog (LOG_DEBUG, "bootfile: %s", breq->bootfile);
    
    vndptr = breq->vendor;
    sprintf (vendor, "Magic cookie:%d.%d.%d.%d ID: %d Length %d Subnet: %d.%d.%d.%d END: 0x%2X", 
        *vndptr, *(vndptr + 1), *(vndptr + 2), *(vndptr + 3), *(vndptr + 4), *(vndptr + 5), *(vndptr + 6), *(vndptr + 7), *(vndptr + 8), *(vndptr + 9), *(vndptr + 10));
    
    syslog (LOG_DEBUG, "vendor: %s", vendor);
    syslog(LOG_DEBUG, "END DEBUG!");
}