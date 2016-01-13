// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchInjection
// Description
//   Remove and inject the mass in the film as it passes over the selected
//   patches.
// SourceFiles
//   patch_injection.cpp
#ifndef patch_injection_hpp_
#define patch_injection_hpp_
#include "injection_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class patchInjection
:
  public injectionModel
{
protected:
    //- Stable film thickness - mass only removed if thickness execeeds
    //  this threhold value
    scalar deltaStable_;
    //- List of patch IDs at which the film is removed
    labelList patchIDs_;
    //- Injected mass for each patch at which the film is removed
    scalarField patchInjectedMasses_;
public:
  //- Runtime type information
  TYPE_NAME("patchInjection");
  // Constructors
    //- Construct from surface film model
    patchInjection(surfaceFilmModel& owner, const dictionary& dict);
    //- Disallow default bitwise copy construct
    patchInjection(const patchInjection&) = delete;
    //- Disallow default bitwise assignment
    patchInjection& operator=(const patchInjection&) = delete;
  //- Destructor
  virtual ~patchInjection();
  // Member Functions
    //- Correct
    virtual void correct
    (
      scalarField& availableMass,
      scalarField& massToInject,
      scalarField& diameterToInject
    );
    //- Accumulate the total mass injected for the patches into the
    //  scalarField provided
    virtual void patchInjectedMassTotals
    (
      scalarField& patchMasses
    ) const;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
