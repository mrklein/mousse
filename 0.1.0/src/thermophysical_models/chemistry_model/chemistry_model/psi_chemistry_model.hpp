// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::psiChemistryModel
// Description
//   Chemistry model for compressibility-based thermodynamics
// SourceFiles
//   psi_chemistry_model_i.hpp
//   psi_chemistry_model.cpp
//   new_chemistry_model.cpp
#ifndef psi_chemistry_model_hpp_
#define psi_chemistry_model_hpp_
#include "basic_chemistry_model.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "psi_reaction_thermo.hpp"
namespace mousse
{
// Forward declaration of classes
class fvMesh;
class psiChemistryModel
:
  public basicChemistryModel
{
  // Private Member Functions
    //- Construct as copy (not implemented)
    psiChemistryModel(const psiChemistryModel&);
    //- Disallow default bitwise assignment
    void operator=(const psiChemistryModel&);
protected:
  // Protected data
    //- Thermo package
    autoPtr<psiReactionThermo> thermo_;
public:
  //- Runtime type information
  TypeName("psi");
  //- Declare run-time constructor selection tables
  declareRunTimeSelectionTable
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
#include "psi_chemistry_model_i.hpp"
#endif
