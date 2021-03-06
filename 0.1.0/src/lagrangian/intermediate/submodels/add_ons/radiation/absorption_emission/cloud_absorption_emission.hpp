#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_ADD_ONS_RADIATION_ABSORPTION_EMISSION_CLOUD_ABSORPTION_EMISSION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_ADD_ONS_RADIATION_ABSORPTION_EMISSION_CLOUD_ABSORPTION_EMISSION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::cloudAbsorptionEmission
// Description
//   Retrieves absorption/emission data from a cloud object

#include "absorption_emission_model.hpp"


namespace mousse {
namespace radiation {

class cloudAbsorptionEmission
:
  public absorptionEmissionModel
{
  // Private data
    //- Coefficients dictionary
    dictionary coeffsDict_;
    //- Cloud name(s)
    const wordList cloudNames_;
public:
  //- Runtime type information
  TYPE_NAME("cloudAbsorptionEmission");
  // Constructors
    //- Construct from components
    cloudAbsorptionEmission(const dictionary& dict, const fvMesh& mesh);
  //- Destructor
  virtual ~cloudAbsorptionEmission();
  // Member Operators
    // Access
      // Absorption coefficient
        //- Absorption coefficient for dispersed phase
        virtual tmp<volScalarField> aDisp(const label bandI = 0) const;
      // Emission coefficient
        //- Emission coefficient for dispersed phase
        virtual tmp<volScalarField> eDisp(const label bandI = 0) const;
      // Emission contribution
        //- Return emission contribution for dispersed phase
        virtual tmp<volScalarField> EDisp(const label bandI = 0) const;
};

}  // namespace radiation
}  // namespace mousse

#endif

