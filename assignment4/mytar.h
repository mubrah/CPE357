#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _SYS_TYPES_H
#include <sys/types.h>
#endif

#ifndef _SYS_STAT_H
#include <sys/stat.h>
#endif

#ifndef _UNISTD_H
#include <unistd.h>
#endif

#ifndef _DIRENT_H
#include <dirent.h>
#endif

#ifndef MYTAR_H
#define MYTAR_H

#define TBLOCKSIZE 512
#define TNAMESIZE 100
#define TPREFIXSIZE 155
#define TCHECKSUMSIZE 8

/* tar Header Block, from POSIX 1003.1-1990.  */

/* POSIX header.  */

struct tarHeader {              /* byte offset */
  char name[TNAMESIZE];         /*   0 */
  char mode[8];                 /* 100 */
  char uid[8];                  /* 108 */
  char gid[8];                  /* 116 */
  char size[12];                /* 124 */
  char mtime[12];               /* 136 */
  char chksum[TCHECKSUMSIZE];   /* 148 */
  char typeflag;                /* 156 */
  char linkname[TNAMESIZE];     /* 157 */
  char magic[6];                /* 257 */
  char version[2];              /* 263 */
  char uname[32];               /* 265 */
  char gname[32];               /* 297 */
  char devmajor[8];             /* 329 */
  char devminor[8];             /* 337 */
  char prefix[TPREFIXSIZE];     /* 345 */
  char padding[12];             /* 500 */
};

#define TMAGIC   "ustar\0"      /* ustar and a null */
#define TMAGLEN  6
#define TVERSION "00"           /* 00 and no null */
#define TVERSLEN 2

/* Values used in typeflag field.  */
#define REGTYPE  '0'            /* regular file */
#define AREGTYPE '\0'           /* regular file */
#define LNKTYPE  '1'            /* link */
#define SYMTYPE  '2'            /* reserved */
#define CHRTYPE  '3'            /* character special */
#define BLKTYPE  '4'            /* block special */
#define DIRTYPE  '5'            /* directory */
#define FIFOTYPE '6'            /* FIFO special */
#define CONTTYPE '7'            /* reserved */


#endif