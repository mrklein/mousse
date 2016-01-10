// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_iso_surface_cell.hpp"
#include "dictionary.hpp"
#include "vol_fields.hpp"
#include "vol_point_interpolation.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_mesh.hpp"
#include "iso_surface_cell.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(sampledIsoSurfaceCell, 0);
ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  sampledSurface,
  sampledIsoSurfaceCell,
  word,
  isoSurfaceCell
);
}
// Private Member Functions 
bool mousse::sampledIsoSurfaceCell::updateGeometry() const
{
  const fvMesh& fvm = static_cast<const fvMesh&>(mesh());
  // no update needed
  if (fvm.time().timeIndex() == prevTimeIndex_)
  {
    return false;
  }
  prevTimeIndex_ = fvm.time().timeIndex();
  // Clear any stored topo
  facesPtr_.clear();
  // Clear derived data
  sampledSurface::clearGeom();
  // Optionally read volScalarField
  autoPtr<volScalarField> readFieldPtr_;
  // 1. see if field in database
  // 2. see if field can be read
  const volScalarField* cellFldPtr = NULL;
  if (fvm.foundObject<volScalarField>(isoField_))
  {
    if (debug)
    {
      Info<< "sampledIsoSurfaceCell::updateGeometry() : lookup "
        << isoField_ << endl;
    }
    cellFldPtr = &fvm.lookupObject<volScalarField>(isoField_);
  }
  else
  {
    // Bit of a hack. Read field and store.
    if (debug)
    {
      Info<< "sampledIsoSurfaceCell::updateGeometry() : reading "
        << isoField_ << " from time " <<fvm.time().timeName()
        << endl;
    }
    readFieldPtr_.reset
    (
      new volScalarField
      (
        IOobject
        (
          isoField_,
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
  tmp<pointScalarField> pointFld
  (
    volPointInterpolation::New(fvm).interpolate(cellFld)
  );
  if (average_)
  {
    //- From point field and interpolated cell.
    scalarField cellAvg(fvm.nCells(), scalar(0.0));
    labelField nPointCells(fvm.nCells(), 0);
    {
      for (label pointI = 0; pointI < fvm.nPoints(); pointI++)
      {
        const labelList& pCells = fvm.pointCells(pointI);
        FOR_ALL(pCells, i)
        {
          label cellI = pCells[i];
          cellAvg[cellI] += pointFld().internalField()[pointI];
          nPointCells[cellI]++;
        }
      }
    }
    FOR_ALL(cellAvg, cellI)
    {
      cellAvg[cellI] /= nPointCells[cellI];
    }
    const isoSurfaceCell iso
    (
      fvm,
      cellAvg,
      pointFld().internalField(),
      isoVal_,
      regularise_
    );
    const_cast<sampledIsoSurfaceCell&>
    (
      *this
    ).triSurface::operator=(iso);
    meshCells_ = iso.meshCells();
  }
  else
  {
    //- Direct from cell field and point field. Gives bad continuity.
    const isoSurfaceCell iso
    (
      fvm,
      cellFld.internalField(),
      pointFld().internalField(),
      isoVal_,
      regularise_
    );
    const_cast<sampledIsoSurfaceCell&>
    (
      *this
    ).triSurface::operator=(iso);
    meshCells_ = iso.meshCells();
  }
  if (debug)
  {
    Pout<< "sampledIsoSurfaceCell::updateGeometry() : constructed iso:"
      << nl
      << "    regularise     : " << regularise_ << nl
      << "    average        : " << average_ << nl
      << "    isoField       : " << isoField_ << nl
      << "    isoValue       : " << isoVal_ << nl
      << "    points         : " << points().size() << nl
      << "    tris           : " << triSurface::size() << nl
      << "    cut cells      : " << meshCells_.size() << endl;
  }
  return true;
}
// Constructors 
mousse::sampledIsoSurfaceCell::sampledIsoSurfaceCell
(
  const word& name,
  const polyMesh& mesh,
  const dictionary& dict
)
:
  sampledSurface(name, mesh, dict),
  isoField_(dict.lookup("isoField")),
  isoVal_(readScalar(dict.lookup("isoValue"))),
  regularise_(dict.lookupOrDefault("regularise", true)),
  average_(dict.lookupOrDefault("average", true)),
  zoneKey_(keyType::null),
  facesPtr_(NULL),
  prevTimeIndex_(-1),
  meshCells_(0)
{
//    dict.readIfPresent("zone", zoneKey_);
//
//    if (debug && zoneKey_.size() && mesh.cellZones().findZoneID(zoneKey_) < 0)
//    {
//        Info<< "cellZone " << zoneKey_
//            << " not found - using entire mesh" << endl;
//    }
}
// Destructor 
mousse::sampledIsoSurfaceCell::~sampledIsoSurfaceCell()
{}
// Member Functions 
bool mousse::sampledIsoSurfaceCell::needsUpdate() const
{
  const fvMesh& fvm = static_cast<const fvMesh&>(mesh());
  return fvm.time().timeIndex() != prevTimeIndex_;
}
bool mousse::sampledIsoSurfaceCell::expire()
{
  facesPtr_.clear();
  // Clear derived data
  sampledSurface::clearGeom();
  // already marked as expired
  if (prevTimeIndex_ == -1)
  {
    return false;
  }
  // force update
  prevTimeIndex_ = -1;
  return true;
}
bool mousse::sampledIsoSurfaceCell::update()
{
  return updateGeometry();
}
mousse::tmp<mousse::scalarField>
mousse::sampledIsoSurfaceCell::sample
(
  const volScalarField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::vectorField>
mousse::sampledIsoSurfaceCell::sample
(
  const volVectorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::sphericalTensorField>
mousse::sampledIsoSurfaceCell::sample
(
  const volSphericalTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::symmTensorField>
mousse::sampledIsoSurfaceCell::sample
(
  const volSymmTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::tensorField>
mousse::sampledIsoSurfaceCell::sample
(
  const volTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::scalarField>
mousse::sampledIsoSurfaceCell::interpolate
(
  const interpolation<scalar>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::vectorField>
mousse::sampledIsoSurfaceCell::interpolate
(
  const interpolation<vector>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::sphericalTensorField>
mousse::sampledIsoSurfaceCell::interpolate
(
  const interpolation<sphericalTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::symmTensorField>
mousse::sampledIsoSurfaceCell::interpolate
(
  const interpolation<symmTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::tensorField>
mousse::sampledIsoSurfaceCell::interpolate
(
  const interpolation<tensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
void mousse::sampledIsoSurfaceCell::print(Ostream& os) const
{
  os  << "sampledIsoSurfaceCell: " << name() << " :"
    << "  field:" << isoField_
    << "  value:" << isoVal_;
    //<< "  faces:" << faces().size()   // possibly no geom yet
    //<< "  points:" << points().size();
}
