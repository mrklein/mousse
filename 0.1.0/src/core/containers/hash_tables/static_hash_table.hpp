// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::StaticHashTable
// Description
//   STL conforming hash table.
// Note
//   Uses straight lists as underlying type.
//   Is slower to insert than the standard HashTable, but should be more
//   memory efficient and faster to access.
// SourceFiles
//   static_hash_table_i.hpp
//   static_hash_table.cpp
//   static_hash_table_io.cpp

#ifndef static_hash_table_hpp_
#define static_hash_table_hpp_

#include "label.hpp"
#include "ulabel.hpp"
#include "word.hpp"
#include "xfer.hpp"
#include "class_name.hpp"
#include "error.hpp"
#include "iostreams.hpp"
#include "ulist.hpp"

namespace mousse
{

// Forward declaration of friend functions and operators
template<class T> class List;
template<class T, class Key, class Hash> class StaticHashTable;
template<class T, class Key, class Hash> Istream& operator>>
(
  Istream&,
  StaticHashTable<T, Key, Hash>&
);
template<class T, class Key, class Hash> Ostream& operator<<
(
  Ostream&,
  const StaticHashTable<T, Key, Hash>&
);

//- Template-invariant bits for StaticHashTable
struct StaticHashTableCore
{
  //- Return a canonical (power-of-two) size
  static label canonicalSize(const label);
  //- Construct null
  StaticHashTableCore()
  {}
  //- Define template name and debug
  CLASS_NAME("StaticHashTable");
  //- A zero-sized end iterator
  struct iteratorEnd
  {
    //- Construct null
    iteratorEnd()
    {}
  };
};


template<class T, class Key=word, class Hash=string::hash>
class StaticHashTable
:
  public StaticHashTableCore
{
  // Private data type for table entries
    //- The lookup keys, ordered per hash value
    List<List<Key> > keys_;
    //- For each key the corresponding object.
    List<List<T> > objects_;
    //- The current number of elements in table
    label nElmts_;
    //- Return a canonical (power-of-two) size
    static label canonicalSize(const label);
    //- Return the hash index of the Key within the current table size.
    //  No checks for zero-sized tables.
    inline label hashKeyIndex(const Key&) const;
    //- Assign a new hashed entry to a possibly already existing key
    bool set(const Key&, const T& newElmt, bool protect);

public:
  // Forward declaration of STL iterators
    template<class TRef, class TableRef>
    class Iterator;
    typedef Iterator
    <
      T&,
      StaticHashTable<T, Key, Hash>&
    > iterator;
    typedef Iterator
    <
      const T&,
      const StaticHashTable<T, Key, Hash>&
    > const_iterator;

  // Declare friendship with the iterators
    friend class Iterator
    <
      T&,
      StaticHashTable<T, Key, Hash>&
    >;

    friend class Iterator
    <
      const T&,
      const StaticHashTable<T, Key, Hash>&
    >;

  // Constructors
    //- Construct given initial table size
    StaticHashTable(const label size = 128);

    //- Construct from Istream
    StaticHashTable(Istream&, const label size = 128);

    //- Construct as copy
    StaticHashTable(const StaticHashTable<T, Key, Hash>&);

    //- Construct by transferring the parameter contents
    StaticHashTable(const Xfer<StaticHashTable<T, Key, Hash> >&);

  //- Destructor
  ~StaticHashTable();

  // Member Functions
    // Access
      //- Return number of elements in table.
      inline label size() const;
      //- Return true if the hash table is empty
      inline bool empty() const;
      //- Return true if hashed entry is found in table
      bool found(const Key& key) const;
      //- Find and return an iterator set at the hashed entry
      //  If not found iterator = end()
      iterator find(const Key& key);
      //- Find and return an const_iterator set at the hashed entry
      //  If not found iterator = end()
      const_iterator find(const Key& key) const;
      //- Return the table of contents
      List<Key> toc() const;
      //- Print information
      Ostream& printInfo(Ostream&) const;
    // Edit
      //- Insert a new hashed entry
      bool insert(const Key& key, const T& newElmt);
      //- Assign a new hashed entry, overwriting existing entries
      inline bool set(const Key&, const T& newElmt);
      //- Erase an hashed entry specified by given iterator
      bool erase(const iterator& it);
      //- Erase an hashed entry specified by given key if in table
      bool erase(const Key& key);
      //- Resize the hash table for efficiency
      void resize(const label newSize);
      //- Remove entries in the given hash table from this hash table
      //  Return the number of elements removed
      label erase(const StaticHashTable<T, Key, Hash>&);
      //- Clear all entries from table
      void clear();
      //- Clear the table entries and the table itself.
      //  Equivalent to clear() followed by resize(1)
      void clearStorage();
      //- Transfer the contents of the argument table into this table
      //  and annul the argument table.
      void transfer(StaticHashTable<T, Key, Hash>&);
      //- Transfer contents to the Xfer container
      inline Xfer<StaticHashTable<T, Key, Hash> > xfer();
  // Member Operators
    //- Find and return an hashed entry
    inline T& operator[](const Key&);
    //- Find and return an hashed entry
    inline const T& operator[](const Key&) const;
    //- Find and return an hashed entry, create it null if not present.
    inline T& operator()(const Key&);
    //- Assignment
    void operator=(const StaticHashTable<T, Key, Hash>&);
    //- Equality. Two hash tables are equal if all contents of first are
    //  also in second and vice versa.
    bool operator==(const StaticHashTable<T, Key, Hash>&) const;
    //- The opposite of the equality operation.
    bool operator!=(const StaticHashTable<T, Key, Hash>&) const;
  // STL type definitions
    //- Type of values the StaticHashTable contains.
    typedef T value_type;
    //- Type that can be used for storing into StaticHashTable::value_type
    //  objects.  This type is usually List::value_type&.
    typedef T& reference;
    //- Type that can be used for storing into constant
    //  StaticHashTable::value_type objects.  This type is usually const
    //  StaticHashTable::value_type&.
    typedef const T& const_reference;
    //- The type that can represent the size of a StaticHashTable.
    typedef label size_type;
  // STL iterator
    //- An STL iterator
    template<class TRef, class TableRef>
    class Iterator
    {
      friend class StaticHashTable;
      template<class TRef2, class TableRef2>
      friend class Iterator;
      // Private data
        //- Reference to the StaticHashTable this is an iterator for
        TableRef hashTable_;
        //- Current hash index
        label hashIndex_;
        //- Index of current element at hashIndex
        label elemIndex_;
    public:
      // Constructors
        //- Construct from hash table, hash index and element index
        inline Iterator
        (
          TableRef,
          label hashIndex_,
          label elemIndex_
        );
        //- Construct from the non-const iterator
        inline Iterator(const iterator&);
      // Member operators
        inline void operator=(const iterator&);
        inline bool operator==(const iterator&) const;
        inline bool operator==(const const_iterator&) const;
        inline bool operator!=(const iterator&) const;
        inline bool operator!=(const const_iterator&) const;
        inline TRef operator*();
        inline TRef operator()();
        inline Iterator& operator++();
        inline Iterator operator++(int);
        inline const Key& key() const;
    };
    //- Iterator set to the beginning of the StaticHashTable
    inline iterator begin();
    //- Iterator set to beyond the end of the StaticHashTable
    inline const iterator& end();
    //- const_iterator set to the beginning of the StaticHashTable
    inline const_iterator cbegin() const;
    //- const_iterator set to beyond the end of the StaticHashTable
    inline const const_iterator& cend() const;
    //- const_iterator set to the beginning of the StaticHashTable
    inline const_iterator begin() const;
    //- const_iterator set to beyond the end of the StaticHashTable
    inline const const_iterator& end() const;
  // IOstream Operator
    friend Istream& operator>> <T, Key, Hash>
    (
      Istream&,
      StaticHashTable<T, Key, Hash>&
    );
    friend Ostream& operator<< <T, Key, Hash>
    (
      Ostream&,
      const StaticHashTable<T, Key, Hash>&
    );
private:
    //- Iterator returned by end()
    iterator endIter_;
    //- const_iterator returned by end()
    const_iterator endConstIter_;
};

}  // namespace mousse

