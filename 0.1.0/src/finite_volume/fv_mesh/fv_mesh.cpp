// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "sliced_vol_fields.hpp"
#include "sliced_surface_fields.hpp"
#include "sub_field.hpp"
#include "demand_driven_data.hpp"
#include "fv_mesh_ldu_addressing.hpp"
#include "map_poly_mesh.hpp"
#include "map_fv_fields.hpp"
#include "fv_mesh_mapper.hpp"
#include "map_clouds.hpp"
#include "_mesh_object.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(fvMesh, 0);
}
// Private Member Functions 
void mousse::fvMesh::clearGeomNotOldVol()
{
  meshObject::clearUpto
  <
    fvMesh,
    GeometricMeshObject,
    MoveableMeshObject
  >(*this);
  meshObject::clearUpto
  <
    lduMesh,
    GeometricMeshObject,
    MoveableMeshObject
  >(*this);
  slicedVolScalarField::DimensionedInternalField* VPtr =
    static_cast<slicedVolScalarField::DimensionedInternalField*>(VPtr_);
  deleteDemandDrivenData(VPtr);
  VPtr_ = NULL;
  deleteDemandDrivenData(SfPtr_);
  deleteDemandDrivenData(magSfPtr_);
  deleteDemandDrivenData(CPtr_);
  deleteDemandDrivenData(CfPtr_);
}
void mousse::fvMesh::updateGeomNotOldVol()
{
  bool haveV = (VPtr_ != NULL);
  bool haveSf = (SfPtr_ != NULL);
  bool haveMagSf = (magSfPtr_ != NULL);
  bool haveCP = (CPtr_ != NULL);
  bool haveCf = (CfPtr_ != NULL);
  clearGeomNotOldVol();
  // Now recreate the fields
  if (haveV)
  {
    (void)V();
  }
  if (haveSf)
  {
    (void)Sf();
  }
  if (haveMagSf)
  {
    (void)magSf();
  }
  if (haveCP)
  {
    (void)C();
  }
  if (haveCf)
  {
    (void)Cf();
  }
}
void mousse::fvMesh::clearGeom()
{
  clearGeomNotOldVol();
  deleteDemandDrivenData(V0Ptr_);
  deleteDemandDrivenData(V00Ptr_);
  // Mesh motion flux cannot be deleted here because the old-time flux
  // needs to be saved.
}
void mousse::fvMesh::clearAddressing(const bool isMeshUpdate)
{
  if (debug)
  {
    Info<< "fvMesh::clearAddressing(const bool) :"
      << " isMeshUpdate:" << isMeshUpdate << endl;
  }
  if (isMeshUpdate)
  {
    // Part of a mesh update. Keep meshObjects that have an updateMesh
    // callback
    meshObject::clearUpto
    <
      fvMesh,
      TopologicalMeshObject,
      UpdateableMeshObject
    >
    (
      *this
    );
    meshObject::clearUpto
    <
      lduMesh,
      TopologicalMeshObject,
      UpdateableMeshObject
    >
    (
      *this
    );
  }
  else
  {
    meshObject::clear<fvMesh, TopologicalMeshObject>(*this);
    meshObject::clear<lduMesh, TopologicalMeshObject>(*this);
  }
  deleteDemandDrivenData(lduPtr_);
}
void mousse::fvMesh::storeOldVol(const scalarField& V)
{
  if (curTimeIndex_ < time().timeIndex())
  {
    if (debug)
    {
      Info<< "fvMesh::storeOldVol(const scalarField&) :"
        << " Storing old time volumes since from time " << curTimeIndex_
        << " and time now " << time().timeIndex()
        << " V:" << V.size()
        << endl;
    }
    if (V00Ptr_ && V0Ptr_)
    {
      // Copy V0 into V00 storage
      *V00Ptr_ = *V0Ptr_;
    }
    if (V0Ptr_)
    {
      // Copy V into V0 storage
      V0Ptr_->scalarField::operator=(V);
    }
    else
    {
      // Allocate V0 storage, fill with V
      V0Ptr_ = new DimensionedField<scalar, volMesh>
      (
        IOobject
        (
          "V0",
          time().timeName(),
          *this,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        ),
        *this,
        dimVolume
      );
      scalarField& V0 = *V0Ptr_;
      // Note: V0 now sized with current mesh, not with (potentially
      //       different size) V.
      V0.setSize(V.size());
      V0 = V;
    }
    curTimeIndex_ = time().timeIndex();
    if (debug)
    {
      Info<< "fvMesh::storeOldVol() :"
        << " Stored old time volumes V0:" << V0Ptr_->size()
        << endl;
      if (V00Ptr_)
      {
        Info<< "fvMesh::storeOldVol() :"
          << " Stored oldold time volumes V00:" << V00Ptr_->size()
          << endl;
      }
    }
  }
}
void mousse::fvMesh::clearOut()
{
  clearGeom();
  surfaceInterpolation::clearOut();
  clearAddressing();
  // Clear mesh motion flux
  deleteDemandDrivenData(phiPtr_);
  polyMesh::clearOut();
}
// Constructors 
mousse::fvMesh::fvMesh(const IOobject& io)
:
  polyMesh(io),
  surfaceInterpolation(*this),
  fvSchemes(static_cast<const objectRegistry&>(*this)),
  fvSolution(static_cast<const objectRegistry&>(*this)),
  data(static_cast<const objectRegistry&>(*this)),
  boundary_(*this, boundaryMesh()),
  lduPtr_(NULL),
  curTimeIndex_(time().timeIndex()),
  VPtr_(NULL),
  V0Ptr_(NULL),
  V00Ptr_(NULL),
  SfPtr_(NULL),
  magSfPtr_(NULL),
  CPtr_(NULL),
  CfPtr_(NULL),
  phiPtr_(NULL)
{
  if (debug)
  {
    Info<< "Constructing fvMesh from IOobject"
      << endl;
  }
  // Check the existance of the cell volumes and read if present
  // and set the storage of V00
  if (isFile(time().timePath()/"V0"))
  {
    V0Ptr_ = new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        "V0",
        time().timeName(),
        *this,
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
      ),
      *this
    );
    V00();
  }
  // Check the existance of the mesh fluxes, read if present and set the
  // mesh to be moving
  if (isFile(time().timePath()/"meshPhi"))
  {
    phiPtr_ = new surfaceScalarField
    (
      IOobject
      (
        "meshPhi",
        time().timeName(),
        *this,
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
      ),
      *this
    );
    // The mesh is now considered moving so the old-time cell volumes
    // will be required for the time derivatives so if they haven't been
    // read initialise to the current cell volumes
    if (!V0Ptr_)
    {
      V0Ptr_ = new DimensionedField<scalar, volMesh>
      (
        IOobject
        (
          "V0",
          time().timeName(),
          *this,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        ),
        V()
      );
    }
    moving(true);
  }
}
mousse::fvMesh::fvMesh
(
  const IOobject& io,
  const Xfer<pointField>& points,
  const cellShapeList& shapes,
  const faceListList& boundaryFaces,
  const wordList& boundaryPatchNames,
  const PtrList<dictionary>& boundaryDicts,
  const word& defaultBoundaryPatchName,
  const word& defaultBoundaryPatchType,
  const bool syncPar
)
:
  polyMesh
  (
    io,
    points,
    shapes,
    boundaryFaces,
    boundaryPatchNames,
    boundaryDicts,
    defaultBoundaryPatchName,
    defaultBoundaryPatchType,
    syncPar
  ),
  surfaceInterpolation(*this),
  fvSchemes(static_cast<const objectRegistry&>(*this)),
  fvSolution(static_cast<const objectRegistry&>(*this)),
  data(static_cast<const objectRegistry&>(*this)),
  boundary_(*this, boundaryMesh()),
  lduPtr_(NULL),
  curTimeIndex_(time().timeIndex()),
  VPtr_(NULL),
  V0Ptr_(NULL),
  V00Ptr_(NULL),
  SfPtr_(NULL),
  magSfPtr_(NULL),
  CPtr_(NULL),
  CfPtr_(NULL),
  phiPtr_(NULL)
{
  if (debug)
  {
    Info<< "Constructing fvMesh from cellShapes" << endl;
  }
}
mousse::fvMesh::fvMesh
(
  const IOobject& io,
  const Xfer<pointField>& points,
  const Xfer<faceList>& faces,
  const Xfer<labelList>& allOwner,
  const Xfer<labelList>& allNeighbour,
  const bool syncPar
)
:
  polyMesh(io, points, faces, allOwner, allNeighbour, syncPar),
  surfaceInterpolation(*this),
  fvSchemes(static_cast<const objectRegistry&>(*this)),
  fvSolution(static_cast<const objectRegistry&>(*this)),
  data(static_cast<const objectRegistry&>(*this)),
  boundary_(*this, boundaryMesh()),
  lduPtr_(NULL),
  curTimeIndex_(time().timeIndex()),
  VPtr_(NULL),
  V0Ptr_(NULL),
  V00Ptr_(NULL),
  SfPtr_(NULL),
  magSfPtr_(NULL),
  CPtr_(NULL),
  CfPtr_(NULL),
  phiPtr_(NULL)
{
  if (debug)
  {
    Info<< "Constructing fvMesh from components" << endl;
  }
}
mousse::fvMesh::fvMesh
(
  const IOobject& io,
  const Xfer<pointField>& points,
  const Xfer<faceList>& faces,
  const Xfer<cellList>& cells,
  const bool syncPar
)
:
  polyMesh(io, points, faces, cells, syncPar),
  surfaceInterpolation(*this),
  fvSchemes(static_cast<const objectRegistry&>(*this)),
  fvSolution(static_cast<const objectRegistry&>(*this)),
  data(static_cast<const objectRegistry&>(*this)),
  boundary_(*this),
  lduPtr_(NULL),
  curTimeIndex_(time().timeIndex()),
  VPtr_(NULL),
  V0Ptr_(NULL),
  V00Ptr_(NULL),
  SfPtr_(NULL),
  magSfPtr_(NULL),
  CPtr_(NULL),
  CfPtr_(NULL),
  phiPtr_(NULL)
{
  if (debug)
  {
    Info<< "Constructing fvMesh from components" << endl;
  }
}
// Destructor 
mousse::fvMesh::~fvMesh()
{
  clearOut();
}
// Member Functions 
void mousse::fvMesh::addFvPatches
(
  const List<polyPatch*> & p,
  const bool validBoundary
)
{
  if (boundary().size())
  {
    FATAL_ERROR_IN
    (
      "fvMesh::addFvPatches(const List<polyPatch*>&, const bool)"
    )   << " boundary already exists"
      << abort(FatalError);
  }
  // first add polyPatches
  addPatches(p, validBoundary);
  boundary_.addPatches(boundaryMesh());
}
void mousse::fvMesh::removeFvBoundary()
{
  if (debug)
  {
    Info<< "void fvMesh::removeFvBoundary(): "
      << "Removing boundary patches."
      << endl;
  }
  // Remove fvBoundaryMesh data first.
  boundary_.clear();
  boundary_.setSize(0);
  polyMesh::removeBoundary();
  clearOut();
}
mousse::polyMesh::readUpdateState mousse::fvMesh::readUpdate()
{
  if (debug)
  {
    Info<< "polyMesh::readUpdateState fvMesh::readUpdate() : "
      << "Updating fvMesh.  ";
  }
  polyMesh::readUpdateState state = polyMesh::readUpdate();
  if (state == polyMesh::TOPO_PATCH_CHANGE)
  {
    if (debug)
    {
      Info<< "Boundary and topological update" << endl;
    }
    boundary_.readUpdate(boundaryMesh());
    clearOut();
  }
  else if (state == polyMesh::TOPO_CHANGE)
  {
    if (debug)
    {
      Info<< "Topological update" << endl;
    }
    clearOut();
  }
  else if (state == polyMesh::POINTS_MOVED)
  {
    if (debug)
    {
      Info<< "Point motion update" << endl;
    }
    clearGeom();
  }
  else
  {
    if (debug)
    {
      Info<< "No update" << endl;
    }
  }
  return state;
}
const mousse::fvBoundaryMesh& mousse::fvMesh::boundary() const
{
  return boundary_;
}
const mousse::lduAddressing& mousse::fvMesh::lduAddr() const
{
  if (!lduPtr_)
  {
    lduPtr_ = new fvMeshLduAddressing(*this);
  }
  return *lduPtr_;
}
void mousse::fvMesh::mapFields(const mapPolyMesh& meshMap)
{
  if (debug)
  {
    Info<< "fvMesh::mapFields :"
      << " nOldCells:" << meshMap.nOldCells()
      << " nCells:" << nCells()
      << " nOldFaces:" << meshMap.nOldFaces()
      << " nFaces:" << nFaces()
      << endl;
  }
  // We require geometric properties valid for the old mesh
  if
  (
    meshMap.cellMap().size() != nCells()
  || meshMap.faceMap().size() != nFaces()
  )
  {
    FATAL_ERROR_IN("fvMesh::mapFields(const mapPolyMesh&)")
      << "mapPolyMesh does not correspond to the old mesh."
      << " nCells:" << nCells()
      << " cellMap:" << meshMap.cellMap().size()
      << " nOldCells:" << meshMap.nOldCells()
      << " nFaces:" << nFaces()
      << " faceMap:" << meshMap.faceMap().size()
      << " nOldFaces:" << meshMap.nOldFaces()
      << exit(FatalError);
  }
  // Create a mapper
  const fvMeshMapper mapper(*this, meshMap);
  // Map all the volFields in the objectRegistry
  MapGeometricFields<scalar, fvPatchField, fvMeshMapper, volMesh>
  (mapper);
  MapGeometricFields<vector, fvPatchField, fvMeshMapper, volMesh>
  (mapper);
  MapGeometricFields<sphericalTensor, fvPatchField, fvMeshMapper, volMesh>
  (mapper);
  MapGeometricFields<symmTensor, fvPatchField, fvMeshMapper, volMesh>
  (mapper);
  MapGeometricFields<tensor, fvPatchField, fvMeshMapper, volMesh>
  (mapper);
  // Map all the surfaceFields in the objectRegistry
  MapGeometricFields<scalar, fvsPatchField, fvMeshMapper, surfaceMesh>
  (mapper);
  MapGeometricFields<vector, fvsPatchField, fvMeshMapper, surfaceMesh>
  (mapper);
  MapGeometricFields<symmTensor, fvsPatchField, fvMeshMapper, surfaceMesh>
  (mapper);
  MapGeometricFields<symmTensor, fvsPatchField, fvMeshMapper, surfaceMesh>
  (mapper);
  MapGeometricFields<tensor, fvsPatchField, fvMeshMapper, surfaceMesh>
  (mapper);
  // Map all the dimensionedFields in the objectRegistry
  MapDimensionedFields<scalar, fvMeshMapper, volMesh>(mapper);
  MapDimensionedFields<vector, fvMeshMapper, volMesh>(mapper);
  MapDimensionedFields<sphericalTensor, fvMeshMapper, volMesh>(mapper);
  MapDimensionedFields<symmTensor, fvMeshMapper, volMesh>(mapper);
  MapDimensionedFields<tensor, fvMeshMapper, volMesh>(mapper);
  // Map all the clouds in the objectRegistry
  mapClouds(*this, meshMap);
  const labelList& cellMap = meshMap.cellMap();
  // Map the old volume. Just map to new cell labels.
  if (V0Ptr_)
  {
    scalarField& V0 = *V0Ptr_;
    scalarField savedV0(V0);
    V0.setSize(nCells());
    FOR_ALL(V0, i)
    {
      if (cellMap[i] > -1)
      {
        V0[i] = savedV0[cellMap[i]];
      }
      else
      {
        V0[i] = 0.0;
      }
    }
    // Inject volume of merged cells
    label nMerged = 0;
    FOR_ALL(meshMap.reverseCellMap(), oldCellI)
    {
      label index = meshMap.reverseCellMap()[oldCellI];
      if (index < -1)
      {
        label cellI = -index-2;
        V0[cellI] += savedV0[oldCellI];
        nMerged++;
      }
    }
    if (debug)
    {
      Info<< "Mapping old time volume V0. Merged "
        << nMerged << " out of " << nCells() << " cells" << endl;
    }
  }
  // Map the old-old volume. Just map to new cell labels.
  if (V00Ptr_)
  {
    scalarField& V00 = *V00Ptr_;
    scalarField savedV00(V00);
    V00.setSize(nCells());
    FOR_ALL(V00, i)
    {
      if (cellMap[i] > -1)
      {
        V00[i] = savedV00[cellMap[i]];
      }
      else
      {
        V00[i] = 0.0;
      }
    }
    // Inject volume of merged cells
    label nMerged = 0;
    FOR_ALL(meshMap.reverseCellMap(), oldCellI)
    {
      label index = meshMap.reverseCellMap()[oldCellI];
      if (index < -1)
      {
        label cellI = -index-2;
        V00[cellI] += savedV00[oldCellI];
        nMerged++;
      }
    }
    if (debug)
    {
      Info<< "Mapping old time volume V00. Merged "
        << nMerged << " out of " << nCells() << " cells" << endl;
    }
  }
}
mousse::tmp<mousse::scalarField> mousse::fvMesh::movePoints(const pointField& p)
{
  // Grab old time volumes if the time has been incremented
  // This will update V0, V00
  if (curTimeIndex_ < time().timeIndex())
  {
    storeOldVol(V());
  }
  if (!phiPtr_)
  {
    // Create mesh motion flux
    phiPtr_ = new surfaceScalarField
    (
      IOobject
      (
        "meshPhi",
        this->time().timeName(),
        *this,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      *this,
      dimVolume/dimTime
    );
  }
  else
  {
    // Grab old time mesh motion fluxes if the time has been incremented
    if (phiPtr_->timeIndex() != time().timeIndex())
    {
      phiPtr_->oldTime();
    }
  }
  surfaceScalarField& phi = *phiPtr_;
  // Move the polyMesh and set the mesh motion fluxes to the swept-volumes
  scalar rDeltaT = 1.0/time().deltaTValue();
  tmp<scalarField> tsweptVols = polyMesh::movePoints(p);
  scalarField& sweptVols = tsweptVols();
  phi.internalField() = scalarField::subField(sweptVols, nInternalFaces());
  phi.internalField() *= rDeltaT;
  const fvPatchList& patches = boundary();
  FOR_ALL(patches, patchI)
  {
    phi.boundaryField()[patchI] = patches[patchI].patchSlice(sweptVols);
    phi.boundaryField()[patchI] *= rDeltaT;
  }
  // Update or delete the local geometric properties as early as possible so
  // they can be used if necessary. These get recreated here instead of
  // demand driven since they might do parallel transfers which can conflict
  // with when they're actually being used.
  // Note that between above "polyMesh::movePoints(p)" and here nothing
  // should use the local geometric properties.
  updateGeomNotOldVol();
  // Update other local data
  boundary_.movePoints();
  surfaceInterpolation::movePoints();
  meshObject::movePoints<fvMesh>(*this);
  meshObject::movePoints<lduMesh>(*this);
  return tsweptVols;
}
void mousse::fvMesh::updateMesh(const mapPolyMesh& mpm)
{
  // Update polyMesh. This needs to keep volume existent!
  polyMesh::updateMesh(mpm);
  if (VPtr_)
  {
    // Grab old time volumes if the time has been incremented
    // This will update V0, V00
    storeOldVol(mpm.oldCellVolumes());
    // Few checks
    if (VPtr_ && (V().size() != mpm.nOldCells()))
    {
      FATAL_ERROR_IN("fvMesh::updateMesh(const mapPolyMesh&)")
        << "V:" << V().size()
        << " not equal to the number of old cells "
        << mpm.nOldCells()
        << exit(FatalError);
    }
    if (V0Ptr_ && (V0Ptr_->size() != mpm.nOldCells()))
    {
      FATAL_ERROR_IN("fvMesh::updateMesh(const mapPolyMesh&)")
        << "V0:" << V0Ptr_->size()
        << " not equal to the number of old cells "
        << mpm.nOldCells()
        << exit(FatalError);
    }
    if (V00Ptr_ && (V00Ptr_->size() != mpm.nOldCells()))
    {
      FATAL_ERROR_IN("fvMesh::updateMesh(const mapPolyMesh&)")
        << "V0:" << V00Ptr_->size()
        << " not equal to the number of old cells "
        << mpm.nOldCells()
        << exit(FatalError);
    }
  }
  // Clear mesh motion flux (note: could instead save & map like volumes)
  deleteDemandDrivenData(phiPtr_);
  // Clear the sliced fields
  clearGeomNotOldVol();
  // Map all fields
  mapFields(mpm);
  // Clear the current volume and other geometry factors
  surfaceInterpolation::clearOut();
  // Clear any non-updateable addressing
  clearAddressing(true);
  meshObject::updateMesh<fvMesh>(*this, mpm);
  meshObject::updateMesh<lduMesh>(*this, mpm);
}
bool mousse::fvMesh::writeObjects
(
  IOstream::streamFormat fmt,
  IOstream::versionNumber ver,
  IOstream::compressionType cmp
) const
{
  return polyMesh::writeObject(fmt, ver, cmp);
}
//- Write mesh using IO settings from the time
bool mousse::fvMesh::write() const
{
  bool ok = true;
  if (phiPtr_)
  {
    ok = phiPtr_->write();
  }
  return ok && polyMesh::write();
}
// Member Operators 
bool mousse::fvMesh::operator!=(const fvMesh& bm) const
{
  return &bm != this;
}
bool mousse::fvMesh::operator==(const fvMesh& bm) const
{
  return &bm == this;
}
