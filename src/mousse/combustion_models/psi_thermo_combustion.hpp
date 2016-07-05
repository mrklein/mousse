#ifndef COMBUSTION_MODELS_PSI_THERMO_COMBUSTION_HPP_
#define COMBUSTION_MODELS_PSI_THERMO_COMBUSTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::psiThermoCombustion
// Description
//   Compressibility-based thermo model wrapper for combustion models

#include "auto_ptr.hpp"
#include "psi_combustion_model.hpp"
#include "psi_reaction_thermo.hpp"


namespace mousse {
namespace combustionModels {

class psiThermoCombustion
:
  public psiCombustionModel
{
protected:
  // Protected data
    //- Pointer to chemistry model
    autoPtr<psiReactionThermo> thermoPtr_;
public:
  // Constructors
    //- Construct from components
    psiThermoCombustion
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
    //- Construct as copy (not implemented)
    psiThermoCombustion(const psiThermoCombustion&) = delete;
    //- Disallow default bitwise assignment
    psiThermoCombustion& operator=(const psiThermoCombustion&) = delete;
  //- Destructor
  virtual ~psiThermoCombustion();
  // Member Functions
    //- Return access to the thermo package
    virtual psiReactionThermo& thermo();
    //- Return const access to the thermo package
    virtual const psiReactionThermo& thermo() const;
    //- Return const access to the density field
    virtual tmp<volScalarField> rho() const;
};

}  // namespace combustionModels
}  // namespace mousse

#endif

