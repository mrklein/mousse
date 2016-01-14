// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::StandardWallInteraction
// Description
//   Wall interaction model. Three choices:
//   - rebound - optionally specify elasticity and resitution coefficients
//   - stick   - particles assigined zero velocity
//   - escape  - remove particle from the domain
//   Example usage:
//     StandardWallInteractionCoeffs
//     {
//       type        rebound; // stick, escape
//       e           1;       // optional - elasticity coeff
//       mu          0;       // optional - restitution coeff
//     }
#ifndef _standard_wall_interaction_hpp_
#define _standard_wall_interaction_hpp_
#include "_patch_interaction_model.hpp"
namespace mousse
{
template<class CloudType>
class StandardWallInteraction
:
  public PatchInteractionModel<CloudType>
{
protected:
  // Protected data
    //- Interaction type
    typename PatchInteractionModel<CloudType>::interactionType
      interactionType_;
    //- Elasticity coefficient
    scalar e_;
    //- Restitution coefficient
    scalar mu_;
    // Counters for particle fates
      //- Number of parcels escaped
      label nEscape_;
      //- Mass of parcels escaped
      scalar massEscape_;
      //- Number of parcels stuck to patches
      label nStick_;
      //- Mass of parcels stuck to patches
      scalar massStick_;
public:
  //- Runtime type information
  TYPE_NAME("standardWallInteraction");
  // Constructors
    //- Construct from dictionary
    StandardWallInteraction(const dictionary& dict, CloudType& cloud);
    //- Construct copy from owner cloud and patch interaction model
    StandardWallInteraction(const StandardWallInteraction<CloudType>& pim);
    //- Construct and return a clone using supplied owner cloud
    virtual autoPtr<PatchInteractionModel<CloudType> > clone() const
    {
      return autoPtr<PatchInteractionModel<CloudType> >
      (
        new StandardWallInteraction<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~StandardWallInteraction();
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
    // I-O
      //- Write patch interaction info to stream
      virtual void info(Ostream& os);
};
}  // namespace mousse
#ifdef NoRepository
#   include "_standard_wall_interaction.cpp"
#endif
#endif
