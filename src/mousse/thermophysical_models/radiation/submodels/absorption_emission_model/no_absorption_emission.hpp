#ifndef THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_ABSORPTION_EMISSION_MODEL_NO_ABSORPTION_EMISSION_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_ABSORPTION_EMISSION_MODEL_NO_ABSORPTION_EMISSION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::noAbsorptionEmission
// Description
//   Dummy absorption-emission model for 'none'

#include "absorption_emission_model.hpp"


namespace mousse {
namespace radiation {

class noAbsorptionEmission
:
  public absorptionEmissionModel
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    noAbsorptionEmission(const dictionary& dict, const fvMesh& mesh);
  //- Destructor
  virtual ~noAbsorptionEmission();
};

}  // namespace radiation
}  // namespace mousse

#endif

