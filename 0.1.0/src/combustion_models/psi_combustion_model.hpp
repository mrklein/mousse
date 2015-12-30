// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::psiCombustionModel
// Description
//   Combustion models for compressibility-based thermodynamics
// SourceFiles
//   psi_combustion_model_i.hpp
//   psi_combustion_model.cpp
//   psi_combustion_model_new.cpp
#ifndef psi_combustion_model_hpp_
#define psi_combustion_model_hpp_
#include "combustion_model.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "psi_reaction_thermo.hpp"
namespace mousse
{
namespace combustionModels
{
class psiCombustionModel
:
  public combustionModel
{
  // Private Member Functions
    //- Construct as copy (not implemented)
    psiCombustionModel(const psiCombustionModel&);
    //- Disallow default bitwise assignment
    void operator=(const psiCombustionModel&);
public:
  //- Runtime type information
  TypeName("psiCombustionModel");
  //- Declare run-time constructor selection tables
  declareRunTimeSelectionTable
  (
    autoPtr,
    psiCombustionModel,
    dictionary,
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    ),
    (modelType, mesh, phaseName)
  );
  // Constructors
    //- Construct from components
    psiCombustionModel
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
  //- Selector
  static autoPtr<psiCombustionModel> New
  (
    const fvMesh& mesh,
    const word& phaseName=word::null
  );
  //- Destructor
  virtual ~psiCombustionModel();
  // Member Functions
    //- Return access to the thermo package
    virtual psiReactionThermo& thermo() = 0;
    //- Return const access to the thermo package
    virtual const psiReactionThermo& thermo() const = 0;
    //- Return tmp of rho
    virtual tmp<volScalarField> rho() const = 0;
  // IO
    //- Update properties from given dictionary
    virtual bool read();
};
}  // namespace combustionModels
}  // namespace mousse
#endif
