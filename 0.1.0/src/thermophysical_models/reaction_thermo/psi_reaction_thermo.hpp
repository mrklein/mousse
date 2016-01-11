// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::psiReactionThermo
// Description
//   mousse::psiReactionThermo
// SourceFiles
//   psi_reaction_thermo.cpp
#ifndef psi_reaction_thermo_hpp_
#define psi_reaction_thermo_hpp_
#include "psi_thermo.hpp"
#include "basic_specie_mixture.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class psiReactionThermo
:
  public psiThermo
{
public:
  //- Runtime type information
  TYPE_NAME("psiReactionThermo");
  //- Declare run-time constructor selection tables
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    psiReactionThermo,
    fvMesh,
    (const fvMesh& mesh, const word& phaseName),
    (mesh, phaseName)
  );
  // Constructors
    //- Construct from mesh and phase name
    psiReactionThermo
    (
      const fvMesh&,
      const word& phaseName
    );
  // Selectors
    //- Standard selection based on fvMesh
    static autoPtr<psiReactionThermo> New
    (
      const fvMesh&,
      const word& phaseName=word::null
    );
  //- Destructor
  virtual ~psiReactionThermo();
  // Member functions
    //- Return the composition of the multi-component mixture
    virtual basicSpecieMixture& composition() = 0;
    //- Return the composition of the multi-component mixture
    virtual const basicSpecieMixture& composition() const = 0;
};
}  // namespace mousse
#endif
