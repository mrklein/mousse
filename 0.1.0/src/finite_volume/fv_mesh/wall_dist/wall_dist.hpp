#ifndef FINITE_VOLUME_FV_MESH_WALL_DIST_WALL_DIST_HPP_
#define FINITE_VOLUME_FV_MESH_WALL_DIST_WALL_DIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallDist
// Description
//   Interface to run-time selectable methods to calculate the distance-to-wall
//   and normal-to-wall fields.
//   Example of the wallDist specification in fvSchemes:
//   \verbatim
//     wallDist
//     {
//       method meshWave;
//       // Optional entry enabling the calculation
//       // of the normal-to-wall field
//       nRequired false;
//     }
//   \endverbatim
// SeeAlso
//   mousse::patchDistMethod::meshWave
//   mousse::patchDistMethod::Poisson
//   mousse::patchDistMethod::advectionDiffusion
// SourceFiles
//   wall_dist.cpp
#include "_mesh_object.hpp"
#include "patch_dist_method.hpp"
#include "vol_fields.hpp"
namespace mousse
{
class wallDist
:
  public MeshObject<fvMesh, UpdateableMeshObject, wallDist>
{
  // Private data
    //- Set of patch IDs
    const labelHashSet patchIDs_;
    //- Name for the patch set, e.g. "wall"
    const word patchTypeName_;
    //- Run-time selected method to generate the distance-to-wall field
    mutable autoPtr<patchDistMethod> pdm_;
    //- Distance-to-wall field
    mutable volScalarField y_;
    //- Flag to indicate if the distance-to-wall field is required
    mutable bool nRequired_;
    //- Normal-to-wall field
    mutable tmp<volVectorField> n_;
  // Private Member Functions
    //- Construct the normal-to-wall field as required
    void constructn() const;
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("wallDist");
  // Constructors
    //- Construct from mesh and optional patch type name
    wallDist
    (
      const fvMesh& mesh,
      const word& patchTypeName = "wall"
    );
    //- Construct from mesh, patch IDs and optional patch type name
    wallDist
    (
      const fvMesh& mesh,
      const labelHashSet& patchIDs,
      const word& patchTypeName = "patch"
    );
    //- Disallow default bitwise copy construct
    wallDist(const wallDist&) = delete;
    //- Disallow default bitwise assignment
    wallDist& operator=(const wallDist&) = delete;
  //- Destructor
  virtual ~wallDist();
  // Member Functions
    //- Return the patchIDs
    const labelHashSet& patchIDs() const
    {
      return patchIDs_;
    }
    //- Return reference to cached distance-to-wall field
    const volScalarField& y() const
    {
      return y_;
    }
    //- Return reference to cached normal-to-wall field
    const volVectorField& n() const;
    //- Update the y-field when the mesh moves
    virtual bool movePoints();
    //- Update the y-field when the mesh changes
    virtual void updateMesh(const mapPolyMesh&);
};
}  // namespace mousse
#endif
