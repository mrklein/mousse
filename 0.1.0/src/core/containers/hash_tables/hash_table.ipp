#ifndef CORE_CONTAINERS_HASH_TABLES_HASH_TABLE_CPP_
#define CORE_CONTAINERS_HASH_TABLES_HASH_TABLE_CPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "hash_table.hpp"
#include "list.hpp"


// Constructors 
template<class T, class Key, class Hash>
mousse::HashTable<T, Key, Hash>::HashTable(const label size)
:
  HashTableCore{},
  nElmts_{0},
  tableSize_{HashTableCore::canonicalSize(size)},
  table_{nullptr}
{
  if (tableSize_) {
    table_ = new hashedEntry*[tableSize_];
    for (label hashIdx = 0; hashIdx < tableSize_; hashIdx++) {
      table_[hashIdx] = 0;
    }
  }
}


template<class T, class Key, class Hash>
mousse::HashTable<T, Key, Hash>::HashTable(const HashTable<T, Key, Hash>& ht)
:
  HashTableCore{},
  nElmts_{0},
  tableSize_{ht.tableSize_},
  table_{nullptr}
{
  if (tableSize_) {
    table_ = new hashedEntry*[tableSize_];
    for (label hashIdx = 0; hashIdx < tableSize_; hashIdx++) {
      table_[hashIdx] = 0;
    }
    for (const_iterator iter = ht.cbegin(); iter != ht.cend(); ++iter) {
      insert(iter.key(), *iter);
    }
  }
}


template<class T, class Key, class Hash>
mousse::HashTable<T, Key, Hash>::HashTable
(
  const Xfer<HashTable<T, Key, Hash> >& ht
)
:
  HashTableCore{},
  nElmts_{0},
  tableSize_{0},
  table_{nullptr}
{
  transfer(ht());
}


// Destructor 
template<class T, class Key, class Hash>
mousse::HashTable<T, Key, Hash>::~HashTable()
{
  if (table_) {
    clear();
    delete[] table_;
  }
}


// Member Functions 
template<class T, class Key, class Hash>
bool mousse::HashTable<T, Key, Hash>::found(const Key& key) const
{
  if (nElmts_) {
    const label hashIdx = hashKeyIndex(key);
    for (hashedEntry* ep = table_[hashIdx]; ep; ep = ep->next_) {
      if (key == ep->key_) {
        return true;
      }
    }
  }
#ifdef FULLDEBUG
  if (debug) {
    Info << "HashTable<T, Key, Hash>::found(const Key& key) : "
      << "Entry " << key << " not found in hash table\n";
  }
#endif

  return false;
}


template<class T, class Key, class Hash>
typename mousse::HashTable<T, Key, Hash>::iterator
mousse::HashTable<T, Key, Hash>::find
(
  const Key& key
)
{
  if (nElmts_) {
    const label hashIdx = hashKeyIndex(key);
    for (hashedEntry* ep = table_[hashIdx]; ep; ep = ep->next_) {
      if (key == ep->key_) {
        return iterator(this, ep, hashIdx);
      }
    }
  }
#ifdef FULLDEBUG
  if (debug) {
    Info << "HashTable<T, Key, Hash>::find(const Key& key) : "
      << "Entry " << key << " not found in hash table\n";
  }
#endif

  return iterator();
}


template<class T, class Key, class Hash>
typename mousse::HashTable<T, Key, Hash>::const_iterator
mousse::HashTable<T, Key, Hash>::find
(
  const Key& key
) const
{
  if (nElmts_) {
    const label hashIdx = hashKeyIndex(key);
    for (hashedEntry* ep = table_[hashIdx]; ep; ep = ep->next_) {
      if (key == ep->key_) {
        return const_iterator(this, ep, hashIdx);
      }
    }
  }
#ifdef FULLDEBUG
  if (debug) {
    Info << "HashTable<T, Key, Hash>::find(const Key& key) const : "
      << "Entry " << key << " not found in hash table\n";
  }
#endif

  return const_iterator();
}


template<class T, class Key, class Hash>
mousse::List<Key> mousse::HashTable<T, Key, Hash>::toc() const
{
  List<Key> keys{nElmts_};
  label keyI = 0;
  for (const_iterator iter = cbegin(); iter != cend(); ++iter) {
    keys[keyI++] = iter.key();
  }
  return keys;
}


