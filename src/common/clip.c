/* Copyright (c) 1986 Regents of the University of California */

#ifndef lint
static char SCCSid[] = "$SunId$ LBL";
#endif

/*
 *  clip.c - routine to clip 3D line segments to a box.
 *
 *     8/28/85
 */

#include  "plocate.h"


clip(ep1, ep2, min, max)	/* clip a line segment to a box */
double  *ep1, *ep2;
double  min[3], max[3];
{
	int  loc1, loc2;
	int  accept;
	double  *dp;
	double  d;
	register int  i, j;

	/*
	 *	The Cohen-Sutherland algorithm is used to determine
	 *  what part (if any) of the given line segment is contained
	 *  in the box specified by the min and max vectors.
	 *	The routine returns non-zero if any segment is left.
	 */
	
	loc1 = plocate(ep1, min, max);
	loc2 = plocate(ep2, min, max);

			/* check for trivial accept and reject */
			/* trivial accept is both points inside */
			/* trivial reject is both points to one side */
	
	while (!((accept = !(loc1 | loc2)) || (loc1 & loc2))) {
	
		if (!loc1) {		/* make sure first point is outside */
			dp = ep1; ep1 = ep2; ep2 = dp;
			i = loc1; loc1 = loc2; loc2 = i;
		}
		
		for (i = 0; i < 3; i++) {		/* chop segment */
			
			if (loc1 & position(i) & BELOW) {
				d = (min[i] - ep1[i])/(ep2[i] - ep1[i]);
				ep1[i] = min[i];
			} else if (loc1 & position(i) & ABOVE) {
				d = (max[i] - ep1[i])/(ep2[i] - ep1[i]);
				ep1[i] = max[i];
			} else
				continue;
			
			for (j = 0; j < 3; j++)
				if (j != i)
					ep1[j] += (ep2[j] - ep1[j])*d;
			break;
		}
		loc1 = plocate(ep1, min, max);
	}
	return(accept);
}
