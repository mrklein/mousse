// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "distance_surface.hpp"
#include "dictionary.hpp"
#include "vol_fields.hpp"
#include "vol_point_interpolation.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_mesh.hpp"
#include "volume_type.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(distanceSurface, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(sampledSurface, distanceSurface, word);
}
// Private Member Functions 
void mousse::distanceSurface::createGeometry()
{
  if (debug)
  {
    Pout<< "distanceSurface::createGeometry :updating geometry." << endl;
  }
  // Clear any stored topologies
  facesPtr_.clear();
  isoSurfCellPtr_.clear();
  isoSurfPtr_.clear();
  // Clear derived data
  clearGeom();
  const fvMesh& fvm = static_cast<const fvMesh&>(mesh());
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
    const pointField& cc = fvm.C();
    scalarField& fld = cellDistance.internalField();
    List<pointIndexHit> nearest;
    surfPtr_().findNearest
    (
      cc,
      scalarField(cc.size(), GREAT),
      nearest
    );
    if (signed_)
    {
      List<volumeType> volType;
      surfPtr_().getVolumeType(cc, volType);
      FOR_ALL(volType, i)
      {
        volumeType vT = volType[i];
        if (vT == volumeType::OUTSIDE)
        {
          fld[i] = mousse::mag(cc[i] - nearest[i].hitPoint());
        }
        else if (vT == volumeType::INSIDE)
        {
          fld[i] = -mousse::mag(cc[i] - nearest[i].hitPoint());
        }
        else
        {
          FATAL_ERROR_IN
          (
            "void mousse::distanceSurface::createGeometry()"
          )   << "getVolumeType failure, neither INSIDE or OUTSIDE"
            << exit(FatalError);
        }
      }
    }
    else
    {
      FOR_ALL(nearest, i)
      {
        fld[i] = mousse::mag(cc[i] - nearest[i].hitPoint());
      }
    }
  }
  // Patch fields
  {
    FOR_ALL(fvm.C().boundaryField(), patchI)
    {
      const pointField& cc = fvm.C().boundaryField()[patchI];
      fvPatchScalarField& fld = cellDistance.boundaryField()[patchI];
      List<pointIndexHit> nearest;
      surfPtr_().findNearest
      (
        cc,
        scalarField(cc.size(), GREAT),
        nearest
      );
      if (signed_)
      {
        List<volumeType> volType;
        surfPtr_().getVolumeType(cc, volType);
        FOR_ALL(volType, i)
        {
          volumeType vT = volType[i];
          if (vT == volumeType::OUTSIDE)
          {
            fld[i] = mousse::mag(cc[i] - nearest[i].hitPoint());
          }
          else if (vT == volumeType::INSIDE)
          {
            fld[i] = -mousse::mag(cc[i] - nearest[i].hitPoint());
          }
          else
          {
            FATAL_ERROR_IN
            (
              "void mousse::distanceSurface::createGeometry()"
            )   << "getVolumeType failure, "
              << "neither INSIDE or OUTSIDE"
              << exit(FatalError);
          }
        }
      }
      else
      {
        FOR_ALL(nearest, i)
        {
          fld[i] = mousse::mag(cc[i] - nearest[i].hitPoint());
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
    List<pointIndexHit> nearest;
    surfPtr_().findNearest
    (
      pts,
      scalarField(pts.size(), GREAT),
      nearest
    );
    if (signed_)
    {
      List<volumeType> volType;
      surfPtr_().getVolumeType(pts, volType);
      FOR_ALL(volType, i)
      {
        volumeType vT = volType[i];
        if (vT == volumeType::OUTSIDE)
        {
          pointDistance_[i] =
            mousse::mag(pts[i] - nearest[i].hitPoint());
        }
        else if (vT == volumeType::INSIDE)
        {
          pointDistance_[i] =
            -mousse::mag(pts[i] - nearest[i].hitPoint());
        }
        else
        {
          FATAL_ERROR_IN
          (
            "void mousse::distanceSurface::createGeometry()"
          )   << "getVolumeType failure, neither INSIDE or OUTSIDE"
            << exit(FatalError);
        }
      }
    }
    else
    {
      FOR_ALL(nearest, i)
      {
        pointDistance_[i] = mousse::mag(pts[i]-nearest[i].hitPoint());
      }
    }
  }
  if (debug)
  {
    Pout<< "Writing cell distance:" << cellDistance.objectPath() << endl;
    cellDistance.write();
    pointScalarField pDist
    (
      IOobject
      (
        "pointDistance",
        fvm.time().timeName(),
        fvm.time(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      pointMesh::New(fvm),
      dimensionedScalar("zero", dimLength, 0)
    );
    pDist.internalField() = pointDistance_;
    Pout<< "Writing point distance:" << pDist.objectPath() << endl;
    pDist.write();
  }
  //- Direct from cell field and point field.
  if (cell_)
  {
    isoSurfCellPtr_.reset
    (
      new isoSurfaceCell
      (
        fvm,
        cellDistance,
        pointDistance_,
        distance_,
        regularise_
      )
    );
  }
  else
  {
    isoSurfPtr_.reset
    (
      new isoSurface
      (
        cellDistance,
        pointDistance_,
        distance_,
        regularise_
      )
    );
  }
  if (debug)
  {
    print(Pout);
    Pout<< endl;
  }
}
// Constructors 
mousse::distanceSurface::distanceSurface
(
  const word& name,
  const polyMesh& mesh,
  const dictionary& dict
)
:
  sampledSurface(name, mesh, dict),
  surfPtr_
  (
    searchableSurface::New
    (
      dict.lookup("surfaceType"),
      IOobject
      (
        dict.lookupOrDefault("surfaceName", name),  // name
        mesh.time().constant(),                     // directory
        "triSurface",                               // instance
        mesh.time(),                                // registry
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      ),
      dict
    )
  ),
  distance_(readScalar(dict.lookup("distance"))),
  signed_(readBool(dict.lookup("signed"))),
  cell_(dict.lookupOrDefault("cell", true)),
  regularise_(dict.lookupOrDefault("regularise", true)),
  average_(dict.lookupOrDefault("average", false)),
  zoneKey_(keyType::null),
  needsUpdate_(true),
  isoSurfCellPtr_(NULL),
  isoSurfPtr_(NULL),
  facesPtr_(NULL)
{
//    dict.readIfPresent("zone", zoneKey_);
//
//    if (debug && zoneKey_.size() && mesh.cellZones().findZoneID(zoneKey_) < 0)
//    {
//        Info<< "cellZone " << zoneKey_
//            << " not found - using entire mesh" << endl;
//    }
}
mousse::distanceSurface::distanceSurface
(
  const word& name,
  const polyMesh& mesh,
  const bool interpolate,
  const word& surfaceType,
  const word& surfaceName,
  const scalar distance,
  const bool signedDistance,
  const bool cell,
  const Switch regularise,
  const Switch average
)
:
  sampledSurface(name, mesh, interpolate),
  surfPtr_
  (
    searchableSurface::New
    (
      surfaceType,
      IOobject
      (
        surfaceName,  // name
        mesh.time().constant(),                     // directory
        "triSurface",                               // instance
        mesh.time(),                                // registry
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      ),
      dictionary()
    )
  ),
  distance_(distance),
  signed_(signedDistance),
  cell_(cell),
  regularise_(regularise),
  average_(average),
  zoneKey_(keyType::null),
  needsUpdate_(true),
  isoSurfCellPtr_(NULL),
  isoSurfPtr_(NULL),
  facesPtr_(NULL)
{}
// Destructor 
mousse::distanceSurface::~distanceSurface()
{}
// Member Functions 
bool mousse::distanceSurface::needsUpdate() const
{
  return needsUpdate_;
}
bool mousse::distanceSurface::expire()
{
  if (debug)
  {
    Pout<< "distanceSurface::expire :"
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
bool mousse::distanceSurface::update()
{
  if (debug)
  {
    Pout<< "distanceSurface::update :"
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
mousse::tmp<mousse::scalarField> mousse::distanceSurface::sample
(
  const volScalarField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::vectorField> mousse::distanceSurface::sample
(
  const volVectorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::sphericalTensorField> mousse::distanceSurface::sample
(
  const volSphericalTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::symmTensorField> mousse::distanceSurface::sample
(
  const volSymmTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::tensorField> mousse::distanceSurface::sample
(
  const volTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::scalarField> mousse::distanceSurface::interpolate
(
  const interpolation<scalar>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::vectorField> mousse::distanceSurface::interpolate
(
  const interpolation<vector>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::sphericalTensorField> mousse::distanceSurface::interpolate
(
  const interpolation<sphericalTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::symmTensorField> mousse::distanceSurface::interpolate
(
  const interpolation<symmTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::tensorField> mousse::distanceSurface::interpolate
(
  const interpolation<tensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
void mousse::distanceSurface::print(Ostream& os) const
{
  os  << "distanceSurface: " << name() << " :"
    << "  surface:" << surfPtr_().name()
    << "  distance:" << distance_
    << "  faces:" << faces().size()
    << "  points:" << points().size();
}
