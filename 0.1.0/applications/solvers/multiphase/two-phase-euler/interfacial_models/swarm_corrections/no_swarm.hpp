// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::swarmCorrections::noSwarm
// Description
// SourceFiles
//   no_swarm.cpp
#ifndef NO_SWARM_HPP_
#define NO_SWARM_HPP_
#include "swarm_correction.hpp"
namespace mousse
{
class phasePair;
namespace swarmCorrections
{
class noSwarm
:
  public swarmCorrection
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from a dictionary and a phase pair
    noSwarm
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~noSwarm();
  // Member Functions
    //- Swarm correction coefficient
    tmp<volScalarField> Cs() const;
};
}  // namespace swarmCorrections
}  // namespace mousse
#endif
