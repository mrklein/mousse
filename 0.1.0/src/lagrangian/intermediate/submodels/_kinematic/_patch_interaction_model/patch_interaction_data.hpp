// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchInteractionData
// Description
//   Helper class for the LocalInteraction patch interaction model
#ifndef patch_interaction_data_hpp_
#define patch_interaction_data_hpp_
#include "istream.hpp"
namespace mousse
{
// Forward declaration of classes
class patchInteractionData;
// Forward declaration of friend functions
Istream& operator>>
(
  Istream& is,
  patchInteractionData& pid
);
class patchInteractionData
{
  // Private data
    //- Interaction type name
    word interactionTypeName_;
    //- Patch name
    word patchName_;
    //- Elasticity coefficient
    scalar e_;
    //- Restitution coefficient
    scalar mu_;
public:
  // Constructor
    //- Construct null
    patchInteractionData();
  // Member functions
    // Access
      //- Return const access to the interaction type name
      const word& interactionTypeName() const;
      //- Return const access to the patch name
      const word& patchName() const;
      //- Return const access to the elasticity coefficient
      scalar e() const;
      //- Return const access to the restitution coefficient
      scalar mu() const;
    // I-O
      //- Istream operator
      friend Istream& operator>>
      (
        Istream& is,
        patchInteractionData& pid
      );
};
}  // namespace mousse
#endif
