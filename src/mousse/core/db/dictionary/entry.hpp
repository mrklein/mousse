#ifndef CORE_DB_DICTIONARY_ENTRY_HPP_
#define CORE_DB_DICTIONARY_ENTRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::entry
// Description
//   A keyword and a list of tokens is an 'entry'.
//   An entry can be read, written and printed, and the types and values of
//   its tokens analysed.  An entry is a high-level building block for data
//   description.  It is a front-end for the token parser. A list of entries
//   can be used as a set of keyword syntax elements, for example.

#include "key_type.hpp"
#include "idl_list.hpp"
#include "file_name.hpp"
#include "auto_ptr.hpp"


namespace mousse {

class ITstream;
class dictionary;
// Forward declaration of friend functions and operators
class entry;
Ostream& operator<<(Ostream&, const entry&);


class entry
:
  public IDLList<entry>::link
{
  // Private data
    //- Keyword of entry
    keyType keyword_;
  // Private Member Functions
    //- Get the next valid keyword otherwise return false
    static bool getKeyword(keyType&, Istream&);
public:
  static int disableFunctionEntries;
  // Constructors
    //- Construct from keyword
    entry(const keyType&);
    //- Construct as copy
    entry(const entry&);
    //- Construct on freestore as copy with reference to the
    //  dictionary the copy belongs to
    virtual autoPtr<entry> clone
    (
      const dictionary& parentDict
    ) const = 0;
    //- Construct on freestore as copy
    //  Note: the parent directory is set to dictionary::null
    virtual autoPtr<entry> clone() const;
    //- Construct from Istream and insert into dictionary
    static bool New(dictionary& parentDict, Istream&);
    //- Construct on freestore from Istream and return
    static autoPtr<entry> New(Istream& is);
  //- Destructor
  virtual ~entry()
  {}
  // Member functions
    //- Return keyword
    const keyType& keyword() const
    {
      return keyword_;
    }
    //- Return non-const access to keyword
    keyType& keyword()
    {
      return keyword_;
    }
    //- Return the dictionary name
    virtual const fileName& name() const = 0;
    //- Return the dictionary name
    virtual fileName& name() = 0;
    //- Return line number of first token in dictionary
    virtual label startLineNumber() const = 0;
    //- Return line number of last token in dictionary
    virtual label endLineNumber() const = 0;
    //- Return true if this entry is a stream
    virtual bool isStream() const
    {
      return false;
    }
    //- Return token stream if this entry is a primitive entry
    virtual ITstream& stream() const = 0;
    //- Return true if this entry is a dictionary
    virtual bool isDict() const
    {
      return false;
    }
    //- Return dictionary if this entry is a dictionary
    virtual const dictionary& dict() const = 0;
    //- Return non-const access to dictionary if this entry is a dictionary
    virtual dictionary& dict() = 0;
    //- Write
    virtual void write(Ostream&) const = 0;
  // Member operators
    void operator=(const entry&);
    bool operator==(const entry&) const;
    bool operator!=(const entry&) const;
  // Ostream operator
    friend Ostream& operator<<(Ostream&, const entry&);
};
}  // namespace mousse
#endif