// Private Member Functions
template<class T, class Key, class Hash>
inline mousse::label
mousse::StaticHashTable<T, Key, Hash>::hashKeyIndex(const Key& key) const
{
  // size is power of two - this is the modulus
  return Hash()(key) & (keys_.size() - 1);
}
// Member Functions
template<class T, class Key, class Hash>
inline mousse::label mousse::StaticHashTable<T, Key, Hash>::size() const
{
  return nElmts_;
}
template<class T, class Key, class Hash>
inline bool mousse::StaticHashTable<T, Key, Hash>::empty() const
{
  return !nElmts_;
}
template<class T, class Key, class Hash>
inline bool mousse::StaticHashTable<T, Key, Hash>::insert
(
  const Key& key,
  const T& newEntry
)
{
  return set(key, newEntry, true);
}
template<class T, class Key, class Hash>
inline bool mousse::StaticHashTable<T, Key, Hash>::set
(
  const Key& key,
  const T& newEntry
)
{
  return set(key, newEntry, false);
}
template<class T, class Key, class Hash>
inline mousse::Xfer<mousse::StaticHashTable<T, Key, Hash> >
mousse::StaticHashTable<T, Key, Hash>::xfer()
{
  return xferMove(*this);
}
// Member Operators
template<class T, class Key, class Hash>
inline T& mousse::StaticHashTable<T, Key, Hash>::operator[](const Key& key)
{
  iterator iter = find(key);
  if (iter == end())
  {
    FATAL_ERROR_IN("StaticHashTable<T, Key, Hash>::operator[](const Key&)")
      << key << " not found in table.  Valid entries: "
      << toc()
      << exit(FatalError);
  }
  return *iter;
}
template<class T, class Key, class Hash>
inline const T& mousse::StaticHashTable<T, Key, Hash>::operator[]
(
  const Key& key
) const
{
  const_iterator iter = find(key);
  if (iter == cend())
  {
    FATAL_ERROR_IN
    (
      "StaticHashTable<T, Key, Hash>::operator[](const Key&) const"
    )   << key << " not found in table.  Valid entries: "
      << toc()
      << exit(FatalError);
  }
  return *iter;
}
template<class T, class Key, class Hash>
inline T& mousse::StaticHashTable<T, Key, Hash>::operator()(const Key& key)
{
  iterator iter = find(key);
  if (iter == end())
  {
    insert(key, T());
    return *find(key);
  }
  else
  {
    return *iter;
  }
}

