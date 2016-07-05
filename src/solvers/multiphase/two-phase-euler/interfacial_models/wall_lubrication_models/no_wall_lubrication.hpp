// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallLubricationModels::noWallLubrication
// Description
// SourceFiles
//   no_wall_lubrication.cpp
#ifndef NO_WALL_LUBRICATION_HPP_
#define NO_WALL_LUBRICATION_HPP_
#include "wall_lubrication_model.hpp"
namespace mousse
{
class phasePair;
namespace wallLubricationModels
{
class noWallLubrication
:
  public wallLubricationModel
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    noWallLubrication
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~noWallLubrication();
  // Member Functions
    //- Return phase-intensive wall lubrication force
    tmp<volVectorField> Fi() const;
    //- Wall lubrication force
    tmp<volVectorField> F() const;
};
}  // namespace wallLubricationModels
}  // namespace mousse
#endif