template<class T, class Key, class Hash>
mousse::List<Key> mousse::HashTable<T, Key, Hash>::sortedToc() const
{
  List<Key> sortedLst = this->toc();
  sort(sortedLst);
  return sortedLst;
}


template<class T, class Key, class Hash>
bool mousse::HashTable<T, Key, Hash>::set
(
  const Key& key,
  const T& newEntry,
  const bool protect
)
{
  if (!tableSize_) {
    resize(2);
  }
  const label hashIdx = hashKeyIndex(key);
  hashedEntry* existing = 0;
  hashedEntry* prev = 0;
  for (hashedEntry* ep = table_[hashIdx]; ep; ep = ep->next_) {
    if (key == ep->key_) {
      existing = ep;
      break;
    }
    prev = ep;
  }
  // not found, insert it at the head
  if (!existing) {
    table_[hashIdx] = new hashedEntry(key, table_[hashIdx], newEntry);
    nElmts_++;
    if (double(nElmts_)/tableSize_ > 0.8 && tableSize_ < maxTableSize) {
#ifdef FULLDEBUG
      if (debug) {
        Info << "HashTable<T, Key, Hash>::set"
          "(const Key& key, T newEntry) : "
          "Doubling table size\n";
      }
#endif
      resize(2*tableSize_);
    }
  } else if (protect) {
    // found - but protected from overwriting
    // this corresponds to the STL 'insert' convention
#ifdef FULLDEBUG
    if (debug) {
      Info << "HashTable<T, Key, Hash>::set"
        "(const Key& key, T newEntry, true) : "
        "Cannot insert " << key << " already in hash table\n";
    }
#endif
    return false;
  } else {
    // found - overwrite existing entry
    // this corresponds to the Perl convention
    hashedEntry* ep = new hashedEntry(key, existing->next_, newEntry);
    // replace existing element - within list or insert at the head
    if (prev) {
      prev->next_ = ep;
    } else {
      table_[hashIdx] = ep;
    }
    delete existing;
  }

  return true;
}


template<class T, class Key, class Hash>
bool mousse::HashTable<T, Key, Hash>::iteratorBase::erase()
{
  // note: entryPtr_ is NULL for end(), so this catches that too
  if (entryPtr_) {
    // Search element before entryPtr_
    hashedEntry* prev = nullptr;
    for
    (
      hashedEntry* ep = hashTable_->table_[hashIndex_];
      ep;
      ep = ep->next_
    ) {
      if (ep == entryPtr_) {
        break;
      }
      prev = ep;
    }
    if (prev) {
      // has an element before entryPtr - reposition to there
      prev->next_ = entryPtr_->next_;
      delete entryPtr_;
      entryPtr_ = prev;
    } else {
      // entryPtr was first element on SLList
      hashTable_->table_[hashIndex_] = entryPtr_->next_;
      delete entryPtr_;
      // assign any non-NULL pointer value so it doesn't look
      // like end()/cend()
      entryPtr_ = reinterpret_cast<hashedEntry*>(this);
      // Mark with special hashIndex value to signal it has been rewound.
      // The next increment will bring it back to the present location.
      //
      // From the current position 'curPos', we wish to continue at
      // prevPos='curPos-1', which we mark as markPos='-curPos-1'.
      // The negative lets us notice it is special, the extra '-1'
      // is needed to avoid ambiguity for position '0'.
      // To retrieve prevPos, we would later use '-(markPos+1) - 1'
      hashIndex_ = -hashIndex_ - 1;
    }
    hashTable_->nElmts_--;
    return true;
  } else {
    return false;
  }
}


// NOTE:
// We use (const iterator&) here, but manipulate its contents anyhow.
// The parameter should be (iterator&), but then the compiler doesn't find
// it correctly and tries to call as (iterator) instead.
//
template<class T, class Key, class Hash>
bool mousse::HashTable<T, Key, Hash>::erase(const iterator& iter)
{
  // adjust iterator after erase
  return const_cast<iterator&>(iter).erase();
}


template<class T, class Key, class Hash>
bool mousse::HashTable<T, Key, Hash>::erase(const Key& key)
{
  return erase(find(key));
}


template<class T, class Key, class Hash>
mousse::label mousse::HashTable<T, Key, Hash>::erase(const UList<Key>& keys)
{
  const label nTotal = nElmts_;
  label count = 0;
  // Remove listed keys from this table - terminates early if possible
  for (label keyI = 0; count < nTotal && keyI < keys.size(); ++keyI) {
    if (erase(keys[keyI])) {
      count++;
    }
  }

  return count;
}


