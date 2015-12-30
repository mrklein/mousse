// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvFieldReconstructor
// Description
//   Finite volume reconstructor for volume and surface fields.
// SourceFiles
//   fv_field_reconstructor.cpp
//   fv_field_reconstructor_reconstruct_fields.cpp
#ifndef fv_field_reconstructor_hpp_
#define fv_field_reconstructor_hpp_
#include "ptr_list.hpp"
#include "fv_mesh.hpp"
#include "ioobject_list.hpp"
#include "fv_patch_field_mapper.hpp"
#include "label_io_list.hpp"
namespace mousse
{
class fvFieldReconstructor
{
  // Private data
    //- Reconstructed mesh reference
    fvMesh& mesh_;
    //- List of processor meshes
    const PtrList<fvMesh>& procMeshes_;
    //- List of processor face addressing lists
    const PtrList<labelIOList>& faceProcAddressing_;
    //- List of processor cell addressing lists
    const PtrList<labelIOList>& cellProcAddressing_;
    //- List of processor boundary addressing lists
    const PtrList<labelIOList>& boundaryProcAddressing_;
    //- Number of fields reconstructed
    label nReconstructed_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    fvFieldReconstructor(const fvFieldReconstructor&);
    //- Disallow default bitwise assignment
    void operator=(const fvFieldReconstructor&);
public:
    //- Mapper for sizing only - does not do any actual mapping.
    class fvPatchFieldReconstructor
    :
      public fvPatchFieldMapper
    {
      label size_;
    public:
      // Constructors
        //- Construct given size
        fvPatchFieldReconstructor(const label size)
        :
          size_(size)
        {}
      // Member functions
        label size() const
        {
          return size_;
        }
        bool direct() const
        {
          return true;
        }
        bool hasUnmapped() const
        {
          return false;
        }
        const labelUList& directAddressing() const
        {
          return labelUList::null();
        }
    };
  // Constructors
    //- Construct from components
    fvFieldReconstructor
    (
      fvMesh& mesh,
      const PtrList<fvMesh>& procMeshes,
      const PtrList<labelIOList>& faceProcAddressing,
      const PtrList<labelIOList>& cellProcAddressing,
      const PtrList<labelIOList>& boundaryProcAddressing
    );
  // Member Functions
    //- Return number of fields reconstructed
    label nReconstructed() const
    {
      return nReconstructed_;
    }
    //- Reconstruct volume internal field
    template<class Type>
    tmp<DimensionedField<Type, volMesh> >
    reconstructFvVolumeInternalField
    (
      const IOobject& fieldIoObject,
      const PtrList<DimensionedField<Type, volMesh> >& procFields
    ) const;
    //- Read and reconstruct volume internal field
    template<class Type>
    tmp<DimensionedField<Type, volMesh> >
    reconstructFvVolumeInternalField(const IOobject& fieldIoObject) const;
    //- Reconstruct volume field
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> >
    reconstructFvVolumeField
    (
      const IOobject& fieldIoObject,
      const PtrList<GeometricField<Type, fvPatchField, volMesh> >&
    ) const;
    //- Read and reconstruct volume field
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> >
    reconstructFvVolumeField(const IOobject& fieldIoObject) const;
    //- Reconstruct surface field
    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    reconstructFvSurfaceField
    (
      const IOobject& fieldIoObject,
      const PtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >&
    ) const;
    //- Read and reconstruct surface field
    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    reconstructFvSurfaceField(const IOobject& fieldIoObject) const;
    //- Read, reconstruct and write all/selected volume internal fields
    template<class Type>
    void reconstructFvVolumeInternalFields
    (
      const IOobjectList& objects,
      const HashSet<word>& selectedFields
    );
    //- Read, reconstruct and write all/selected volume fields
    template<class Type>
    void reconstructFvVolumeFields
    (
      const IOobjectList& objects,
      const HashSet<word>& selectedFields
    );
    //- Read, reconstruct and write all/selected surface fields
    template<class Type>
    void reconstructFvSurfaceFields
    (
      const IOobjectList& objects,
      const HashSet<word>& selectedFields
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "fv_field_reconstructor_reconstruct_fields.cpp"
#endif
#endif
