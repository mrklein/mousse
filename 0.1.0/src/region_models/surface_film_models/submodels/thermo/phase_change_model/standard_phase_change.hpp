// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::standardPhaseChange
// Description
//   Standard phase change model with modification for boiling
// SourceFiles
//   standard_phase_change.cpp
#ifndef standard_phase_change_hpp_
#define standard_phase_change_hpp_
#include "phase_change_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class standardPhaseChange
:
  public phaseChangeModel
{
private:
  // Private member functions
    //- Disallow default bitwise copy construct
    standardPhaseChange(const standardPhaseChange&);
    //- Disallow default bitwise assignment
    void operator=(const standardPhaseChange&);
protected:
  // Protected data
    //- Minimum film height for model to be active
    const scalar deltaMin_;
    //- Length scale / [m]
    const scalar L_;
    //- Boiling temperature factor / []
    //  Used to set max limit on temperature to Tb*TbFactor
    const scalar TbFactor_;
  // Protected member functions
    //- Return Sherwood number as a function of Reynolds and Schmidt numbers
    scalar Sh(const scalar Re, const scalar Sc) const;
public:
  //- Runtime type information
  TypeName("standardPhaseChange");
  // Constructors
    //- Construct from surface film model
    standardPhaseChange
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  //- Destructor
  virtual ~standardPhaseChange();
  // Member Functions
    // Evolution
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
