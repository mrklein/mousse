// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef map_vol_fields_hpp_
#define map_vol_fields_hpp_
#include "geometric_field.hpp"
#include "mesh_to_mesh0.hpp"
#include "ioobject_list.hpp"
namespace mousse
{
template<class Type, class CombineOp>
void MapVolFields
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
  (
    GeometricField<Type, fvPatchField, volMesh>::typeName
  );
  IOobjectList fields = objects.lookupClass(fieldClassName);
  forAllIter(IOobjectList, fields, fieldIter)
  {
    IOobject fieldTargetIOobject
    (
      fieldIter()->name(),
      meshTarget.time().timeName(),
      meshTarget,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    );
    if (fieldTargetIOobject.headerOk())
    {
      Info<< "    interpolating " << fieldIter()->name()
        << endl;
      // Read field fieldSource
      GeometricField<Type, fvPatchField, volMesh> fieldSource
      (
        *fieldIter(),
        meshSource
      );
      // Read fieldTarget
      GeometricField<Type, fvPatchField, volMesh> fieldTarget
      (
        fieldTargetIOobject,
        meshTarget
      );
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
    }
  }
}
}  // namespace mousse
#endif
