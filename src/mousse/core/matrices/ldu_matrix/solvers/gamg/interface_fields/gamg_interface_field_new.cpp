// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gamg_interface_field.hpp"


// Selectors
mousse::autoPtr<mousse::GAMGInterfaceField> mousse::GAMGInterfaceField::New
(
  const GAMGInterface& GAMGCp,
  const lduInterfaceField& fineInterface
)
{
  const word coupleType{fineInterface.interfaceFieldType()};
  lduInterfaceFieldConstructorTable::iterator cstrIter =
    lduInterfaceFieldConstructorTablePtr_->find(coupleType);
  if (cstrIter == lduInterfaceFieldConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "GAMGInterfaceField::New"
      "(const GAMGInterface& GAMGCp, "
      "const lduInterfaceField& fineInterface)"
    )
    << "Unknown GAMGInterfaceField type "
    << coupleType << nl
    << "Valid GAMGInterfaceField types are :"
    << lduInterfaceFieldConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<GAMGInterfaceField>{cstrIter()(GAMGCp, fineInterface)};
}


mousse::autoPtr<mousse::GAMGInterfaceField> mousse::GAMGInterfaceField::New
(
  const GAMGInterface& GAMGCp,
  const bool doTransform,
  const int rank
)
{
  const word coupleType{GAMGCp.type()};
  lduInterfaceConstructorTable::iterator cstrIter =
    lduInterfaceConstructorTablePtr_->find(coupleType);
  if (cstrIter == lduInterfaceConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "GAMGInterfaceField::New"
      "(const word&, const GAMGInterface&, const bool, const int)"
    )
    << "Unknown GAMGInterfaceField type "
    << coupleType << nl
    << "Valid GAMGInterfaceField types are :"
    << lduInterfaceConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<GAMGInterfaceField>{cstrIter()(GAMGCp, doTransform, rank)};
}
