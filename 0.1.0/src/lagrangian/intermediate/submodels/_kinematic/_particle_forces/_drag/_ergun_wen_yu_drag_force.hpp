#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TDRAG_TERGUN_WEN_YU_DRAG_FORCE_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TDRAG_TERGUN_WEN_YU_DRAG_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ErgunWenYuDragForce
// Description
//   Ergun-Wen-Yu drag model for solid spheres.

#include "_particle_force.hpp"
#include "vol_fields_fwd.hpp"


namespace mousse {

template<class CloudType>
class ErgunWenYuDragForce
:
  public ParticleForce<CloudType>
{
  // Private Data
    //- Reference to the carrier volume fraction field
    const volScalarField& alphac_;
  // Private Member Functions
    //- Drag coefficient multiplied by Reynolds number
    scalar CdRe(const scalar Re) const;
public:
  //- Runtime type information
  TYPE_NAME("ErgunWenYuDrag");
  // Constructors
    //- Construct from mesh
    ErgunWenYuDragForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Construct copy
    ErgunWenYuDragForce(const ErgunWenYuDragForce<CloudType>& df);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType>> clone() const
    {
      return
        autoPtr<ParticleForce<CloudType>>
        {
          new ErgunWenYuDragForce<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~ErgunWenYuDragForce();
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

#include "_ergun_wen_yu_drag_force.ipp"

#endif
