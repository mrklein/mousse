// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "itoa.hpp"


namespace mousse {

word itoa(const label n)
{
  const label offset = '0';
  const label length = 4;
  char val[length + 1];
  label leftOfN = n;
  for (label i=0; i<length; i++) {
    label j = label(leftOfN/pow(10, length - i - 1));
    leftOfN -= j*pow(10, length - i - 1);
    val[i] = offset + j;
  }
  val[length] = 0;
  return val;
}

}  // namespace mousse

