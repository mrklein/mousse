// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_coordinate_system_transform.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "time.hpp"
#include "transform_geometric_field.hpp"
template<class Type>
void mousse::fieldCoordinateSystemTransform::transformField
(
  const Type& field
) const
{
  const word& fieldName = field.name() + ":Transformed";
  if (!obr_.foundObject<Type>(fieldName))
  {
    obr_.store
    (
      new Type
      (
        IOobject
        (
          fieldName,
          obr_.time().timeName(),
          obr_,
          IOobject::READ_IF_PRESENT,
          IOobject::NO_WRITE
        ),
        field
      )
    );
  }
  Type& transField =
    const_cast<Type&>(obr_.lookupObject<Type>(fieldName));
  transField == field;
  dimensionedTensor R("R", field.dimensions(), coordSys_.R().R());
  mousse::transform(transField, R, transField);
  Info<< "    writing field " << transField.name() << nl << endl;
  transField.write();
}
template<class Type>
void mousse::fieldCoordinateSystemTransform::transform
(
  const word& fieldName
) const
{
  typedef GeometricField<Type, fvPatchField, volMesh> vfType;
  typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;
  if (obr_.foundObject<vfType>(fieldName))
  {
    if (debug)
    {
      Info<< type() << ": Field " << fieldName << " already in database"
        << endl;
    }
    transformField<vfType>(obr_.lookupObject<vfType>(fieldName));
  }
  else if (obr_.foundObject<sfType>(fieldName))
  {
    if (debug)
    {
      Info<< type() << ": Field " << fieldName << " already in database"
        << endl;
    }
    transformField<sfType>(obr_.lookupObject<sfType>(fieldName));
  }
  else
  {
    IOobject fieldHeader
    (
      fieldName,
      obr_.time().timeName(),
      obr_,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    );
    if
    (
      fieldHeader.headerOk()
    && fieldHeader.headerClassName() == vfType::typeName
    )
    {
      if (debug)
      {
        Info<< type() << ": Field " << fieldName << " read from file"
          << endl;
      }
      transformField<vfType>(obr_.lookupObject<vfType>(fieldName));
    }
    else if
    (
      fieldHeader.headerOk()
    && fieldHeader.headerClassName() == sfType::typeName
    )
    {
      if (debug)
      {
        Info<< type() << ": Field " << fieldName << " read from file"
          << endl;
      }
      transformField<sfType>(obr_.lookupObject<sfType>(fieldName));
    }
  }
}