// STL iterator
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline mousse::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::Iterator
(
  TableRef hashTbl,
  label hashIndex,
  label elemIndex
)
:
  hashTable_(hashTbl),
  hashIndex_(hashIndex),
  elemIndex_(elemIndex)
{}
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline mousse::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::Iterator
(
  const iterator& iter
)
:
  hashTable_(iter.hashTable_),
  hashIndex_(iter.hashIndex_),
  elemIndex_(iter.elemIndex_)
{}
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline void
mousse::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator=
(
  const iterator& iter
)
{
  this->hashIndex_ = iter.hashIndex_;
  this->elemIndex_ = iter.elemIndex_;
}
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline bool
mousse::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator==
(
  const iterator& iter
) const
{
  return hashIndex_ == iter.hashIndex_ && elemIndex_ == iter.elemIndex_;
}
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline bool
mousse::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator==
(
  const const_iterator& iter
) const
{
  return hashIndex_ == iter.hashIndex_ && elemIndex_ == iter.elemIndex_;
}
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline bool
mousse::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator!=
(
  const iterator& iter
) const
{
  return !operator==(iter);
}
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline bool
mousse::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator!=
(
  const const_iterator& iter
) const
{
  return !operator==(iter);
}
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline TRef
mousse::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator*()
{
  return hashTable_.objects_[hashIndex_][elemIndex_];
}
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline TRef
mousse::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator()()
{
  return operator*();
}
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline
typename mousse::StaticHashTable<T, Key, Hash>::template Iterator
<
  TRef,
  TableRef
