// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudeModels::wedge
// Description
//   Extrudes by rotating a surface symmetrically around axis by 1 layer.
// SeeAlso
//   mousse::extrudeModels::sector
#ifndef wedge_hpp_
#define wedge_hpp_
#include "sector.hpp"
namespace mousse
{
namespace extrudeModels
{
class wedge
:
  public sector
{
public:
  //- Runtime type information
  TypeName("wedge");
  // Constructors
    //- Construct from dictionary
    wedge(const dictionary& dict);
  //- Destructor
  virtual ~wedge();
};
}  // namespace extrudeModels
}  // namespace mousse
#endif
