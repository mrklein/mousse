// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar_ranges.hpp"
#include "dynamic_list.hpp"
#include "list_ops.hpp"
#include "istream.hpp"

// Constructors 
mousse::scalarRanges::scalarRanges()
:
  List<scalarRange>(0)
{}
mousse::scalarRanges::scalarRanges(Istream& is)
:
  List<scalarRange>(0)
{
  DynamicList<scalarRange> lst;
  while (is.good())
  {
    scalarRange sr(is);
    if (sr.valid())
    {
      lst.append(sr);
    }
  }
  transfer(lst);
}
// Member Functions 
bool mousse::scalarRanges::selected(const scalar value) const
{
  FOR_ALL(*this, i)
  {
    if (operator[](i).selected(value))
    {
      return true;
    }
  }
  return false;
}
mousse::List<bool> mousse::scalarRanges::selected
(
  const List<scalar>& values
) const
{
  List<bool> lst(values.size(), false);
  // check ranges
  FOR_ALL(values, i)
  {
    if (selected(values[i]))
    {
      lst[i] = true;
    }
  }
  // check specific values
  FOR_ALL(*this, rangeI)
  {
    if (operator[](rangeI).isExact())
    {
      scalar target = operator[](rangeI).value();
      int nearestIndex = -1;
      scalar nearestDiff = mousse::GREAT;
      FOR_ALL(values, timeIndex)
      {
        scalar diff = fabs(values[timeIndex] - target);
        if (diff < nearestDiff)
        {
          nearestDiff = diff;
          nearestIndex = timeIndex;
        }
      }
      if (nearestIndex >= 0)
      {
        lst[nearestIndex] = true;
      }
    }
  }
  return lst;
}
mousse::List<mousse::scalar> mousse::scalarRanges::select
(
  const List<scalar>& values
) const
{
  return subset(selected(values), values);
}
void mousse::scalarRanges::inplaceSelect
(
  List<scalar>& values
) const
{
  inplaceSubset(selected(values), values);
}
