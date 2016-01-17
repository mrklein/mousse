// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef map_lagrangian_fields_hpp_
#define map_lagrangian_fields_hpp_
#include "cloud.hpp"
#include "geometric_field.hpp"
#include "mesh_to_mesh.hpp"
#include "ioobject_list.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
//- Gets the indices of (source)particles that have been appended to the
//  target cloud and maps the lagrangian fields accordingly.
template<class Type>
void MapLagrangianFields
(
  const string& cloudName,
  const IOobjectList& objects,
  const polyMesh& meshTarget,
  const labelList& addParticles
)
{
  {
    IOobjectList fields = objects.lookupClass(IOField<Type>::typeName);
    FOR_ALL_ITER(IOobjectList, fields, fieldIter)
    {
      const word& fieldName = fieldIter()->name();
      Info<< "    mapping lagrangian field " << fieldName << endl;
      // Read field (does not need mesh)
      IOField<Type> fieldSource(*fieldIter());
      // Map
      IOField<Type> fieldTarget
      (
        IOobject
        (
          fieldName,
          meshTarget.time().timeName(),
          cloud::prefix/cloudName,
          meshTarget,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        ),
        addParticles.size()
      );
      FOR_ALL(addParticles, i)
      {
        fieldTarget[i] = fieldSource[addParticles[i]];
      }
      // Write field
      fieldTarget.write();
    }
  }
  {
    IOobjectList fieldFields =
      objects.lookupClass(IOField<Field<Type> >::typeName);
    FOR_ALL_ITER(IOobjectList, fieldFields, fieldIter)
    {
      const word& fieldName = fieldIter()->name();
      Info<< "    mapping lagrangian fieldField " << fieldName << endl;
      // Read field (does not need mesh)
      IOField<Field<Type> > fieldSource(*fieldIter());
      // Map - use CompactIOField to automatically write in
      // compact form for binary format.
      CompactIOField<Field<Type>, Type> fieldTarget
      (
        IOobject
        (
          fieldName,
          meshTarget.time().timeName(),
          cloud::prefix/cloudName,
          meshTarget,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        ),
        addParticles.size()
      );
      FOR_ALL(addParticles, i)
      {
        fieldTarget[i] = fieldSource[addParticles[i]];
      }
      // Write field
      fieldTarget.write();
    }
  }
  {
    IOobjectList fieldFields =
      objects.lookupClass(CompactIOField<Field<Type>, Type>::typeName);
    FOR_ALL_ITER(IOobjectList, fieldFields, fieldIter)
    {
      Info<< "    mapping lagrangian fieldField "
        << fieldIter()->name() << endl;
      // Read field (does not need mesh)
      CompactIOField<Field<Type>, Type> fieldSource(*fieldIter());
      // Map
      CompactIOField<Field<Type>, Type> fieldTarget
      (
        IOobject
        (
          fieldIter()->name(),
          meshTarget.time().timeName(),
          cloud::prefix/cloudName,
          meshTarget,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        ),
        addParticles.size()
      );
      FOR_ALL(addParticles, i)
      {
        fieldTarget[i] = fieldSource[addParticles[i]];
      }
      // Write field
      fieldTarget.write();
    }
  }
}
}  // namespace mousse
#endif
