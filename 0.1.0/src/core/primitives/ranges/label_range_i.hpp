// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::labelRange::labelRange()
:
  start_(0),
  size_(0)
{}
inline mousse::labelRange::labelRange(const label start, const label size)
:
  start_(start),
  size_(size)
{
  // disallow invalid sizes
  if (size_ <= 0)
  {
    this->clear();
  }
}
// Iterators
inline mousse::labelRange::const_iterator::const_iterator()
:
 range_(*reinterpret_cast< mousse::labelRange* >(0)),
 index_(-1)
{}
inline mousse::labelRange::const_iterator::const_iterator(const labelRange& range)
:
 range_(range),
 index_(range_.empty() ? -1 : 0)
{}
inline bool mousse::labelRange::const_iterator::operator==
(
  const const_iterator& iter
) const
{
  return (this->index_ == iter.index_);
}
inline bool mousse::labelRange::const_iterator::operator!=
(
  const const_iterator& iter
) const
{
  return !(this->operator==(iter));
}
inline mousse::label mousse::labelRange::const_iterator::operator*()
{
  return range_[index_];
}
inline mousse::label mousse::labelRange::const_iterator::operator()()
{
  return range_[index_];
}
inline mousse::labelRange::const_iterator&
mousse::labelRange::const_iterator::operator++()
{
  if (++index_ >= range_.size())
  {
    // equivalent to end iterator
    index_ = -1;
  }
  return *this;
}
inline mousse::labelRange::const_iterator
mousse::labelRange::const_iterator::operator++(int)
{
  const_iterator old = *this;
  this->operator++();
  return old;
}
inline mousse::labelRange::const_iterator mousse::labelRange::cbegin() const
{
  return const_iterator(*this);
}
inline const mousse::labelRange::const_iterator& mousse::labelRange::cend() const
{
  return endIter_;
}
inline mousse::labelRange::const_iterator mousse::labelRange::begin() const
{
  return const_iterator(*this);
}
inline const mousse::labelRange::const_iterator& mousse::labelRange::end() const
{
  return endIter_;
}
// Member Functions 
inline void mousse::labelRange::clear()
{
  start_ = size_ = 0;
}
inline bool mousse::labelRange::empty() const
{
  return !size_;
}
inline mousse::label mousse::labelRange::size() const
{
  return size_;
}
inline mousse::label mousse::labelRange::first() const
{
  return start_;
}
inline mousse::label mousse::labelRange::last() const
{
  return start_ + size_ - 1;
}
inline bool mousse::labelRange::contains(const label value) const
{
  return value >= this->first() && value <= this->last();
}
// Member Operators 
inline mousse::label mousse::labelRange::operator[](const label i) const
{
  return start_ + i;
}
inline bool mousse::labelRange::operator<(const labelRange& rhs) const
{
  return this->first() < rhs.first();
}
inline bool mousse::labelRange::operator==(const labelRange& rhs) const
{
  return start_ == rhs.start_ && size_ == rhs.size_;
}
inline bool mousse::labelRange::operator!=(const labelRange& rhs) const
{
  return !(operator==(rhs));
}
