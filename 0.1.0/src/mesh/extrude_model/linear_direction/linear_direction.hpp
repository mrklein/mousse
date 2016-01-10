// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudeModels::linearDirection
// Description
//   Extrudes by transforming points in a specified direction by a given distance
#ifndef linear_direction_hpp_
#define linear_direction_hpp_
#include "point.hpp"
#include "extrude_model.hpp"
namespace mousse
{
namespace extrudeModels
{
class linearDirection
:
  public extrudeModel
{
  // Private data
    //- Extrude direction
    vector direction_;
    //- Layer thickness
    scalar thickness_;
public:
  //- Runtime type information
  TYPE_NAME("linearDirection");
  // Constructors
    //- Construct from dictionary
    linearDirection(const dictionary& dict);
  //- Destructor
  virtual ~linearDirection();
  // Member Operators
    point operator()
    (
      const point& surfacePoint,
      const vector& surfaceNormal,
      const label layer
    ) const;
};
}  // namespace extrudeModels
}  // namespace mousse
#endif
