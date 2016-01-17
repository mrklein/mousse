// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nearWallDist
// Description
//   Distance calculation for cells with face on a wall.
//   Searches pointNeighbours to find closest.
// SourceFiles
//   near_wall_dist.cpp
#ifndef near_wall_dist_hpp_
#define near_wall_dist_hpp_
#include "vol_fields.hpp"
namespace mousse
{
class fvMesh;
class nearWallDist
:
  public volScalarField::GeometricBoundaryField
{
  // Private data
    //- Reference to mesh
    const fvMesh& mesh_;
  // Private Member Functions
    //- Do all calculations
    void calculate();
public:
  // Constructors
    //- Construct from components
    nearWallDist(const fvMesh& mesh);
    //- Disallow default bitwise copy construct
    nearWallDist(const nearWallDist&) = delete;
    //- Disallow default bitwise assignment
    nearWallDist& operator=(const nearWallDist&) = delete;
  //- Destructor
  virtual ~nearWallDist();
  // Member Functions
    const volScalarField::GeometricBoundaryField& y() const
    {
      return *this;
    }
    //- Correct for mesh geom/topo changes
    virtual void correct();
};
}  // namespace mousse
#endif
