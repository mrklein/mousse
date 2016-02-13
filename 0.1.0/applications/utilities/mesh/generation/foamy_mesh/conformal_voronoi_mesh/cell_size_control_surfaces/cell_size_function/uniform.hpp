// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniform
// Description
// SourceFiles
//   uniform.cpp
#ifndef uniform_hpp_
#define uniform_hpp_
#include "cell_size_function.hpp"
namespace mousse
{
class uniform
:
  public cellSizeFunction
{
private:
  // Private data
public:
  //- Runtime type information
  TYPE_NAME("uniform");
  // Constructors
    //- Construct from components
    uniform
    (
      const dictionary& initialPointsDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize,
      const labelList regionIndices
    );
  //- Destructor
  virtual ~uniform()
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
