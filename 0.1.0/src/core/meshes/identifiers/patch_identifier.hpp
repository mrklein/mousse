#ifndef CORE_MESHES_IDENTIFIERS_PATCH_IDENTIFIER_HPP_
#define CORE_MESHES_IDENTIFIERS_PATCH_IDENTIFIER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchIdentifier
// Description
//   Identifies patch by name, patch index and physical type
// SourceFiles
//   patch_identifier.cpp
#include "word_list.hpp"
#include "label.hpp"
namespace mousse
{
class dictionary;
// Forward declaration of friend functions and operators
class patchIdentifier;
Ostream& operator<<(Ostream&, const patchIdentifier&);
class patchIdentifier
{
  // Private data
    //- Name of patch
    word name_;
    //- Index of patch in boundary
    label index_;
    //- Optional physical type
    mutable word physicalType_;
    //- Optional groups patch belongs to
    wordList inGroups_;
public:
  // Constructors
    //- Construct from components
    patchIdentifier
    (
      const word& name,
      const label index,
      const word& physicalType = word::null,
      const wordList& inGroups = wordList()
    );
    //- Construct from dictionary
    patchIdentifier
    (
      const word& name,
      const dictionary&,
      const label index
    );
    //- Construct from geometric patch, resetting the index
    patchIdentifier
    (
      const patchIdentifier&,
      const label index
    );
  //- Destructor
  virtual ~patchIdentifier();
  // Member Functions
    //- Return name
    const word& name() const
    {
      return name_;
    }
    //- Return name for modification
    word& name()
    {
      return name_;
    }
    //- Return the optional physical type of the patch
    const word& physicalType() const
    {
      return physicalType_;
    }
    //- Return the optional physical type of the patch for modification
    word& physicalType()
    {
      return physicalType_;
    }
    //- Return the index of this patch in the boundaryMesh
    label index() const
    {
      return index_;
    }
    //- Return the index of this patch in the boundaryMesh for modification
    label& index()
    {
      return index_;
    }
    //- Return the optional groups patch belongs to
    const wordList& inGroups() const
    {
      return inGroups_;
    }
    //- Return the optional groups patch belongs to for modification
    wordList& inGroups()
    {
      return inGroups_;
    }
    //- Test if in group
    bool inGroup(const word&) const;
    //- Write patchIdentifier as a dictionary
    void write(Ostream&) const;
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const patchIdentifier&);
};
}  // namespace mousse
#endif
