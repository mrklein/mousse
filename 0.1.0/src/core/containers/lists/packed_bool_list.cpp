// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "packed_bool_list.hpp"
#include "iostreams.hpp"


// Private Member Functions 
bool mousse::PackedBoolList::bitorPrepare
(
  const PackedList<1>& lst,
  label& maxPackLen
)
{
  const StorageList& lhs = this->storage();
  const StorageList& rhs = lst.storage();
  const label packLen1 = this->packedLength();
  const label packLen2 = lst.packedLength();
  // check how the lists interact and if bit trimming is needed
  bool needTrim = false;
  maxPackLen = packLen1;
  if (packLen1 == packLen2) {
    // identical packed lengths - only resize if absolutely necessary
    if (this->size() != lst.size() && maxPackLen
        && rhs[maxPackLen-1] > lhs[maxPackLen-1]) {
      // second list has a higher bit set
      // extend addressable area and use trim
      resize(lst.size());
      needTrim = true;
    }
  } else if (packLen2 < packLen1) {
    // second list is shorter, this limits the or
    maxPackLen = packLen2;
  } else {
    // second list is longer, find the highest bit set
    for (label storeI = packLen1; storeI < packLen2; ++storeI) {
      if (rhs[storeI]) {
        maxPackLen = storeI+1;
      }
    }
    // the upper limit moved - resize for full coverage and trim later
    if (maxPackLen > packLen1) {
      resize(maxPackLen * packing());
      needTrim = true;
    }
  }
  return needTrim;
}


template<class LabelListType>
mousse::label mousse::PackedBoolList::setIndices(const LabelListType& indices)
{
  // no better information, just guess something about the size
  reserve(indices.size());
  label cnt = 0;
  FOR_ALL(indices, elemI) {
    if (set(indices[elemI])) {
      ++cnt;
    }
  }
  return cnt;
}


template<class LabelListType>
mousse::label mousse::PackedBoolList::unsetIndices(const LabelListType& indices)
{
  label cnt = 0;
  FOR_ALL(indices, elemI) {
    if (unset(indices[elemI])) {
      ++cnt;
    }
  }
  return cnt;
}


template<class LabelListType>
mousse::label mousse::PackedBoolList::subsetIndices(const LabelListType& indices)
{
  // handle trivial case
  if (empty() || indices.empty()) {
    clear();
    return 0;
  }
  // normal case
  PackedBoolList anded;
  anded.reserve(size());
  label cnt = 0;
  FOR_ALL(indices, elemI) {
    const label& index = indices[elemI];
    if (operator[](index)) {
      anded.set(index);
      ++cnt;
    }
  }
  transfer(anded);
  return cnt;
}


// Constructors 
mousse::PackedBoolList::PackedBoolList(Istream& is)
:
  PackedList<1>{}
{
  is  >> *this;
}


// Member Functions 
void mousse::PackedBoolList::set(const PackedList<1>& lst)
{
  // extend addressable area if needed, return maximum size possible
  label len = 0;
  const bool needTrim = bitorPrepare(lst, len);
  // operate directly with the underlying storage
  StorageList& lhs = this->storage();
  const StorageList& rhs = lst.storage();
  for (label i=0; i < len; ++i) {
    lhs[i] |= rhs[i];
  }
  if (needTrim) {
    trim();
  }
}


mousse::label mousse::PackedBoolList::set(const labelUList& indices)
{
  return setIndices(indices);
}


mousse::label mousse::PackedBoolList::set(const UIndirectList<label>& indices)
{
  return setIndices(indices);
}


void mousse::PackedBoolList::unset(const PackedList<1>& lst)
{
  // operate directly with the underlying storage
  StorageList& lhs = this->storage();
  const StorageList& rhs = lst.storage();
  // overlapping storage size
  const label len = min(this->packedLength(), lst.packedLength());
  for (label i=0; i < len; ++i) {
    lhs[i] &= ~rhs[i];
  }
}


mousse::label mousse::PackedBoolList::unset(const labelUList& indices)
{
  return unsetIndices(indices);
}


mousse::label mousse::PackedBoolList::unset(const UIndirectList<label>& indices)
{
  return unsetIndices(indices);
}


void mousse::PackedBoolList::subset(const PackedList<1>& lst)
{
  // shrink addressable area if needed
  if (this->size() > lst.size()) {
    this->resize(lst.size());
  }
  // operate directly with the underlying storage
  StorageList& lhs = this->storage();
  const StorageList& rhs = lst.storage();
  const label len = this->packedLength();
  for (label i=0; i < len; ++i) {
    lhs[i] &= rhs[i];
  }
}


mousse::label mousse::PackedBoolList::subset(const labelUList& indices)
{
  return subsetIndices(indices);
}


mousse::label mousse::PackedBoolList::subset(const UIndirectList<label>& indices)
{
  return subsetIndices(indices);
}


mousse::Xfer<mousse::labelList> mousse::PackedBoolList::used() const
{
  labelList lst{this->count()};
  if (lst.size()) {
    label nElem = 0;
    FOR_ALL(*this, elemI) {
      if (get(elemI)) {
        lst[nElem++] = elemI;
      }
    }
    lst.setSize(nElem);
  }
  return lst.xfer();
}


// Member Operators 
mousse::PackedBoolList&
mousse::PackedBoolList::operator=(const mousse::UList<bool>& lst)
{
  this->setSize(lst.size());
  // overwrite with new true/false values
  FOR_ALL(*this, elemI) {
    set(elemI, lst[elemI]);
  }
  return *this;
}


mousse::PackedBoolList&
mousse::PackedBoolList::operator^=(const PackedList<1>& lst)
{
  // extend addressable area if needed, return maximum size possible
  label len = 0;
  const bool needTrim = bitorPrepare(lst, len);
  // operate directly with the underlying storage
  StorageList& lhs = this->storage();
  const StorageList& rhs = lst.storage();
  for (label i=0; i < len; ++i) {
    lhs[i] ^= rhs[i];
  }
  if (needTrim) {
    trim();
  }
  return *this;
}


//  Global Operators
mousse::PackedBoolList mousse::operator&
(
  const PackedBoolList& lst1,
  const PackedBoolList& lst2
)
{
  PackedBoolList result{lst1};
  result &= lst2;
  // trim to bits actually used
  result.trim();
  return result;
}


mousse::PackedBoolList mousse::operator^
(
  const PackedBoolList& lst1,
  const PackedBoolList& lst2
)
{
  PackedBoolList result{lst1};
  result ^= lst2;
  // trim to bits actually used
  result.trim();
  return result;
}


mousse::PackedBoolList mousse::operator|
(
  const PackedBoolList& lst1,
  const PackedBoolList& lst2
)
{
  PackedBoolList result{lst1};
  result |= lst2;
  return result;
}

