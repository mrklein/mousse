// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MultiInteraction
// Description
//   Runs multiple patch interaction models in turn. Takes dictionary
//   where all the subdictionaries are the interaction models.
//     // Exit upon first successful interaction or continue doing other
//     // models. Returned nteraction status will be true if there has been any
//     // interaction (so logical or)
//     oneInteractionOnly true;
//     model1
//     {
//       patchInteractionModel coincidentBaffleInteraction;
//       coincidentBaffleInteractionCoeffs
//       {
//         coincidentPatches
//         (
//           (pipetteWall_A pipetteCyclic_half0)
//           (pipetteWall_B pipetteCyclic_half1)
//         );
//       }
//     }
//     model2
//     {
//       patchInteractionModel localInteraction;
//       localInteractionCoeffs
//       {
//         patches
//         (
//           cWall
//           {
//             type rebound;
//           }
//           pipetteWall_A
//           {
//             type rebound;
//           }
//           pipetteWall_B
//           {
//             type rebound;
//           }
//         );
//       }
//     }
#ifndef _multi_interaction_hpp_
#define _multi_interaction_hpp_
#include "_patch_interaction_model.hpp"
namespace mousse
{
template<class CloudType>
class MultiInteraction
:
  public PatchInteractionModel<CloudType>
{
  // Private data
    Switch oneInteractionOnly_;
    //- Submodels
    PtrList<PatchInteractionModel<CloudType> > models_;
  // Private Member Functions
    //- Read settings
    bool read(const dictionary&);
public:
  //- Runtime type information
  TypeName("multiInteraction");
  // Constructors
    //- Construct from dictionary
    MultiInteraction(const dictionary& dict, CloudType& cloud);
    //- Construct copy from owner cloud and patch interaction model
    MultiInteraction(const MultiInteraction<CloudType>& pim);
    //- Construct and return a clone using supplied owner cloud
    virtual autoPtr<PatchInteractionModel<CloudType> > clone() const
    {
      return autoPtr<PatchInteractionModel<CloudType> >
      (
        new MultiInteraction<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~MultiInteraction();
  // Member Functions
    //- Flag to indicate whether model activates patch interaction model
    virtual bool active() const;
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
#   include "_multi_interaction.cpp"
#endif
#endif