template<class T, class Key, class Hash>
template<class AnyType, class AnyHash>
mousse::label mousse::HashTable<T, Key, Hash>::erase
(
  const HashTable<AnyType, Key, AnyHash>& rhs
)
{
  label count = 0;
  // Remove rhs keys from this table - terminates early if possible
  // Could optimize depending on which hash is smaller ...
  for (iterator iter = begin(); iter != end(); ++iter) {
    if (rhs.found(iter.key()) && erase(iter)) {
      count++;
    }
  }
  return count;
}


template<class T, class Key, class Hash>
void mousse::HashTable<T, Key, Hash>::resize(const label sz)
{
  label newSize = HashTableCore::canonicalSize(sz);
  if (newSize == tableSize_) {
#ifdef FULLDEBUG
    if (debug) {
      Info << "HashTable<T, Key, Hash>::resize(const label) : "
        << "new table size == old table size\n";
    }
#endif
    return;
  }
  autoPtr<HashTable<T, Key, Hash>> tmpTable
  {
    new HashTable<T, Key, Hash>{newSize}
  };
  for (const_iterator iter = cbegin(); iter != cend(); ++iter) {
    tmpTable->insert(iter.key(), *iter);
  }
  label oldSize = tableSize_;
  tableSize_ = tmpTable->tableSize_;
  tmpTable->tableSize_ = oldSize;
  hashedEntry** oldTable = table_;
  table_ = tmpTable->table_;
  tmpTable->table_ = oldTable;
}


template<class T, class Key, class Hash>
void mousse::HashTable<T, Key, Hash>::clear()
{
  if (nElmts_) {
    for (label hashIdx = 0; hashIdx < tableSize_; hashIdx++) {
      if (table_[hashIdx]) {
        hashedEntry* ep = table_[hashIdx];
        while (hashedEntry* next = ep->next_) {
          delete ep;
          ep = next;
        }
        delete ep;
        table_[hashIdx] = 0;
      }
    }
    nElmts_ = 0;
  }
}


template<class T, class Key, class Hash>
void mousse::HashTable<T, Key, Hash>::clearStorage()
{
  clear();
  resize(0);
}


template<class T, class Key, class Hash>
void mousse::HashTable<T, Key, Hash>::shrink()
{
  const label newSize = HashTableCore::canonicalSize(nElmts_);
  if (newSize < tableSize_) {
    // avoid having the table disappear on us
    resize(newSize ? newSize : 2);
  }
}


template<class T, class Key, class Hash>
void mousse::HashTable<T, Key, Hash>::transfer(HashTable<T, Key, Hash>& ht)
{
  // as per the Destructor
  if (table_) {
    clear();
    delete[] table_;
  }
  tableSize_ = ht.tableSize_;
  ht.tableSize_ = 0;
  table_ = ht.table_;
  ht.table_ = nullptr;
  nElmts_ = ht.nElmts_;
  ht.nElmts_ = 0;
}


// Member Operators 
template<class T, class Key, class Hash>
void mousse::HashTable<T, Key, Hash>::operator=
(
  const HashTable<T, Key, Hash>& rhs
)
{
  // Check for assignment to self
  if (this == &rhs) {
    FATAL_ERROR_IN
    (
      "HashTable<T, Key, Hash>::operator="
      "(const HashTable<T, Key, Hash>&)"
    )
    << "attempted assignment to self"
    << abort(FatalError);
  }
  // could be zero-sized from a previous transfer()
  if (!tableSize_) {
    resize(rhs.tableSize_);
  } else {
    clear();
  }
  for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter) {
    insert(iter.key(), *iter);
  }
}


template<class T, class Key, class Hash>
bool mousse::HashTable<T, Key, Hash>::operator==
(
  const HashTable<T, Key, Hash>& rhs
) const
{
  // sizes (number of keys) must match
  if (size() != rhs.size()) {
    return false;
  }
  for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter) {
    const_iterator fnd = find(iter.key());
    if (fnd == cend() || fnd() != iter()) {
      return false;
    }
  }
  return true;
}


template<class T, class Key, class Hash>
bool mousse::HashTable<T, Key, Hash>::operator!=
(
  const HashTable<T, Key, Hash>& rhs
) const
{
  return !(operator==(rhs));
}

// Friend Operators 
#include "hash_table_io.ipp"

#endif
