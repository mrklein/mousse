// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::conductivityModels::HrenyaSinclair
// Description
// SourceFiles
//   hrenya_sinclair.cpp
#ifndef HrenyaSinclair_H
#define HrenyaSinclair_H
#include "conductivity_model.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
namespace conductivityModels
{
class HrenyaSinclair
:
  public conductivityModel
{
    dictionary coeffDict_;
    //- Characteristic length of geometry
    dimensionedScalar L_;
public:
    //- Runtime type information
    TYPE_NAME("HrenyaSinclair");
  // Constructors
    //- Construct from components
    HrenyaSinclair(const dictionary& dict);
  //- Destructor
  virtual ~HrenyaSinclair();
  // Member Functions
    tmp<volScalarField> kappa
    (
      const volScalarField& alpha1,
      const volScalarField& Theta,
      const volScalarField& g0,
      const volScalarField& rho1,
      const volScalarField& da,
      const dimensionedScalar& e
    ) const;
    virtual bool read();
};
}  // namespace conductivityModels
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
