#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TNON_INERTIAL_FRAME_FORCE_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TNON_INERTIAL_FRAME_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NonInertialFrameForce
// Description
//   Calculates particle non-inertial reference frame force.  Variable names as
//   from Landau and Lifshitz, Mechanics, 3rd Ed, p126-129.
// SourceFiles
//   _non_inertial_frame_force.cpp
#include "_particle_force.hpp"
namespace mousse
{
class fvMesh;
template<class CloudType>
class NonInertialFrameForce
:
  public ParticleForce<CloudType>
{
  // Private data
    //- Name of the linear acceleration field
    word WName_;
    //- The linear acceleration of the reference frame
    vector W_;
    //- Name of the angular velocity field
    word omegaName_;
    //- The angular velocity of the reference frame
    vector omega_;
    //- Name of the angular acceleration field
    word omegaDotName_;
    //- The angular acceleration of the reference frame
    vector omegaDot_;
    //- Name of the centre of rotation field
    word centreOfRotationName_;
    //- The centre of rotation of the reference frame
    vector centreOfRotation_;
public:
  //- Runtime type information
  TYPE_NAME("nonInertialFrame");
  // Constructors
    //- Construct from mesh
    NonInertialFrameForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Construct copy
    NonInertialFrameForce(const NonInertialFrameForce& niff);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new ParticleForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NonInertialFrameForce();
  // Member Functions
    // Access
      //- Return the linear acceleration of the reference frame
      inline const vector& W() const;
      //- Return the angular velocity of the reference frame
      inline const vector& omega() const;
      //- Return the angular acceleration of the reference frame
      inline const vector& omegaDot() const;
      //- Return the centre of rotation of the reference frame
      inline const vector& centreOfRotation() const;
    // Evaluation
      //- Cache fields
      virtual void cacheFields(const bool store);
      //- Calculate the non-coupled force
      virtual forceSuSp calcNonCoupled
      (
        const typename CloudType::parcelType& p,
        const scalar dt,
        const scalar mass,
        const scalar Re,
        const scalar muc
      ) const;
};
}  // namespace mousse

template<class CloudType>
inline const mousse::vector& mousse::NonInertialFrameForce<CloudType>::W() const
{
  return W_;
}
template<class CloudType>
inline const mousse::vector& mousse::NonInertialFrameForce<CloudType>::omega() const
{
  return omega_;
}
template<class CloudType>
inline const mousse::vector&
mousse::NonInertialFrameForce<CloudType>::omegaDot() const
{
  return omegaDot_;
}
template<class CloudType>
inline const mousse::vector&
mousse::NonInertialFrameForce<CloudType>::centreOfRotation() const
{
  return centreOfRotation_;
}
#ifdef NoRepository
  #include "_non_inertial_frame_force.cpp"
#endif
#endif
