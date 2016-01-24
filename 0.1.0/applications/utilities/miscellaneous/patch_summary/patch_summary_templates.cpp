// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_summary_templates.hpp"
#include "iomanip.hpp"
template<class GeoField>
void mousse::addToFieldList
(
  PtrList<GeoField>& fieldList,
  const IOobject& obj,
  const label fieldI,
  const typename GeoField::Mesh& mesh
)
{
  if (obj.headerClassName() == GeoField::typeName)
  {
    fieldList.set
    (
      fieldI,
      new GeoField{obj, mesh}
    );
    Info<< "    " << GeoField::typeName << tab << obj.name() << endl;
  }
}
template<class GeoField>
void mousse::outputFieldList
(
  const PtrList<GeoField>& fieldList,
  const label patchI
)
{
  FOR_ALL(fieldList, fieldI)
  {
    if (fieldList.set(fieldI))
    {
      Info<< "    " << pTraits<typename GeoField::value_type>::typeName
        << tab << tab
        << fieldList[fieldI].name() << tab << tab
        << fieldList[fieldI].boundaryField()[patchI].type() << nl;
    }
  }
}
template<class GeoField>
void mousse::collectFieldList
(
  const PtrList<GeoField>& fieldList,
  const label patchI,
  HashTable<word>& fieldToType
)
{
  FOR_ALL(fieldList, fieldI)
  {
    if (fieldList.set(fieldI))
    {
      fieldToType.insert
      (
        fieldList[fieldI].name(),
        fieldList[fieldI].boundaryField()[patchI].type()
      );
    }
  }
}
