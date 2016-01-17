// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformValue
// Description
// SourceFiles
//   uniform_value.cpp
#ifndef uniform_value_hpp_
#define uniform_value_hpp_
#include "surface_cell_size_function.hpp"
namespace mousse
{
class uniformValue
:
  public surfaceCellSizeFunction
{
private:
  // Private data
    scalar surfaceCellSize_;
public:
  //- Runtime type information
  TypeName("uniformValue");
  // Constructors
    //- Construct from components
    uniformValue
    (
      const dictionary& cellSizeFunctionDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize
    );
  //- Destructor
  virtual ~uniformValue()
  {}
  // Member Functions
    virtual scalar interpolate
    (
      const point& pt,
      const label index
    ) const;
};
}  // namespace mousse
#endif
