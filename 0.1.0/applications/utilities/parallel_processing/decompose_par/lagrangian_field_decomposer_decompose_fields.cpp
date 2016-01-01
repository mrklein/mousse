// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lagrangian_field_decomposer.hpp"
#include "ioobject_list.hpp"
// Member Functions 
template<class Type>
void mousse::lagrangianFieldDecomposer::readFields
(
  const label cloudI,
  const IOobjectList& lagrangianObjects,
  PtrList<PtrList<IOField<Type> > >& lagrangianFields
)
{
  // Search list of objects for lagrangian fields
  IOobjectList lagrangianTypeObjects
  (
    lagrangianObjects.lookupClass(IOField<Type>::typeName)
  );
  lagrangianFields.set
  (
    cloudI,
    new PtrList<IOField<Type> >
    (
      lagrangianTypeObjects.size()
    )
  );
  label lagrangianFieldi = 0;
  forAllIter(IOobjectList, lagrangianTypeObjects, iter)
  {
    lagrangianFields[cloudI].set
    (
      lagrangianFieldi++,
      new IOField<Type>(*iter())
    );
  }
}
template<class Type>
void mousse::lagrangianFieldDecomposer::readFieldFields
(
  const label cloudI,
  const IOobjectList& lagrangianObjects,
  PtrList<PtrList<CompactIOField<Field<Type>, Type> > >& lagrangianFields
)
{
  // Search list of objects for lagrangian fields
  IOobjectList lagrangianTypeObjectsA
  (
    lagrangianObjects.lookupClass(IOField<Field<Type> >::typeName)
  );
  IOobjectList lagrangianTypeObjectsB
  (
    lagrangianObjects.lookupClass
    (
      CompactIOField<Field<Type>,
      Type>::typeName
    )
  );
  lagrangianFields.set
  (
    cloudI,
    new PtrList<CompactIOField<Field<Type>, Type> >
    (
      lagrangianTypeObjectsA.size() + lagrangianTypeObjectsB.size()
    )
  );
  label lagrangianFieldi = 0;
  forAllIter(IOobjectList, lagrangianTypeObjectsA, iter)
  {
    lagrangianFields[cloudI].set
    (
      lagrangianFieldi++,
      new CompactIOField<Field<Type>, Type>(*iter())
    );
  }
  forAllIter(IOobjectList, lagrangianTypeObjectsB, iter)
  {
    lagrangianFields[cloudI].set
    (
      lagrangianFieldi++,
      new CompactIOField<Field<Type>, Type>(*iter())
    );
  }
}
template<class Type>
mousse::tmp<mousse::IOField<Type> >
mousse::lagrangianFieldDecomposer::decomposeField
(
  const word& cloudName,
  const IOField<Type>& field
) const
{
  // Create and map the internal field values
  Field<Type> procField(field, particleIndices_);
  // Create the field for the processor
  return tmp<IOField<Type> >
  (
    new IOField<Type>
    (
      IOobject
      (
        field.name(),
        procMesh_.time().timeName(),
        cloud::prefix/cloudName,
        procMesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      procField
    )
  );
}
template<class Type>
mousse::tmp<mousse::CompactIOField<mousse::Field<Type>, Type> >
mousse::lagrangianFieldDecomposer::decomposeFieldField
(
  const word& cloudName,
  const CompactIOField<Field<Type>, Type>& field
) const
{
  // Create and map the internal field values
  Field<Field<Type> > procField(field, particleIndices_);
  // Create the field for the processor
  return tmp<CompactIOField<Field<Type>, Type> >
  (
    new CompactIOField<Field<Type>, Type>
    (
      IOobject
      (
        field.name(),
        procMesh_.time().timeName(),
        cloud::prefix/cloudName,
        procMesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      procField
    )
  );
}
template<class GeoField>
void mousse::lagrangianFieldDecomposer::decomposeFields
(
  const word& cloudName,
  const PtrList<GeoField>& fields
) const
{
  if (particleIndices_.size())
  {
    forAll(fields, fieldI)
    {
      decomposeField(cloudName, fields[fieldI])().write();
    }
  }
}
template<class GeoField>
void mousse::lagrangianFieldDecomposer::decomposeFieldFields
(
  const word& cloudName,
  const PtrList<GeoField>& fields
) const
{
  if (particleIndices_.size())
  {
    forAll(fields, fieldI)
    {
      decomposeFieldField(cloudName, fields[fieldI])().write();
    }
  }
}