>&
mousse::StaticHashTable<T, Key, Hash>::Iterator
<
  TRef,
  TableRef
>::operator++()
{
  // A negative index is a special value from erase
  // (see notes in HashTable)
  if (hashIndex_ < 0)
  {
    hashIndex_ = -(hashIndex_+1) - 1;
  }
  else
  {
    // Try the next element on the local list
    elemIndex_++;
    if (elemIndex_ < hashTable_.objects_[hashIndex_].size())
    {
      return *this;
    }
  }
  // Step to the next table entry
  elemIndex_ = 0;
  while
  (
    ++hashIndex_ < hashTable_.objects_.size()
  && !hashTable_.objects_[hashIndex_].size()
  )
  {}
  if (hashIndex_ >= hashTable_.objects_.size())
  {
    // make end iterator
    hashIndex_ = hashTable_.keys_.size();
  }
  return *this;
}
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline
typename mousse::StaticHashTable<T, Key, Hash>::template Iterator
<
  TRef,
  TableRef
>
mousse::StaticHashTable<T, Key, Hash>::Iterator
<
  TRef,
  TableRef
>::operator++
(
  int
)
{
  iterator tmp = *this;
  ++*this;
  return tmp;
}
template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline const Key&
mousse::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::key() const
{
  return hashTable_.keys_[hashIndex_][elemIndex_];
}
template<class T, class Key, class Hash>
inline typename mousse::StaticHashTable<T, Key, Hash>::iterator
mousse::StaticHashTable<T, Key, Hash>::begin()
{
  // Find first non-empty entry
  FOR_ALL(keys_, hashIdx)
  {
    if (keys_[hashIdx].size())
    {
      return iterator(*this, hashIdx, 0);
    }
  }
#ifdef FULLDEBUG
  if (debug)
  {
    Info<< "StaticHashTable is empty\n";
  }
#endif
  return StaticHashTable<T, Key, Hash>::endIter_;
}
template<class T, class Key, class Hash>
inline const typename mousse::StaticHashTable<T, Key, Hash>::iterator&
mousse::StaticHashTable<T, Key, Hash>::end()
{
  return StaticHashTable<T, Key, Hash>::endIter_;
}
template<class T, class Key, class Hash>
inline typename mousse::StaticHashTable<T, Key, Hash>::const_iterator
mousse::StaticHashTable<T, Key, Hash>::cbegin() const
{
  // Find first non-empty entry
  FOR_ALL(keys_, hashIdx)
  {
    if (keys_[hashIdx].size())
    {
      return const_iterator(*this, hashIdx, 0);
    }
  }
#   ifdef FULLDEBUG
  if (debug)
  {
    Info<< "StaticHashTable is empty\n";
  }
#   endif
  return StaticHashTable<T, Key, Hash>::endConstIter_;
}
template<class T, class Key, class Hash>
inline const typename mousse::StaticHashTable<T, Key, Hash>::const_iterator&
mousse::StaticHashTable<T, Key, Hash>::cend() const
{
  return StaticHashTable<T, Key, Hash>::endConstIter_;
}
template<class T, class Key, class Hash>
inline typename mousse::StaticHashTable<T, Key, Hash>::const_iterator
mousse::StaticHashTable<T, Key, Hash>::begin() const
{
  return this->cbegin();
}
template<class T, class Key, class Hash>
inline const typename mousse::StaticHashTable<T, Key, Hash>::const_iterator&
mousse::StaticHashTable<T, Key, Hash>::end() const
{
  return StaticHashTable<T, Key, Hash>::endConstIter_;
}

#ifndef NoStaticHashTableC
#ifdef NoRepository
#   include "static_hash_table.cpp"
#endif
#endif
#endif
