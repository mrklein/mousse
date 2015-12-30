// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_acmi_ldu_interface_field.hpp"
#include "diag_tensor_field.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(cyclicACMILduInterfaceField, 0);
}
// Destructor 
mousse::cyclicACMILduInterfaceField::~cyclicACMILduInterfaceField()
{}
// Member Functions 
void mousse::cyclicACMILduInterfaceField::transformCoupleField
(
  scalarField& f,
  const direction cmpt
) const
{
  cyclicAMILduInterfaceField::transformCoupleField(f, cmpt);
}
