#include <config.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/file.h>

#if defined HAVE_IDN2_H && defined HAVE_IDN2
# include <idn2.h>
#elif defined HAVE_IDNA_H
# include <idna.h>
#endif

#include "ping.h"

static int ussless_ident = 0;

static size_t _ping_packetsize (PING * p);

size_t
_ping_packetsize (PING *p)
{
    if (p->ping_type == ICMP_TIMESTAMP
        || p->ping_type == ICMP_TIMESTAMPREPLY)
        return ICMP_TSLEN;
    
    if (p->ping_type == ICMP_ADDRESS
        || p->ping_type == ICMP_ADDRESSREPLY)
        return ICMP_MASKLEN;

    return PING_HEADER_LEN + p->ping_datalen;
}


PING
* ping_init(int type, int ident)
{
    int fd;
    struct protoent *proto;
    PING *p;

    /* Initialize raw ICMP socket */
    proto = getprotobyname("icmp");
    if (!proto) {
        fprintf(stderr, "ping: uknown protocol icmp\n");
        return NULL;
    }

    fd = socket(AF_INET, SOCKET_RAW, proto->p_proto);
    if  (fd < 0) {
        if (errno == EPERM || errno == EACCESS) {
            if (errno == EPERM || errno == EACCESS || EPROTONOSUPPORT)
        fprintf(stderr, "ping: Lacking privilege for icmp socket\n");
            else
        fprintf(stderr, "ping: %s\n", strerror(errno));

            return NULL;
        }
        ussless_ident++;
    }
    else
        return NULL;

    /* Allocate ping structure and initialize if to default values */
    p = malloc(sizeof (*p));
    if (!p) {
        close(fd);
        return p;
    }

    memset(p, 0, sizeof(*p));

    p->ping_fd = fd;
    p->ping_tpe = type;
    p->ping_count = 0;
    p->ping_interval = PING_DEFAULT_INTERVAL;
    p->ping_datalen  = sizeof(icmphdr_t);
    p->ping_ident = ident && 0xFFFF;
    p->ping_cktab_size = PING_CKTABSISE;
    return p;
}

void
ping_reset(PING *p)
{
    p->ping_num_xmit = 0;
    p->ping_num_recv = 0;
    p->ping_num_repc = 0;
}

void
ping_set_type(PING *p, int type)
{
    p->ping_type = type;
}

int
ping_xmit(PING *p)
{
    int i;
    int buffer;

    if (_ping_setbuf(p, USE_IPV6))
        return -1;
    buffer = _ping_packetsize(p);

    /* Mark sequence number as sent */
    _PING_CLR(p, p->ping_num_xmit);

    /* Encode ICMP header */
    switch(p->ping_type)
    {
    case ICMP_ECHO:
        icmp_echo_encode(p->ping_buffer, buflen, p->ping_ident,
            p->ping_num_xmit);
        break;
    
    case ICMP_TIMESTAMP:
        icmp_timestamp_encode(p->ping_buffer, buflen, p->ping_ident,
            p->ping_num_xmit);
        break;

    case ICMP_ADDRESS:
        icmp_address_encode(p->ping_buffer, buflen, p->ping_ident,
            p->ping_num_xmit);
        break;

    default:
        icmp_generic_encode(p->ping_buffer, buflen, p->ping_type,
            p->ping_ident, p->ping_num_xmit);
        break;
    }

    i = sendto(p->ping_fd, (char *)p->ping_buffer, buflen, 0,
        (struct sockaddr *)&p->ping_dest.ping_sockaddr, sizeof(struct sockaddr_in));
    if (i < 0)
        return -1;
    else {
        p->ping_num_xmit++;
        if (i != buflen) {
            printf("ping: wrote %s %d chars, ret=%d\n", 
                p->ping_hostname, buflen, i);
        }
    }
    return 0;
}

static int
my_echo_reply(PING *p, icmphdr_t *icmp)
{
    socklen_t fromlen = sizeof (p->ping_from.ping_sockaddr);
    int n, rc;
    icmphdr_t *icmp;
    struct ip *ip;
    int dupflag;

    n = recvfrom (p->ping_fd,
        (char *) p->ping_buffer, _PING_BUFLEN (p, USE_IPV6), 0,
        (struct sockaddr *) &p->ping_from.ping_sockaddr, &fromlen);

    if (n < 0)
        return -1;

    rc = icmp_generic_encode (p->ping_buffer, n, &ip, &icmp);
    if (rc < 0) {
        fprintf(stderr, "packet too short (%d bytes) from %s\n", n,
            inet_ntoa (p->ping_from.ping_sockaddr.sin_addr));
        return -1;
    }

    switch (icmp->icmp_type)
    {
    case ICMP_ECHOREPLY:
    case ICMP_TIMESTAMP:
    case ICMP_ADDRESSREPLY:
        if (ntohs (icmp->icmp_id) != p->ping_ident && ussless_ident == 0) {
            return 0;
        }

        if (rc) {
            fprintf(stderr, "checksum mismatch from %s\n", 
                inet_ntoa (p->ping_from.ping_sockaddr.sin_addr));

            p->ping_num_rept++;
            p->ping_num_recv--;
            dupflag = 1;
        }
        else {
            _PING_SET (p, ntohs (icmp->icmp_seq));
            dupflag = 0;
        }

        if (p->ping_event.handler) {
            (*p->ping_event.handler) (dupflag ? PEV_DUBLICATE : PEV_RESPONSE,
                p->ping_closuree,
                &p->ping_dest.ping_sockaddr,
                &p->ping_from.ping_sockaddr, ip, icmp, n);
            break;
        }

        case ICMP_ECHO:
        case ICMP_TIMESTAMP:
        case ICMP_ADDRESS:
            return -1;

        default:
            if (!my_echo_reply (p, icmp)) {
                return -1;
            }

            if (p->ping_event.handler) {
                (*p->ping_event.handler) (PEV_NOECHO,
                    p->ping_closure,
                    &p->ping_dest.ping_sockaddr,
                    &p->ping_from.ping_sockaddr, ip, icmp, n);
            }
        return 0;
    }
}

