// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudeModels::plane
// Description
//   Extrudes by transforming points normal to the surface by 1 layer over
//   a given distance.
// SeeAlso
//   mousse::extrudeModels::linearNormal
#ifndef plane_H
#define plane_H
#include "linear_normal.hpp"
namespace mousse
{
namespace extrudeModels
{
class plane
:
  public linearNormal
{
public:
  //- Runtime type information
  TypeName("plane");
  // Constructors
    //- Construct from dictionary
    plane(const dictionary& dict);
  //- Destructor
  virtual ~plane();
};
}  // namespace extrudeModels
}  // namespace mousse
#endif
