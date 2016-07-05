// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_aspect_ratio_control.hpp"
#include "vector_tools.hpp"


// Constructors 
mousse::cellAspectRatioControl::cellAspectRatioControl
(
  const dictionary& motionDict
)
:
  aspectRatioDict_{motionDict.subOrEmptyDict("cellAspectRatioControl")},
  aspectRatio_{aspectRatioDict_.lookupOrDefault<scalar>("aspectRatio", 1.0)},
  aspectRatioDirection_
  {
    aspectRatioDict_.lookupOrDefault<vector>
    (
      "aspectRatioDirection",
      vector::zero
    )
  }
{
  // Normalise the direction
  aspectRatioDirection_ /= mag(aspectRatioDirection_) + SMALL;
  Info << nl
    << "Cell Aspect Ratio Control" << nl
    << "    Ratio     : " << aspectRatio_ << nl
    << "    Direction : " << aspectRatioDirection_
    << endl;
}


// Destructor 
mousse::cellAspectRatioControl::~cellAspectRatioControl()
{}


// Member Functions 
void mousse::cellAspectRatioControl::updateCellSizeAndFaceArea
(
  vector& alignmentDir,
  scalar& targetFaceArea,
  scalar& targetCellSize
) const
{
  const scalar cosAngle =
    mag(vectorTools::cosPhi(alignmentDir, aspectRatioDirection_));
  // Change target face area based on aspect ratio
  targetFaceArea +=
    targetFaceArea*(aspectRatio_ - 1.0)*(1.0 - cosAngle);
  // Change target cell size based on aspect ratio
  targetCellSize +=
    targetCellSize*(aspectRatio_ - 1.0)*cosAngle;
  alignmentDir *= 0.5*targetCellSize;
}


void mousse::cellAspectRatioControl::updateDeltaVector
(
  const vector& alignmentDir,
  const scalar targetCellSize,
  const scalar rABMag,
  vector& delta
) const
{
  const scalar cosAngle =
    mag(vectorTools::cosPhi(alignmentDir, aspectRatioDirection_));
  delta +=
    0.5*delta*cosAngle*(targetCellSize/rABMag)*(aspectRatio_ - 1.0);
}

