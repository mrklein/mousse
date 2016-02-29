#ifndef COMBUSTION_MODELS_PSI_COMBUSTION_MODEL_HPP_
#define COMBUSTION_MODELS_PSI_COMBUSTION_MODEL_HPP_

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
public:
  //- Runtime type information
  TYPE_NAME("psiCombustionModel");
  //- Declare run-time constructor selection tables
  DECLARE_RUN_TIME_SELECTION_TABLE
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
    //- Construct as copy (not implemented)
    psiCombustionModel(const psiCombustionModel&) = delete;
    //- Disallow default bitwise assignment
    psiCombustionModel& operator=(const psiCombustionModel&) = delete;
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
