// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::viscosityModels::HrenyaSinclair
// Description
// SourceFiles
//   hrenya_sinclair_viscosity.cpp
#ifndef HrenyaSinclair_H
#define HrenyaSinclair_H
#include "viscosity_model.hpp"
namespace mousse
{
namespace kineticTheoryModels
{
namespace viscosityModels
{
class HrenyaSinclair
:
  public viscosityModel
{
  // Private data
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
  // Member functions
    tmp<volScalarField> nu
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
}  // namespace viscosityModels
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
