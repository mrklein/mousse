#ifndef FINITE_VOLUME_FV_MESH_WALL_DIST_NEAR_WALL_DIST_NO_SEARCH_HPP_
#define FINITE_VOLUME_FV_MESH_WALL_DIST_NEAR_WALL_DIST_NO_SEARCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nearWallDistNoSearch
// Description
//   Distance calculation for cells with face on a wall. Does not search
//   anything, just takes normal component of distance.
// SourceFiles
//   near_wall_dist_no_search.cpp
#include "vol_fields.hpp"
namespace mousse
{
class fvMesh;
class nearWallDistNoSearch
:
  public volScalarField::GeometricBoundaryField
{
  // Private data
    //- Reference to mesh
    const fvMesh& mesh_;
  // Private Member Functions
    //- Do all calculations.
    void doAll();
public:
  // Constructors
    //- Construct from components
    nearWallDistNoSearch(const fvMesh& mesh);
    //- Disallow default bitwise copy construct
    nearWallDistNoSearch(const nearWallDistNoSearch&) = delete;
    //- Disallow default bitwise assignment
    nearWallDistNoSearch& operator=(const nearWallDistNoSearch&) = delete;
  //- Destructor
  virtual ~nearWallDistNoSearch();
  // Member Functions
    //- Correct for mesh geom/topo changes
    virtual void correct();
};
}  // namespace mousse
#endif
