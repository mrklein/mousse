// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gamg_interface.hpp"


template<class Type>
mousse::tmp<mousse::Field<Type>> mousse::GAMGInterface::interfaceInternalField
(
  const UList<Type>& iF
) const
{
  tmp<Field<Type>> tresult{new Field<Type>(size())};
  interfaceInternalField(iF, tresult());
  return tresult;
}


template<class Type>
void mousse::GAMGInterface::interfaceInternalField
(
  const UList<Type>& iF,
  List<Type>& result
) const
{
  result.setSize(size());
  FOR_ALL(result, elemI) {
    result[elemI] = iF[faceCells_[elemI]];
  }
}
