# $Id: Makefile,v 2.2 2006/05/15 17:12:10 rbb Exp $
#
# Copyright 2003 SRC Computers, Inc.  All Rights Reserved.
#
#       Manufactured in the United States of America.
#
# SRC Computers, Inc.
# 4240 N Nevada Avenue
# Colorado Springs, CO 80907
# (v) (719) 262-0213
# (f) (719) 262-0223
#
# No permission has been granted to distribute this software
# without the express permission of SRC Computers, Inc.
#
# This program is distributed WITHOUT ANY WARRANTY OF ANY KIND.
#
# -----------------------------------

# ----------------------------------
# User defines FILES, MAPFILES, and BIN here
# ----------------------------------
FILES       = main.c

MAPFILES    = ex_delay_queues.mc

BIN         = ex_delay_queues

MAPTARGET = map_m

# ----------------------------------
# Multi chip info provided here
# (Leave commented out if not used)
# ----------------------------------
#PRIMARY	= <primary file 1>   <primary file 2>

#SECONDARY	= <secondary file 1> <secondary file 2>

#CHIP2		= <file to compile to user chip 2>

#-----------------------------------
# User defined directory of code routines
# that are to be inlined
#------------------------------------

#INLINEDIR	=

# -----------------------------------
# User defined macros info supplied here
#
# (Leave commented out if not used)
# -----------------------------------
#MACROS		= <directory-name/macro-file>

#MY_BLKBOX 	= <directory-name/blackbox-file>
#MY_NGO_DIR 	= <directory-name>
#MY_INFO 	= <directory-name/info-file>
# -----------------------------------
# Floating point macros selection 
# -----------------------------------

#FPMODE		= SRC_IEEE_V1 # Default SRC version IEEE
#FPMODE		= SRC_IEEE_V2 # Size reduced SRC IEEE with 
			      # special rounding mode
# -----------------------------------
# User supplied MCC and MFTN flags
# -----------------------------------

MCCFLAGS 	= -log
MFTNFLAGS	= -log

# -----------------------------------
# User supplied flags for C & Fortran compilers
# -----------------------------------

CC		= gcc 	# icc   for Intel cc for Gnu
LD		= gcc	# for C codes

CFLAGS 		= 
FFLAGS		= 
LDFLAGS		=	# Flags to include libs if needed
# -----------------------------------
# VCS simulation settings
# (Set as needed, otherwise just leave commented out)
# -----------------------------------

#USEVCS		= yes	# YES or yes to use vcs instead of vcsi
#VCSDUMP	= yes	# YES or yes to generate vcd+ trace dump
# -----------------------------------
# MODELSIM simulation settings
# (Set as needed, otherwise just leave commented out)
# -----------------------------------

# -----------------------------------
# No modifications are required below
# -----------------------------------
MAKIN   ?= $(MC_ROOT)/opt/srcci/comp/lib/AppRules.make
include $(MAKIN)
