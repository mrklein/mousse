// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rhoReactionThermo
// Description
//   mousse::rhoReactionThermo
// SourceFiles
//   rho_reaction_thermo.cpp
#ifndef rho_reaction_thermo_hpp_
#define rho_reaction_thermo_hpp_
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
  TypeName("rhoReactionThermo");
  //- Declare run-time constructor selection tables
  declareRunTimeSelectionTable
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
