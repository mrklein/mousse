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
#ifndef near_wall_dist_no_search_hpp_
#define near_wall_dist_no_search_hpp_
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
    //- Disallow default bitwise copy construct
    nearWallDistNoSearch(const nearWallDistNoSearch&);
    //- Disallow default bitwise assignment
    void operator=(const nearWallDistNoSearch&);
public:
  // Constructors
    //- Construct from components
    nearWallDistNoSearch(const fvMesh& mesh);
  //- Destructor
  virtual ~nearWallDistNoSearch();
  // Member Functions
    //- Correct for mesh geom/topo changes
    virtual void correct();
};
}  // namespace mousse
#endif
