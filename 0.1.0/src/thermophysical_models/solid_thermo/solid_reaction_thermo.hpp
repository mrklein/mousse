#ifndef THERMOPHYSICAL_MODELS_SOLID_THERMO_SOLID_REACTION_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_THERMO_SOLID_REACTION_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidReactionThermo
// Description
//   mousse::solidReactionThermo
// SourceFiles
//   solid_reaction_thermo.cpp
#include "basic_specie_mixture.hpp"
#include "solid_thermo.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class solidReactionThermo
:
  public solidThermo
{
public:
  //- Runtime type information
  TYPE_NAME("solidReactionThermo");
  //- Declare run-time constructor selection tables
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    solidReactionThermo,
    fvMesh,
    (const fvMesh& mesh, const word& phaseName),
    (mesh, phaseName)
  );
  // Declare run-time constructor selection tables
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    solidReactionThermo,
    dictionary,
    (const fvMesh& mesh, const dictionary& dict, const word& phaseName),
    (mesh, dict, phaseName)
  );
  // Constructors
    //- Construct from mesh and phase name
    solidReactionThermo
    (
      const fvMesh&,
      const word& phaseName
    );
    //- Construct from mesh, dictionary and phase name
    solidReactionThermo
    (
      const fvMesh&,
      const dictionary&,
      const word& phaseName
    );
  // Selectors
    //- Standard selection based on fvMesh
    static autoPtr<solidReactionThermo> New
    (
      const fvMesh&,
      const word& phaseName=word::null
    );
    //- Standard selection based on fvMesh amd dictionary
    static autoPtr<solidReactionThermo> New
    (
      const fvMesh&,
      const dictionary&,
      const word& phaseName=word::null
    );
  //- Destructor
  virtual ~solidReactionThermo();
  // Member functions
    //- Return the composition of the multi-component mixture
    virtual basicSpecieMixture& composition() = 0;
    //- Return the composition of the multi-component mixture
    virtual const basicSpecieMixture& composition() const = 0;
};
}  // namespace mousse
#endif
