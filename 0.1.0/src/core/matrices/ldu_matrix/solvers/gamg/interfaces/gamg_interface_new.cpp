// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gamg_interface.hpp"
#include "gamg_agglomeration.hpp"
#include "ldu_matrix.hpp"
// Selectors
mousse::autoPtr<mousse::GAMGInterface> mousse::GAMGInterface::New
(
  const label index,
  const lduInterfacePtrsList& coarseInterfaces,
  const lduInterface& fineInterface,
  const labelField& localRestrictAddressing,
  const labelField& neighbourRestrictAddressing,
  const label fineLevelIndex,
  const label coarseComm
)
{
  const word coupleType(fineInterface.type());
  lduInterfaceConstructorTable::iterator cstrIter =
    lduInterfaceConstructorTablePtr_->find(coupleType);
  if (cstrIter == lduInterfaceConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "GAMGInterface::New"
      "(const lduInterface& fineInterface, "
      "const labelField& localRestrictAddressing, "
      "const labelField& neighbourRestrictAddressing)"
    )   << "Unknown GAMGInterface type " << coupleType << ".\n"
      << "Valid GAMGInterface types are :"
      << lduInterfaceConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<GAMGInterface>
  (
    cstrIter()
    (
      index,
      coarseInterfaces,
      fineInterface,
      localRestrictAddressing,
      neighbourRestrictAddressing,
      fineLevelIndex,
      coarseComm
    )
  );
}
mousse::autoPtr<mousse::GAMGInterface> mousse::GAMGInterface::New
(
  const word& coupleType,
  const label index,
  const lduInterfacePtrsList& coarseInterfaces,
  Istream& is
)
{
  IstreamConstructorTable::iterator cstrIter =
    IstreamConstructorTablePtr_->find(coupleType);
  if (cstrIter == IstreamConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "GAMGInterface::New"
      "(const word&, const label, const lduInterfacePtrsList&, Istream&)"
    )   << "Unknown GAMGInterface type " << coupleType << ".\n"
      << "Valid GAMGInterface types are :"
      << IstreamConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<GAMGInterface>(cstrIter()(index, coarseInterfaces, is));
}
