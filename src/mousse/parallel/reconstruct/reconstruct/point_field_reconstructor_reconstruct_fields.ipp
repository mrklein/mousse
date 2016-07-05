// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_field_reconstructor.hpp"


// Member Functions 
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::pointPatchField, mousse::pointMesh>>
mousse::pointFieldReconstructor::reconstructField(const IOobject& fieldIoObject)
{
  // Read the field for all the processors
  PtrList<GeometricField<Type, pointPatchField, pointMesh>> procFields
  {
    procMeshes_.size()
  };
  FOR_ALL(procMeshes_, proci) {
    procFields.set
    (
      proci,
      new GeometricField<Type, pointPatchField, pointMesh>
      {
        IOobject
        {
          fieldIoObject.name(),
          procMeshes_[proci]().time().timeName(),
          procMeshes_[proci](),
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        },
        procMeshes_[proci]
      }
    );
  }
  // Create the internalField
  Field<Type> internalField{mesh_.size()};
  // Create the patch fields
  PtrList<pointPatchField<Type>> patchFields{mesh_.boundary().size()};
  FOR_ALL(procMeshes_, proci) {
    const GeometricField<Type, pointPatchField, pointMesh>&
      procField = procFields[proci];
    // Get processor-to-global addressing for use in rmap
    const labelList& procToGlobalAddr = pointProcAddressing_[proci];
    // Set the cell values in the reconstructed field
    internalField.rmap(procField.internalField(), procToGlobalAddr);
    // Set the boundary patch values in the reconstructed field
    FOR_ALL(boundaryProcAddressing_[proci], patchi) {
      // Get patch index of the original patch
      const label curBPatch = boundaryProcAddressing_[proci][patchi];
      // check if the boundary patch is not a processor patch
      if (curBPatch >= 0) {
        if (!patchFields(curBPatch)) {
          patchFields.set
            (
              curBPatch,
              pointPatchField<Type>::New
              (
                procField.boundaryField()[patchi],
                mesh_.boundary()[curBPatch],
                DimensionedField<Type, pointMesh>::null(),
                pointPatchFieldReconstructor(mesh_.boundary()[curBPatch].size())
              )
            );
        }
        patchFields[curBPatch].rmap(procField.boundaryField()[patchi],
                                    patchPointAddressing_[proci][patchi]);
      }
    }
  }
  // Construct and write the field
  // setting the internalField and patchFields
  return
    tmp<GeometricField<Type, pointPatchField, pointMesh>>
    {
      new GeometricField<Type, pointPatchField, pointMesh>
      {
        {
          fieldIoObject.name(),
          mesh_().time().timeName(),
          mesh_(),
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        mesh_,
        procFields[0].dimensions(),
        internalField,
        patchFields
      }
    };
}


// Reconstruct and write all point fields
template<class Type>
void mousse::pointFieldReconstructor::reconstructFields
(
  const IOobjectList& objects,
  const HashSet<word>& selectedFields
)
{
  word fieldClassName
  {
    GeometricField<Type, pointPatchField, pointMesh>::typeName
  };
  IOobjectList fields = objects.lookupClass(fieldClassName);
  if (fields.size()) {
    Info << "    Reconstructing " << fieldClassName << "s\n" << endl;
    FOR_ALL_CONST_ITER(IOobjectList, fields, fieldIter) {
      if (!selectedFields.size() || selectedFields.found(fieldIter()->name())) {
        Info << "        " << fieldIter()->name() << endl;
        reconstructField<Type>(*fieldIter())().write();
      }
    }
    Info << endl;
  }
}

