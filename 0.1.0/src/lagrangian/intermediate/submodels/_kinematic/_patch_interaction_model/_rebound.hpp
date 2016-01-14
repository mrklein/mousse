// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Rebound
// Description
//   Simple rebound patch interaction model
#ifndef _rebound_hpp_
#define _rebound_hpp_
#include "_patch_interaction_model.hpp"
namespace mousse
{
template<class CloudType>
class Rebound
:
  public PatchInteractionModel<CloudType>
{
  // Private data
   //- Factor applied to velocity on rebound
   //  Normal rebound = 1
   scalar UFactor_;
public:
  //- Runtime type information
  TYPE_NAME("rebound");
  // Constructors
    //- Construct from dictionary
    Rebound(const dictionary& dict, CloudType& cloud);
    //- Construct copy
    Rebound(const Rebound<CloudType>& pim);
    //- Construct and return a clone
    virtual autoPtr<PatchInteractionModel<CloudType> > clone() const
    {
      return autoPtr<PatchInteractionModel<CloudType> >
      (
        new Rebound<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~Rebound();
  // Member Functions
    //- Apply velocity correction
    //  Returns true if particle remains in same cell
    virtual bool correct
    (
      typename CloudType::parcelType& p,
      const polyPatch& pp,
      bool& keepParticle,
      const scalar trackFraction,
      const tetIndices& tetIs
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "_rebound.cpp"
#endif
#endif
