// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fft
// Description
//   Fast fourier transform derived from the Numerical
//   Recipes in C routine.
//   The complex transform field is returned in the field supplied.  The
//   direction of transform is supplied as an argument (1 = forward, -1 =
//   reverse).  The dimensionality and organisation of the array of values
//   in space is supplied in the nn indexing array.
// SourceFiles
//   fft.cpp
#ifndef fft_hpp_
#define fft_hpp_
#include "complex_fields.hpp"
#include "label_list.hpp"
namespace mousse
{
class fft
{
public:
  enum transformDirection
  {
    FORWARD_TRANSFORM = 1,
    REVERSE_TRANSFORM = -1
  };
  static void transform
  (
    complexField& field,
    const labelList& nn,
    transformDirection fftDirection
  );
  static tmp<complexField> forwardTransform
  (
    const tmp<complexField>& field,
    const labelList& nn
  );
  static tmp<complexField> reverseTransform
  (
    const tmp<complexField>& field,
    const labelList& nn
  );
  static tmp<complexVectorField> forwardTransform
  (
    const tmp<complexVectorField>& field,
    const labelList& nn
  );
  static tmp<complexVectorField> reverseTransform
  (
    const tmp<complexVectorField>& field,
    const labelList& nn
  );
};
}  // namespace mousse
#endif
