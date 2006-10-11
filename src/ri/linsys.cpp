//////////////////////////////////////////////////////////////////////
//
//                             Pixie
//
// Copyright � 1999 - 2003, Okan Arikan
//
// Contact: okan@cs.berkeley.edu
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
//  File				:	linsys.cpp
//  Classes				:	-
//  Description			:	Functions for solving linear system of equations
//
////////////////////////////////////////////////////////////////////////
#include <math.h>

#include "common/os.h"
#include "linsys.h"

#define TINY 1.0e-20

///////////////////////////////////////////////////////////////////////
// Function				:	ludcmp
// Description			:	LU Decomposition routine
// Return Value			:	-
// Comments				:
// Date last edited		:	1/12/2003
template <class T> void ludcmp(T **a, int n, int *indx, T *d) {
	int i,imax,j,k;
	T	 big,dum,sum,temp;
	T	 *vv;

	vv	=	(T *) alloca((n+1)*sizeof(T));

	*d=	(T)	1.0;
	for (i=1;i<=n;i++) {
		big=0.0;
		for (j=1;j<=n;j++)
			if ((temp=(T) fabs(a[i][j])) > big) big=(T) temp;
		if (big == 0.0) return;
		vv[i]=(T) (1.0/big);
	}
	for (j=1;j<=n;j++) {
		for (i=1;i<j;i++) {
			sum=a[i][j];
			for (k=1;k<i;k++) sum -= a[i][k]*a[k][j];
			a[i][j]=sum;
		}
		big=0.0;
		for (i=j;i<=n;i++) {
			sum=a[i][j];
			for (k=1;k<j;k++)
				sum -= a[i][k]*a[k][j];
			a[i][j]=sum;
			if ( (dum=(T) (vv[i]*fabs(sum))) >= big) {
				big=dum;
				imax=i;
			}
		}
		if (j != imax) {
			for (k=1;k<=n;k++) {
				dum=a[imax][k];
				a[imax][k]=a[j][k];
				a[j][k]=dum;
			}
			*d = -(*d);
			vv[imax]=vv[j];
		}
		indx[j]=imax;
		if (a[j][j] == 0.0) a[j][j]=(T) TINY;
		if (j != n) {
			dum=(T) (1.0/(a[j][j]));
			for (i=j+1;i<=n;i++) a[i][j] *= dum;
		}
	}
}

///////////////////////////////////////////////////////////////////////
// Function				:	ludcmp
// Description			:	LU backward substitution code
// Return Value			:	-
// Comments				:
// Date last edited		:	1/12/2003
template <class T> void lubksb(T **a, int n, int *indx, T b[]) {
	int		i,ii=0,ip,j;
	T		sum;

	for (i=1;i<=n;i++) {
		ip=indx[i];
		sum=b[ip];
		b[ip]=b[i];
		if (ii)
			for (j=ii;j<=i-1;j++) sum -= a[i][j]*b[j];
		else if (sum) ii=i;
		b[i]=sum;
	}
	for (i=n;i>=1;i--) {
		sum=b[i];
		for (j=i+1;j<=n;j++) sum -= a[i][j]*b[j];
		b[i]=sum/a[i][i];
	}
}

///////////////////////////////////////////////////////////////////////
// Function				:	linSolve
// Description			:	Solve Ax = b
// Return Value			:	x , in b
// Comments				:
// Date last edited		:	1/12/2003
void	linSolve(float *A,float *b,int n,int nrhs) {
	float	**cA;
	int		i;
	int		*index	=	(int *) alloca((n+1)*sizeof(int));
	float	d;

	cA	=	(float **) alloca(n*sizeof(float *));
	for (i=0;i<n;i++) {
		cA[i+1]	=	A-1;	A	+=	n;
	}

	ludcmp<float>(cA,n,index,&d);

	for (i=0;i<nrhs;i++) {
		lubksb<float>(cA,n,index,b - 1);	b	+=	9;
	}
}
