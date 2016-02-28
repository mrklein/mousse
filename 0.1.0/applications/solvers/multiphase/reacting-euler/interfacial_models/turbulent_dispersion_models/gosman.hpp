// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulentDispersionModels::Gosman
// Description
//   Turbulent dispersion model of Gosman et al.
//   Reference:
//   \verbatim
//     "Multidimensional modeling of turbulent two-phase flows in stirred
//     vessels"
//     Gosman, A.D., Lekakou, C., Politis, S., Issa, R.I., and Looney, M.K.,
//     AIChE Journal
//     Volume 38, Issue 12, 1992, pp. 1946-1956
//   \endverbatim
// SourceFiles
//   gosman.cpp
#ifndef gosman_hpp_
#define gosman_hpp_
#include "turbulent_dispersion_model.hpp"
namespace mousse
{
class phasePair;
namespace turbulentDispersionModels
{
class Gosman
:
  public turbulentDispersionModel
{
  // Private data
    //- Schmidt number
    const dimensionedScalar sigma_;
public:
  //- Runtime type information
  TYPE_NAME("Gosman");
  // Constructors
    //- Construct from a dictionary and a phase pair
    Gosman
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~Gosman();
  // Member Functions
    //- Turbulent diffusivity
    //  multiplying the gradient of the phase-fraction
    virtual tmp<volScalarField> D() const;
};
}  // namespace turbulentDispersionModels
}  // namespace mousse
#endif
