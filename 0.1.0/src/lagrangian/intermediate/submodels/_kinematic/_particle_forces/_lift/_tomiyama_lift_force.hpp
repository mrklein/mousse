#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TLIFT_TTOMIYAMA_LIFT_FORCE_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TLIFT_TTOMIYAMA_LIFT_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TomiyamaLiftForce
// Description
//   Tomiyama particle lift force model applicable to deformable bubbles.
// SourceFiles
//   _tomiyama_lift_force.cpp
#include "_lift_force.hpp"
namespace mousse
{
template<class CloudType>
class TomiyamaLiftForce
:
  public LiftForce<CloudType>
{
protected:
  // Protected data
    //- Surface tension
    scalar sigma_;
  // Protected Member Functions
    //- Calculate the lift coefficient
    virtual scalar Cl
    (
      const typename CloudType::parcelType& p,
      const vector& curlUc,
      const scalar Re,
      const scalar muc
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("TomiyamaLift");
  // Constructors
    //- Construct from mesh
    TomiyamaLiftForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& forceType = typeName
    );
    //- Construct copy
    TomiyamaLiftForce(const TomiyamaLiftForce& lf);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new TomiyamaLiftForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~TomiyamaLiftForce();
};
}  // namespace mousse
#ifdef NoRepository
  #include "_tomiyama_lift_force.cpp"
#endif
#endif
