#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TDRAG_TPLESSIS_MASLIYAH_DRAG_FORCE_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TDRAG_TPLESSIS_MASLIYAH_DRAG_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PlessisMasliyahDragForce
// Description
//   PlessisMasliyahDragForce drag model for solid spheres.

#include "_particle_force.hpp"
#include "vol_fields_fwd.hpp"


namespace mousse {

template<class CloudType>
class PlessisMasliyahDragForce
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
  TYPE_NAME("PlessisMasliyahDrag");
  // Constructors
    //- Construct from mesh
    PlessisMasliyahDragForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Construct copy
    PlessisMasliyahDragForce(const PlessisMasliyahDragForce<CloudType>& df);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new PlessisMasliyahDragForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~PlessisMasliyahDragForce();
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

#include "_plessis_masliyah_drag_force.ipp"

#endif
