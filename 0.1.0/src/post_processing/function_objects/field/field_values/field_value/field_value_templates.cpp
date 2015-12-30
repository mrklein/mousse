// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_value.hpp"
#include "list_list_ops.hpp"
#include "pstream.hpp"
// Member Functions 
template<class Type>
void mousse::fieldValue::combineFields(Field<Type>& field)
{
  List<Field<Type> > allValues(Pstream::nProcs());
  allValues[Pstream::myProcNo()] = field;
  Pstream::gatherList(allValues);
  if (Pstream::master())
  {
    field =
      ListListOps::combine<Field<Type> >
      (
        allValues,
        accessOp<Field<Type> >()
      );
  }
}
template<class Type>
void mousse::fieldValue::combineFields(tmp<Field<Type> >& field)
{
  combineFields(field());
}
