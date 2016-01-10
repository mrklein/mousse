// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "istream.hpp"
#include "token.hpp"
#include <climits>

template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::max_bits()
{
  return sizeof(StorageType)*CHAR_BIT - 1;
}
template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::max_value()
{
  return (1u << nBits) - 1;
}
template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::packing()
{
  return sizeof(StorageType)*CHAR_BIT / nBits;
}
template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::maskLower(unsigned offset)
{
  // return (1u << (nBits * offset)) - 1;
  // The next one works more reliably with overflows
  // eg, when compiled without optimization
  return (~0u >> ( sizeof(StorageType)*CHAR_BIT - nBits * offset));
}
template<unsigned nBits>
inline mousse::label mousse::PackedList<nBits>::packedLength(const label nElem)
{
  return (nElem + packing() - 1) / packing();
}
namespace mousse
{
  // Template specialization for bool entries
  template<>
  inline unsigned int mousse::PackedList<1>::readValue(Istream& is)
  {
    return readBool(is);
  }
  // Template specialization for bool entries
  template<>
  inline void mousse::PackedList<1>::setPair(Istream& is)
  {
    set(readLabel(is), true);
  }
  // Template specialization for bool entries
  template<>
  inline bool mousse::PackedList<1>::iteratorBase::writeIfSet(Ostream& os) const
  {
    if (this->get())
    {
      os  << index_;
      return true;
    }
    else
    {
      return false;
    }
  }
}
template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::readValue(Istream& is)
{
  const unsigned int val = readLabel(is);
  if (val > max_value())
  {
    FATAL_IO_ERROR_IN
    (
      "PackedList<nBits>::readValue(Istream&)",
      is
    )
    << "Out-of-range value " << val << " for PackedList<" << nBits
    << ">. Maximum permitted value is " << max_value() << "."
    << exit(FatalIOError);
  }
  return val;
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::setPair(Istream& is)
{
  is.readBegin("Tuple2<label, unsigned int>");
  const label ind = readLabel(is);
  const unsigned int val = readLabel(is);
  is.readEnd("Tuple2<label, unsigned int>");
  if (val > max_value())
  {
    FATAL_IO_ERROR_IN
    (
      "PackedList<nBits>::setPair(Istream&)",
      is
    )
    << "Out-of-range value " << val << " for PackedList<" << nBits
    << "> at index " << ind
    << ". Maximum permitted value is " << max_value() << "."
    << exit(FatalIOError);
  }
  set(ind, val);
  // Check state of Istream
  is.check("PackedList<nBits>::setPair(Istream&)");
}
template<unsigned nBits>
inline bool mousse::PackedList<nBits>::iteratorBase::writeIfSet(Ostream& os) const
{
  const label val = this->get();
  if (val)
  {
    os  << token::BEGIN_LIST
      << index_ << token::SPACE << val
      << token::END_LIST;
    return true;
  }
  else
  {
    return false;
  }
}
// Constructors 
template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList()
:
  PackedListCore(),
  StorageList(),
  size_(0)
{}
template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(const label size)
:
  PackedListCore(),
  StorageList(packedLength(size), 0u),
  size_(size)
{}
template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList
(
  const label size,
  const unsigned int val
)
:
  PackedListCore(),
  StorageList(packedLength(size), 0u),
  size_(size)
{
  if (val)
  {
    operator=(val);
  }
}
template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(Istream& is)
:
  PackedListCore(),
  StorageList(),
  size_(0)
{
  read(is);
}
template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(const PackedList<nBits>& lst)
:
  PackedListCore(),
  StorageList(lst),
  size_(lst.size_)
{}
template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(const Xfer<PackedList<nBits> >& lst)
{
  transfer(lst());
}
template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(const labelUList& lst)
:
  PackedListCore(),
  StorageList(packedLength(lst.size()), 0u),
  size_(lst.size())
{
  FOR_ALL(lst, i)
  {
    set(i, lst[i]);
  }
}
template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(const UIndirectList<label>& lst)
:
  PackedListCore(),
  StorageList(packedLength(lst.size()), 0u),
  size_(lst.size())
{
  FOR_ALL(lst, i)
  {
    set(i, lst[i]);
  }
}
template<unsigned nBits>
inline mousse::autoPtr<mousse::PackedList<nBits> >
mousse::PackedList<nBits>::clone() const
{
  return autoPtr<PackedList<nBits> >(new PackedList<nBits>(*this));
}
// Iterators
// iteratorBase
template<unsigned nBits>
inline mousse::PackedList<nBits>::iteratorBase::iteratorBase()
:
  list_(0),
  index_(0)
{}
template<unsigned nBits>
inline mousse::PackedList<nBits>::iteratorBase::iteratorBase
(
  const PackedList<nBits>* lst,
  const label i
)
:
  list_(const_cast<PackedList<nBits>*>(lst)),
  index_(i)
{}
template<unsigned nBits>
inline unsigned int
mousse::PackedList<nBits>::iteratorBase::get() const
{
  const unsigned int seg = index_ / packing();
  const unsigned int off = index_ % packing();
  const unsigned int& stored = list_->StorageList::operator[](seg);
  return (stored >> (nBits * off)) & max_value();
}
template<unsigned nBits>
inline bool
mousse::PackedList<nBits>::iteratorBase::set(const unsigned int val)
{
  const unsigned int seg = index_ / packing();
  const unsigned int off = index_ % packing();
  const unsigned int startBit = nBits * off;
  const unsigned int mask = max_value() << startBit;
  unsigned int& stored = list_->StorageList::operator[](seg);
  const unsigned int prev = stored;
  if (val >= max_value())
  {
    // overflow is max_value, fill everything
    stored |= mask;
  }
  else
  {
    stored &= ~mask;
    stored |= mask & (val << startBit);
  }
  return prev != stored;
}
template<unsigned nBits>
inline mousse::label mousse::PackedList<nBits>::iteratorBase::key() const
{
  return index_;
}
template<unsigned nBits>
inline bool mousse::PackedList<nBits>::iteratorBase::operator==
(
  const iteratorBase& iter
) const
{
  return this->get() == iter.get();
}
template<unsigned nBits>
inline bool mousse::PackedList<nBits>::iteratorBase::operator!=
(
  const iteratorBase& iter
) const
{
  return this->get() != iter.get();
}
template<unsigned nBits>
inline unsigned int
mousse::PackedList<nBits>::iteratorBase::operator=(const iteratorBase& iter)
{
  const unsigned int val = iter.get();
  this->set(val);
  return val;
}
template<unsigned nBits>
inline unsigned int
mousse::PackedList<nBits>::iteratorBase::operator=(const unsigned int val)
{
  // lazy evaluation - increase size on assigment
  if (index_ >= list_->size_)
  {
    list_->resize(index_ + 1);
  }
  this->set(val);
  return val;
}
template<unsigned nBits>
inline mousse::PackedList<nBits>::iteratorBase::operator
unsigned int () const
{
  // lazy evaluation - return 0 for out-of-range
  if (index_ >= list_->size_)
  {
    return 0;
  }
  return this->get();
}
// const_iterator, iterator
template<unsigned nBits>
inline mousse::PackedList<nBits>::iterator::iterator()
:
  iteratorBase()
{}
template<unsigned nBits>
inline mousse::PackedList<nBits>::const_iterator::const_iterator()
:
  iteratorBase()
{}
template<unsigned nBits>
inline mousse::PackedList<nBits>::iterator::iterator
(
  const iteratorBase& iter
)
:
  iteratorBase(iter)
{
  // avoid going past end()
  // eg, iter = iterator(list, Inf)
  if (this->index_ > this->list_->size_)
  {
    this->index_ = this->list_->size_;
  }
}
template<unsigned nBits>
inline mousse::PackedList<nBits>::const_iterator::const_iterator
(
  const iteratorBase& iter
)
:
  iteratorBase(iter)
{
  // avoid going past end()
  // eg, iter = iterator(list, Inf)
  if (this->index_ > this->list_->size_)
  {
    this->index_ = this->list_->size_;
  }
}
template<unsigned nBits>
inline mousse::PackedList<nBits>::iterator::iterator
(
  const PackedList<nBits>* lst,
  const label i
)
:
  iteratorBase(lst, i)
{}
template<unsigned nBits>
inline mousse::PackedList<nBits>::const_iterator::const_iterator
(
  const PackedList<nBits>* lst,
  const label i
)
:
  iteratorBase(lst, i)
{}
template<unsigned nBits>
inline mousse::PackedList<nBits>::const_iterator::const_iterator
(
  const iterator& iter
)
:
  iteratorBase(static_cast<const iteratorBase&>(iter))
{}
template<unsigned nBits>
inline bool mousse::PackedList<nBits>::iterator::operator==
(
  const iteratorBase& iter
) const
{
  return this->index_ == iter.index_;
}
template<unsigned nBits>
inline bool mousse::PackedList<nBits>::iterator::operator!=
(
  const iteratorBase& iter
) const
{
  return this->index_ != iter.index_;
}
template<unsigned nBits>
inline bool mousse::PackedList<nBits>::const_iterator::operator==
(
  const iteratorBase& iter
) const
{
  return this->index_ == iter.index_;
}
template<unsigned nBits>
inline bool mousse::PackedList<nBits>::const_iterator::operator!=
(
  const iteratorBase& iter
) const
{
  return this->index_ != iter.index_;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator&
mousse::PackedList<nBits>::iterator::operator=(const iteratorBase& iter)
{
  this->list_  = iter.list_;
  this->index_ = iter.index_;
  // avoid going past end()
  // eg, iter = iterator(list, Inf)
  if (this->index_ > this->list_->size_)
  {
    this->index_ = this->list_->size_;
  }
  return *this;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator&
mousse::PackedList<nBits>::const_iterator::operator=(const iteratorBase& iter)
{
  this->list_  = iter.list_;
  this->index_ = iter.index_;
  // avoid going past end()
  // eg, iter = iterator(list, Inf)
  if (this->index_ > this->list_->size_)
  {
    this->index_ = this->list_->size_;
  }
  return *this;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator&
mousse::PackedList<nBits>::iterator::operator++()
{
  ++this->index_;
  return *this;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator&
mousse::PackedList<nBits>::const_iterator::operator++()
{
  ++this->index_;
  return *this;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator
mousse::PackedList<nBits>::iterator::operator++(int)
{
  iterator old = *this;
  ++this->index_;
  return old;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::const_iterator::operator++(int)
{
  const_iterator old = *this;
  ++this->index_;
  return old;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator&
mousse::PackedList<nBits>::iterator::operator--()
{
  --this->index_;
  return *this;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator&
mousse::PackedList<nBits>::const_iterator::operator--()
{
  --this->index_;
  return *this;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator
mousse::PackedList<nBits>::iterator::operator--(int)
{
  iterator old = *this;
  --this->index_;
  return old;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::const_iterator::operator--(int)
{
  const_iterator old = *this;
  --this->index_;
  return old;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iteratorBase&
mousse::PackedList<nBits>::iterator::operator*()
{
  return static_cast<iteratorBase&>(*this);
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iteratorBase&
mousse::PackedList<nBits>::iterator::operator()()
{
  return static_cast<iteratorBase&>(*this);
}
template<unsigned nBits>
inline unsigned int
mousse::PackedList<nBits>::const_iterator::operator*() const
{
  return this->get();
}
template<unsigned nBits>
inline unsigned int
mousse::PackedList<nBits>::const_iterator::operator()() const
{
  return this->get();
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator
mousse::PackedList<nBits>::begin()
{
  return iterator(this, 0);
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::begin() const
{
  return const_iterator(this, 0);
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::cbegin() const
{
  return const_iterator(this, 0);
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator
mousse::PackedList<nBits>::end()
{
  return iterator(this, size_);
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::end() const
{
  return const_iterator(this, size_);
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::cend() const
{
  return const_iterator(this, size_);
}
// Member Functions 
template<unsigned nBits>
inline mousse::label mousse::PackedList<nBits>::size() const
{
  return size_;
}
template<unsigned nBits>
inline bool mousse::PackedList<nBits>::empty() const
{
  return !size_;
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::resize
(
  const label newSize,
  const unsigned int& val
)
{
  reserve(newSize);
  const label oldSize = size_;
  size_ = newSize;
  if (size_ > oldSize)
  {
    // fill new elements or newly exposed elements
    if (val)
    {
      // fill value for complete segments
      unsigned int fill = val;
      if (val >= max_value())
      {
        // fill everything
        fill = maskLower(packing());
      }
      else
      {
        for (unsigned int i = 1; i < packing(); ++i)
        {
          fill |= (fill << nBits);
        }
      }
      // fill in complete segments
      const label oldLen = packedLength(oldSize);
      const label newLen = packedLength(size_);
      for (label i=oldLen; i < newLen; ++i)
      {
        StorageList::operator[](i) = fill;
      }
      // finish previous partial segment, preserve existing value
      {
        const unsigned int off = oldSize % packing();
        if (off)
        {
          const unsigned int seg = oldSize / packing();
          const unsigned int mask = maskLower(off);
          StorageList::operator[](seg) &= mask;
          StorageList::operator[](seg) |= ~mask & fill;
        }
      }
      // mask off the (new) final partial segment
      {
        const unsigned int off = size_ % packing();
        if (off)
        {
          const unsigned int seg = size_ / packing();
          StorageList::operator[](seg) &= maskLower(off);
        }
      }
    }
  }
  else if (size_ < oldSize)
  {
    // resize shrinking
    // - clear newly exposed elements
    // fill in complete segments
    const label oldLen = packedLength(oldSize);
    const label newLen = packedLength(size_);
    for (label i=newLen; i < oldLen; ++i)
    {
      StorageList::operator[](i) = 0u;
    }
    // mask off the final partial segment
    {
      const unsigned int off = size_ % packing();
      if (off)
      {
        const unsigned int seg = size_ / packing();
        StorageList::operator[](seg) &= maskLower(off);
      }
    }
  }
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::setSize
(
  const label newSize,
  const unsigned int& val
)
{
  resize(newSize, val);
}
template<unsigned nBits>
inline mousse::label mousse::PackedList<nBits>::capacity() const
{
  return packing() * StorageList::size();
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::setCapacity(const label nElem)
{
  StorageList::setSize(packedLength(nElem), 0u);
  // truncate addressed size too
  if (size_ > nElem)
  {
    size_ = nElem;
    // mask off the final partial segment
    const unsigned int off = size_ % packing();
    if (off)
    {
      const unsigned int seg = size_ / packing();
      StorageList::operator[](seg) &= maskLower(off);
    }
  }
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::reserve(const label nElem)
{
  const label len = packedLength(nElem);
  // need more capacity?
  if (len > StorageList::size())
  {
    // Like DynamicList with SizeInc=0, SizeMult=2, SizeDiv=1
    StorageList::setSize
    (
      max
      (
        len,
        StorageList::size()*2
      ),
      0u
    );
  }
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::reset()
{
  StorageList::operator=(0u);
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::clear()
{
  reset();
  size_ = 0;
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::clearStorage()
{
  StorageList::clear();
  size_ = 0;
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::shrink()
{
  // any uneed space allocated?
  const label len = packedLength();
  if (len < StorageList::size())
  {
    StorageList::setSize(len);
  }
}
template<unsigned nBits>
inline mousse::List<unsigned int>& mousse::PackedList<nBits>::storage()
{
  return static_cast<StorageList&>(*this);
}
template<unsigned nBits>
inline const mousse::List<unsigned int>& mousse::PackedList<nBits>::storage() const
{
  return static_cast<const StorageList&>(*this);
}
template<unsigned nBits>
inline mousse::label mousse::PackedList<nBits>::packedLength() const
{
  return packedLength(size_);
}
template<unsigned nBits>
inline std::streamsize mousse::PackedList<nBits>::byteSize() const
{
  return packedLength() * sizeof(StorageType);
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::transfer(PackedList<nBits>& lst)
{
  size_ = lst.size_;
  lst.size_ = 0;
  StorageList::transfer(lst);
}
template<unsigned nBits>
inline mousse::Xfer<mousse::PackedList<nBits> > mousse::PackedList<nBits>::xfer()
{
  return xferMove(*this);
}
template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::get(const label i) const
{
  // lazy evaluation - return 0 for out-of-range
  if (i < 0 || i >= size_)
  {
    return 0;
  }
  else
  {
    return iteratorBase(this, i).get();
  }
}
template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::operator[](const label i) const
{
  // lazy evaluation - return 0 for out-of-range
  if (i < 0 || i >= size_)
  {
    return 0;
  }
  else
  {
    return iteratorBase(this, i).get();
  }
}
template<unsigned nBits>
inline bool mousse::PackedList<nBits>::set
(
  const label i,
  const unsigned int val
)
{
  if (i < 0)
  {
    // lazy evaluation - ignore out-of-bounds
    return false;
  }
  else if (i >= size_)
  {
    // lazy evaluation - increase size on assigment
    resize(i + 1);
  }
  return iteratorBase(this, i).set(val);
}
template<unsigned nBits>
inline bool mousse::PackedList<nBits>::unset(const label i)
{
  // lazy evaluation - ignore out-of-bounds
  if (i < 0 || i >= size_)
  {
    return false;
  }
  else
  {
    return iteratorBase(this, i).set(0u);
  }
}
template<unsigned nBits>
inline mousse::PackedList<nBits>&
mousse::PackedList<nBits>::append(const unsigned int val)
{
  const label elemI = size_;
  reserve(elemI + 1);
  size_++;
  iteratorBase(this, elemI).set(val);
  return *this;
}
template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::remove()
{
  if (!size_)
  {
    FATAL_ERROR_IN
    (
      "mousse::PackedList<nBits>::remove()"
    )   << "List is empty" << abort(FatalError);
  }
  label elemI = size_ - 1;
  const unsigned int val = iteratorBase(this, elemI).get();
  resize(elemI);
  return val;
}
template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iteratorBase
mousse::PackedList<nBits>::operator[](const label i)
{
  return iteratorBase(this, i);
}
template<unsigned nBits>
inline mousse::PackedList<nBits>&
mousse::PackedList<nBits>::operator=(const unsigned int val)
{
  const label packLen = packedLength();
  if (val && size_)
  {
    unsigned int fill = val;
    if (val >= max_value())
    {
      // fill everything
      fill = maskLower(packing());
    }
    else
    {
      for (unsigned int i = 1; i < packing(); ++i)
      {
        fill |= (fill << nBits);
      }
    }
    for (label i=0; i < packLen; ++i)
    {
      StorageList::operator[](i) = fill;
    }
    // mask off the final partial segment
    {
      const unsigned int off = size_ % packing();
      if (off)
      {
        const unsigned int seg = size_ / packing();
        StorageList::operator[](seg) &= maskLower(off);
      }
    }
  }
  else
  {
    for (label i=0; i < packLen; ++i)
    {
      StorageList::operator[](i) = 0u;
    }
  }
  return *this;
}
