#ifndef THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_MODEL_RHO_CHEMISTRY_MODEL_HPP_
#define THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_MODEL_RHO_CHEMISTRY_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rhoChemistryModel
// Description
//   Chemistry model for density-based thermodynamics
// SourceFiles
//   rho_chemistry_model.cpp
//   new_chemistry_model.cpp
#include "basic_chemistry_model.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "rho_reaction_thermo.hpp"
namespace mousse
{
// Forward declaration of classes
class fvMesh;
class rhoChemistryModel
:
  public basicChemistryModel
{
protected:
  // Protected data
    //- Thermo package
    autoPtr<rhoReactionThermo> thermo_;
public:
  //- Runtime type information
  TYPE_NAME("rho");
  //- Declare run-time constructor selection tables
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    rhoChemistryModel,
    fvMesh,
    (const fvMesh& mesh, const word& phaseName),
    (mesh, phaseName)
  );
  // Constructors
    //- Construct from mesh and phase name
    rhoChemistryModel(const fvMesh& mesh, const word& phaseName);
    //- Construct as copy (not implemented)
    rhoChemistryModel(const rhoChemistryModel&) = delete;
    //- Disallow default bitwise assignment
    rhoChemistryModel& operator=(const rhoChemistryModel&) = delete;
  //- Selector
  static autoPtr<rhoChemistryModel> New
  (
    const fvMesh& mesh,
    const word& phaseName=word::null
  );
  //- Destructor
  virtual ~rhoChemistryModel();
  // Member Functions
    //- Return access to the thermo package
    inline rhoReactionThermo& thermo();
    //- Return const access to the thermo package
    inline const rhoReactionThermo& thermo() const;
};
}  // namespace mousse

// Member Functions 
inline mousse::rhoReactionThermo& mousse::rhoChemistryModel::thermo()
{
  return thermo_();
}
inline const mousse::rhoReactionThermo& mousse::rhoChemistryModel::thermo() const
{
  return thermo_();
}
#endif
