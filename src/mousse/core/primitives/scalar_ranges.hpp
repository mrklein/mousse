#ifndef CORE_PRIMITIVES_RANGES_SCALAR_RANGES_HPP_
#define CORE_PRIMITIVES_RANGES_SCALAR_RANGES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::scalarRanges
// Description
//   A List of scalarRange.
// SourceFiles
//   scalar_ranges.cpp

#include "scalar_range.hpp"
#include "list.hpp"


namespace mousse {

class scalarRanges
:
  public List<scalarRange>
{
public:
  // Constructors
    //- Construct Null
    scalarRanges();
    //- Construct from Istream.
    //  The list items are comma-delimited.
    scalarRanges(Istream&);
  // Member Functions
    //- Return true if the given value is within the ranges
    bool selected(const scalar) const;
    //- Return the set of selected entries in the given list
    //  that are within the ranges
    List<bool> selected(const List<scalar>&) const;
    //- Select a list of values that are within the ranges
    List<scalar> select(const List<scalar>&) const;
    //- Select a list of values that are within the ranges
    void inplaceSelect(List<scalar>&) const;
};

}  // namespace mousse

#endif
