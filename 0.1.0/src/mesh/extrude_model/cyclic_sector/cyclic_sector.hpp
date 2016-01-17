// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudeModels::cyclicSector
// Description
//   Extrudes a sector.
// SeeAlso
//   mousse::extrudeModels::sector
#ifndef cyclic_sector_hpp_
#define cyclic_sector_hpp_
#include "sector.hpp"
namespace mousse
{
namespace extrudeModels
{
class cyclicSector
:
  public sector
{
public:
  //- Runtime type information
  TYPE_NAME("cyclicSector");
  // Constructors
    //- Construct from dictionary
    cyclicSector(const dictionary& dict);
  //- Destructor
  virtual ~cyclicSector();
};
}  // namespace extrudeModels
}  // namespace mousse
#endif
