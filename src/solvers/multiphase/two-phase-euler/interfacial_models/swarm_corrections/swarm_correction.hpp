// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::swarmCorrection
// Description
// SourceFiles
//   swarm_correction.cpp
//   new_swarm_correction.cpp
#ifndef SWARM_CORRECTION_HPP_
#define SWARM_CORRECTION_HPP_
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class phasePair;
class swarmCorrection
{
protected:
  // Protected data
    //- Phase pair
    const phasePair& pair_;
public:
  //- Runtime type information
  TYPE_NAME("swarmCorrection");
  // Declare runtime construction
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    swarmCorrection,
    dictionary,
    (
      const dictionary& dict,
      const phasePair& pair
    ),
    (dict, pair)
  );
  // Constructors
    //- Construct from a dictionary and a phase pair
    swarmCorrection
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~swarmCorrection();
  // Selectors
    static autoPtr<swarmCorrection> New
    (
      const dictionary& dict,
      const phasePair& pair
    );
  // Member Functions
    //- Swarm correction coefficient
    virtual tmp<volScalarField> Cs() const = 0;
};
}  // namespace mousse
#endif
