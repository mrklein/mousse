#ifndef THERMOPHYSICAL_MODELS_LAMINAR_FLAME_SPEED_LAMINAR_FLAME_SPEED_HPP_
#define THERMOPHYSICAL_MODELS_LAMINAR_FLAME_SPEED_LAMINAR_FLAME_SPEED_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::laminarFlameSpeed
// Description
//   Abstract class for laminar flame speed

#include "psiu_reaction_thermo.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

class laminarFlameSpeed
{
protected:
  // Protected data
    const psiuReactionThermo& psiuReactionThermo_;
    //- Fuel
    word fuel_;
    //- Equivalence ratio of a homogeneous mixture
    scalar equivalenceRatio_;
public:
  //- Runtime type information
  TYPE_NAME("laminarFlameSpeed");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      laminarFlameSpeed,
      dictionary,
      (
        const dictionary& dict,
        const psiuReactionThermo& ct
      ),
      (dict, ct)
    );
  // Constructors
    //- Construct from dictionary and psiuReactionThermo
    laminarFlameSpeed
    (
      const dictionary&,
      const psiuReactionThermo&
    );

    laminarFlameSpeed(const laminarFlameSpeed&) = delete;
    void operator=(const laminarFlameSpeed&) = delete;

  // Selector
    static autoPtr<laminarFlameSpeed> New
    (
      const psiuReactionThermo&
    );
  //- Destructor
  virtual ~laminarFlameSpeed();
  // Member functions
    //- Return the laminar flame speed [m/s]
    virtual tmp<volScalarField> operator()() const = 0;
};

}  // namespace mousse

#endif

