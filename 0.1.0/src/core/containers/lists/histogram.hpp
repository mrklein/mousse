#ifndef CORE_CONTAINERS_LISTS_HISTOGRAM_HPP_
#define CORE_CONTAINERS_LISTS_HISTOGRAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Histogram
// Description
//   Calculates the counts per bin of a list.
// SourceFiles
//   histogram.cpp


#include "label_list.hpp"

namespace mousse
{

template<class List>
class Histogram
{
  // Private data

    //- Counts per bin
    labelList counts_;

    //- Number of <= lowest bin
    label nLow_;

    //- Number of > highest bin
    label nHigh_;

  // Private Member Functions

    void count(const List& bins, const List& l);

public:
  // Constructors

    //- Construct given bin values and input list
    Histogram(const List& bins, const List& l);

    //- Construct given min, max, number of bins and input list
    Histogram
    (
      const typename List::const_reference min,
      const typename List::const_reference max,
      const label nBins,
      const List& l
    );

    //- Disallow default bitwise copy construct
    Histogram(const Histogram&) = delete;

    //- Disallow default bitwise assignment
    Histogram& operator=(const Histogram&) = delete;

    // Access

      //- Return the counts per bin
      inline const labelList& counts() const
      {
        return counts_;
      }

      //- Return the number of elements <= bins[0]
      //  (so inclusive lowest bin value)
      inline label nLow() const
      {
        return nLow_;
      }

      //- Return the number of elements > bins[bins.size()-1]
      //  (so exclusive highest bin value)
      inline label nHigh() const
      {
        return nHigh_;
      }
};
}  // namespace mousse
#ifdef NoRepository
#   include "histogram.cpp"
#endif
#endif
