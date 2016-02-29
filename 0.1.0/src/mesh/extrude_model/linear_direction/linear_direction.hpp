#ifndef MESH_EXTRUDE_MODEL_LINEAR_DIRECTION_LINEAR_DIRECTION_HPP_
#define MESH_EXTRUDE_MODEL_LINEAR_DIRECTION_LINEAR_DIRECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudeModels::linearDirection
// Description
//   Extrudes by transforming points in a specified direction by a given distance
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
