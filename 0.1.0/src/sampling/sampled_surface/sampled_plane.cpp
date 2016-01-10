// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_plane.hpp"
#include "dictionary.hpp"
#include "poly_mesh.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(sampledPlane, 0);
ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE(sampledSurface, sampledPlane, word, plane);
}
// Constructors 
mousse::sampledPlane::sampledPlane
(
  const word& name,
  const polyMesh& mesh,
  const plane& planeDesc,
  const keyType& zoneKey,
  const bool triangulate
)
:
  sampledSurface{name, mesh},
  cuttingPlane{planeDesc},
  zoneKey_{zoneKey},
  triangulate_{triangulate},
  needsUpdate_{true}
{
  if (debug && zoneKey_.size() && mesh.cellZones().findIndex(zoneKey_) < 0)
  {
    Info<< "cellZone " << zoneKey_
      << " not found - using entire mesh" << endl;
  }
}
mousse::sampledPlane::sampledPlane
(
  const word& name,
  const polyMesh& mesh,
  const dictionary& dict
)
:
  sampledSurface{name, mesh, dict},
  cuttingPlane{plane(dict.lookup("basePoint"), dict.lookup("normalVector"))},
  zoneKey_{keyType::null},
  triangulate_{dict.lookupOrDefault("triangulate", true)},
  needsUpdate_{true}
{
  // make plane relative to the coordinateSystem (Cartesian)
  // allow lookup from global coordinate systems
  if (dict.found("coordinateSystem"))
  {
    coordinateSystem cs(mesh, dict.subDict("coordinateSystem"));
    point  base = cs.globalPosition(planeDesc().refPoint());
    vector norm = cs.globalVector(planeDesc().normal());
    // assign the plane description
    static_cast<plane&>(*this) = plane(base, norm);
  }
  dict.readIfPresent("zone", zoneKey_);
  if (debug && zoneKey_.size() && mesh.cellZones().findIndex(zoneKey_) < 0)
  {
    Info<< "cellZone " << zoneKey_
      << " not found - using entire mesh" << endl;
  }
}
// Destructor 
mousse::sampledPlane::~sampledPlane()
{}
// Member Functions 
bool mousse::sampledPlane::needsUpdate() const
{
  return needsUpdate_;
}
bool mousse::sampledPlane::expire()
{
  // already marked as expired
  if (needsUpdate_)
  {
    return false;
  }
  sampledSurface::clearGeom();
  needsUpdate_ = true;
  return true;
}
bool mousse::sampledPlane::update()
{
  if (!needsUpdate_)
  {
    return false;
  }
  sampledSurface::clearGeom();
  labelList selectedCells = mesh().cellZones().findMatching(zoneKey_).used();
  if (selectedCells.empty())
  {
    reCut(mesh(), triangulate_);
  }
  else
  {
    reCut(mesh(), triangulate_, selectedCells);
  }
  if (debug)
  {
    print(Pout);
    Pout<< endl;
  }
  needsUpdate_ = false;
  return true;
}
mousse::tmp<mousse::scalarField> mousse::sampledPlane::sample
(
  const volScalarField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::vectorField> mousse::sampledPlane::sample
(
  const volVectorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::sphericalTensorField> mousse::sampledPlane::sample
(
  const volSphericalTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::symmTensorField> mousse::sampledPlane::sample
(
  const volSymmTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::tensorField> mousse::sampledPlane::sample
(
  const volTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::scalarField> mousse::sampledPlane::interpolate
(
  const interpolation<scalar>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::vectorField> mousse::sampledPlane::interpolate
(
  const interpolation<vector>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::sphericalTensorField> mousse::sampledPlane::interpolate
(
  const interpolation<sphericalTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::symmTensorField> mousse::sampledPlane::interpolate
(
  const interpolation<symmTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::tensorField> mousse::sampledPlane::interpolate
(
  const interpolation<tensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
void mousse::sampledPlane::print(Ostream& os) const
{
  os  << "sampledPlane: " << name() << " :"
    << "  base:" << refPoint()
    << "  normal:" << normal()
    << "  triangulate:" << triangulate_
    << "  faces:" << faces().size()
    << "  points:" << points().size();
}
