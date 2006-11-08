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
//  File				:	stochastic.h
//  Classes				:	CStochastic
//  Description			:
//
////////////////////////////////////////////////////////////////////////
#ifndef STOCHASTIC_H
#define STOCHASTIC_H

#include "common/global.h"
#include "reyes.h"
#include "occlusion.h"
#include "random.h"


///////////////////////////////////////////////////////////////////////
// Class				:	CStochastic
// Description			:	This is the stochastic hider (a scanline renderer)
// Comments				:
// Date last edited		:	7/31/2002
class	CStochastic : public CReyes, public COcclusionCuller {
public:
				CStochastic(COptions *,CXform *,SOCKET);
				~CStochastic();

				// The functions inherited from the CReyes
	void		rasterBegin(int,int,int,int,int);
	void		rasterDrawPrimitives(CRasterGrid *);
	void		rasterEnd(float *,int);
private:

	///////////////////////////////////////////////////////////////////////
	// Class				:	CFragment
	// Description			:	This class holds a sample
	// Comments				:
	// Date last edited		:	8/29/2004
	class	CFragment {
	public:
		vector			color;					// Color of the sample
		vector			opacity;				// Opacity of the sample
		float			z;						// Depth of the sample
		CFragment		*next;					// The next fragment wrt. depth
		CFragment		*prev;					// The previous fragment wrt. depth
	};

	///////////////////////////////////////////////////////////////////////
	// Class				:	CPixel
	// Description			:	This class holds a pixel
	// Comments				:
	// Date last edited		:	8/29/2004
	class	CPixel {
	public:
		float			jx,jy;					// The sampling jitter
		float			jt;						// The time jitter
		float			jdx,jdy;				// The aperture jitter	(Gaussian)
		float			jimp;					// The relative important // GSHHACK
		float			z;						// The farthest opaque z value
		float			zold;					// This is the old Z value (for depth filtering)
		int				numSplats;				// The number of splats to this pixel (used by the avg. depth filter);
		float			xcent,ycent;			// The center of the sampling window
		CFragment		first,last;				// The first and last fragments always exist
		CFragment		*update;				// The last fragment to be saved
		float			*extraSamples;			// Pointer to the extra samples array if any
		COcclusionNode	*node;					// The occlusion sample
	};


	void		filterSamples(int,CFragment **,float *);
	void		deepShadowCompute();

	int			totalWidth,totalHeight;
	CPixel		**fb;

	CFragment	*freeFragments;
	float		*extraSampleMemory;
	float		*pixelFilterWeights;
	
	int			width,height;
	int			top,left,right,bottom;
	int			sampleWidth,sampleHeight;
	
	CSobol<2>	apertureGenerator;
	
	// Define prototypes for the rasterization functions
	
	#define DEFINE_STOCHASTIC_FUNPROTOS
	#include "stochasticPrimitives.h"
	#undef DEFINE_STOCHASTIC_FUNPROTOS
};

#endif




