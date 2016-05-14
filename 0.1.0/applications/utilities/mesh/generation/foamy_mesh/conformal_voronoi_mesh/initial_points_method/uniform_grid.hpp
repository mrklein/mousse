#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_INITIAL_POINTS_METHOD_UNIFORM_GRID_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_INITIAL_POINTS_METHOD_UNIFORM_GRID_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformGrid
// Description
//   Generate a uniform grid of points inside the surfaces to be
//   conformed to of the conformalVoronoiMesh

#include "initial_points_method.hpp"


namespace mousse {

class uniformGrid
:
  public initialPointsMethod
{
private:
  // Private data
  //- The initial cell spacing
  scalar initialCellSize_;
  //- Should the initial positions be randomised
  Switch randomiseInitialGrid_;
  //- Randomise the initial positions by fraction of the initialCellSize_
  scalar randomPerturbationCoeff_;
public:
  //- Runtime type information
  TYPE_NAME("uniformGrid");
  // Constructors
    //- Construct from components
    uniformGrid
    (
      const dictionary& initialPointsDict,
      const Time& runTime,
      Random& rndGen,
      const conformationSurfaces& geometryToConformTo,
      const cellShapeControl& cellShapeControls,
      const autoPtr<backgroundMeshDecomposition>& decomposition
    );
  //- Destructor
  virtual ~uniformGrid()
  {}
  // Member Functions
    //- Return the initial points for the conformalVoronoiMesh
    virtual List<Vb::Point> initialPoints() const;
};

}  // namespace mousse

#endif

