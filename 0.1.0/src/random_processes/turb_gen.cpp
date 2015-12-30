// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fft.hpp"
#include "turb_gen.hpp"
#include "kmesh.hpp"
#include "primitive_fields.hpp"
#include "ek.hpp"
#include "mathematical_constants.hpp"
// Constructors 
mousse::turbGen::turbGen(const Kmesh& k, const scalar EA, const scalar K0)
:
  K(k),
  Ea(EA),
  k0(K0),
  RanGen(label(0))
{}
// Member Functions 
mousse::vectorField mousse::turbGen::U()
{
  vectorField s(K.size());
  scalarField rndPhases(K.size());
  forAll(K, i)
  {
    s[i] = RanGen.vector01();
    rndPhases[i] = RanGen.scalar01();
  }
  s = K ^ s;
  s = s/(mag(s) + 1.0e-20);
  s = Ek(Ea, k0, mag(K))*s;
  complexVectorField up
  (
    fft::reverseTransform
    (
      ComplexField(cos(constant::mathematical::twoPi*rndPhases)*s,
      sin(constant::mathematical::twoPi*rndPhases)*s),
      K.nn()
    )
  );
  return ReImSum(up);
}
