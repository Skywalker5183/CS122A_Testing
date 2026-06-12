#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

#define NO_SYS                      1
#define LWIP_SOCKET                 0
#define LWIP_NETCONN                0

// Required for httpc
#define LWIP_TCP                    1
#define LWIP_DNS                    1       // <-- needed for DNS resolution of api.open-meteo.com
#define LWIP_CALLBACK_API           1

#define LWIP_ALTCP                  1
#define LWIP_ALTCP_TLS              0

#define LWIP_HTTPD                  0       // we're a CLIENT not a server
#define LWIP_HTTP_CLIENT            1       // <-- enable the HTTP client (httpc)

#define LWIP_TIMERS                 1
#define LWIP_TIMERS_CUSTOM          0

#define LWIP_ARP                    1
#define LWIP_ETHERNET               1
#define LWIP_ICMP                   1
#define LWIP_RAW                    1
#define LWIP_DHCP                   1
#define LWIP_IPV4                   1
#define LWIP_UDP                    1

#define TCP_WND                     (8 * TCP_MSS)
#define TCP_MSS                     1460
#define SYS_LIGHTWEIGHT_PROT        0
#define MEM_ALIGNMENT               4
#define MEM_SIZE                    8000    // bump up for HTTP responses

#endif