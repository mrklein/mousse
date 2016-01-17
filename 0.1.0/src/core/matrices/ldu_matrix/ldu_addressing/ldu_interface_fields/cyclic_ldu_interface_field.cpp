// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_ldu_interface_field.hpp"
#include "diag_tensor_field.hpp"

// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cyclicLduInterfaceField, 0);

}

// Destructor
mousse::cyclicLduInterfaceField::~cyclicLduInterfaceField()
{}

// Member Functions
void mousse::cyclicLduInterfaceField::transformCoupleField
(
  scalarField& f,
  const direction cmpt
) const
{
  if (doTransform())
  {
    if (forwardT().size() == 1)
    {
      f *= pow(diag(forwardT()[0]).component(cmpt), rank());
    }
    else
    {
      f *= pow(diag(forwardT())().component(cmpt), rank());
    }
  }
}
