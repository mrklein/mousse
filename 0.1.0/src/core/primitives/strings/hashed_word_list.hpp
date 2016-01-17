// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hashedWordList
// Description
//   A wordList with hashed indices for faster lookup by name.
// SourceFiles
//   hashed_word_list.cpp
#ifndef hashed_word_list_hpp_
#define hashed_word_list_hpp_
#include "word_list.hpp"
#include "hash_table.hpp"
namespace mousse
{
class hashedWordList;
// Forward declaration of friend functions and operators
Istream& operator>>(Istream&, hashedWordList&);
Ostream& operator<<(Ostream&, const hashedWordList&);
class hashedWordList
:
  public List<word>
{
  // Private data
    HashTable<label, word> indices_;
  // Private Member Functions
    //- Rebuild the hash of indices
    void rehash();
public:
  // Constructors
    //- Construct null
    hashedWordList();
    //- Copy constructor.
    hashedWordList(const hashedWordList&);
    //- Construct from list of names
    hashedWordList(const UList<word>& names);
    //- Construct by transferring the parameter contents
    hashedWordList(const Xfer< List<word> >& names);
    //- Construct from number and list of names
    hashedWordList(const label nNames, const char** names);
    //- Construct from a NULL-terminated list of names
    hashedWordList(const char** names);
    //- Construct from Istream
    hashedWordList(Istream&);
  // Member Functions
    //- Clear the list, i.e. set size to zero.
    void clear();
    //- Append an element at the end of the list
    void append(const word&);
    //- Does the list contain the specified name
    inline bool contains(const word&) const;
    //- Transfer the contents of the argument List into this list
    //  and annul the argument list.
    void transfer(List<word>&);
  // Member Operators
    //- Assignment operator from list of names
    inline void operator=(const UList<word>& names);
    //- Assignment operator.
    inline void operator=(const hashedWordList&);
    //- Return name corresponding to specified index
    inline const word& operator[](const label index) const;
    //- Return index corresponding to specified name
    inline label operator[](const word&) const;
  // Istream operators
    friend Istream& operator>>(Istream&, hashedWordList&);
    friend Ostream& operator<<(Ostream&, const hashedWordList&);
};
}  // namespace mousse

// Member Functions 
inline bool mousse::hashedWordList::contains(const word& name) const
{
  return indices_.found(name);
}
// Member Operators 
inline void mousse::hashedWordList::operator=(const UList<word>& lst)
{
  List<word>::operator=(lst);
  rehash();
}
inline void mousse::hashedWordList::operator=(const hashedWordList& lst)
{
  operator=(static_cast<const UList<word>&>(lst));
}
inline const mousse::word& mousse::hashedWordList::operator[]
(
  const label index
) const
{
  return List<word>::operator[](index);
}
// could return -1 instead of bombing out
inline mousse::label mousse::hashedWordList::operator[](const word& name) const
{
  return indices_[name];
}
#endif
