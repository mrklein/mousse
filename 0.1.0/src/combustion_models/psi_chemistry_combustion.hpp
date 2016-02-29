#ifndef COMBUSTION_MODELS_PSI_CHEMISTRY_COMBUSTION_HPP_
#define COMBUSTION_MODELS_PSI_CHEMISTRY_COMBUSTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::psiChemistryCombustion
// Description
//   Compressibility-based chemistry model wrapper for combustion models
// SourceFiles
//   psi_chemistry_combustion.cpp
#include "auto_ptr.hpp"
#include "psi_combustion_model.hpp"
#include "psi_chemistry_model.hpp"
namespace mousse
{
namespace combustionModels
{
class psiChemistryCombustion
:
  public psiCombustionModel
{
protected:
  // Protected data
    //- Pointer to chemistry model
    autoPtr<psiChemistryModel> chemistryPtr_;
public:
  // Constructors
    //- Construct from components and thermo
    psiChemistryCombustion
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
    //- Construct as copy (not implemented)
    psiChemistryCombustion(const psiChemistryCombustion&) = delete;
    //- Disallow default bitwise assignment
    psiChemistryCombustion& operator=(const psiChemistryCombustion&) = delete;
  //- Destructor
  virtual ~psiChemistryCombustion();
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
