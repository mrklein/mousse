// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::HashTable
// Description
//   An STL-conforming hash table.
// Note
//   Hashing index collisions are handled via chaining using a singly-linked
//   list with the colliding entry being added to the head of the linked
//   list. Thus copying the hash table (or indeed even resizing it) will
//   often result in a different hash order. Use a sorted table-of-contents
//   when the hash order is important.
// SourceFiles
//   hash_table.cpp
//   hash_table_io.cpp

#ifndef hash_table_hpp_
#define hash_table_hpp_

#include "label.hpp"
#include "ulabel.hpp"
#include "word.hpp"
#include "xfer.hpp"
#include "class_name.hpp"
#include "error.hpp"

namespace mousse
{

// Forward declaration of friend functions and operators
template<class T> class List;
template<class T> class UList;
template<class T, class Key, class Hash> class HashTable;
template<class T, class Key, class Hash> class HashPtrTable;
template<class T, class Key, class Hash>
Istream& operator>>(Istream&, HashTable<T, Key, Hash>&);
template<class T, class Key, class Hash>
Ostream& operator<<(Ostream&, const HashTable<T, Key, Hash>&);

//- Template-invariant bits for HashTable
struct HashTableCore
{
  //- Return a canonical (power-of-two) size
  static label canonicalSize(const label);

  //- Maximum allowable table size
  static const label maxTableSize;

  //- Construct null
  HashTableCore()
  {}

  //- Define template name and debug
  CLASS_NAME("HashTable");

  //- A zero-sized end iterator
  struct iteratorEnd
  {
    //- Construct null
    iteratorEnd()
    {}
  };

  //- iteratorEnd set to beyond the end of any HashTable
  inline static iteratorEnd cend()
  {
    return iteratorEnd();
  }

