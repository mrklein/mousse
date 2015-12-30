// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParticleStressModels::exponential
// Description
//   Exponential inter-particle stress model of the same form as used in
//   mousse-two-phase-euler
// SourceFiles
//   exponential.cpp
#ifndef exponential_hpp_
#define exponential_hpp_
#include "particle_stress_model.hpp"
namespace mousse
{
namespace ParticleStressModels
{
class exponential
:
  public ParticleStressModel
{
  // Private data
    //- Pre-exponential factor
    scalar preExp_;
    //- Maximum limit of the exponential
    scalar expMax_;
    //- Front coefficient
    scalar g0_;
public:
  //- Runtime type information
  TypeName("exponential");
  //- Constructors
    //- Construct from components
    exponential(const dictionary& dict);
    //- Construct copy
    exponential(const exponential& hc);
    //- Clone
    virtual autoPtr<ParticleStressModel> clone() const
    {
      return autoPtr<ParticleStressModel>
      (
        new exponential(*this)
      );
    }
  //- Destructor
  virtual ~exponential();
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
