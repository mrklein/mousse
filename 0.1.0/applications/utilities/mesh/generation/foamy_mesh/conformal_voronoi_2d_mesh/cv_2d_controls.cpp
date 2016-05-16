// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cv_2d_controls.hpp"


// Constructors 
mousse::cv2DControls::cv2DControls
(
  const dictionary& controlDict,
  const boundBox& bb
)
:
  motionControl_{controlDict.subDict("motionControl")},
  conformationControl_{controlDict.subDict("surfaceConformation")},
  minCellSize_{readScalar(motionControl_.lookup("minCellSize"))},
  minCellSize2_{mousse::sqr(minCellSize_)},
  maxQuadAngle_{readScalar(conformationControl_.lookup("maxQuadAngle"))},
  nearWallAlignedDist_
  {
    readScalar(motionControl_.lookup("nearWallAlignedDist"))*minCellSize_
  },
  nearWallAlignedDist2_{mousse::sqr(nearWallAlignedDist_)},
  insertSurfaceNearestPointPairs_
  {
      conformationControl_.lookup("insertSurfaceNearestPointPairs")
  },
  mirrorPoints_{conformationControl_.lookup("mirrorPoints")},
  insertSurfaceNearPointPairs_
  {
    conformationControl_.lookup("insertSurfaceNearPointPairs")
  },
  objOutput_{motionControl_.lookupOrDefault<Switch>("objOutput", false)},
  meshedSurfaceOutput_
  {
    motionControl_.lookupOrDefault<Switch>("meshedSurfaceOutput", false)
  },
  randomiseInitialGrid_{conformationControl_.lookup("randomiseInitialGrid")},
  randomPerturbation_
  {
    readScalar(conformationControl_.lookup("randomPerturbation"))
  },
  maxBoundaryConformingIter_
  {
    readLabel(conformationControl_.lookup("maxBoundaryConformingIter"))
  },
  span_
  {
    max(mag(bb.max().x()), mag(bb.min().x()))
    + max(mag(bb.max().y()), mag(bb.min().y()))
  },
  span2_{mousse::sqr(span_)},
  minEdgeLen_
  {
    readScalar(conformationControl_.lookup("minEdgeLenCoeff"))
   *minCellSize_
  },
  minEdgeLen2_{mousse::sqr(minEdgeLen_)},
  maxNotchLen_
  {
    readScalar(conformationControl_.lookup("maxNotchLenCoeff"))*minCellSize_
  },
  maxNotchLen2_{mousse::sqr(maxNotchLen_)},
  minNearPointDist_
  {
    readScalar(conformationControl_.lookup("minNearPointDistCoeff"))
    *minCellSize_
  },
  minNearPointDist2_{mousse::sqr(minNearPointDist_)},
  ppDist_
  {
    readScalar(conformationControl_.lookup("pointPairDistanceCoeff"))
    *minCellSize_
  }
{}


// Destructor 
mousse::cv2DControls::~cv2DControls()
{}


// Member Functions 
void mousse::cv2DControls::write(Ostream& os) const
{
  os.indentLevel() = 1;
  os.precision(2);
  os.flags(ios_base::scientific);
  os << nl << "Outputting CV2D Mesher controls:" << nl
   << token::BEGIN_BLOCK << nl
   << indent << "minCellSize2_         : " << minCellSize2_ << nl
   << indent << "span_ / span2_        : " << span_ << " / " << span2_ << nl
   << indent << "maxNotchLen2_         : " << maxNotchLen2_ << nl
   << indent << "minNearPointDist2_    : " << minNearPointDist2_ << nl
   << indent << "nearWallAlignedDist2_ : " << nearWallAlignedDist2_ << nl
   << indent << "ppDist_               : " << ppDist_ << nl
   << indent << "minEdgeLen2_          : " << minEdgeLen2_ << nl
   << token::END_BLOCK << endl;
}


// IOStream operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const cv2DControls& s)
{
  s.write(os);
  return os;
}

