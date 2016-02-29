#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_RHO_REACTION_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_RHO_REACTION_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rhoReactionThermo
// Description
//   mousse::rhoReactionThermo
// SourceFiles
//   rho_reaction_thermo.cpp
#include "rho_thermo.hpp"
#include "basic_specie_mixture.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class rhoReactionThermo
:
  public rhoThermo
{
public:
  //- Runtime type information
  TYPE_NAME("rhoReactionThermo");
  //- Declare run-time constructor selection tables
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    rhoReactionThermo,
    fvMesh,
    (const fvMesh& mesh, const word& phaseName),
    (mesh, phaseName)
  );
  // Constructors
    //- Construct from mesh and phase name
    rhoReactionThermo
    (
      const fvMesh&,
      const word& phaseName
    );
  // Selectors
    //- Standard selection based on fvMesh
    static autoPtr<rhoReactionThermo> New
    (
      const fvMesh&,
      const word& phaseName=word::null
    );
  //- Destructor
  virtual ~rhoReactionThermo();
  // Member functions
    //- Return the composition of the multi-component mixture
    virtual basicSpecieMixture& composition() = 0;
    //- Return the composition of the multi-component mixture
    virtual const basicSpecieMixture& composition() const = 0;
};
}  // namespace mousse
#endif
