// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_field_reconstructor.hpp"
#include "time.hpp"
#include "ptr_list.hpp"
#include "fv_patch_fields.hpp"
#include "empty_fv_patch.hpp"
#include "empty_fv_patch_field.hpp"
#include "empty_fvs_patch_field.hpp"


// Member Functions 
template<class Type>
mousse::tmp<mousse::DimensionedField<Type, mousse::volMesh>>
mousse::fvFieldReconstructor::reconstructFvVolumeInternalField
(
  const IOobject& fieldIoObject,
  const PtrList<DimensionedField<Type, volMesh>>& procFields
) const
{
  // Create the internalField
  Field<Type> internalField{mesh_.nCells()};
  FOR_ALL(procMeshes_, procI) {
    const DimensionedField<Type, volMesh>& procField = procFields[procI];
    // Set the cell values in the reconstructed field
    internalField.rmap(procField.field(), cellProcAddressing_[procI]);
  }
  return
    tmp<DimensionedField<Type, volMesh>>
    {
      new DimensionedField<Type, volMesh>
      {
        fieldIoObject,
          mesh_,
          procFields[0].dimensions(),
          internalField
      }
    };
}


template<class Type>
mousse::tmp<mousse::DimensionedField<Type, mousse::volMesh>>
mousse::fvFieldReconstructor::reconstructFvVolumeInternalField
(
  const IOobject& fieldIoObject
) const
{
  // Read the field for all the processors
  PtrList<DimensionedField<Type, volMesh>> procFields{procMeshes_.size()};
  FOR_ALL(procMeshes_, procI) {
    procFields.set
    (
      procI,
      new DimensionedField<Type, volMesh>
      {
        {
          fieldIoObject.name(),
          procMeshes_[procI].time().timeName(),
          procMeshes_[procI],
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        },
        procMeshes_[procI]
      }
    );
  }
  return
    reconstructFvVolumeInternalField
    (
      IOobject
      {
        fieldIoObject.name(),
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      procFields
    );
}


template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh>>
mousse::fvFieldReconstructor::reconstructFvVolumeField
(
  const IOobject& fieldIoObject,
  const PtrList<GeometricField<Type, fvPatchField, volMesh>>& procFields
) const
{
  // Create the internalField
  Field<Type> internalField{mesh_.nCells()};
  // Create the patch fields
  PtrList<fvPatchField<Type>> patchFields{mesh_.boundary().size()};
  FOR_ALL(procFields, procI) {
    const GeometricField<Type, fvPatchField, volMesh>& procField =
      procFields[procI];
    // Set the cell values in the reconstructed field
    internalField.rmap(procField.internalField(), cellProcAddressing_[procI]);
    // Set the boundary patch values in the reconstructed field
    FOR_ALL(boundaryProcAddressing_[procI], patchI) {
      // Get patch index of the original patch
      const label curBPatch = boundaryProcAddressing_[procI][patchI];
      // Get addressing slice for this patch
      const labelList::subList cp =
        procField.mesh().boundary()[patchI].patchSlice
        (
          faceProcAddressing_[procI]
        );
      // check if the boundary patch is not a processor patch
      if (curBPatch >= 0) {
        // Regular patch. Fast looping
        if (!patchFields(curBPatch)) {
          patchFields.set
            (
              curBPatch,
              fvPatchField<Type>::New
              (
                procField.boundaryField()[patchI],
                mesh_.boundary()[curBPatch],
                DimensionedField<Type, volMesh>::null(),
                fvPatchFieldReconstructor
                (
                  mesh_.boundary()[curBPatch].size()
                )
              )
            );
        }
        const label curPatchStart = mesh_.boundaryMesh()[curBPatch].start();
        labelList reverseAddressing{cp.size()};
        FOR_ALL(cp, faceI) {
          // Check
          if (cp[faceI] <= 0) {
            FATAL_ERROR_IN
            (
              "fvFieldReconstructor::reconstructFvVolumeField\n"
              "(\n"
              "    const IOobject&,\n"
              "    const PtrList<GeometricField<Type,"
              " fvPatchField, volMesh>>&\n"
              ") const\n"
            )
            << "Processor " << procI
            << " patch "
            << procField.mesh().boundary()[patchI].name()
            << " face " << faceI
            << " originates from reversed face since "
            << cp[faceI]
            << exit(FatalError);
          }
          // Subtract one to take into account offsets for
          // face direction.
          reverseAddressing[faceI] = cp[faceI] - 1 - curPatchStart;
        }
        patchFields[curBPatch].rmap(procField.boundaryField()[patchI],
                                    reverseAddressing);
      } else {
        const Field<Type>& curProcPatch = procField.boundaryField()[patchI];
        // In processor patches, there's a mix of internal faces (some
        // of them turned) and possible cyclics. Slow loop
        FOR_ALL(cp, faceI) {
          // Subtract one to take into account offsets for
          // face direction.
          label curF = cp[faceI] - 1;
          // Is the face on the boundary?
          if (curF >= mesh_.nInternalFaces()) {
            label curBPatch = mesh_.boundaryMesh().whichPatch(curF);
            if (!patchFields(curBPatch)) {
              patchFields.set
                (
                  curBPatch,
                  fvPatchField<Type>::New
                  (
                    mesh_.boundary()[curBPatch].type(),
                    mesh_.boundary()[curBPatch],
                    DimensionedField<Type, volMesh>::null()
                  )
                );
            }
            // add the face
            label curPatchFace =
              mesh_.boundaryMesh()[curBPatch].whichFace(curF);
            patchFields[curBPatch][curPatchFace] = curProcPatch[faceI];
          }
        }
      }
    }
  }
  FOR_ALL(mesh_.boundary(), patchI) {
    // add empty patches
    if (isType<emptyFvPatch>(mesh_.boundary()[patchI]) && !patchFields(patchI)) {
      patchFields.set
        (
          patchI,
          fvPatchField<Type>::New
          (
            emptyFvPatchField<Type>::typeName,
            mesh_.boundary()[patchI],
            DimensionedField<Type, volMesh>::null()
          )
        );
    }
  }
  // Now construct and write the field
  // setting the internalField and patchFields
  return
    tmp<GeometricField<Type, fvPatchField, volMesh>>
    {
      new GeometricField<Type, fvPatchField, volMesh>
      {
        fieldIoObject,
        mesh_,
        procFields[0].dimensions(),
        internalField,
        patchFields
      }
    };
}


template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh>>
mousse::fvFieldReconstructor::reconstructFvVolumeField
(
  const IOobject& fieldIoObject
) const
{
  // Read the field for all the processors
  PtrList<GeometricField<Type, fvPatchField, volMesh>> procFields
  {
    procMeshes_.size()
  };
  FOR_ALL(procMeshes_, procI) {
    procFields.set
      (
        procI,
        new GeometricField<Type, fvPatchField, volMesh>
        {
          {
            fieldIoObject.name(),
            procMeshes_[procI].time().timeName(),
            procMeshes_[procI],
            IOobject::MUST_READ,
            IOobject::NO_WRITE
          },
          procMeshes_[procI]
        }
      );
  }
  return
    reconstructFvVolumeField
    (
      IOobject
      {
        fieldIoObject.name(),
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      procFields
    );
}


template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvsPatchField, mousse::surfaceMesh>>
mousse::fvFieldReconstructor::reconstructFvSurfaceField
(
  const IOobject& fieldIoObject,
  const PtrList<GeometricField<Type, fvsPatchField, surfaceMesh>>& procFields
) const
{
  // Create the internalField
  Field<Type> internalField{mesh_.nInternalFaces()};
  // Create the patch fields
  PtrList<fvsPatchField<Type>> patchFields{mesh_.boundary().size()};
  FOR_ALL(procMeshes_, procI) {
    const GeometricField<Type, fvsPatchField, surfaceMesh>& procField =
      procFields[procI];
    // Set the face values in the reconstructed field
    // It is necessary to create a copy of the addressing array to
    // take care of the face direction offset trick.
    //
    {
      const labelList& faceMap = faceProcAddressing_[procI];
      // Correctly oriented copy of internal field
      Field<Type> procInternalField{procField.internalField()};
      // Addressing into original field
      labelList curAddr{procInternalField.size()};
      FOR_ALL(procInternalField, addrI) {
        curAddr[addrI] = mag(faceMap[addrI])-1;
        if (faceMap[addrI] < 0) {
          procInternalField[addrI] = -procInternalField[addrI];
        }
      }
      // Map
      internalField.rmap(procInternalField, curAddr);
    }
    // Set the boundary patch values in the reconstructed field
    FOR_ALL(boundaryProcAddressing_[procI], patchI) {
      // Get patch index of the original patch
      const label curBPatch = boundaryProcAddressing_[procI][patchI];
      // Get addressing slice for this patch
      const labelList::subList cp =
        procMeshes_[procI].boundary()[patchI].patchSlice
        (
          faceProcAddressing_[procI]
        );
      // check if the boundary patch is not a processor patch
      if (curBPatch >= 0) {
        // Regular patch. Fast looping
        if (!patchFields(curBPatch)) {
          patchFields.set
            (
              curBPatch,
              fvsPatchField<Type>::New
              (
                procField.boundaryField()[patchI],
                mesh_.boundary()[curBPatch],
                DimensionedField<Type, surfaceMesh>::null(),
                fvPatchFieldReconstructor
                (
                  mesh_.boundary()[curBPatch].size()
                )
              )
            );
        }
        const label curPatchStart = mesh_.boundaryMesh()[curBPatch].start();
        labelList reverseAddressing{cp.size()};
        FOR_ALL(cp, faceI) {
          // Subtract one to take into account offsets for
          // face direction.
          reverseAddressing[faceI] = cp[faceI] - 1 - curPatchStart;
        }
        patchFields[curBPatch].rmap
          (
            procField.boundaryField()[patchI],
            reverseAddressing
          );
      } else {
        const Field<Type>& curProcPatch = procField.boundaryField()[patchI];
        // In processor patches, there's a mix of internal faces (some
        // of them turned) and possible cyclics. Slow loop
        FOR_ALL(cp, faceI) {
          label curF = cp[faceI] - 1;
          // Is the face turned the right side round
          if (curF >= 0) {
            // Is the face on the boundary?
            if (curF >= mesh_.nInternalFaces()) {
              label curBPatch = mesh_.boundaryMesh().whichPatch(curF);
              if (!patchFields(curBPatch)) {
                patchFields.set
                  (
                    curBPatch,
                    fvsPatchField<Type>::New
                    (
                      mesh_.boundary()[curBPatch].type(),
                      mesh_.boundary()[curBPatch],
                      DimensionedField<Type, surfaceMesh>
                      ::null()
                    )
                  );
              }
              // add the face
              label curPatchFace =
                mesh_.boundaryMesh()[curBPatch].whichFace(curF);
              patchFields[curBPatch][curPatchFace] =
                curProcPatch[faceI];
            } else {
              // Internal face
              internalField[curF] = curProcPatch[faceI];
            }
          }
        }
      }
    }
  }
  FOR_ALL(mesh_.boundary(), patchI) {
    // add empty patches
    if (isType<emptyFvPatch>(mesh_.boundary()[patchI]) && !patchFields(patchI)) {
      patchFields.set
        (
          patchI,
          fvsPatchField<Type>::New
          (
            emptyFvsPatchField<Type>::typeName,
            mesh_.boundary()[patchI],
            DimensionedField<Type, surfaceMesh>::null()
          )
        );
    }
  }
  // Now construct and write the field
  // setting the internalField and patchFields
  return
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
    {
      new GeometricField<Type, fvsPatchField, surfaceMesh>
      {
        fieldIoObject,
        mesh_,
        procFields[0].dimensions(),
        internalField,
        patchFields
      }
    };
}


template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvsPatchField, mousse::surfaceMesh>>
mousse::fvFieldReconstructor::reconstructFvSurfaceField
(
  const IOobject& fieldIoObject
) const
{
  // Read the field for all the processors
  PtrList<GeometricField<Type, fvsPatchField, surfaceMesh>> procFields
  {
    procMeshes_.size()
  };
  FOR_ALL(procMeshes_, procI) {
    procFields.set
      (
        procI,
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        {
          IOobject
          {
            fieldIoObject.name(),
            procMeshes_[procI].time().timeName(),
            procMeshes_[procI],
            IOobject::MUST_READ,
            IOobject::NO_WRITE
          },
          procMeshes_[procI]
        }
      );
  }
  return
    reconstructFvSurfaceField
    (
      IOobject
      {
        fieldIoObject.name(),
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      procFields
    );
}


template<class Type>
void mousse::fvFieldReconstructor::reconstructFvVolumeInternalFields
(
  const IOobjectList& objects,
  const HashSet<word>& selectedFields
)
{
  const word& fieldClassName = DimensionedField<Type, volMesh>::typeName;
  IOobjectList fields = objects.lookupClass(fieldClassName);
  if (fields.size()) {
    Info<< "    Reconstructing " << fieldClassName << "s\n" << endl;
    FOR_ALL_CONST_ITER(IOobjectList, fields, fieldIter) {
      if (selectedFields.empty()
          || selectedFields.found(fieldIter()->name())) {
        Info << "        " << fieldIter()->name() << endl;
        reconstructFvVolumeInternalField<Type>(*fieldIter())().write();
        nReconstructed_++;
      }
    }
    Info << endl;
  }
}


template<class Type>
void mousse::fvFieldReconstructor::reconstructFvVolumeFields
(
  const IOobjectList& objects,
  const HashSet<word>& selectedFields
)
{
  const word& fieldClassName =
    GeometricField<Type, fvPatchField, volMesh>::typeName;
  IOobjectList fields = objects.lookupClass(fieldClassName);
  if (fields.size()) {
    Info << "    Reconstructing " << fieldClassName << "s\n" << endl;
    FOR_ALL_CONST_ITER(IOobjectList, fields, fieldIter) {
      if (selectedFields.empty()
          || selectedFields.found(fieldIter()->name())) {
        Info << "        " << fieldIter()->name() << endl;
        reconstructFvVolumeField<Type>(*fieldIter())().write();
        nReconstructed_++;
      }
    }
    Info << endl;
  }
}


template<class Type>
void mousse::fvFieldReconstructor::reconstructFvSurfaceFields
(
  const IOobjectList& objects,
  const HashSet<word>& selectedFields
)
{
  const word& fieldClassName =
    GeometricField<Type, fvsPatchField, surfaceMesh>::typeName;
  IOobjectList fields = objects.lookupClass(fieldClassName);
  if (fields.size()) {
    Info << "    Reconstructing " << fieldClassName << "s\n" << endl;
    FOR_ALL_CONST_ITER(IOobjectList, fields, fieldIter) {
      if (selectedFields.empty()
          || selectedFields.found(fieldIter()->name())) {
        Info << "        " << fieldIter()->name() << endl;
        reconstructFvSurfaceField<Type>(*fieldIter())().write();
        nReconstructed_++;
      }
    }
    Info << endl;
  }
}

