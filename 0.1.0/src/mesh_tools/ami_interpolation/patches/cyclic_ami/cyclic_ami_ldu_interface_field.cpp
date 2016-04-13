// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_ami_ldu_interface_field.hpp"
#include "diag_tensor_field.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cyclicAMILduInterfaceField, 0);

}


// Destructor 
mousse::cyclicAMILduInterfaceField::~cyclicAMILduInterfaceField()
{}


// Member Functions 
void mousse::cyclicAMILduInterfaceField::transformCoupleField
(
  scalarField& f,
  const direction cmpt
) const
{
  if (!doTransform())
    return;
  if (forwardT().size() == 1) {
    f *= pow(diag(forwardT()[0]).component(cmpt), rank());
  } else {
    f *= pow(diag(forwardT())().component(cmpt), rank());
  }
}
