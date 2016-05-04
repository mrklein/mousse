#ifndef COMBUSTION_MODELS_NO_COMBUSTION_HPP_
#define COMBUSTION_MODELS_NO_COMBUSTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::combustionModels::noCombustion
// Description
//   Dummy combustion model for 'no combustion'

namespace mousse {
namespace combustionModels {

template<class CombThermoType>
class noCombustion
:
  public CombThermoType
{
public:
  //- Runtime type information
  TYPE_NAME("noCombustion");
  // Constructors
    //- Construct from components
    noCombustion
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
    //- Disallow copy construct
    noCombustion(const noCombustion&) = delete;
    //- Disallow default bitwise assignment
    noCombustion& operator=(const noCombustion&) = delete;
  //- Destructor
  virtual ~noCombustion();
  // Member Functions
    // Evolution
      //- Correct combustion rate
      virtual void correct();
      //- Fuel consumption rate matrix
      virtual tmp<fvScalarMatrix> R(volScalarField& Y) const;
      //- Heat release rate calculated from fuel consumption rate matrix
      virtual tmp<volScalarField> dQ() const;
      //-  Return source for enthalpy equation [kg/m/s3]
      virtual tmp<volScalarField> Sh() const;
  // IO
      //- Update properties from given dictionary
      virtual bool read();
};

}  // namespace combustionModels
}  // namespace mousse

#include "no_combustion.ipp"

#endif
