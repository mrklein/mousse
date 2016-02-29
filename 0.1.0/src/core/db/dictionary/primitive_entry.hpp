#ifndef CORE_DB_DICTIONARY_PRIMITIVE_ENTRY_HPP_
#define CORE_DB_DICTIONARY_PRIMITIVE_ENTRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::primitiveEntry
// Description
//   A keyword and a list of tokens is a 'primitiveEntry'.
//   An primitiveEntry can be read, written and printed, and the types and
//   values of its tokens analysed.
//   A primitiveEntry is a high-level building block for data description. It
//   is a front-end for the token parser. A list of entries can be used as a
//   set of keyword syntax elements, for example.
// SourceFiles
//   primitive_entry.cpp
//   primitive_entry_io.cpp
#include "istring_stream.hpp"
#include "ostring_stream.hpp"
#include "entry.hpp"
#include "itstream.hpp"
#include "info_proxy.hpp"
namespace mousse
{
class dictionary;
class primitiveEntry
:
  public entry,
  public ITstream
{
  // Private Member Functions
    //- Append the given tokens starting at the current tokenIndex
    void append(const UList<token>&);
    //- Append the given token to this entry
    void append
    (
      const token& currToken,
      const dictionary&,
      Istream&
    );
    //- Expand the given variable (keyword starts with $)
    bool expandVariable(const string&, const dictionary&);
    //- Expand the given function (keyword starts with #)
    bool expandFunction
    (
      const word&,
      const dictionary&,
      Istream&
    );
    //- Read the complete entry from the given stream
    void readEntry(const dictionary&, Istream&);
public:
  // Constructors
    //- Construct from keyword and a Istream
    primitiveEntry(const keyType&, Istream&);
    //- Construct from keyword, parent dictionary and Istream
    primitiveEntry(const keyType&, const dictionary& parentDict, Istream&);
    //- Construct from keyword and a ITstream
    primitiveEntry(const keyType&, const ITstream&);
    //- Construct from keyword and a single token
    primitiveEntry(const keyType&, const token&);
    //- Construct from keyword and a list of tokens
    primitiveEntry(const keyType&, const UList<token>&);
    //- Construct from keyword and by transferring a list of tokens
    primitiveEntry(const keyType&, const Xfer<List<token> >&);
    //- Construct from keyword and a T
    template<class T>
    primitiveEntry(const keyType&, const T&);
    autoPtr<entry> clone(const dictionary&) const
    {
      return autoPtr<entry>(new primitiveEntry(*this));
    }
  // Member functions
    //- Inherit read from ITstream
    using ITstream::read;
    //- Return the dictionary name
    const fileName& name() const
    {
      return ITstream::name();
    }
    //- Return the dictionary name
    fileName& name()
    {
      return ITstream::name();
    }
    //- Return line number of first token in dictionary
    label startLineNumber() const;
    //- Return line number of last token in dictionary
    label endLineNumber() const;
    //- Return true because this entry is a stream
    bool isStream() const
    {
      return true;
    }
    //- Return token stream if this entry is a primitive entry
    ITstream& stream() const;
    //- This entry is not a dictionary,
    //  calling this function generates a FatalError
    const dictionary& dict() const;
    //- This entry is not a dictionary,
    //  calling this function generates a FatalError
    dictionary& dict();
    //- Read tokens from the given stream
    virtual bool read(const dictionary&, Istream&);
    //- Write
    void write(Ostream&) const;
    //- Write, optionally with contents only (no keyword, etc)
    void write(Ostream&, const bool contentsOnly) const;
    //- Return info proxy.
    //  Used to print token information to a stream
    InfoProxy<primitiveEntry> info() const
    {
      return *this;
    }
};
template<>
Ostream& operator<<(Ostream&, const InfoProxy<primitiveEntry>&);
}  // namespace mousse
#ifdef NoRepository
#   include "primitive_entry_templates.cpp"
#endif
#endif
