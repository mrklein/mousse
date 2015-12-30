// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::noAbsorptionEmission
// Description
//   Dummy absorption-emission model for 'none'
// SourceFiles
//   no_absorption_emission.cpp
#ifndef radiationNoAbsorptionEmission_H
#define radiationNoAbsorptionEmission_H
#include "absorption_emission_model.hpp"
namespace mousse
{
namespace radiation
{
class noAbsorptionEmission
:
  public absorptionEmissionModel
{
public:
  //- Runtime type information
  TypeName("none");
  // Constructors
    //- Construct from components
    noAbsorptionEmission(const dictionary& dict, const fvMesh& mesh);
  //- Destructor
  virtual ~noAbsorptionEmission();
};
}  // namespace radiation
}  // namespace mousse
#endif
