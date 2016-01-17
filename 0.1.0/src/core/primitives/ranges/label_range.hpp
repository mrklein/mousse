// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::labelRange
// Description
//   A label range specifier.
// SourceFiles
//   label_range.cpp
#ifndef label_range_hpp_
#define label_range_hpp_
#include "label.hpp"
namespace mousse
{
// Forward declaration of classes
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class labelRange;
Istream& operator>>(Istream&, labelRange&);
Ostream& operator<<(Ostream&, const labelRange&);
class labelRange
{
  // Private data
    label start_;
    label size_;
public:
    static int debug;
  // Public classes
    //- Less function class for sorting labelRange
    class less
    {
    public:
      bool operator()(const labelRange& a, const labelRange& b)
      {
        return a.first() < b.first();
      }
    };
  // Constructors
    //- Construct an empty range
    inline labelRange();
    //- Construct a range
    //  A negative size is autmatically changed to zero.
    inline labelRange(const label start, const label size);
    //- Construct from Istream.
    labelRange(Istream&);
  // Member Functions
    //- Reset to zero size
    inline void clear();
    //- Is the range empty?
    inline bool empty() const;
    //- Return the effective size of the range
    inline label size() const;
    //- The (inclusive) lower value of the range
    inline label first() const;
    //- The (inclusive) upper value of the range
    inline label last() const;
    //- Return true if the value is within the range
    inline bool contains(const label) const;
    //- Return true if the ranges intersect
    //  Optional test for ranges that also just touch each other
    bool intersects(const labelRange&, const bool touches = false) const;
    //- Return a joined range, squashing any gaps in between
    //  A prior intersects() check can be used to avoid squashing gaps.
    labelRange join(const labelRange&) const;
  // Member Operators
    //- Return element in range, no bounds checking
    inline label operator[](const label) const;
    //- Comparison function for sorting, compares the start
    inline bool operator<(const labelRange& rhs) const;
    //- Join ranges, squashing any gaps in between
    //  A prior intersects() check can be used to avoid squashing gaps.
    labelRange& operator+=(const labelRange&);
    inline bool operator==(const labelRange&) const;
    inline bool operator!=(const labelRange&) const;
  // STL iterator
    //- An STL const_iterator
    class const_iterator
    {
      // Private data
        //- Reference to the range for which this is an iterator
        const labelRange& range_;
        //- Current index
        label index_;
    public:
      // Constructors
        //- Construct null - equivalent to an 'end' position
        inline const_iterator();
        //- Construct from range, moving to its 'begin' position
        inline explicit const_iterator(const labelRange&);
      // Member operators
        inline bool operator==(const const_iterator&) const;
        inline bool operator!=(const const_iterator&) const;
        inline label operator*();
        inline label operator()();
        inline const_iterator& operator++();
        inline const_iterator operator++(int);
    };
    //- const_iterator set to the beginning of the range
    inline const_iterator cbegin() const;
    //- const_iterator set to beyond the end of the range
    inline const const_iterator& cend() const;
    //- const_iterator set to the beginning of the range
    inline const_iterator begin() const;
    //- const_iterator set to beyond the end of the range
    inline const const_iterator& end() const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, labelRange&);
    friend Ostream& operator<<(Ostream&, const labelRange&);
private:
    //- const_iterator returned by end(), cend()
    static const const_iterator endIter_;
};
}  // namespace mousse

// Constructors 
inline mousse::labelRange::labelRange()
:
  start_{0},
  size_{0}
{}
inline mousse::labelRange::labelRange(const label start, const label size)
:
  start_{start},
  size_{size}
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
  range_{*reinterpret_cast< mousse::labelRange* >(0)},
  index_{-1}
{}
inline mousse::labelRange::const_iterator::const_iterator(const labelRange& range)
:
  range_{range},
  index_{range_.empty() ? -1 : 0}
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
#endif
