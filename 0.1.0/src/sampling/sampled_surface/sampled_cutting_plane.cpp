// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_cutting_plane.hpp"
#include "dictionary.hpp"
#include "vol_fields.hpp"
#include "vol_point_interpolation.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_mesh.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(sampledCuttingPlane, 0);
ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  sampledSurface,
  sampledCuttingPlane,
  word,
  cuttingPlane
);
}
// Private Member Functions 
void mousse::sampledCuttingPlane::createGeometry()
{
  if (debug)
  {
    Pout<< "sampledCuttingPlane::createGeometry :updating geometry."
      << endl;
  }
  // Clear any stored topologies
  facesPtr_.clear();
  isoSurfPtr_.ptr();
  pointDistance_.clear();
  cellDistancePtr_.clear();
  // Clear derived data
  clearGeom();
  // Get any subMesh
  if (zoneID_.index() != -1 && !subMeshPtr_.valid())
  {
    const polyBoundaryMesh& patches = mesh().boundaryMesh();
    // Patch to put exposed internal faces into
    const label exposedPatchI = patches.findPatchID(exposedPatchName_);
    if (debug)
    {
      Info<< "Allocating subset of size "
        << mesh().cellZones()[zoneID_.index()].size()
        << " with exposed faces into patch "
        << patches[exposedPatchI].name() << endl;
    }
    subMeshPtr_.reset
    (
      new fvMeshSubset(static_cast<const fvMesh&>(mesh()))
    );
    subMeshPtr_().setLargeCellSubset
    (
      labelHashSet(mesh().cellZones()[zoneID_.index()]),
      exposedPatchI
    );
  }
  // Select either the submesh or the underlying mesh
  const fvMesh& fvm =
  (
    subMeshPtr_.valid()
   ? subMeshPtr_().subMesh()
   : static_cast<const fvMesh&>(mesh())
  );
  // Distance to cell centres
  // ~~~~~~~~~~~~~~~~~~~~~~~~
  cellDistancePtr_.reset
  (
    new volScalarField
    (
      IOobject
      (
        "cellDistance",
        fvm.time().timeName(),
        fvm.time(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      fvm,
      dimensionedScalar("zero", dimLength, 0)
    )
  );
  volScalarField& cellDistance = cellDistancePtr_();
  // Internal field
  {
    const pointField& cc = fvm.cellCentres();
    scalarField& fld = cellDistance.internalField();
    FOR_ALL(cc, i)
    {
      // Signed distance
      fld[i] = (cc[i] - plane_.refPoint()) & plane_.normal();
    }
  }
  // Patch fields
  {
    FOR_ALL(cellDistance.boundaryField(), patchI)
    {
      if
      (
        isA<emptyFvPatchScalarField>
        (
          cellDistance.boundaryField()[patchI]
        )
      )
      {
        cellDistance.boundaryField().set
        (
          patchI,
          new calculatedFvPatchScalarField
          (
            fvm.boundary()[patchI],
            cellDistance
          )
        );
        const polyPatch& pp = fvm.boundary()[patchI].patch();
        pointField::subField cc = pp.patchSlice(fvm.faceCentres());
        fvPatchScalarField& fld = cellDistance.boundaryField()[patchI];
        fld.setSize(pp.size());
        FOR_ALL(fld, i)
        {
          fld[i] = (cc[i] - plane_.refPoint()) & plane_.normal();
        }
      }
      else
      {
        const pointField& cc = fvm.C().boundaryField()[patchI];
        fvPatchScalarField& fld = cellDistance.boundaryField()[patchI];
        FOR_ALL(fld, i)
        {
          fld[i] = (cc[i] - plane_.refPoint()) & plane_.normal();
        }
      }
    }
  }
  // On processor patches the mesh.C() will already be the cell centre
  // on the opposite side so no need to swap cellDistance.
  // Distance to points
  pointDistance_.setSize(fvm.nPoints());
  {
    const pointField& pts = fvm.points();
    FOR_ALL(pointDistance_, i)
    {
      pointDistance_[i] = (pts[i] - plane_.refPoint()) & plane_.normal();
    }
  }
  if (debug)
  {
    Pout<< "Writing cell distance:" << cellDistance.objectPath() << endl;
    cellDistance.write();
    pointScalarField pDist
    {
      IOobject
      {
        "pointDistance",
        fvm.time().timeName(),
        fvm.time(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      pointMesh::New(fvm),
      dimensionedScalar{"zero", dimLength, 0}
    };
    pDist.internalField() = pointDistance_;
    Pout<< "Writing point distance:" << pDist.objectPath() << endl;
    pDist.write();
  }
  //- Direct from cell field and point field.
  isoSurfPtr_.reset
  (
    new isoSurface
    {
      cellDistance,
      pointDistance_,
      0.0,
      regularise_,
      mergeTol_
    }
  );
  if (debug)
  {
    print(Pout);
    Pout<< endl;
  }
}
// Constructors 
mousse::sampledCuttingPlane::sampledCuttingPlane
(
  const word& name,
  const polyMesh& mesh,
  const dictionary& dict
)
:
  sampledSurface{name, mesh, dict},
  plane_{dict},
  mergeTol_{dict.lookupOrDefault("mergeTol", 1e-6)},
  regularise_{dict.lookupOrDefault("regularise", true)},
  average_{dict.lookupOrDefault("average", false)},
  zoneID_{dict.lookupOrDefault("zone", word::null), mesh.cellZones()},
  exposedPatchName_{word::null},
  needsUpdate_{true},
  subMeshPtr_{NULL},
  cellDistancePtr_{NULL},
  isoSurfPtr_{NULL},
  facesPtr_{NULL}
{
  if (zoneID_.index() != -1)
  {
    dict.lookup("exposedPatchName") >> exposedPatchName_;
    if (mesh.boundaryMesh().findPatchID(exposedPatchName_) == -1)
    {
      FATAL_ERROR_IN
      (
        "sampledCuttingPlane::sampledCuttingPlane"
        "(const word&, const polyMesh&, const dictionary&)"
      )
      << "Cannot find patch " << exposedPatchName_
      << " in which to put exposed faces." << endl
      << "Valid patches are " << mesh.boundaryMesh().names()
      << exit(FatalError);
    }
    if (debug && zoneID_.index() != -1)
    {
      Info<< "Restricting to cellZone " << zoneID_.name()
        << " with exposed internal faces into patch "
        << exposedPatchName_ << endl;
    }
  }
}
// Destructor 
mousse::sampledCuttingPlane::~sampledCuttingPlane()
{}
// Member Functions 
bool mousse::sampledCuttingPlane::needsUpdate() const
{
  return needsUpdate_;
}
bool mousse::sampledCuttingPlane::expire()
{
  if (debug)
  {
    Pout<< "sampledCuttingPlane::expire :"
      << " have-facesPtr_:" << facesPtr_.valid()
      << " needsUpdate_:" << needsUpdate_ << endl;
  }
  // Clear any stored topologies
  facesPtr_.clear();
  // Clear derived data
  clearGeom();
  // already marked as expired
  if (needsUpdate_)
  {
    return false;
  }
  needsUpdate_ = true;
  return true;
}
bool mousse::sampledCuttingPlane::update()
{
  if (debug)
  {
    Pout<< "sampledCuttingPlane::update :"
      << " have-facesPtr_:" << facesPtr_.valid()
      << " needsUpdate_:" << needsUpdate_ << endl;
  }
  if (!needsUpdate_)
  {
    return false;
  }
  createGeometry();
  needsUpdate_ = false;
  return true;
}
mousse::tmp<mousse::scalarField>
mousse::sampledCuttingPlane::sample
(
  const volScalarField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::vectorField>
mousse::sampledCuttingPlane::sample
(
  const volVectorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::sphericalTensorField>
mousse::sampledCuttingPlane::sample
(
  const volSphericalTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::symmTensorField>
mousse::sampledCuttingPlane::sample
(
  const volSymmTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::tensorField>
mousse::sampledCuttingPlane::sample
(
  const volTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::scalarField>
mousse::sampledCuttingPlane::interpolate
(
  const interpolation<scalar>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::vectorField>
mousse::sampledCuttingPlane::interpolate
(
  const interpolation<vector>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::sphericalTensorField>
mousse::sampledCuttingPlane::interpolate
(
  const interpolation<sphericalTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::symmTensorField>
mousse::sampledCuttingPlane::interpolate
(
  const interpolation<symmTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::tensorField>
mousse::sampledCuttingPlane::interpolate
(
  const interpolation<tensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
void mousse::sampledCuttingPlane::print(Ostream& os) const
{
  os  << "sampledCuttingPlane: " << name() << " :"
    << "  plane:" << plane_
    << "  faces:" << faces().size()
    << "  points:" << points().size();
}