  //- iteratorEnd set to beyond the end of any HashTable
  inline static iteratorEnd end()
  {
    return iteratorEnd();
  }

};

template<class T, class Key=word, class Hash=string::hash>
class HashTable
:
  public HashTableCore
{

  // Private data type for table entries

    //- Structure to hold a hashed entry with SLList for collisions
    struct hashedEntry
    {
      //- The lookup key
      Key key_;

      //- Pointer to next hashedEntry in sub-list
      hashedEntry* next_;

      //- The data object
      T obj_;

      //- Construct from key, next pointer and object
      inline hashedEntry(const Key&, hashedEntry* next, const T&);

      //- Disallow default bitwise copy construct
      hashedEntry(const hashedEntry&) = delete;

      //- Disallow default bitwise assignment
      hashedEntry& operator=(const hashedEntry&) = delete;
    };

  // Private data: size of table, the table and current number of elements

    //- The current number of elements in table
    label nElmts_;

    //- Number of primary entries allocated in table
    label tableSize_;

    //- The table of primary entries
    hashedEntry** table_;

  // Private Member Functions

    //- Return a canonical (power-of-two) size
    static label canonicalSize(const label);

    //- Return the hash index of the Key within the current table size.
    //  No checks for zero-sized tables.
    inline label hashKeyIndex(const Key&) const;

    //- Assign a new hashedEntry to a possibly already existing key
    bool set(const Key&, const T& newElmt, bool protect);

public:

  // Forward declaration of iterators

    class iteratorBase;
    class iterator;
    class const_iterator;

    //- Declare friendship with the HashPtrTable class
    template<class T2, class Key2, class Hash2>
    friend class HashPtrTable;

    //- Declare friendship with the iteratorBase
    friend class iteratorBase;

    //- Declare friendship with the iterator
    friend class iterator;

    //- Declare friendship with the const_iterator
    friend class const_iterator;

  // Constructors

    //- Construct given initial table size
    HashTable(const label size = 128);

    //- Construct from Istream
    HashTable(Istream&, const label size = 128);

    //- Construct as copy
    HashTable(const HashTable<T, Key, Hash>&);

    //- Construct by transferring the parameter contents
    HashTable(const Xfer<HashTable<T, Key, Hash> >&);

  //- Destructor
  ~HashTable();

  // Member Functions

    // Access

      //- The size of the underlying table
      inline label capacity() const;

      //- Return number of elements in table
      inline label size() const;

      //- Return true if the hash table is empty
      inline bool empty() const;

      //- Return true if hashedEntry is found in table
      bool found(const Key&) const;

      //- Find and return an iterator set at the hashedEntry
      //  If not found iterator = end()
      iterator find(const Key&);

      //- Find and return an const_iterator set at the hashedEntry
      //  If not found iterator = end()
      const_iterator find(const Key&) const;

      //- Return the table of contents
      List<Key> toc() const;

      //- Return the table of contents as a sorted list
      List<Key> sortedToc() const;

      //- Print information
      Ostream& printInfo(Ostream&) const;

    // Edit

      //- Insert a new hashedEntry
      inline bool insert(const Key&, const T& newElmt);

      //- Assign a new hashedEntry, overwriting existing entries
      inline bool set(const Key&, const T& newElmt);

      //- Erase a hashedEntry specified by given iterator
      //  This invalidates the iterator until the next operator++
      bool erase(const iterator&);

      //- Erase a hashedEntry specified by the given key
      bool erase(const Key&);

      //- Remove entries given by the listed keys from this HashTable
      //  Return the number of elements removed
      label erase(const UList<Key>&);

      //- Remove entries given by the given keys from this HashTable
      //  Return the number of elements removed.
      //  The parameter HashTable needs the same type of key, but the
      //  type of values held and the hashing function are arbitrary.
      template<class AnyType, class AnyHash>
      label erase(const HashTable<AnyType, Key, AnyHash>&);

      //- Resize the hash table for efficiency
      void resize(const label newSize);

      //- Clear all entries from table
      void clear();

      //- Clear the table entries and the table itself.
      //  Equivalent to clear() followed by resize(0)
      void clearStorage();

      //- Shrink the allocated table to approx. twice number of elements
      void shrink();

      //- Transfer the contents of the argument table into this table
      //  and annul the argument table.
      void transfer(HashTable<T, Key, Hash>&);

      //- Transfer contents to the Xfer container
      inline Xfer<HashTable<T, Key, Hash> > xfer();

  // Member Operators

    //- Find and return a hashedEntry
    inline T& operator[](const Key&);

    //- Find and return a hashedEntry
    inline const T& operator[](const Key&) const;

    //- Find and return a hashedEntry, create it null if not present
    inline T& operator()(const Key&);

    //- Assignment
    void operator=(const HashTable<T, Key, Hash>&);

    //- Equality. Hash tables are equal if the keys and values are equal.
    //  Independent of table storage size and table order.
    bool operator==(const HashTable<T, Key, Hash>&) const;

    //- The opposite of the equality operation. Takes linear time.
    bool operator!=(const HashTable<T, Key, Hash>&) const;

  // STL type definitions

    //- Type of values the HashTable contains.
    typedef T value_type;

    //- Type that can be used for storing into HashTable::value_type
    //  objects.  This type is usually List::value_type&.
    typedef T& reference;

    //- Type that can be used for storing into constant
    //  HashTable::value_type objects.  This type is usually const
    //  HashTable::value_type&.
    typedef const T& const_reference;

    //- The type that can represent the size of a HashTable.
    typedef label size_type;

  // Iterators and helpers

    //- The iterator base for HashTable
    //  Note: data and functions are protected, to allow reuse by iterator
    //  and prevent most external usage.
    //  iterator and const_iterator have the same size, allowing
    //  us to reinterpret_cast between them (if desired)
    class iteratorBase
    {
      // Private Data

        //- Pointer to the HashTable for which this is an iterator
        //  This also lets us use the default bitwise copy/assignment
        HashTable<T, Key, Hash>* hashTable_;

        //- Current element
        hashedEntry* entryPtr_;

        //- Current hash index
        label hashIndex_;

    protected:

      // Constructors

        //- Construct null - equivalent to an 'end' position
        inline iteratorBase();

        //- Construct from hash table, moving to its 'begin' position
        inline explicit iteratorBase
        (
          const HashTable<T, Key, Hash>* curHashTable
        );

        //- Construct from hash table, element and hash index
        inline iteratorBase
        (
          const HashTable<T, Key, Hash>* curHashTable,
          const hashedEntry* elmt,
          const label hashIndex
        );

      // Protected Member Functions

        //- Increment to the next position
        inline void increment();

        //- Erase the HashTable element at the current position
        bool erase();

        //- Return non-const access to referenced object
        inline T& object();

        //- Return const access to referenced object
        inline const T& cobject() const;

    public:

      // Member operators

        // Access
        //- Return the Key corresponding to the iterator
        inline const Key& key() const;

        //- Compare hashedEntry element pointers
        inline bool operator==(const iteratorBase&) const;
        inline bool operator!=(const iteratorBase&) const;

        //- Compare hashedEntry to iteratorEnd pointers
        inline bool operator==(const iteratorEnd& unused) const;
        inline bool operator!=(const iteratorEnd& unused) const;
    };

    //- An STL-conforming iterator
    class iterator
    :
      public iteratorBase
    {
      friend class HashTable;

      // Private Member Functions

        //- Construct from hash table, moving to its 'begin' position
        inline explicit iterator
        (
          HashTable<T, Key, Hash>* curHashTable
        );

        //- Construct from hash table, element and hash index
        inline iterator
        (
          HashTable<T, Key, Hash>* curHashTable,
          hashedEntry* elmt,
          const label hashIndex
        );

    public:

      // Constructors

        //- Construct null (end iterator)
        inline iterator();

        //- Construct end iterator
        inline iterator(const iteratorEnd& unused);

      // Member operators

        //- Return referenced hash value
        inline T& operator*();
        inline T& operator()();

        //- Return referenced hash value
        inline const T& operator*() const;
        inline const T& operator()() const;
        inline iterator& operator++();
        inline iterator operator++(int);
    };

    //- Iterator set to the beginning of the HashTable
    inline iterator begin();

  // STL const_iterator

    //- An STL-conforming const_iterator
    class const_iterator
    :
      public iteratorBase
    {
      friend class HashTable;

      // Private Member Functions

        //- Construct from hash table, moving to its 'begin' position
        inline explicit const_iterator
        (
          const HashTable<T, Key, Hash>* curHashTable
        );

        //- Construct from hash table, element and hash index
        inline const_iterator
        (
          const HashTable<T, Key, Hash>* curHashTable,
          const hashedEntry* elmt,
          const label hashIndex
        );

    public:

      // Constructors

        //- Construct null (end iterator)
        inline const_iterator();

        //- Construct from iterator
        inline const_iterator(const iterator&);

        //- Construct end iterator
        inline const_iterator(const iteratorEnd& unused);

      // Member operators

        //- Return referenced hash value
        inline const T& operator*() const;
        inline const T& operator()() const;
        inline const_iterator& operator++();
        inline const_iterator operator++(int);
    };

    //- const_iterator set to the beginning of the HashTable
    inline const_iterator cbegin() const;

    //- const_iterator set to the beginning of the HashTable
    inline const_iterator begin() const;

  // IOstream Operator

    friend Istream& operator>> <T, Key, Hash>
    (
      Istream&,
      HashTable<T, Key, Hash>&
    );

    friend Ostream& operator<< <T, Key, Hash>
    (
      Ostream&,
      const HashTable<T, Key, Hash>&
    );
};

}  // namespace mousse

