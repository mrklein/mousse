#ifndef CORE_CONTAINERS_HASH_TABLES_HASH_PTR_TABLE_HPP_
#define CORE_CONTAINERS_HASH_TABLES_HASH_PTR_TABLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::HashPtrTable
// Description
//   A HashTable specialization for hashing pointers.

#include "hash_table.hpp"


namespace mousse {

class Istream;
class Ostream;

// Forward declaration of friend functions and operators
template<class T, class Key, class Hash> class HashPtrTable;
template<class T, class Key, class Hash>
Istream& operator>>(Istream&, HashPtrTable<T, Key, Hash>&);
template<class T, class Key, class Hash>
Ostream& operator<<(Ostream&, const HashPtrTable<T, Key, Hash>&);

template<class T, class Key=word, class Hash=string::hash>
class HashPtrTable
:
  public HashTable<T*, Key, Hash>
{
  // Private Member Functions
    //- Read from Istream using given Istream constructor class
    template<class INew>
    void read(Istream&, const INew& inewt);
    //- Read from dictionary using given dictionary constructor class
    template<class INew>
    void read(const dictionary& dict, const INew& inewt);
public:
  typedef typename HashTable<T*, Key, Hash>::iterator iterator;
  typedef typename HashTable<T*, Key, Hash>::const_iterator const_iterator;
  // Constructors
    //- Construct given initial table size
    HashPtrTable(const label size = 128);
    //- Construct from Istream using given Istream constructor class
    template<class INew>
    HashPtrTable(Istream&, const INew&);
    //- Construct from Istream using default Istream constructor class
    HashPtrTable(Istream&);
    //- Construct from dictionary using default dictionary constructor
    //  class
    HashPtrTable(const dictionary&);
    //- Construct as copy
    HashPtrTable(const HashPtrTable<T, Key, Hash>&);
  //- Destructor
  ~HashPtrTable();
  // Member Functions
    // Edit
      //- Remove and return the pointer specified by given iterator
      T* remove(iterator&);
      //- Erase an hashedEntry specified by given iterator
      bool erase(iterator&);
      //- Clear all entries from table
      void clear();
      //- Write
      void write(Ostream& os) const;
  // Member Operators
    void operator=(const HashPtrTable<T, Key, Hash>&);
  // IOstream Operators
    friend Istream& operator>> <T, Key, Hash>
    (
      Istream&,
      HashPtrTable<T, Key, Hash>&
    );
    friend Ostream& operator<< <T, Key, Hash>
    (
      Ostream&,
      const HashPtrTable<T, Key, Hash>&
    );
};

}  // namespace mousse

#include "hash_ptr_table.ipp"

#endif
