#ifndef CORE_CONTAINERS_NAMED_ENUM_NAMED_ENUM_HPP_
#define CORE_CONTAINERS_NAMED_ENUM_NAMED_ENUM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NamedEnum
// Description
//   Initialise the NamedEnum HashTable from the static list of names.

#include "hash_table.hpp"
#include "static_assert.hpp"
#include "string_list.hpp"
#include "word_list.hpp"


namespace mousse {

template<class Enum, int nEnum>
class NamedEnum
:
  public HashTable<int>
{
  //- nEnum must be positive (non-zero)
  STATIC_ASSERT(nEnum > 0);

public:
  // Static data members

    //- The set of names corresponding to the enumeration Enum
    static const char* names[nEnum];

  // Constructors

    //- Construct from names
    NamedEnum();

    //- Disallow default bitwise copy construct
    NamedEnum(const NamedEnum&) = delete;

    //- Disallow default bitwise assignment
    NamedEnum& operator=(const NamedEnum&) = delete;

  // Member Functions

    //- Read a word from Istream and return the corresponding
    //  enumeration element
    Enum read(Istream&) const;

    //- Write the name representation of the enumeration to an Ostream
    void write(const Enum e, Ostream&) const;

    //- The set of names as a list of strings
    static stringList strings();

    //- The set of names as a list of words
    static wordList words();

  // Member Operators

    //- Return the enumeration element corresponding to the given name
    const Enum operator[](const char* name) const
    {
      return Enum(HashTable<int>::operator[](name));
    }

    //- Return the enumeration element corresponding to the given name
    const Enum operator[](const word& name) const
    {
      return Enum(HashTable<int>::operator[](name));
    }

    //- Return the name of the given enumeration element
    const char* operator[](const Enum e) const
    {
      return names[e];
    }
};

}  // namespace mousse

#include "named_enum.ipp"

#endif
