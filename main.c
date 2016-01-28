static char const cvsid[] = "$Id: main.c,v 2.1 2005/06/14 22:16:53 jls Exp $";

/*
 * Copyright 2005 SRC Computers, Inc.  All Rights Reserved.
 *
 *	Manufactured in the United States of America.
 *
 * SRC Computers, Inc.
 * 4240 N Nevada Avenue
 * Colorado Springs, CO 80907
 * (v) (719) 262-0213
 * (f) (719) 262-0223
 *
 * No permission has been granted to distribute this software
 * without the express permission of SRC Computers, Inc.
 *
 * This program is distributed WITHOUT ANY WARRANTY OF ANY KIND.
 */

#include <libmap.h>

void read_image (char *filename);
void dump_image (int64_t *X, char *name);
uint8_t cpu_median (uint8_t v[9]);
void cpu_compute (char *name);
uint8_t cpu_edge (uint8_t v[9]);
void subr ();

#define REF(_A,_i,_j) ((_A)[(_i)*SM+(_j)])
#define WN 5
#define WM 5

int SM, SN, SZ;
int64_t *A=NULL, *B=NULL;

int main (int argc, char *argv[]) {
    int64_t time;

    if (argc < 2) {
        fprintf (stderr, "need image file source as arg\n");
	exit (1);
	}

    map_allocate (1);

    read_image (argv[1]);

    subr (A, B, SN, SM, SZ/8, &time, 0);

    printf ("%lld clocks\n", time);

    dump_image (B, "res_map.pgm");

    map_free (1);

    cpu_compute ("res_cpu.pgm");

    exit (0);
    }

void read_image (char *filename) {
    FILE *fp;
    int i, j, mx;
    char str[1024];
    int64_t *img, v0;

    if ((fp = fopen (filename, "r")) == NULL) {
        fprintf (stderr, "couldn't open image file\n");
        exit (1);
        }

    fscanf (fp, "%s", str);
    if (strcmp ("P2", str) != 0) {
        fprintf (stderr, "image format error; must be a 'P2' pgm file\n");
        exit (1);
        }

    fscanf (fp, "%d %d", &SM, &SN);
    fscanf (fp, "%d", &mx);

    printf ("image size is %dx%d\n", SN, SM);

    SZ = SN*SM*8;
    if (SZ & 0x1f)
        SZ = (SZ+32) & 0xffffffe0;

    A = (int64_t*) Cache_Aligned_Allocate (SZ);
    B = (int64_t*) Cache_Aligned_Allocate (SZ);

    for (i=0; i<SN; i++)
        for (j=0; j<SM; j++) {
            fscanf (fp, "%lld", &v0);
            REF(A,i,j) = v0;
            }
    }

void cpu_compute (char *name) {
    int i, j, mx;
    char str[1024];
    int64_t *img, v0;
    int64_t *BC0, *BC1;
    uint8_t V[9], rr;

    BC0 = (int64_t*) Cache_Aligned_Allocate (SZ);
    BC1 = (int64_t*) Cache_Aligned_Allocate (SZ);

    for (i=0; i<SN-(WN-1)+2; i++)
        for (j=0; j<SM-(WM-1)+2; j++) {
            V[0] = REF(A,i,j);
            V[1] = REF(A,i,j+1);
            V[2] = REF(A,i,j+2);
            V[3] = REF(A,i+1,j);
            V[4] = REF(A,i+1,j+1);
            V[5] = REF(A,i+1,j+2);
            V[6] = REF(A,i+2,j);
            V[7] = REF(A,i+2,j+1);
            V[8] = REF(A,i+2,j+2);
	    rr = cpu_median (V);
	    REF(BC0,i,j) = rr;
            }

    for (i=0; i<SN-(WN-1); i++)
        for (j=0; j<SM-(WM-1); j++) {
            V[0] = REF(BC0,i,j);
            V[1] = REF(BC0,i,j+1);
            V[2] = REF(BC0,i,j+2);
            V[3] = REF(BC0,i+1,j);
            V[4] = REF(BC0,i+1,j+1);
            V[5] = REF(BC0,i+1,j+2);
            V[6] = REF(BC0,i+2,j);
            V[7] = REF(BC0,i+2,j+1);
            V[8] = REF(BC0,i+2,j+2);
	    rr = cpu_edge (V);
	    REF(BC1,i,j) = rr;
            }

    dump_image (BC1, name);
    }

void dump_image (int64_t *X, char *name) {
    FILE *fp;
    int64_t v0;
    int i, j;
    
    if ((fp = fopen (name, "w")) == NULL) {
        fprintf (stderr, "couldn't open file '%s'\n", name);
        exit (1);
        }
    
    fprintf (fp, "P2\n");
    fprintf (fp, "%d %d\n", SM-(WM-1), SN-(WN-1)); 
    fprintf (fp, "255\n");

    for (i=0; i<SN-(WN-1); i++) {
        for (j=0; j<SM-(WM-1); j++) {
            v0 = REF(X,i,j);
            fprintf (fp, " %d", (int)v0);
            }
        fprintf (fp, "\n");
        }
    }

uint8_t cpu_edge (uint8_t v[9]) {
    int i, j;
    int v0, v2, h0, h2, vs, hs, res;

    v0 = v[0] + v[1] + v[2];
    v2 = v[6] + v[7] + v[8];
    vs = v0 - v2;
    h0 = v[0] + v[3] + v[6];
    h2 = v[2] + v[5] + v[8];
    hs = h0 - h2;
    res = sqrt (vs*vs + hs*hs);
    if (res>255)
        res = 255;

    // printf ("%4u %4u %4u\n", v[0], v[1], v[2]);
    // printf ("%4u %4u %4u\n", v[3], v[4], v[5]);
    // printf ("%4u %4u %4u\n", v[6], v[7], v[8]);
    // printf ("  res = %u\n", res);
    return (uint8_t)res;
    }

uint8_t cpu_median (uint8_t v[9]) {
    int i, j;
    uint8_t tmp;

    for (i=8; i>=1; i--)
        for (j=0; j<i; j++)
            if (v[j] > v[j+1]) {
                tmp = v[j];
                v[j] = v[j+1];
                v[j+1] = tmp;
                }

    return v[4];
    }

