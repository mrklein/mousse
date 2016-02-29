#ifndef COMBUSTION_MODELS_RHO_CHEMISTRY_COMBUSTION_HPP_
#define COMBUSTION_MODELS_RHO_CHEMISTRY_COMBUSTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rhoChemistryCombustion
// Description
//   Density-based chemistry model wrapper for combustion models
// SourceFiles
//   rho_chemistry_combustion.cpp
#include "auto_ptr.hpp"
#include "rho_combustion_model.hpp"
#include "rho_chemistry_model.hpp"
namespace mousse
{
namespace combustionModels
{
class rhoChemistryCombustion
:
  public rhoCombustionModel
{
protected:
  // Protected data
    //- Pointer to chemistry model
    autoPtr<rhoChemistryModel> chemistryPtr_;
public:
  // Constructors
    //- Construct from components and thermo
    rhoChemistryCombustion
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
    //- Construct as copy (not implemented)
    rhoChemistryCombustion(const rhoChemistryCombustion&) = delete;
    //- Disallow default bitwise assignment
    rhoChemistryCombustion& operator=(const rhoChemistryCombustion&) = delete;
  //- Destructor
  virtual ~rhoChemistryCombustion();
  // Member Functions
    //- Return access to the thermo package
    virtual rhoReactionThermo& thermo();
    //- Return const access to the thermo package
    virtual const rhoReactionThermo& thermo() const;
    //- Return const access to the density field
    virtual tmp<volScalarField> rho() const;
};
}  // namespace combustionModels
}  // namespace mousse
#endif
