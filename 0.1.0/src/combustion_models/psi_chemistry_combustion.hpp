// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::psiChemistryCombustion
// Description
//   Compressibility-based chemistry model wrapper for combustion models
// SourceFiles
//   psi_chemistry_combustion.cpp
#ifndef psi_chemistry_combustion_hpp_
#define psi_chemistry_combustion_hpp_
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
  // Private Member Functions
    //- Construct as copy (not implemented)
    psiChemistryCombustion(const psiChemistryCombustion&);
    //- Disallow default bitwise assignment
    void operator=(const psiChemistryCombustion&);
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
