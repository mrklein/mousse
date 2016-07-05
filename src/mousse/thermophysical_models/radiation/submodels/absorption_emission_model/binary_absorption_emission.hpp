#ifndef THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_ABSORPTION_EMISSION_MODEL_BINARY_ABSORPTION_EMISSION_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_ABSORPTION_EMISSION_MODEL_BINARY_ABSORPTION_EMISSION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::binaryAbsorptionEmission
// Description
//   Radiation coefficient based on two absorption models

#include "absorption_emission_model.hpp"


namespace mousse {
namespace radiation {

class binaryAbsorptionEmission
:
  public absorptionEmissionModel
{
  // Private data
    //- Coefficients dictionary
    dictionary coeffsDict_;
    //- First absorption model
    autoPtr<absorptionEmissionModel> model1_;
    //- Second absorption model
    autoPtr<absorptionEmissionModel> model2_;
public:
  //- Runtime type information
  TYPE_NAME("binaryAbsorptionEmission");
  // Constructors
    //- Construct from components
    binaryAbsorptionEmission(const dictionary& dict, const fvMesh& mesh);
  //- Destructor
  virtual ~binaryAbsorptionEmission();
  // Member Functions
    // Access
      // Absorption coefficient
        //- Absorption coefficient for continuous phase
        virtual tmp<volScalarField> aCont(const label bandI = 0) const;
        //- Absorption coefficient for dispersed phase
        virtual tmp<volScalarField> aDisp(const label bandI = 0) const;
      // Emission coefficient
        //- Emission coefficient for continuous phase
        virtual tmp<volScalarField> eCont(const label bandI = 0) const;
        //- Emission coefficient for dispersed phase
        virtual tmp<volScalarField> eDisp(const label bandI = 0) const;
      // Emission contribution
        //- Emission contribution for continuous phase
        virtual tmp<volScalarField> ECont(const label bandI = 0) const;
        //- Emission contribution for continuous phase
        virtual tmp<volScalarField> EDisp(const label bandI = 0) const;
};

}  // namespace radiation
}  // namespace mousse

#endif

