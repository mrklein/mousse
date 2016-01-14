// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::WenYuDragForce
// Description
//   Wen-Yu drag model for solid spheres.
#ifndef _wen_yu_drag_force_hpp_
#define _wen_yu_drag_force_hpp_
#include "_particle_force.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
template<class CloudType>
class WenYuDragForce
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
  TYPE_NAME("WenYuDrag");
  // Constructors
    //- Construct from mesh
    WenYuDragForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Construct copy
    WenYuDragForce(const WenYuDragForce<CloudType>& df);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new WenYuDragForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~WenYuDragForce();
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
#ifdef NoRepository
#   include "_wen_yu_drag_force.cpp"
#endif
#endif
