// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParticleStressModels::Lun
// Description
//   Inter-particle stress model of Lun et al
//   The stress value takes the following form:
//   \f[
//     \left( \alpha \rho + \alpha^2 \rho (1 + e) \frac{3}{5}
//     \left( 1 - \left( \frac{\alpha}{\alpha_{pack}} \right)^\frac{1}{3}
//     \right) \right) \frac{1}{3} \sigma^2
//   \f]
//   Here, \f$\alpha\f$ is the volume fraction of the dispersed phase,
//   \f$\rho\f$ is the density of the dispersed phase, \f$e\f$ is a coefficient
//   of restitution, and \f$\sigma\f$ is the RMS velocity fluctuation.
//   Reference:
//   \verbatim
//     "Kinetic theories for granular flow: inelastic particles in Couette
//     flow and slightly inelastic particles in a general flowfield"
//     C Lun, S Savage, G Jeffrey, N Chepurniy
//     Journal of Fluid Mechanics
//     Volume 140, Pages 223-256, 1984
//   \endverbatim
// SourceFiles
//   lun.cpp
#ifndef lun_hpp_
#define lun_hpp_
#include "particle_stress_model.hpp"
namespace mousse
{
namespace ParticleStressModels
{
class Lun
:
  public ParticleStressModel
{
  // Private data
    //- Coefficient of restitution
    scalar e_;
    //- Smallest allowable difference from the packed volume fraction
    scalar eps_;
public:
  //- Runtime type information
  TYPE_NAME("Lun");
  //- Constructors
    //- Construct from components
    Lun(const dictionary& dict);
    //- Construct copy
    Lun(const Lun& hc);
    //- Clone
    virtual autoPtr<ParticleStressModel> clone() const
    {
      return autoPtr<ParticleStressModel>
      (
        new Lun(*this)
      );
    }
  //- Destructor
  virtual ~Lun();
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