// Private Member Classes
template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::hashedEntry::hashedEntry
(
  const Key& key,
  hashedEntry* next,
  const T& obj
)
:
  key_{key},
  next_{next},
  obj_{obj}
{}

// Private Member Functions 
template<class T, class Key, class Hash>
inline mousse::label
mousse::HashTable<T, Key, Hash>::hashKeyIndex(const Key& key) const
{
  // size is power of two - this is the modulus
  return Hash()(key) & (tableSize_ - 1);
}

// Member Functions
template<class T, class Key, class Hash>
inline mousse::label mousse::HashTable<T, Key, Hash>::capacity() const
{
  return tableSize_;
}

template<class T, class Key, class Hash>
inline mousse::label mousse::HashTable<T, Key, Hash>::size() const
{
  return nElmts_;
}

template<class T, class Key, class Hash>
inline bool mousse::HashTable<T, Key, Hash>::empty() const
{
  return !nElmts_;
}

template<class T, class Key, class Hash>
inline bool mousse::HashTable<T, Key, Hash>::insert
(
  const Key& key,
  const T& newEntry
)
{
  return this->set(key, newEntry, true);
}

template<class T, class Key, class Hash>
inline bool mousse::HashTable<T, Key, Hash>::set
(
  const Key& key,
  const T& newEntry
)
{
  return this->set(key, newEntry, false);
}

template<class T, class Key, class Hash>
inline mousse::Xfer<mousse::HashTable<T, Key, Hash> >
mousse::HashTable<T, Key, Hash>::xfer()
{
  return xferMove(*this);
}

