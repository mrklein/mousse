#ifndef CORE_MESHES_IDENTIFIERS_COUPLE_GROUP_IDENTIFIER_HPP_
#define CORE_MESHES_IDENTIFIERS_COUPLE_GROUP_IDENTIFIER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coupleGroupIdentifier
// Description
//   Encapsulates using patchGroups to specify coupled patch
// SourceFiles
//   couple_group_identifier_i.hpp
//   couple_group_identifier.cpp
//   couple_group_identifier_io.cpp
#include "word.hpp"
#include "label.hpp"
namespace mousse
{
// Forward declaration of classes
class dictionary;
class polyMesh;
class polyPatch;
class Ostream;
// Forward declaration of friend functions and operators
class coupleGroupIdentifier;
Ostream& operator<<(Ostream&, const coupleGroupIdentifier&);
class coupleGroupIdentifier
{
  // Private data
    //- Name of patchGroup
    word name_;
  // Private Member Functions
    //- Find other patch in specified mesh. Returns index of patch or -1.
    label findOtherPatchID(const polyMesh&, const polyPatch&) const;
public:
  // Constructors
    //- Construct null
    coupleGroupIdentifier();
    //- Construct from components
    coupleGroupIdentifier(const word& patchGroupName);
    //- Construct from dictionary
    coupleGroupIdentifier(const dictionary&);
  // Member Functions
    //- Name of patchGroup
    inline const word& name() const;
    //- Is a valid patchGroup
    inline bool valid() const;
    //- Find other patch in same region. Returns index of patch or -1.
    label findOtherPatchID(const polyPatch&) const;
    //- Find other patch and region. Returns index of patch and sets
    //  otherRegion to name of region. Fatal error if patch not found
    label findOtherPatchID(const polyPatch&, word&) const;
    //- Write the data as a dictionary
    void write(Ostream&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const coupleGroupIdentifier&);
};
}  // namespace mousse

// Member Functions 
const mousse::word& mousse::coupleGroupIdentifier::name() const
{
  return name_;
}
bool mousse::coupleGroupIdentifier::valid() const
{
  return !name_.empty();
}
#endif
