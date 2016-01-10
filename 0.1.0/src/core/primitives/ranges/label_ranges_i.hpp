// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::labelRanges::labelRanges()
:
  ParentType()
{}
inline mousse::labelRanges::labelRanges(const label nElem)
:
  ParentType(nElem)
{}
// Iterators
inline mousse::labelRanges::const_iterator::const_iterator()
:
 list_(*reinterpret_cast< mousse::labelRanges* >(0)),
 index_(-1),
 subIndex_(-1)
{}
inline mousse::labelRanges::const_iterator::const_iterator(const labelRanges& lst)
:
 list_(lst),
 index_(0),
 subIndex_(0)
{
  if (list_.empty())
  {
    // equivalent to end iterator
    index_ = subIndex_ = -1;
  }
}
inline bool mousse::labelRanges::const_iterator::operator==
(
  const const_iterator& iter
) const
{
  return
  (
    this->index_ == iter.index_
  && this->subIndex_ == iter.subIndex_
  );
}
inline bool mousse::labelRanges::const_iterator::operator!=
(
  const const_iterator& iter
) const
{
  return !(this->operator==(iter));
}
inline mousse::label mousse::labelRanges::const_iterator::operator*()
{
  return list_[index_][subIndex_];
}
inline mousse::label mousse::labelRanges::const_iterator::operator()()
{
  return list_[index_][subIndex_];
}
inline mousse::labelRanges::const_iterator&
mousse::labelRanges::const_iterator::operator++()
{
  if (++subIndex_ >= list_[index_].size())
  {
    // go to next list entry
    subIndex_ = 0;
    if (++index_ >= list_.size())
    {
      // equivalent to end iterator
      index_ = subIndex_ = -1;
    }
  }
  return *this;
}
inline mousse::labelRanges::const_iterator
mousse::labelRanges::const_iterator::operator++(int)
{
  const_iterator old = *this;
  this->operator++();
  return old;
}
inline mousse::labelRanges::const_iterator mousse::labelRanges::cbegin() const
{
  return const_iterator(*this);
}
inline const mousse::labelRanges::const_iterator& mousse::labelRanges::cend() const
{
  return endIter_;
}
inline mousse::labelRanges::const_iterator mousse::labelRanges::begin() const
{
  return const_iterator(*this);
}
inline const mousse::labelRanges::const_iterator& mousse::labelRanges::end() const
{
  return endIter_;
}
// Member Functions 
inline bool mousse::labelRanges::contains(const label value) const
{
  FOR_ALL(*this, i)
  {
    if (this->ParentType::operator[](i).contains(value))
    {
      return true;
    }
  }
  return false;
}
