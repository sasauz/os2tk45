#ifndef __IP_32H
#define __IP_32H

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 *
 *      @(#)ip.h        7.6.1.1 (Berkeley) 3/15/88
 */
#define LITTLE_ENDIAN   1234    /* least-significant byte first (vax) */
#define BIG_ENDIAN      4321    /* most-significant byte first (IBM, net) */
#define PDP_ENDIAN      3412    /* LSB first in word, MSW first in long (pdp) */

#ifdef OS2
#define BYTE_ORDER      LITTLE_ENDIAN
#else
#ifndef BYTE_ORDER
/*
 * Definitions for byte order,
 * according to byte significance from low address to high.
 */

#ifdef vax
#define BYTE_ORDER      LITTLE_ENDIAN
#else
#define BYTE_ORDER      BIG_ENDIAN      /* mc68000, tahoe, most others */
#endif
#endif
#endif

/*
 * Definitions for internet protocol version 4.
 * Per RFC 791, September 1981.
 */
#define IPVERSION       4

/*
 * Structure of an internet header, naked of options.
 *
 * We declare ip_len and ip_off to be short, rather than u_short
 * pragmatically since otherwise unsigned comparisons can result
 * against negative integers quite easily, and fail in subtle ways.
 */
struct ip {
#if BYTE_ORDER == LITTLE_ENDIAN
        int     ip_hl:4,                /* header length */
                ip_v:4;                 /* version */
#endif
#if BYTE_ORDER == BIG_ENDIAN
        u_char  ip_v:4,                 /* version */
                ip_hl:4;                /* header length */
#endif
        u_char  ip_tos;                 /* type of service */
        short   ip_len;                 /* total length */
        u_short ip_id;                  /* identification */
        short   ip_off;                 /* fragment offset field */
#define IP_DF 0x4000                    /* dont fragment flag */
#define IP_MF 0x2000                    /* more fragments flag */
        u_char  ip_ttl;                 /* time to live */
        u_char  ip_p;                   /* protocol */
        u_short ip_sum;                 /* checksum */
        struct  in_addr ip_src,ip_dst;  /* source and dest address */
};
#ifndef OS2
#define IP_MAXPACKET    65535           /* maximum packet size */
#else
#define IP_MAXPACKET    32767           /* maximum packet size */
#endif

/*
 * Definitions for options.
 */
#define IPOPT_COPIED(o)         ((o)&0x80)
#define IPOPT_CLASS(o)          ((o)&0x60)
#define IPOPT_NUMBER(o)         ((o)&0x1f)

#define IPOPT_CONTROL           0x00
#define IPOPT_RESERVED1         0x20
#define IPOPT_DEBMEAS           0x40
#define IPOPT_RESERVED2         0x60

#define IPOPT_EOL               0               /* end of option list */
#define IPOPT_NOP               1               /* no operation */

#define IPOPT_RR                7               /* record packet route */
#define IPOPT_TS                68              /* timestamp */
#define IPOPT_SECURITY          130             /* provide s,c,h,tcc */
#define IPOPT_LSRR              131             /* loose source route */
#define IPOPT_SATID             136             /* satnet id */
#define IPOPT_SSRR              137             /* strict source route */

/*
 * Offsets to fields in options other than EOL and NOP.
 */
#define IPOPT_OPTVAL            0               /* option ID */
#define IPOPT_OLEN              1               /* option length */
#define IPOPT_OFFSET            2               /* offset within option */
#define IPOPT_MINOFF            4               /* min value of above */

/*
 * Time stamp option structure.
 */
struct  ip_timestamp {
        u_char  ipt_code;               /* IPOPT_TS */
        u_char  ipt_len;                /* size of structure (variable) */
        u_char  ipt_ptr;                /* index of current entry */
#if BYTE_ORDER == LITTLE_ENDIAN
        int     ipt_flg:4,              /* flags, see below */
                ipt_oflw:4;             /* overflow counter */
#endif
#if BYTE_ORDER == BIG_ENDIAN
        u_char  ipt_oflw:4,             /* overflow counter */
                ipt_flg:4;              /* flags, see below */
#endif
        union ipt_timestamp {
                n_long  ipt_time[1];
                struct  ipt_ta {
                        struct in_addr ipt_addr;
                        n_long ipt_time;
                } ipt_ta[1];
        } ipt_timestamp;
};

/* flag bits for ipt_flg */
#define IPOPT_TS_TSONLY         0               /* timestamps only */
#define IPOPT_TS_TSANDADDR      1               /* timestamps and addresses */
#define IPOPT_TS_PRESPEC        2               /* specified modules only */

/* bits for security (not byte swapped) */
#define IPOPT_SECUR_UNCLASS     0x0000
#define IPOPT_SECUR_CONFID      0xf135
#define IPOPT_SECUR_EFTO        0x789a
#define IPOPT_SECUR_MMMM        0xbc4d
#define IPOPT_SECUR_RESTR       0xaf13
#define IPOPT_SECUR_SECRET      0xd788
#define IPOPT_SECUR_TOPSECRET   0x6bc5

/*
 * Internet implementation parameters.
 */
#define MAXTTL          255             /* maximum time to live (seconds) */
#define IPFRAGTTL       60              /* time to live for frags, slowhz */
#define IPTTLDEC        1               /* subtracted when forwarding */

#define IP_MSS          576             /* default maximum segment size */

#pragma pack(1)
struct  ipstat {
        long    ips_total;              /* total packets received */
        long    ips_badsum;             /* checksum bad */
        long    ips_tooshort;           /* packet too short */
        long    ips_toosmall;           /* not enough data */
        long    ips_badhlen;            /* ip header length < data size */
        long    ips_badlen;             /* ip length < ip header length */
        long    ips_fragments;          /* fragments received */
        long    ips_fragdropped;        /* frags dropped (dups, out of space) */
        long    ips_fragtimeout;        /* fragments timed out */
        long    ips_forward;            /* packets forwarded */
        long    ips_cantforward;        /* packets rcvd for unreachable dest */
        long    ips_redirectsent;       /* packets forwarded on same net */
        long    ips_ipOutRequests;      /* requested for transmission */
        long    ips_ipOutNoRoutes;      /* discarded - no route */
        long    ips_ipFragFails;        /* had to fragment but could not */
        long    ips_ipFragOKs;          /* successfully fragmented datagrams */
        long    ips_ipFragCreates;      /* number of fragments created */
        long    ips_ipReasmOKs;         /* datagrams successfully assembled */
        long    ips_ipOutDiscards;      /* discarded due to lack of space */
        long    ips_ipInDiscards;       /* discarded due to lack of space 0 */
        long    ips_ipInAddrErrors;     /* discarded due to lack of space 0 */
        long    ips_ipInDelivers;       /* datagrams successfully delivered */
        long    ips_ipInUnknownProtos;  /* datagrams with unknown protocol  */
};
#pragma pack()

#endif /* __IP_32H */
