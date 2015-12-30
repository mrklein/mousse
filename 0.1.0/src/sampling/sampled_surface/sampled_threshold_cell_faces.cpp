// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_threshold_cell_faces.hpp"
#include "dictionary.hpp"
#include "vol_fields.hpp"
#include "vol_point_interpolation.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_mesh.hpp"
#include "threshold_cell_faces.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(sampledThresholdCellFaces, 0);
  addNamedToRunTimeSelectionTable
  (
    sampledSurface,
    sampledThresholdCellFaces,
    word,
    thresholdCellFaces
  );
}
// Private Member Functions 
bool mousse::sampledThresholdCellFaces::updateGeometry() const
{
  const fvMesh& fvm = static_cast<const fvMesh&>(mesh());
  // no update needed
  if (fvm.time().timeIndex() == prevTimeIndex_)
  {
    return false;
  }
  prevTimeIndex_ = fvm.time().timeIndex();
  // Optionally read volScalarField
  autoPtr<volScalarField> readFieldPtr_;
  // 1. see if field in database
  // 2. see if field can be read
  const volScalarField* cellFldPtr = NULL;
  if (fvm.foundObject<volScalarField>(fieldName_))
  {
    if (debug)
    {
      Info<< "sampledThresholdCellFaces::updateGeometry() : lookup "
        << fieldName_ << endl;
    }
    cellFldPtr = &fvm.lookupObject<volScalarField>(fieldName_);
  }
  else
  {
    // Bit of a hack. Read field and store.
    if (debug)
    {
      Info<< "sampledThresholdCellFaces::updateGeometry() : reading "
        << fieldName_ << " from time " << fvm.time().timeName()
        << endl;
    }
    readFieldPtr_.reset
    (
      new volScalarField
      (
        IOobject
        (
          fieldName_,
          fvm.time().timeName(),
          fvm,
          IOobject::MUST_READ,
          IOobject::NO_WRITE,
          false
        ),
        fvm
      )
    );
    cellFldPtr = readFieldPtr_.operator->();
  }
  const volScalarField& cellFld = *cellFldPtr;
  thresholdCellFaces surf
  (
    fvm,
    cellFld.internalField(),
    lowerThreshold_,
    upperThreshold_,
    triangulate_
  );
  const_cast<sampledThresholdCellFaces&>
  (
    *this
  ).MeshedSurface<face>::transfer(surf);
  meshCells_.transfer(surf.meshCells());
  // clear derived data
  sampledSurface::clearGeom();
  if (debug)
  {
    Pout<< "sampledThresholdCellFaces::updateGeometry() : constructed"
      << nl
      << "    field         : " << fieldName_ << nl
      << "    lowerLimit    : " << lowerThreshold_ << nl
      << "    upperLimit    : " << upperThreshold_ << nl
      << "    point         : " << points().size() << nl
      << "    faces         : " << faces().size() << nl
      << "    cut cells     : " << meshCells_.size() << endl;
  }
  return true;
}
// Constructors 
mousse::sampledThresholdCellFaces::sampledThresholdCellFaces
(
  const word& name,
  const polyMesh& mesh,
  const dictionary& dict
)
:
  sampledSurface(name, mesh, dict),
  fieldName_(dict.lookup("field")),
  lowerThreshold_(dict.lookupOrDefault<scalar>("lowerLimit", -VGREAT)),
  upperThreshold_(dict.lookupOrDefault<scalar>("upperLimit", VGREAT)),
  zoneKey_(keyType::null),
  triangulate_(dict.lookupOrDefault("triangulate", false)),
  prevTimeIndex_(-1),
  meshCells_(0)
{
  if (!dict.found("lowerLimit") && !dict.found("upperLimit"))
  {
    FatalErrorIn
      (
        "sampledThresholdCellFaces::sampledThresholdCellFaces(..)"
      )
      << "require at least one of 'lowerLimit' or 'upperLimit'" << endl
      << abort(FatalError);
  }
//    dict.readIfPresent("zone", zoneKey_);
//
//    if (debug && zoneKey_.size() && mesh.cellZones().findZoneID(zoneKey_) < 0)
//    {
//        Info<< "cellZone " << zoneKey_
//            << " not found - using entire mesh" << endl;
//    }
}
// Destructor 
mousse::sampledThresholdCellFaces::~sampledThresholdCellFaces()
{}
// Member Functions 
bool mousse::sampledThresholdCellFaces::needsUpdate() const
{
  const fvMesh& fvm = static_cast<const fvMesh&>(mesh());
  return fvm.time().timeIndex() != prevTimeIndex_;
}
bool mousse::sampledThresholdCellFaces::expire()
{
  // already marked as expired
  if (prevTimeIndex_ == -1)
  {
    return false;
  }
  // force update
  prevTimeIndex_ = -1;
  return true;
}
bool mousse::sampledThresholdCellFaces::update()
{
  return updateGeometry();
}
mousse::tmp<mousse::scalarField> mousse::sampledThresholdCellFaces::sample
(
  const volScalarField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::vectorField> mousse::sampledThresholdCellFaces::sample
(
  const volVectorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::sphericalTensorField> mousse::sampledThresholdCellFaces::sample
(
  const volSphericalTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::symmTensorField> mousse::sampledThresholdCellFaces::sample
(
  const volSymmTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::tensorField> mousse::sampledThresholdCellFaces::sample
(
  const volTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::scalarField> mousse::sampledThresholdCellFaces::interpolate
(
  const interpolation<scalar>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::vectorField> mousse::sampledThresholdCellFaces::interpolate
(
  const interpolation<vector>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::sphericalTensorField>
mousse::sampledThresholdCellFaces::interpolate
(
  const interpolation<sphericalTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::symmTensorField> mousse::sampledThresholdCellFaces::interpolate
(
  const interpolation<symmTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::tensorField> mousse::sampledThresholdCellFaces::interpolate
(
  const interpolation<tensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
void mousse::sampledThresholdCellFaces::print(Ostream& os) const
{
  os  << "sampledThresholdCellFaces: " << name() << " :"
    << "  field:" << fieldName_
    << "  lowerLimit:" << lowerThreshold_
    << "  upperLimit:" << upperThreshold_;
    //<< "  faces:" << faces().size()   // possibly no geom yet
    //<< "  points:" << points().size();
}
