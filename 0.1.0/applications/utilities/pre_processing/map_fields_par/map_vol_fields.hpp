// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef MapConsistentVolFields_H
#define MapConsistentVolFields_H
#include "geometric_field.hpp"
#include "mesh_to_mesh.hpp"
#include "ioobject_list.hpp"
namespace mousse
{
template<class Type, class CombineOp>
void MapVolFields
(
  const IOobjectList& objects,
  const HashSet<word>& selectedFields,
  const meshToMesh& interp,
  const CombineOp& cop
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  const fvMesh& meshSource = static_cast<const fvMesh&>(interp.srcRegion());
  const fvMesh& meshTarget = static_cast<const fvMesh&>(interp.tgtRegion());
  IOobjectList fields = objects.lookupClass(fieldType::typeName);
  FOR_ALL_ITER(IOobjectList, fields, fieldIter)
  {
    const word& fieldName = fieldIter()->name();
    if (selectedFields.empty() || selectedFields.found(fieldName))
    {
      Info<< "    interpolating " << fieldName << endl;
      const fieldType fieldSource(*fieldIter(), meshSource);
      IOobject targetIO
      (
        fieldName,
        meshTarget.time().timeName(),
        meshTarget,
        IOobject::MUST_READ
      );
      if (targetIO.headerOk())
      {
        fieldType fieldTarget(targetIO, meshTarget);
        interp.mapSrcToTgt(fieldSource, cop, fieldTarget);
        fieldTarget.write();
      }
      else
      {
        targetIO.readOpt() = IOobject::NO_READ;
        tmp<fieldType>
          tfieldTarget(interp.mapSrcToTgt(fieldSource, cop));
        fieldType fieldTarget(targetIO, tfieldTarget);
        fieldTarget.write();
      }
    }
  }
}
}  // namespace mousse
#endif
