// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_patch_interaction_model.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "vol_fields.hpp"
// Static Data Members
template<class CloudType>
mousse::wordList mousse::PatchInteractionModel<CloudType>::interactionTypeNames_
(
  IStringStream
  (
    "(rebound stick escape)"
  )()
);
// Member Functions 
template<class CloudType>
mousse::word mousse::PatchInteractionModel<CloudType>::interactionTypeToWord
(
  const interactionType& itEnum
)
{
  word it = "other";
  switch (itEnum)
  {
    case itRebound:
    {
      it = "rebound";
      break;
    }
    case itStick:
    {
      it = "stick";
      break;
    }
    case itEscape:
    {
      it = "escape";
      break;
    }
    default:
    {
    }
  }
  return it;
}
template<class CloudType>
typename mousse::PatchInteractionModel<CloudType>::interactionType
mousse::PatchInteractionModel<CloudType>::wordToInteractionType
(
  const word& itWord
)
{
  if (itWord == "rebound")
  {
    return itRebound;
  }
  else if (itWord == "stick")
  {
    return itStick;
  }
  else if (itWord == "escape")
  {
    return itEscape;
  }
  else
  {
    return itOther;
  }
}
// Constructors 
template<class CloudType>
mousse::PatchInteractionModel<CloudType>::PatchInteractionModel
(
  CloudType& owner
)
:
  CloudSubModelBase<CloudType>(owner),
  UName_("unknown_UName")
{}
template<class CloudType>
mousse::PatchInteractionModel<CloudType>::PatchInteractionModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>(owner, dict, typeName, type),
  UName_(this->coeffDict().lookupOrDefault("UName", word("U")))
{}
template<class CloudType>
mousse::PatchInteractionModel<CloudType>::PatchInteractionModel
(
  const PatchInteractionModel<CloudType>& pim
)
:
  CloudSubModelBase<CloudType>(pim),
  UName_(pim.UName_)
{}
// Destructor 
template<class CloudType>
mousse::PatchInteractionModel<CloudType>::~PatchInteractionModel()
{}
// Member Functions 
template<class CloudType>
const mousse::word& mousse::PatchInteractionModel<CloudType>::UName() const
{
  return UName_;
}
template<class CloudType>
void mousse::PatchInteractionModel<CloudType>::info(Ostream& os)
{
  // do nothing
}
#include "_patch_interaction_model_new.cpp"
