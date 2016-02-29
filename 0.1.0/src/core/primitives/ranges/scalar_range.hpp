#ifndef CORE_PRIMITIVES_RANGES_SCALAR_RANGE_HPP_
#define CORE_PRIMITIVES_RANGES_SCALAR_RANGE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::scalarRange
// Description
//   A scalar range specifier.
//   The range selector can be specified as an "LOWER:UPPER" range, as a
//   "LOWER:" bound, as an ":UPPER" bound or simply as an "EXACT" value.
//   The read constructor uses a colon (:) as a range marker and a comma (,)
//   to delimit the next possible range selector.
// SourceFiles
//   scalar_range.cpp
#include "scalar.hpp"
namespace mousse
{
// Forward declaration of classes
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class scalarRange;
Istream& operator>>(Istream&, scalarRange&);
Ostream& operator<<(Ostream&, const scalarRange&);
class scalarRange
{
  //- Enumeration defining the types of token
  enum rangeType
  {
    EMPTY = 0,
    EXACT,
    LOWER,
    UPPER,
    RANGE
  };
  // Private data
    enum rangeType type_;
    scalar value_;
    scalar value2_;
public:
    static int debug;
  // Constructors
    //- Construct an empty range
    scalarRange();
    //- Construct a range from lower to upper
    scalarRange(const scalar lower, const scalar upper);
    //- Construct from Istream.
    //  Since commas can be used as list delimiters,
    //  leading and trailing commas are ignored.
    scalarRange(Istream&);
  // Member Functions
    //- Is the range empty?
    bool empty() const;
    //- Is the range non-empty?
    bool valid() const;
    //- Is the range 'EXACT'?
    bool isExact() const;
    //- The value constituting an 'EXACT' match
    //  or the values for 'UPPER' or 'LOWER' limits
    scalar value() const;
    //- The lower limit
    scalar lower() const;
    //- The upper limit
    scalar upper() const;
    //- Return true if the value is within the range
    bool selected(const scalar) const;
  // Member Operators
    bool operator==(const scalarRange&) const;
    bool operator!=(const scalarRange&) const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, scalarRange&);
    friend Ostream& operator<<(Ostream&, const scalarRange&);
};
}  // namespace mousse
#endif
