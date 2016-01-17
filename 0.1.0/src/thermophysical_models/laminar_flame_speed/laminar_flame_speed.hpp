// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::laminarFlameSpeed
// Description
//   Abstract class for laminar flame speed
// SourceFiles
//   laminar_flame_speed.cpp
//   laminar_flame_speed_new.cpp
#ifndef laminar_flame_speed_hpp_
#define laminar_flame_speed_hpp_
#include "psiu_reaction_thermo.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class laminarFlameSpeed
{
protected:
  // Protected data
    const psiuReactionThermo& psiuReactionThermo_;
    //- Fuel
    word fuel_;
    //- Equivalence ratio of a homogeneous mixture
    scalar equivalenceRatio_;
private:
  // Private Member Functions
    //- Construct as copy (not implemented)
    laminarFlameSpeed(const laminarFlameSpeed&);
    void operator=(const laminarFlameSpeed&);
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
