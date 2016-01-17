// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DictionaryBase
// Description
//   Base dictionary class templated on both the form of doubly-linked list
//   it uses as well as the type it holds.
//   The double templating allows for the instantiation of forms with or
//   without storage management.
// Note
//   The IDLListType parameter should itself be a template but this confused
//   gcc 2.95.2 so it has to be instantiated for T when an instantiation of
//   DictionaryBase is requested
// See Also
//   Dictionary and UDictionary
// SourceFiles
//   dictionary_base.cpp
//   dictionary_base_io.cpp

#ifndef dictionary_base_hpp_
#define dictionary_base_hpp_

#include "hash_table.hpp"
#include "word_list.hpp"

namespace mousse
{

// Forward declaration of friend functions and operators
template<class IDLListType, class T>
class DictionaryBase;

template<class IDLListType, class T>
Ostream& operator<<(Ostream&, const DictionaryBase<IDLListType, T>&);

template<class IDLListType, class T>
class DictionaryBase
:
  public IDLListType
{
protected:
  // Protected data
    //- HashTable of the entries held on the IDLListType for quick lookup
    HashTable<T*> hashedTs_;

  // Protected Member functions
    // Add the IDLListType entries into the HashTable
    void addEntries();
public:
  // Constructors
    //- Construct given initial table size
    DictionaryBase(const label size = 128);

    //- Copy construct
    DictionaryBase(const DictionaryBase&);

    //- Construct from Istream using given Istream constructor class
    template<class INew>
    DictionaryBase(Istream&, const INew&);

    //- Construct from Istream using default Istream constructor class
    DictionaryBase(Istream&);
  // Member functions
    // Search and lookup
      //- Search DictionaryBase for given keyword
      bool found(const word&) const;

      //- Find and return an entry if present, otherwise return NULL
      const T* lookupPtr(const word&) const;

      //- Find and return an entry if present, otherwise return NULL
      T* lookupPtr(const word&);

      //- Find and return entry
      const T* lookup(const word&) const;

      //- Find and return entry
      T* lookup(const word&);

      //- Return the table of contents
      wordList toc() const;

    // Editing
      //- Add at head of dictionary
      void insert(const word&, T*);

      //- Add at tail of dictionary
      void append(const word&, T*);

      //- Remove and return entry specified by keyword.
      //  Return NULL if the keyword was not found.
      T* remove(const word&);

      //- Clear the dictionary
      void clear();

      //- Transfer the contents of the argument into this DictionaryBase
      //  and annul the argument.
      void transfer(DictionaryBase<IDLListType, T>&);

  // Member operators
    void operator=(const DictionaryBase&);

    //- Find and return entry
    const T* operator[](const word& key) const
    {
      return lookup(key);
    }

    //- Find and return entry
    T* operator[](const word& key)
    {
      return lookup(key);
    }

  // Ostream operator
    friend Ostream& operator<< <IDLListType, T>
    (
      Ostream&,
      const DictionaryBase<IDLListType, T>&
    );

};

}  // namespace mousse
#ifdef NoRepository
#   include "dictionary_base.cpp"
#endif
#endif
