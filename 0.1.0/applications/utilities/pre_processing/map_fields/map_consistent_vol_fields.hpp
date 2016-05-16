#ifndef UTILITIES_PRE_PROCESSING_MAP_FIELDS_MAP_CONSISTENT_VOL_FIELDS_HPP_
#define UTILITIES_PRE_PROCESSING_MAP_FIELDS_MAP_CONSISTENT_VOL_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "geometric_field.hpp"
#include "mesh_to_mesh0.hpp"
#include "ioobject_list.hpp"


namespace mousse {

template<class Type, class CombineOp>
void MapConsistentVolFields
(
  const IOobjectList& objects,
  const meshToMesh0& meshToMesh0Interp,
  const meshToMesh0::order& mapOrder,
  const CombineOp& cop
)
{
  const fvMesh& meshSource = meshToMesh0Interp.fromMesh();
  const fvMesh& meshTarget = meshToMesh0Interp.toMesh();
  word fieldClassName
  {
    GeometricField<Type, fvPatchField, volMesh>::typeName
  };
  IOobjectList fields = objects.lookupClass(fieldClassName);
  FOR_ALL_ITER(IOobjectList, fields, fieldIter) {
    Info << "    interpolating " << fieldIter()->name()
      << endl;
    // Read field
    GeometricField<Type, fvPatchField, volMesh> fieldSource
    {
      *fieldIter(),
      meshSource
    };
    IOobject fieldTargetIOobject
    {
      fieldIter()->name(),
      meshTarget.time().timeName(),
      meshTarget,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    };
    if (fieldTargetIOobject.headerOk()) {
      // Read fieldTarget
      GeometricField<Type, fvPatchField, volMesh> fieldTarget
      {
        fieldTargetIOobject,
        meshTarget
      };
      // Interpolate field
      meshToMesh0Interp.interpolate
      (
        fieldTarget,
        fieldSource,
        mapOrder,
        cop
      );
      // Write field
      fieldTarget.write();
    } else {
      fieldTargetIOobject.readOpt() = IOobject::NO_READ;
      // Interpolate field
      GeometricField<Type, fvPatchField, volMesh> fieldTarget
      {
        fieldTargetIOobject,
        meshToMesh0Interp.interpolate
        (
          fieldSource,
          mapOrder,
          cop
        )
      };
      // Write field
      fieldTarget.write();
    }
  }
}

}  // namespace mousse

#endif

