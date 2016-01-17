// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fft_renumber.hpp"
namespace mousse
{
// recursively evaluate the indexing necessary to do the folding of
// the fft data. We recurse until we have the indexing ncessary for
// the folding in all directions.
void fftRenumberRecurse
(
  List<complex>& data,
  List<complex>& renumData,
  const labelList& nn,
  label nnprod,
  label ii,
  label l1,
  label l2
)
{
  if (ii == nn.size())
  {
    // we've worked out the renumbering scheme. Now copy
    // the components across
    data[l1] = complex(renumData[l2].Re(),renumData[l2].Im());
  }
  else
  {
    // do another level of folding. First work out the
    // multiplicative value of the index
    nnprod /= nn[ii];
    label i_1(0);
    for (label i=0; i<nn[ii]; i++)
    {
      // now evaluate the indices (both from array 1 and to
      // array 2). These get multiplied by nnprod to (cumulatively)
      // find the real position in the list corresponding to
      // this set of indices.
      if (i<nn[ii]/2)
      {
        i_1 = i + nn[ii]/2;
      }
      else
      {
        i_1 = i - nn[ii]/2;
      }
      // go to the next level of recursion.
      fftRenumberRecurse
      (
        data,
        renumData,
        nn,
        nnprod,
        ii+1,
        l1+i*nnprod,
        l2+i_1*nnprod
      );
    }
  }
}
// fftRenumber : fold the n-d data array to get the fft components in
// the right places.
#include "fft_renumber.hpp"
void fftRenumber
(
  List<complex>& data,
  const labelList& nn
)
{
  List<complex> renumData(data);
  label nnprod(1);
  FOR_ALL(nn, i)
  {
    nnprod *= nn[i];
  }
  label ii(0), l1(0), l2(0);
  fftRenumberRecurse
  (
    data,
    renumData,
    nn,
    nnprod,
    ii,
    l1,
    l2
  );
}
}  // namespace mousse
