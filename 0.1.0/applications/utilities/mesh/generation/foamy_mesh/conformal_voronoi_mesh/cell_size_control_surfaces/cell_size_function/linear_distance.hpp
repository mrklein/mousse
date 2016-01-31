// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linearDistance
// Description
// SourceFiles
//   linear_distance.cpp
#ifndef linear_distance_hpp_
#define linear_distance_hpp_
#include "cell_size_function.hpp"
namespace mousse
{
class linearDistance
:
  public cellSizeFunction
{
private:
  // Private data
    //- Cell size at distance_ from the surface
    scalar distanceCellSize_;
    //- Distance from the surface to control over
    scalar distance_;
    //- Distance squared
    scalar distanceSqr_;
  // Private Member Functions
    //- Calculate the cell size as a function of the given distance
    scalar sizeFunction(const point& pt, scalar d, label index) const;
public:
  //- Runtime type information
  TYPE_NAME("linearDistance");
  // Constructors
    //- Construct from components
    linearDistance
    (
      const dictionary& initialPointsDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize,
      const labelList regionIndices
    );
  //- Destructor
  virtual ~linearDistance()
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
