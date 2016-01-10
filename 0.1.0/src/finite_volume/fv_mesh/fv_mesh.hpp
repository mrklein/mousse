// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvMesh
// Description
//   Mesh data needed to do the Finite Volume discretisation.
//   NOTE ON USAGE:
//   fvMesh contains all the topological and geometric information
//   related to the mesh.  It is also responsible for keeping the data
//   up-to-date.  This is done by deleting the cell volume, face area,
//   cell/face centre, addressing and other derived information as
//   required and recalculating it as necessary.  The fvMesh therefore
//   reserves the right to delete the derived information upon every
//   topological (mesh refinement/morphing) or geometric change (mesh
//   motion).  It is therefore unsafe to keep local references to the
//   derived data outside of the time loop.
// SourceFiles
//   fv_mesh.cpp
//   fv_mesh_geometry.cpp
#ifndef fv_mesh_hpp_
#define fv_mesh_hpp_
#include "poly_mesh.hpp"
#include "ldu_mesh.hpp"
#include "primitive_mesh.hpp"
#include "fv_boundary_mesh.hpp"
#include "surface_interpolation.hpp"
#include "fv_schemes.hpp"
#include "fv_solution.hpp"
#include "data.hpp"
#include "dimensioned_field.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "point_fields_fwd.hpp"
#include "sliced_vol_fields_fwd.hpp"
#include "sliced_surface_fields_fwd.hpp"
#include "class_name.hpp"
namespace mousse
{
class fvMeshLduAddressing;
class volMesh;
class fvMesh
:
  public polyMesh,
  public lduMesh,
  public surfaceInterpolation,
  public fvSchemes,
  public fvSolution,
  public data
{
  // Private data
    //- Boundary mesh
    fvBoundaryMesh boundary_;
  // Demand-driven data
    mutable fvMeshLduAddressing* lduPtr_;
    //- Current time index for cell volumes
    //  Note.  The whole mechanism will be replaced once the
    //  dimensionedField is created and the dimensionedField
    //  will take care of the old-time levels.
    mutable label curTimeIndex_;
    //- Cell volumes old time level
    mutable void* VPtr_;
    //- Cell volumes old time level
    mutable DimensionedField<scalar, volMesh>* V0Ptr_;
    //- Cell volumes old-old time level
    mutable DimensionedField<scalar, volMesh>* V00Ptr_;
    //- Face area vectors
    mutable slicedSurfaceVectorField* SfPtr_;
    //- Mag face area vectors
    mutable surfaceScalarField* magSfPtr_;
    //- Cell centres
    mutable slicedVolVectorField* CPtr_;
    //- Face centres
    mutable slicedSurfaceVectorField* CfPtr_;
    //- Face motion fluxes
    mutable surfaceScalarField* phiPtr_;
  // Private Member Functions
    // Storage management
      //- Clear geometry but not the old-time cell volumes
      void clearGeomNotOldVol();
      //- Clear geometry like clearGeomNotOldVol but recreate any
      //  geometric demand-driven data that was set
      void updateGeomNotOldVol();
      //- Clear geometry
      void clearGeom();
      //- Clear addressing
      void clearAddressing(const bool isMeshUpdate = false);
      //- Preserve old volume(s)
      void storeOldVol(const scalarField&);
   // Make geometric data
      void makeSf() const;
      void makeMagSf() const;
      void makeC() const;
      void makeCf() const;
    //- Disallow construct as copy
    fvMesh(const fvMesh&);
    //- Disallow assignment
    void operator=(const fvMesh&);
public:
  // Public typedefs
    typedef fvMesh Mesh;
    typedef fvBoundaryMesh BoundaryMesh;
  // Declare name of the class and its debug switch
  CLASS_NAME("fvMesh");
  // Constructors
    //- Construct from IOobject
    explicit fvMesh(const IOobject& io);
    //- Construct from cellShapes with boundary.
    fvMesh
    (
      const IOobject& io,
      const Xfer<pointField>& points,
      const cellShapeList& shapes,
      const faceListList& boundaryFaces,
      const wordList& boundaryPatchNames,
      const PtrList<dictionary>& boundaryDicts,
      const word& defaultBoundaryPatchName,
      const word& defaultBoundaryPatchType,
      const bool syncPar = true
    );
    //- Construct from components without boundary.
    //  Boundary is added using addFvPatches() member function
    fvMesh
    (
      const IOobject& io,
      const Xfer<pointField>& points,
      const Xfer<faceList>& faces,
      const Xfer<labelList>& allOwner,
      const Xfer<labelList>& allNeighbour,
      const bool syncPar = true
    );
    //- Construct without boundary from cells rather than owner/neighbour.
    //  Boundary is added using addPatches() member function
    fvMesh
    (
      const IOobject& io,
      const Xfer<pointField>& points,
      const Xfer<faceList>& faces,
      const Xfer<cellList>& cells,
      const bool syncPar = true
    );
  //- Destructor
  virtual ~fvMesh();
  // Member Functions
    // Helpers
      //- Add boundary patches. Constructor helper
      void addFvPatches
      (
        const List<polyPatch*>&,
        const bool validBoundary = true
      );
      //- Update the mesh based on the mesh files saved in time
      //  directories
      virtual readUpdateState readUpdate();
    // Access
      //- Return the top-level database
      const Time& time() const
      {
        return polyMesh::time();
      }
      //- Return the object registry - resolve conflict polyMesh/lduMesh
      virtual const objectRegistry& thisDb() const
      {
        return polyMesh::thisDb();
      }
      //- Return reference to name
      //  Note: name() is currently ambiguous due to derivation from
      //  surfaceInterpolation
      const word& name() const
      {
        return polyMesh::name();
      }
      //- Return reference to boundary mesh
      const fvBoundaryMesh& boundary() const;
      //- Return ldu addressing
      virtual const lduAddressing& lduAddr() const;
      //- Return a list of pointers for each patch
      //  with only those pointing to interfaces being set
      virtual lduInterfacePtrsList interfaces() const
      {
        return boundary().interfaces();
      }
      //- Return communicator used for parallel communication
      virtual label comm() const
      {
        return polyMesh::comm();
      }
      //- Internal face owner
      const labelUList& owner() const
      {
        return lduAddr().lowerAddr();
      }
      //- Internal face neighbour
      const labelUList& neighbour() const
      {
        return lduAddr().upperAddr();
      }
      //- Return cell volumes
      const DimensionedField<scalar, volMesh>& V() const;
      //- Return old-time cell volumes
      const DimensionedField<scalar, volMesh>& V0() const;
      //- Return old-old-time cell volumes
      const DimensionedField<scalar, volMesh>& V00() const;
      //- Return sub-cycle cell volumes
      tmp<DimensionedField<scalar, volMesh> > Vsc() const;
      //- Return sub-cycl old-time cell volumes
      tmp<DimensionedField<scalar, volMesh> > Vsc0() const;
      //- Return cell face area vectors
      const surfaceVectorField& Sf() const;
      //- Return cell face area magnitudes
      const surfaceScalarField& magSf() const;
      //- Return cell face motion fluxes
      const surfaceScalarField& phi() const;
      //- Return cell centres as volVectorField
      const volVectorField& C() const;
      //- Return face centres as surfaceVectorField
      const surfaceVectorField& Cf() const;
      //- Return face deltas as surfaceVectorField
      tmp<surfaceVectorField> delta() const;
    // Edit
      //- Clear all geometry and addressing
      void clearOut();
      //- Update mesh corresponding to the given map
      virtual void updateMesh(const mapPolyMesh& mpm);
      //- Move points, returns volumes swept by faces in motion
      virtual tmp<scalarField> movePoints(const pointField&);
      //- Map all fields in time using given map.
      virtual void mapFields(const mapPolyMesh& mpm);
      //- Remove boundary patches. Warning: fvPatchFields hold ref to
      //  these fvPatches.
      void removeFvBoundary();
      //- Return cell face motion fluxes
      surfaceScalarField& setPhi();
      //- Return old-time cell volumes
      DimensionedField<scalar, volMesh>& setV0();
    // Write
      //- Write the underlying polyMesh and other data
      virtual bool writeObjects
      (
        IOstream::streamFormat fmt,
        IOstream::versionNumber ver,
        IOstream::compressionType cmp
      ) const;
      //- Write mesh using IO settings from time
      virtual bool write() const;
  // Member Operators
    bool operator!=(const fvMesh&) const;
    bool operator==(const fvMesh&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "fv_patch_fv_mesh_templates.cpp"
#endif
#endif
