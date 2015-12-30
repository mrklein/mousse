// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rhoChemistryModel
// Description
//   Chemistry model for density-based thermodynamics
// SourceFiles
//   rho_chemistry_model_i.hpp
//   rho_chemistry_model.cpp
//   new_chemistry_model.cpp
#ifndef rho_chemistry_model_hpp_
#define rho_chemistry_model_hpp_
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
  // Private Member Functions
    //- Construct as copy (not implemented)
    rhoChemistryModel(const rhoChemistryModel&);
    //- Disallow default bitwise assignment
    void operator=(const rhoChemistryModel&);
protected:
  // Protected data
    //- Thermo package
    autoPtr<rhoReactionThermo> thermo_;
public:
  //- Runtime type information
  TypeName("rho");
  //- Declare run-time constructor selection tables
  declareRunTimeSelectionTable
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
#include "rho_chemistry_model_i.hpp"
#endif
