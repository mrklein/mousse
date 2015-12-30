// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noPhaseChange
// Description
//   Dummy phase change model for 'none'
// SourceFiles
//   no_phase_change.cpp
#ifndef no_phase_change_hpp_
#define no_phase_change_hpp_
#include "phase_change_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class noPhaseChange
:
  public phaseChangeModel
{
private:
  // Private member functions
    //- Disallow default bitwise copy construct
    noPhaseChange(const noPhaseChange&);
    //- Disallow default bitwise assignment
    void operator=(const noPhaseChange&);
public:
  //- Runtime type information
  TypeName("none");
  // Constructors
    //- Construct from surface film model
    noPhaseChange(surfaceFilmModel& owner, const dictionary& dict);
  //- Destructor
  virtual ~noPhaseChange();
  // Member Functions
    // Evolution
      //- Return the model 'active' status - default active = true
      virtual bool active() const;
      //- Correct
      virtual void correctModel
      (
        const scalar dt,
        scalarField& availableMass,
        scalarField& dMass,
        scalarField& dEnergy
      );
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
