#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TDRAG_TDISTORTED_SPHERE_DRAG_FORCE_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TDRAG_TDISTORTED_SPHERE_DRAG_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DistortedSphereDragForce
// Description
//   Drag model based on assumption of distorted spheres according to:
//   \verbatim
//     "Effects of Drop Drag and Breakup on Fuel Sprays"
//     Liu, A.B., Mather, D., Reitz, R.D.,
//     SAE Paper 930072,
//     SAE Transactions, Vol. 102, Section 3, Journal of Engines, 1993,
//     pp. 63-95
//   \endverbatim

#include "_particle_force.hpp"


namespace mousse {

template<class CloudType>
class DistortedSphereDragForce
:
  public ParticleForce<CloudType>
{
  // Private Member Functions
    //- Drag coefficient multiplied by Reynolds number
    scalar CdRe(const scalar Re) const;
public:
  //- Runtime type information
  TYPE_NAME("distortedSphereDrag");
  // Constructors
    //- Construct from mesh
    DistortedSphereDragForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Construct copy
    DistortedSphereDragForce(const DistortedSphereDragForce<CloudType>& df);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType>> clone() const
    {
      return
        autoPtr<ParticleForce<CloudType>>
        {
          new DistortedSphereDragForce<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~DistortedSphereDragForce();
  // Member Functions
    // Evaluation
      //- Calculate the coupled force
      virtual forceSuSp calcCoupled
      (
        const typename CloudType::parcelType& p,
        const scalar dt,
        const scalar mass,
        const scalar Re,
        const scalar muc
      ) const;
};

}  // namespace mousse

#include "_distorted_sphere_drag_force.ipp"

#endif
