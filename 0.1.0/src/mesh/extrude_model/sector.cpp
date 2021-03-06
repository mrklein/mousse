// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sector.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "unit_conversion.hpp"


namespace mousse {
namespace extrudeModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(sector, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(extrudeModel, sector, dictionary);


// Constructors 
sector::sector(const dictionary& dict)
:
  extrudeModel{typeName, dict},
  axisPt_{coeffDict_.lookup("axisPt")},
  axis_{coeffDict_.lookup("axis")},
  angle_{degToRad(readScalar(coeffDict_.lookup("angle")))}
{}


// Destructor 
sector::~sector()
{}


// Operators
point sector::operator()
(
  const point& surfacePoint,
  const vector& /*surfaceNormal*/,
  const label layer
) const
{
  scalar sliceAngle;
  // For the case of a single layer extrusion assume a
  // symmetric sector about the reference plane is required
  if (nLayers_ == 1) {
    if (layer == 0) {
      sliceAngle = -angle_/2.0;
    } else {
      sliceAngle = angle_/2.0;
    }
  } else {
    sliceAngle = angle_*sumThickness(layer);
  }
  // Find projection onto axis (or rather decompose surfacePoint
  // into vector along edge (proj), vector normal to edge in plane
  // of surface point and surface normal.
  point d = surfacePoint - axisPt_;
  d -= (axis_ & d)*axis_;
  scalar dMag = mag(d);
  point edgePt = surfacePoint - d;
  // Rotate point around sliceAngle.
  point rotatedPoint = edgePt;
  if (dMag > VSMALL) {
    vector n = (d/dMag) ^ axis_;
    // Use either n or surfaceNormal
    rotatedPoint += + cos(sliceAngle)*d - sin(sliceAngle)*mag(d)*n;
  }
  return rotatedPoint;
}

}  // namespace extrudeModels
}  // namespace mousse

