// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "histogram.hpp"
#include "list_ops.hpp"

// Private Member Functions 
template<class List>
void mousse::Histogram<List>::count(const List& bins, const List& l)
{
  if (bins.size() < 2)
  {
    FATAL_ERROR_IN("Histogram<List>::count(const List&, const List&)")
      << "Should have at least two values in bins. Now:" << bins
      << exit(FatalError);
  }
  counts_.setSize(bins.size()-1);
  counts_ = 0;
  nLow_ = 0;
  nHigh_ = 0;
  FOR_ALL(l, i)
  {
    label index = findLower(bins, l[i]);
    if (index == -1)
    {
      nLow_++;
    }
    else if (index == bins.size()-1)
    {
      nHigh_++;
    }
    else
    {
      counts_[index]++;
    }
  }
}


// Constructors 
template<class List>
mousse::Histogram<List>::Histogram(const List& bins, const List& l)
{
  count(bins, l);
}


template<class List>
mousse::Histogram<List>::Histogram
(
  const typename List::const_reference min,
  const typename List::const_reference max,
  const label nBins,
  const List& l
)
{
  List bins{nBins+1};
  typename List::value_type span = (max-min)/nBins;
  bins[0] = min;
  for (label i = 1; i < nBins; i++)
  {
    bins[i] = bins[i-1] + span;
  }
  // Set max directly to avoid truncation errors.
  bins[nBins] = max;
  count(bins, l);
}
