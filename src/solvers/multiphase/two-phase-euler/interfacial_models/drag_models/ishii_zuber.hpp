// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::IshiiZuber
// Description
//   Ishii and Zuber (1979) drag model for dense dispersed bubbly flows.
//   Reference:
//   \verbatim
//     "Drag Coefficient and relative velocity in bubbly, droplet and
//     particulate flows",
//     Ishii, M., Zuber, N.,
//     AIChE Journal 5, Vol. 25, 1979, pp. 843-855.
//   \endverbatim
// SourceFiles
//   ishii_zuber.cpp
#ifndef ISHII_ZUBER_HPP_
#define ISHII_ZUBER_HPP_
#include "drag_model.hpp"
namespace mousse
{
class phasePair;
namespace dragModels
{
class IshiiZuber
:
  public dragModel
{
public:
  //- Runtime type information
  TYPE_NAME("IshiiZuber");
  // Constructors
    //- Construct from a dictionary and a phase pair
    IshiiZuber
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~IshiiZuber();
  // Member Functions
    //- Drag coefficient
    virtual tmp<volScalarField> CdRe() const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
