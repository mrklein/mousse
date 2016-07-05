#ifndef THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_MODEL_PSI_CHEMISTRY_MODEL_HPP_
#define THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_MODEL_PSI_CHEMISTRY_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::psiChemistryModel
// Description
//   Chemistry model for compressibility-based thermodynamics

#include "basic_chemistry_model.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "psi_reaction_thermo.hpp"


namespace mousse {

// Forward declaration of classes
class fvMesh;


class psiChemistryModel
:
  public basicChemistryModel
{
protected:
  // Protected data
    //- Thermo package
    autoPtr<psiReactionThermo> thermo_;
public:
  //- Runtime type information
  TYPE_NAME("psi");
  //- Declare run-time constructor selection tables
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    psiChemistryModel,
    fvMesh,
    (const fvMesh& mesh, const word& phaseName),
    (mesh, phaseName)
  );
  // Constructors
    //- Construct from mesh and phase name
    psiChemistryModel(const fvMesh& mesh, const word& phaseName);
    //- Construct as copy (not implemented)
    psiChemistryModel(const psiChemistryModel&) = delete;
    //- Disallow default bitwise assignment
    psiChemistryModel& operator=(const psiChemistryModel&) = delete;
  //- Selector
  static autoPtr<psiChemistryModel> New
  (
    const fvMesh& mesh,
    const word& phaseName=word::null
  );
  //- Destructor
  virtual ~psiChemistryModel();
  // Member Functions
    //- Return access to the thermo package
    inline psiReactionThermo& thermo();
    //- Return const access to the thermo package
    inline const psiReactionThermo& thermo() const;
};

}  // namespace mousse


// Member Functions 
inline mousse::psiReactionThermo& mousse::psiChemistryModel::thermo()
{
  return thermo_();
}


inline const mousse::psiReactionThermo& mousse::psiChemistryModel::thermo() const
{
  return thermo_();
}

#endif
