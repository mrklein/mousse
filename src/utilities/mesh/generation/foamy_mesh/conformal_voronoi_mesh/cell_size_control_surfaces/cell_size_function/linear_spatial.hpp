#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_CELL_SIZE_FUNCTION_LINEAR_SPATIAL_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_CELL_SIZE_FUNCTION_LINEAR_SPATIAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linearSpatial

#include "cell_size_function.hpp"


namespace mousse {

class linearSpatial
:
  public cellSizeFunction
{
private:
  // Private data
    //- Reference point for spatial size grading
    point referencePoint_;
    //- Cell size at reference point
    scalar referenceCellSize_;
    //- Direction of cell size grading, stored as unit vector, may be
    //  supplied with any magnitude
    vector direction_;
    //- Gradient of cell size change in direction of direction_
    scalar cellSizeGradient_;
  // Private Member Functions
    //- Calculate the cell size as a function of the given position
    scalar sizeFunction(const point& pt) const;
public:
  //- Runtime type information
  TYPE_NAME("linearSpatial");
  // Constructors
    //- Construct from components
    linearSpatial
    (
      const dictionary& initialPointsDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize,
      const labelList regionIndices
    );
  //- Destructor
  virtual ~linearSpatial()
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
};

}  // namespace mousse

#endif