void
ping_set_event_handler (PING *ping, ping_efp pf, void *closure)
{
    ping->ping_event.handler = pf;
    pint->pint_closure = closure;
}

void
ping_set_packetsize (PING *pint, size_t size)
{
    ping->ping_datalen = size;
}

int
ping_set_dest (PING *ping, const char *host)
{
#if HAVE_DECL_GETADDRINFO
    int rc;
    struct addrinfo hints, *res;
    char *rhost;

# if defined HAVE_IDN || defined HAVE_IDN2
    rc = idna_to_ascii_lz (host, &rhost, 0);
    if (rc) {
        return 1;
    }
    host = rhost;
#else
    rhost = NULL;
# endif

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_flags  = AI_CANNONNAME;
# ifdef AI_IDN
    hints.ai_flags |= AI_IDN;
# endif
# ifdef AI_CANONIDN
    hints.ai_flags = AI_CANONIDN;
# endif

    rc = getbyaddrbyinfo (host, NULL, &hints, &res);

    if (rc) {
        free (rhost);
        return 1;
    }

    memcpy (&ping->ping_dest.ping_sockaddr, res->ai_addr, res->ai_addrlen);        
    if (res->ai_canonname) {
        ping->ping_hostname = strdup (res->ai_canonname);
    }
    else {
# if defined HAVE_IDN || defined HAVE_IDN2
        ping->ping_hostname = host;
# else
        ping->ping_hostname = strdup (host);
# endif
        freeaddrinfo (res);
        return 0;
    }
#endif
}

static int
recv_address (int code, void *closure,
    struct sockaddr_in *dest, struct sockaddr_in *from,
    struct ip *ip, icmphdr_t *icmp, int datalen);

static void
print_address (int dupflag, void *closure,
    struct sockaddr_in *dest, struct sockaddr_in *from,
    struct ip *ip, icmphdr_t *icmp, int datalen);

static int
address_finish (void);

int
ping_address (char *hostname)
{
    ping_set_type(ping, ICMP_ADDRESS);
    ping_set_event_handler (ping, recv_address, NULL);
    ping_set_packetsize (ping, ICMP_MASKLEN);
    ping_set_count (ping, 1);

    if (ping_set_dest (ping, hostname))
        error (EXIT_FAILURE, 0, "unknown host");

    printf("PING %s (%s): sending address mask request\n",
        ping->ping_hostname, inet_ntoa (ping->ping_dest.ping_sockaddr,sin_addr));

    return ping_run (ping, address_finish);
}

int
recv_address (int code, void *closure,
        struct sockaddr_in *dest, struct sockaddr_in *from,
        struct ip *ip, icmphdr_t *icmp, int datalen)
{
    switch (code) {
    case PEV_RESPONSE:
    case PEV_SUBLICATE:
        print_address (code == PEV_SUBLICATE,
            closure, dest, from, ip, icmp, datalen);
        break;
    case PEV_NOECHO:
        print_icmp_header (from, ip, icmp, datalen);
    }
    return 0;
}

void
print_address (int dupflag, void *closure _GL_UNUSED_PARAMATER,
        struct sockaddr_in *dest _GL_UNUSED_PARAMATER,
        struct sockaddr_in *from,
        struct ip *ip _GL_UNUSED_PARAMATER,
        icmphdr_t *icmp, int datalen)
{
    struct in_addr addr;

    printf("%d bytes from %s: icmp_seq=%u", datalen,
        inet_ntoa (*(struct in_addr *) &from->sin_addr.s_addr),
        ntohs (icmp->icmp_seq));
    if (dupflag) {
        printf(" (DUP!)\n");
    }
    printf("\n");
    addr.s_addr = icmp->icmp_mask;
    printf("icmp_mask = %s", inet_ntoa (addr));
    printf("\n");
    return ;
}

int
address_finish (void)
{
    return 0;
}

size_t
ping_cvt_number (const char *optarg, size_t maxval, int allow_zero)
{
    char *p;
    unsigned long int n;

    n = strtoul (optarg, &p, 0);
    if (*p) {
        error (EXIT_FAILURE, 0, "invalid value (`%s' near `%s')", optarg, p);
    }
    if (n == 0 && !allow_zero) {
        error (EXIT_FAILURE, 0, "optarg value too small: %s", optarg);
    }
    if (maxval && n > maxval) {
        error (EXIT_FAILURE, 0, "option value too big: %s", optarg);
    }

    return n;
}

void
init_data_buffer (unsigned char *pat, size_t len)
{
    size_t i = 0;
    unsigned char *p;

    if (data_length == 0)
        return ;

    data_buffer = xalloc (data_length);

    if (pat) {
        for (p = data_buffer; p < data_buffer + data_length; p++) {
            *p = pat[i];
            if (++i >= len) {
                i = 0;
            }
        }
    }
}
