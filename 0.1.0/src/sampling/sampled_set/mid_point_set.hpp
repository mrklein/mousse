// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::midPointSet
// Description
// SourceFiles
//   mid_point_set.cpp
#ifndef mid_point_set_hpp_
#define mid_point_set_hpp_
#include "face_only_set.hpp"
namespace mousse
{
// Forward declaration of classes
class meshSearch;
class midPointSet
:
  public faceOnlySet
{
  // Private Member Functions
    void genSamples();
public:
  //- Runtime type information
  TYPE_NAME("midPoint");
  // Constructors
    //- Construct from components
    midPointSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const word& axis,
      const point& start,
      const point& end
    );
    //- Construct from dictionary
    midPointSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
  //- Destructor
  virtual ~midPointSet();
};
}  // namespace mousse
#endif