// Member Operators 
template<class T, class Key, class Hash>
inline T& mousse::HashTable<T, Key, Hash>::operator[](const Key& key)
{
  iterator iter = this->find(key);
  if (iter == this->end())
  {
    FATAL_ERROR_IN("HashTable<T, Key, Hash>::operator[](const Key&)")
      << key << " not found in table.  Valid entries: "
      << toc()
      << exit(FatalError);
  }
  return *iter;
}

template<class T, class Key, class Hash>
inline const T& mousse::HashTable<T, Key, Hash>::operator[](const Key& key) const
{
  const_iterator iter = this->find(key);
  if (iter == this->cend())
  {
    FATAL_ERROR_IN("HashTable<T, Key, Hash>::operator[](const Key&) const")
      << key << " not found in table.  Valid entries: "
      << toc()
      << exit(FatalError);
  }
  return *iter;
}

template<class T, class Key, class Hash>
inline T& mousse::HashTable<T, Key, Hash>::operator()(const Key& key)
{
  iterator iter = this->find(key);
  if (iter == this->end())
  {
    this->insert(key, T());
    return *find(key);
  }
  else
  {
    return *iter;
  }
}

// iterator base
template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::iteratorBase::iteratorBase()
:
  hashTable_{0},
  entryPtr_{0},
  hashIndex_{0}
{}

template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::iteratorBase::iteratorBase
(
  const HashTable<T, Key, Hash>* hashTbl
)
:
  hashTable_{const_cast<HashTable<T, Key, Hash>*>(hashTbl)},
  entryPtr_{0},
  hashIndex_{0}
{
  if (hashTable_->nElmts_)
  {
    // find first non-NULL table entry
    while
    (
      !(entryPtr_ = hashTable_->table_[hashIndex_])
    && ++hashIndex_ < hashTable_->tableSize_
    )
    {}
    if (hashIndex_ >= hashTable_->tableSize_)
    {
      // make into an end iterator
      entryPtr_ = 0;
      hashIndex_ = 0;
    }
  }
}

template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::iteratorBase::iteratorBase
(
  const HashTable<T, Key, Hash>* hashTbl,
  const hashedEntry* elmt,
  const label hashIndex
)
:
  hashTable_{const_cast<HashTable<T, Key, Hash>*>(hashTbl)},
  entryPtr_{const_cast<hashedEntry*>(elmt)},
  hashIndex_{hashIndex}
{}

template<class T, class Key, class Hash>
inline void
mousse::HashTable<T, Key, Hash>::iteratorBase::increment()
{
  // A negative index is a special value from erase
  if (hashIndex_ < 0)
  {
    // the markPos='-curPos-1', but we wish to continue at 'curPos-1'
    // thus use '-(markPos+1) -1'
    hashIndex_ = -(hashIndex_+1) - 1;
  }
  else if (entryPtr_)
  {
    if (entryPtr_->next_)
    {
      // Move to next element on the SLList
      entryPtr_ = entryPtr_->next_;
      return;
    }
  }
  // else
  // {
  //     // if we reach here (entryPtr_ is NULL) it is already at the end()
  //     // we should probably stop
  // }

  // Step to the next table entry
  while (++hashIndex_ < hashTable_->tableSize_
         && !(entryPtr_ = hashTable_->table_[hashIndex_]))
  {}

  if (hashIndex_ >= hashTable_->tableSize_)
  {
    // make into an end iterator
    entryPtr_ = 0;
    hashIndex_ = 0;
  }
}

template<class T, class Key, class Hash>
inline
const Key& mousse::HashTable<T, Key, Hash>::iteratorBase::key() const
{
  return entryPtr_->key_;
}

template<class T, class Key, class Hash>
inline T&
mousse::HashTable<T, Key, Hash>::iteratorBase::object()
{
  return entryPtr_->obj_;
}

template<class T, class Key, class Hash>
inline const T&
mousse::HashTable<T, Key, Hash>::iteratorBase::cobject() const
{
  return entryPtr_->obj_;
}

template<class T, class Key, class Hash>
inline bool mousse::HashTable<T, Key, Hash>::iteratorBase::operator==
(
  const iteratorBase& iter
) const
{
  return entryPtr_ == iter.entryPtr_;
}

template<class T, class Key, class Hash>
inline bool mousse::HashTable<T, Key, Hash>::iteratorBase::operator!=
(
  const iteratorBase& iter
) const
{
  return entryPtr_ != iter.entryPtr_;
}

