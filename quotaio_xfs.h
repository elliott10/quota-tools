/*
 * Copyright (c) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 * Mountain View, CA  94043, or:
 *
 * http://www.sgi.com
 *
 * For further information regarding this notice, see:
 *
 * http://oss.sgi.com/projects/GenInfo/SGIGPLNoticeExplan/
 */
#ifndef GUARD_QUOTAIO_XFS_H
#define GUARD_QUOTAIO_XFS_H

#include <asm/types.h>

#define XQM_CMD(cmd)	( ('X'<<8)+(cmd) )
#define IS_XQM_CMD(cmd)	( ((int)(cmd)>>8) == 'X' )

/*
 * Disk quota - quotactl(2) commands for XFS Quota Manager (XQM).
 */
#define Q_XQUOTAON   XQM_CMD(0x1)	/* enable quota accounting/enforcement */
#define Q_XQUOTAOFF  XQM_CMD(0x2)	/* disable quota accounting/enforcement */
#define Q_XGETQUOTA  XQM_CMD(0x3)	/* get disk limits & usage */
#define Q_XSETQLIM   XQM_CMD(0x4)	/* set disk limits only */
#define Q_XGETQSTAT  XQM_CMD(0x5)	/* returns fs_quota_stat_t struct */
#define Q_XQUOTARM   XQM_CMD(0x6)	/* free quota files' space */
#define Q_XGETNEXTQUOTA	XQM_CMD(0x9)	/* get disk limits and usage >= ID */

/*
 * fs_disk_quota structure:
 *
 * This contains the current quota information regarding a user/proj/group.
 * It is 64-bit aligned, and all the blk units are in BBs (Basic Blocks) of
 * 512 bytes.
 */
#define FS_DQUOT_VERSION	1	/* fs_disk_quota.d_version */
typedef struct fs_disk_quota {
	__s8 d_version;		/* version of this structure */
	__s8 d_flags;		/* XFS_{USER,PROJ,GROUP}_QUOTA */
	__u16 d_fieldmask;	/* field specifier */
	__u32 d_id;		/* user, project, or group ID */
	__u64 d_blk_hardlimit;	/* absolute limit on disk blks */
	__u64 d_blk_softlimit;	/* preferred limit on disk blks */
	__u64 d_ino_hardlimit;	/* maximum # allocated inodes */
	__u64 d_ino_softlimit;	/* preferred inode limit */
	__u64 d_bcount;		/* # disk blocks owned by the user */
	__u64 d_icount;		/* # inodes owned by the user */
	__s32 d_itimer;		/* zero if within inode limits */
	/* if not, we refuse service */
	__s32 d_btimer;		/* similar to above; for disk blocks */
	__u16 d_iwarns;		/* # warnings issued wrt num inodes */
	__u16 d_bwarns;		/* # warnings issued wrt disk blocks */
	__s32 d_padding2;	/* padding2 - for future use */
	__u64 d_rtb_hardlimit;	/* absolute limit on realtime blks */
	__u64 d_rtb_softlimit;	/* preferred limit on RT disk blks */
	__u64 d_rtbcount;	/* # realtime blocks owned */
	__s32 d_rtbtimer;	/* similar to above; for RT disk blks */
	__u16 d_rtbwarns;	/* # warnings issued wrt RT disk blks */
	__s16 d_padding3;	/* padding3 - for future use */
	char d_padding4[8];	/* yet more padding */
} fs_disk_quota_t;

/*
 * These fields are sent to Q_XSETQLIM to specify fields that need to change.
 */
#define FS_DQ_ISOFT	(1<<0)
#define FS_DQ_IHARD	(1<<1)
#define FS_DQ_BSOFT	(1<<2)
#define FS_DQ_BHARD 	(1<<3)
#define FS_DQ_RTBSOFT	(1<<4)
#define FS_DQ_RTBHARD	(1<<5)
#define FS_DQ_LIMIT_MASK	(FS_DQ_ISOFT | FS_DQ_IHARD | FS_DQ_BSOFT | \
				 FS_DQ_BHARD | FS_DQ_RTBSOFT | FS_DQ_RTBHARD)
/*
 * These timers can only be set in super user's dquot. For others, timers are
 * automatically started and stopped. Superusers timer values set the limits
 * for the rest.  In case these values are zero, the DQ_{F,B}TIMELIMIT values
 * defined below are used. 
 * These values also apply only to the d_fieldmask field for Q_XSETQLIM.
 */
#define FS_DQ_BTIMER	(1<<6)
#define FS_DQ_ITIMER	(1<<7)
#define FS_DQ_RTBTIMER 	(1<<8)
#define FS_DQ_TIMER_MASK	(FS_DQ_BTIMER | FS_DQ_ITIMER | FS_DQ_RTBTIMER)

/*
 * Accounting values.  These can only be set for filesystem with
 * non-transactional quotas that require quotacheck(8) in userspace.                   
 */
#define FS_DQ_BCOUNT            (1<<12)
#define FS_DQ_ICOUNT            (1<<13)
#define FS_DQ_RTBCOUNT          (1<<14)
#define FS_DQ_ACCT_MASK         (FS_DQ_BCOUNT | FS_DQ_ICOUNT | FS_DQ_RTBCOUNT)

/*
 * Various flags related to quotactl(2).  Only relevant to XFS filesystems.
 */
#define XFS_QUOTA_UDQ_ACCT	(1<<0)	/* user quota accounting */
#define XFS_QUOTA_UDQ_ENFD	(1<<1)	/* user quota limits enforcement */
#define XFS_QUOTA_GDQ_ACCT	(1<<2)	/* group quota accounting */
#define XFS_QUOTA_GDQ_ENFD	(1<<3)	/* group quota limits enforcement */

#define XFS_USER_QUOTA		(1<<0)	/* user quota type */
#define XFS_PROJ_QUOTA		(1<<1)	/* (IRIX) project quota type */
#define XFS_GROUP_QUOTA		(1<<2)	/* group quota type */

/*
 * fs_quota_stat is the struct returned in Q_XGETQSTAT for a given file system.
 * Provides a centralized way to get meta infomation about the quota subsystem.
 * eg. space taken up for user and group quotas, number of dquots currently
 * incore.
 */
#define FS_QSTAT_VERSION	1	/* fs_quota_stat.qs_version */

/*
 * Some basic infomation about 'quota files'.
 */
typedef struct fs_qfilestat {
	__u64 qfs_ino;		/* inode number */
	__u64 qfs_nblks;	/* number of BBs 512-byte-blks */
	__u32 qfs_nextents;	/* number of extents */
} fs_qfilestat_t;

typedef struct fs_quota_stat {
	__s8 qs_version;	/* version number for future changes */
	__u16 qs_flags;		/* XFS_QUOTA_{U,P,G}DQ_{ACCT,ENFD} */
	__s8 qs_pad;		/* unused */
	fs_qfilestat_t qs_uquota;	/* user quota storage information */
	fs_qfilestat_t qs_gquota;	/* group quota storage information */
	__u32 qs_incoredqs;	/* number of dquots incore */
	__s32 qs_btimelimit;	/* limit for blks timer */
	__s32 qs_itimelimit;	/* limit for inodes timer */
	__s32 qs_rtbtimelimit;	/* limit for rt blks timer */
	__u16 qs_bwarnlimit;	/* limit for num warnings */
	__u16 qs_iwarnlimit;	/* limit for num warnings */
} fs_quota_stat_t;

#endif /* GUARD_QUOTAIO_XFS_H */
