// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParticleStressModels::HarrisCrighton
// Description
//   Inter-particle stress model of Harris and Crighton
//   The stress value takes the following form:
//   \f[
//     \frac{P_s \alpha^\beta}{ \mathrm{max} \left( \alpha_{pack} - \alpha ,
//     \epsilon ( 1 - \alpha ) \right) }
//   \f]
//   Here, \f$\alpha\f$ is the volume fraction of the dispersed phase, and the
//   other values are modelling constants. A small value \f$\epsilon\f$ is used
//   to limit the denominator to ensure numerical stability.
//   Reference:
//   \verbatim
//     "Solitons, solitary waves, and voidage disturbances in gas-fluidized
//     beds"
//     S Harris and D Crighton,
//     Journal of Fluid Mechanics
//     Volume 266, Pages 243-276, 1994
//   \endverbatim
// SourceFiles
//   harris_crighton.cpp
#ifndef harris_crighton_hpp_
#define harris_crighton_hpp_
#include "particle_stress_model.hpp"
namespace mousse
{
namespace ParticleStressModels
{
class HarrisCrighton
:
  public ParticleStressModel
{
  // Private data
    //- Solid pressure coefficient
    scalar pSolid_;
    //- Exponent of the volume fraction
    scalar beta_;
    //- Smallest allowable difference from the packed volume fraction
    scalar eps_;
  // Private member functions
    //- Return the limited denominator of the radial distribution function
    tmp<Field<scalar> > denominator(const Field<scalar>& alpha) const;
public:
  //- Runtime type information
  TypeName("HarrisCrighton");
  //- Constructors
    //- Construct from components
    HarrisCrighton(const dictionary& dict);
    //- Construct copy
    HarrisCrighton(const HarrisCrighton& hc);
    //- Clone
    virtual autoPtr<ParticleStressModel> clone() const
    {
      return autoPtr<ParticleStressModel>
      (
        new HarrisCrighton(*this)
      );
    }
  //- Destructor
  virtual ~HarrisCrighton();
  //- Member Functions
    //- Collision stress
    tmp<Field<scalar> > tau
    (
      const Field<scalar>& alpha,
      const Field<scalar>& rho,
      const Field<scalar>& uRms
    ) const;
    //- Collision stress derivaive w.r.t. the volume fraction
    tmp<Field<scalar> > dTaudTheta
    (
      const Field<scalar>& alpha,
      const Field<scalar>& rho,
      const Field<scalar>& uRms
    ) const;
};
}  // namespace ParticleStressModels
}  // namespace mousse
#endif
