#ifndef FINITE_VOLUME_FV_MESH_WALL_DIST_PATCH_DIST_METHODS_MESH_WAVE_PATCH_DIST_METHOD_HPP_
#define FINITE_VOLUME_FV_MESH_WALL_DIST_PATCH_DIST_METHODS_MESH_WAVE_PATCH_DIST_METHOD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchDistMethods::meshWave
// Description
//   Fast topological mesh-wave method for calculating the distance to nearest
//   patch for all cells and boundary.
//   For regular/un-distorted meshes this method is accurate but for skewed,
//   non-orthogonal meshes it is approximate with the error increasing with the
//   degree of mesh distortion.  The distance from the near-wall cells to the
//   boundary may optionally be corrected for mesh distortion by setting
//   correctWalls = true.
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
//   mousse::patchDistMethod::Poisson
//   mousse::wallDist
// SourceFiles
//   mesh_wave_patch_dist_method.cpp
#include "patch_dist_method.hpp"
namespace mousse
{
namespace patchDistMethods
{
class meshWave
:
  public patchDistMethod
{
  // Private Member Data
    //- Do accurate distance calculation for near-wall cells.
    const bool correctWalls_;
    //- Number of unset cells and faces.
    mutable label nUnset_;
public:
  //- Runtime type information
  TYPE_NAME("meshWave");
  // Constructors
    //- Construct from coefficients dictionary, mesh
    //  and fixed-value patch set
    meshWave
    (
      const dictionary& dict,
      const fvMesh& mesh,
      const labelHashSet& patchIDs
    );
    //- Construct from mesh, fixed-value patch set and flag specifying
    //  whether or not to correct wall.
    //  Calculate for all cells. correctWalls : correct wall (face&point)
    //  cells for correct distance, searching neighbours.
    meshWave
    (
      const fvMesh& mesh,
      const labelHashSet& patchIDs,
      const bool correctWalls = true
    );
    //- Disallow default bitwise copy construct
    meshWave(const meshWave&) = delete;
    //- Disallow default bitwise assignment
    meshWave& operator=(const meshWave&) = delete;
  // Member Functions
    label nUnset() const
    {
      return nUnset_;
    }
    //- Correct the given distance-to-patch field
    virtual bool correct(volScalarField& y);
    //- Correct the given distance-to-patch and normal-to-patch fields
    virtual bool correct(volScalarField& y, volVectorField& n);
};
}  // namespace patchDistMethods
}  // namespace mousse
#endif
