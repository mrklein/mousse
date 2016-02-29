#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_CELL_SIZE_FUNCTION_UNIFORM_DISTANCE_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_CELL_SIZE_FUNCTION_UNIFORM_DISTANCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformDistance
// Description
// SourceFiles
//   uniform_distance.cpp
#include "cell_size_function.hpp"
namespace mousse
{
class uniformDistance
:
  public cellSizeFunction
{
private:
  // Private data
    //- Distance
    scalar distance_;
    //- Distance squared
    scalar distanceSqr_;
public:
  //- Runtime type information
  TYPE_NAME("uniformDistance");
  // Constructors
    //- Construct from components
    uniformDistance
    (
      const dictionary& initialPointsDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize,
      const labelList regionIndices
    );
  //- Destructor
  virtual ~uniformDistance()
  {}
  // Member Functions
    virtual bool sizeLocations
    (
      const pointIndexHit& hitPt,
      const vector& n,
      pointField& shapePts,
      scalarField& shapeSizes
    ) const;
    //- Modify scalar argument to the cell size specified by function.
    //  Return a boolean specifying if the function was used, i.e. false if
    //  the point was not in range of the surface for a spatially varying
    //  size.
    virtual bool cellSize
    (
      const point& pt,
      scalar& size
    ) const;
    //- Adapt local cell size. Return true if anything changed.
    virtual bool setCellSize
    (
      const pointField& pts
    );
};
}  // namespace mousse
#endif
