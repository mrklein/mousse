// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iofield.hpp"
#include "compact_io_field.hpp"
#include "time.hpp"
// Global Functions 
template<class Type>
mousse::tmp<mousse::IOField<Type> > mousse::reconstructLagrangianField
(
  const word& cloudName,
  const polyMesh& mesh,
  const PtrList<fvMesh>& meshes,
  const word& fieldName
)
{
  // Construct empty field on mesh
  tmp<IOField<Type> > tfield
  (
    new IOField<Type>
    (
      IOobject
      (
        fieldName,
        mesh.time().timeName(),
        cloud::prefix/cloudName,
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      Field<Type>(0)
    )
  );
  Field<Type>& field = tfield();
  forAll(meshes, i)
  {
    // Check object on local mesh
    IOobject localIOobject
    (
      fieldName,
      meshes[i].time().timeName(),
      cloud::prefix/cloudName,
      meshes[i],
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    );
    if (localIOobject.headerOk())
    {
      IOField<Type> fieldi(localIOobject);
      label offset = field.size();
      field.setSize(offset + fieldi.size());
      forAll(fieldi, j)
      {
        field[offset + j] = fieldi[j];
      }
    }
  }
  return tfield;
}
template<class Type>
mousse::tmp<mousse::CompactIOField<mousse::Field<Type>, Type> >
mousse::reconstructLagrangianFieldField
(
  const word& cloudName,
  const polyMesh& mesh,
  const PtrList<fvMesh>& meshes,
  const word& fieldName
)
{
  // Construct empty field on mesh
  tmp<CompactIOField<Field<Type>, Type > > tfield
  (
    new CompactIOField<Field<Type>, Type>
    (
      IOobject
      (
        fieldName,
        mesh.time().timeName(),
        cloud::prefix/cloudName,
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      Field<Field<Type> >(0)
    )
  );
  Field<Field<Type> >& field = tfield();
  forAll(meshes, i)
  {
    // Check object on local mesh
    IOobject localIOobject
    (
      fieldName,
      meshes[i].time().timeName(),
      cloud::prefix/cloudName,
      meshes[i],
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    );
    if (localIOobject.headerOk())
    {
      CompactIOField<Field<Type>, Type> fieldi(localIOobject);
      label offset = field.size();
      field.setSize(offset + fieldi.size());
      forAll(fieldi, j)
      {
        field[offset + j] = fieldi[j];
      }
    }
  }
  return tfield;
}
template<class Type>
void mousse::reconstructLagrangianFields
(
  const word& cloudName,
  const polyMesh& mesh,
  const PtrList<fvMesh>& meshes,
  const IOobjectList& objects,
  const HashSet<word>& selectedFields
)
{
  const word fieldClassName(IOField<Type>::typeName);
  IOobjectList fields = objects.lookupClass(fieldClassName);
  if (fields.size())
  {
    Info<< "    Reconstructing lagrangian "
      << fieldClassName << "s\n" << endl;
    forAllConstIter(IOobjectList, fields, fieldIter)
    {
      if
      (
        selectedFields.empty()
      || selectedFields.found(fieldIter()->name())
      )
      {
        Info<< "        " << fieldIter()->name() << endl;
        reconstructLagrangianField<Type>
        (
          cloudName,
          mesh,
          meshes,
          fieldIter()->name()
        )().write();
      }
    }
    Info<< endl;
  }
}
template<class Type>
void mousse::reconstructLagrangianFieldFields
(
  const word& cloudName,
  const polyMesh& mesh,
  const PtrList<fvMesh>& meshes,
  const IOobjectList& objects,
  const HashSet<word>& selectedFields
)
{
  {
    const word fieldClassName(CompactIOField<Field<Type>, Type>::typeName);
    IOobjectList fields = objects.lookupClass(fieldClassName);
    if (fields.size())
    {
      Info<< "    Reconstructing lagrangian "
        << fieldClassName << "s\n" << endl;
      forAllConstIter(IOobjectList, fields, fieldIter)
      {
        if
        (
          selectedFields.empty()
        || selectedFields.found(fieldIter()->name())
        )
        {
          Info<< "        " << fieldIter()->name() << endl;
          reconstructLagrangianFieldField<Type>
          (
            cloudName,
            mesh,
            meshes,
            fieldIter()->name()
          )().write();
        }
      }
      Info<< endl;
    }
  }
  {
    const word fieldClassName(IOField<Field<Type> >::typeName);
    IOobjectList fields = objects.lookupClass(fieldClassName);
    if (fields.size())
    {
      Info<< "    Reconstructing lagrangian "
        << fieldClassName << "s\n" << endl;
      forAllConstIter(IOobjectList, fields, fieldIter)
      {
        if
        (
          selectedFields.empty()
        || selectedFields.found(fieldIter()->name())
        )
        {
          Info<< "        " << fieldIter()->name() << endl;
          reconstructLagrangianFieldField<Type>
          (
            cloudName,
            mesh,
            meshes,
            fieldIter()->name()
          )().write();
        }
      }
      Info<< endl;
    }
  }
}
