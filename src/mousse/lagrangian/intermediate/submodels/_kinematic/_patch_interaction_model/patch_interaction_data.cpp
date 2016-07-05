// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_interaction_data.hpp"
#include "dictionary_entry.hpp"
#include "_patch_interaction_model.hpp"


// Constructors 
mousse::patchInteractionData::patchInteractionData()
:
  interactionTypeName_{"unknownInteractionTypeName"},
  patchName_{"unknownPatch"},
  e_{0.0},
  mu_{0.0}
{}


// Member Functions 
const mousse::word& mousse::patchInteractionData::interactionTypeName() const
{
  return interactionTypeName_;
}


const mousse::word& mousse::patchInteractionData::patchName() const
{
  return patchName_;
}


mousse::scalar mousse::patchInteractionData::e() const
{
  return e_;
}


mousse::scalar mousse::patchInteractionData::mu() const
{
  return mu_;
}


// IOstream Operators 
mousse::Istream& mousse::operator>>
(
  Istream& is,
  patchInteractionData& pid
)
{
  is.check("Istream& operator>>(Istream&, patchInteractionData&)");
  const dictionaryEntry entry(dictionary::null, is);
  pid.patchName_ = entry.keyword();
  entry.lookup("type") >> pid.interactionTypeName_;
  pid.e_ = entry.lookupOrDefault<scalar>("e", 1.0);
  pid.mu_ = entry.lookupOrDefault<scalar>("mu", 0.0);
  return is;
}