template<class T, class Key, class Hash>
inline bool mousse::HashTable<T, Key, Hash>::iteratorBase::operator==
(
  const iteratorEnd&
) const
{
  return !entryPtr_;
}

template<class T, class Key, class Hash>
inline bool mousse::HashTable<T, Key, Hash>::iteratorBase::operator!=
(
  const iteratorEnd&
) const
{
  return entryPtr_;
}

// STL iterator 
template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::iterator::iterator()
:
  iteratorBase{}
{}

template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::iterator::iterator
(
  const iteratorEnd&
)
:
  iteratorBase{}
{}

template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::iterator::iterator
(
  HashTable<T, Key, Hash>* hashTbl
)
:
  iteratorBase{hashTbl}
{}

template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::iterator::iterator
(
  HashTable<T, Key, Hash>* hashTbl,
  hashedEntry* elmt,
  const label hashIndex
)
:
  iteratorBase{hashTbl, elmt, hashIndex}
{}

template<class T, class Key, class Hash>
inline T&
mousse::HashTable<T, Key, Hash>::iterator::operator*()
{
  return this->object();
}

template<class T, class Key, class Hash>
inline T&
mousse::HashTable<T, Key, Hash>::iterator::operator()()
{
  return this->object();
}

template<class T, class Key, class Hash>
inline const T&
mousse::HashTable<T, Key, Hash>::iterator::operator*() const
{
  return this->cobject();
}

template<class T, class Key, class Hash>
inline const T&
mousse::HashTable<T, Key, Hash>::iterator::operator()() const
{
  return this->cobject();
}

template<class T, class Key, class Hash>
inline
typename mousse::HashTable<T, Key, Hash>::iterator&
mousse::HashTable<T, Key, Hash>::iterator::operator++()
{
  this->increment();
  return *this;
}

template<class T, class Key, class Hash>
inline typename mousse::HashTable<T, Key, Hash>::iterator
mousse::HashTable<T, Key, Hash>::iterator::operator++(int)
{
  iterator old = *this;
  this->increment();
  return old;
}

template<class T, class Key, class Hash>
inline typename mousse::HashTable<T, Key, Hash>::iterator
mousse::HashTable<T, Key, Hash>::begin()
{
  return iterator(this);
}

// STL const_iterator
template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::const_iterator::const_iterator()
:
  iteratorBase{}
{}

template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::const_iterator::const_iterator
(
  const HashTable<T, Key, Hash>::iterator& iter
)
:
  iteratorBase{iter}
{}

template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::const_iterator::const_iterator
(
  const iteratorEnd&
)
:
  iteratorBase{}
{}

template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::const_iterator::const_iterator
(
  const HashTable<T, Key, Hash>* hashTbl
)
:
  iteratorBase{hashTbl}
{}

template<class T, class Key, class Hash>
inline mousse::HashTable<T, Key, Hash>::const_iterator::const_iterator
(
  const HashTable<T, Key, Hash>* hashTbl,
  const hashedEntry* elmt,
  const label hashIndex
)
:
  iteratorBase{hashTbl, elmt, hashIndex}
{}

template<class T, class Key, class Hash>
inline const T&
mousse::HashTable<T, Key, Hash>::const_iterator::operator*() const
{
  return this->cobject();
}

template<class T, class Key, class Hash>
inline const T&
mousse::HashTable<T, Key, Hash>::const_iterator::operator()() const
{
  return this->cobject();
}

template<class T, class Key, class Hash>
inline
typename mousse::HashTable<T, Key, Hash>::const_iterator&
mousse::HashTable<T, Key, Hash>::const_iterator::operator++()
{
  this->increment();
  return *this;
}

template<class T, class Key, class Hash>
inline typename mousse::HashTable<T, Key, Hash>::const_iterator
mousse::HashTable<T, Key, Hash>::const_iterator::operator++(int)
{
  const_iterator old = *this;
  this->increment();
  return old;
}

template<class T, class Key, class Hash>
inline typename mousse::HashTable<T, Key, Hash>::const_iterator
mousse::HashTable<T, Key, Hash>::cbegin() const
{
  return const_iterator(this);
}

template<class T, class Key, class Hash>
inline typename mousse::HashTable<T, Key, Hash>::const_iterator
mousse::HashTable<T, Key, Hash>::begin() const
{
  return this->cbegin();
}

#ifndef NoHashTableC
#ifdef NoRepository
#   include "hash_table.cpp"
#endif
#endif
#endif
