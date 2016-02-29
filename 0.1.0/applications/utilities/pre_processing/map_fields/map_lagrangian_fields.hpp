#ifndef UTILITIES_PRE_PROCESSING_MAP_FIELDS_MAP_LAGRANGIAN_FIELDS_HPP_
#define UTILITIES_PRE_PROCESSING_MAP_FIELDS_MAP_LAGRANGIAN_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cloud.hpp"
#include "geometric_field.hpp"
#include "mesh_to_mesh0.hpp"
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
  const meshToMesh0& meshToMesh0Interp,
  const labelList& addParticles
)
{
  const fvMesh& meshTarget = meshToMesh0Interp.toMesh();
  {
    IOobjectList fields = objects.lookupClass(IOField<Type>::typeName);
    FOR_ALL_ITER(IOobjectList, fields, fieldIter)
    {
      Info<< "    mapping lagrangian field "
        << fieldIter()->name() << endl;
      // Read field (does not need mesh)
      IOField<Type> fieldSource{*fieldIter()};
      // Map
      IOField<Type> fieldTarget
      {
        // IOobject
        {
          fieldIter()->name(),
          meshTarget.time().timeName(),
          cloud::prefix/cloudName,
          meshTarget,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        addParticles.size()
      };
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
      objects.lookupClass(IOField<Field<Type>>::typeName);
    FOR_ALL_ITER(IOobjectList, fieldFields, fieldIter)
    {
      Info<< "    mapping lagrangian fieldField "
        << fieldIter()->name() << endl;
      // Read field (does not need mesh)
      IOField<Field<Type>> fieldSource{*fieldIter()};
      // Map - use CompactIOField to automatically write in
      // compact form for binary format.
      CompactIOField<Field<Type>, Type> fieldTarget
      {
        // IOobject
        {
          fieldIter()->name(),
          meshTarget.time().timeName(),
          cloud::prefix/cloudName,
          meshTarget,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        addParticles.size()
      };
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
      CompactIOField<Field<Type>, Type> fieldSource{*fieldIter()};
      // Map
      CompactIOField<Field<Type>, Type> fieldTarget
      {
        // IOobject
        {
          fieldIter()->name(),
          meshTarget.time().timeName(),
          cloud::prefix/cloudName,
          meshTarget,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        addParticles.size()
      };
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
