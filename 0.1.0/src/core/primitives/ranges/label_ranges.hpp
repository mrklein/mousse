#ifndef CORE_PRIMITIVES_RANGES_LABEL_RANGES_HPP_
#define CORE_PRIMITIVES_RANGES_LABEL_RANGES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::labelRanges
// Description
//   A list of labelRange.
// SourceFiles
//   label_ranges.cpp
#include "label_range.hpp"
#include "dynamic_list.hpp"
namespace mousse
{
// Forward declaration of classes
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class labelRanges;
Istream& operator>>(Istream&, labelRanges&);
Ostream& operator<<(Ostream&, const labelRanges&);
class labelRanges
:
  private DynamicList<labelRange>
{
  // Private typedefs for convenience
    typedef DynamicList<labelRange> ParentType;
  // Private Member Functions
    //- Insert range before specified insertion index, by copying up
    void insertBefore(const label, const labelRange&);
    //- Purge empty ranges, by copying down
    void purgeEmpty();
    //- Print the range for debugging purposes
    Ostream& printRange(Ostream&, const labelRange&) const;
public:
  // Constructors
    //- Construct null
    inline labelRanges();
    //- Construct given size
    inline explicit labelRanges(const label);
    //- Construct from Istream.
    labelRanges(Istream&);
  // Member Functions
    //- Clear the addressed list
    using DynamicList<labelRange>::clear;
    //- Return true if the list is empty
    using DynamicList<labelRange>::empty;
    //- Return true if the value is within any of the ranges
    inline bool contains(const label) const;
    //- Add the range to the list
    bool add(const labelRange&);
    //- Remove the range from the list
    bool remove(const labelRange&);
  // STL iterator
    //- An STL const_iterator
    class const_iterator
    {
      // Private data
        //- Reference to the list for which this is an iterator
        const labelRanges& list_;
        //- Current list index
        label index_;
        //- Index of current element at listIndex
        label subIndex_;
    public:
      // Constructors
        //- Construct null - equivalent to an 'end' position
        inline const_iterator();
        //- Construct from list, moving to its 'begin' position
        inline explicit const_iterator(const labelRanges&);
      // Member operators
        inline bool operator==(const const_iterator&) const;
        inline bool operator!=(const const_iterator&) const;
        inline label operator*();
        inline label operator()();
        inline const_iterator& operator++();
        inline const_iterator operator++(int);
    };
    //- const_iterator set to the beginning of the list
    inline const_iterator cbegin() const;
    //- const_iterator set to beyond the end of the list
    inline const const_iterator& cend() const;
    //- const_iterator set to the beginning of the list
    inline const_iterator begin() const;
    //- const_iterator set to beyond the end of the list
    inline const const_iterator& end() const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, labelRanges&);
    friend Ostream& operator<<(Ostream&, const labelRanges&);
private:
    //- const_iterator returned by end(), cend()
    static const const_iterator endIter_;
};
}  // namespace mousse

// Constructors 
inline mousse::labelRanges::labelRanges()
:
  ParentType{}
{}
inline mousse::labelRanges::labelRanges(const label nElem)
:
  ParentType{nElem}
{}
// Iterators
inline mousse::labelRanges::const_iterator::const_iterator()
:
 list_{*reinterpret_cast< mousse::labelRanges* >(0)},
 index_{-1},
 subIndex_{-1}
{}
inline mousse::labelRanges::const_iterator::const_iterator(const labelRanges& lst)
:
 list_{lst},
 index_{0},
 subIndex_{0}
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
  return this->index_ == iter.index_ && this->subIndex_ == iter.subIndex_;
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
#endif
