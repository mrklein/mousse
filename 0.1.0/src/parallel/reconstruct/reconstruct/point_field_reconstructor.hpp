// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointFieldReconstructor
// Description
//   Point field reconstructor.
// SourceFiles
//   point_field_reconstructor.cpp
#ifndef point_field_reconstructor_hpp_
#define point_field_reconstructor_hpp_
#include "point_mesh.hpp"
#include "point_fields.hpp"
#include "point_patch_field_mapper_patch_ref.hpp"
#include "ioobject_list.hpp"
namespace mousse
{
class pointFieldReconstructor
{
  // Private data
    //- Reconstructed mesh reference
    const pointMesh& mesh_;
    //- List of processor meshes
    const PtrList<pointMesh>& procMeshes_;
    //- List of processor point addressing lists
    const PtrList<labelIOList>& pointProcAddressing_;
    //- List of processor boundary addressing lists
    const PtrList<labelIOList>& boundaryProcAddressing_;
    //- Point patch addressing
    labelListListList patchPointAddressing_;
    //- Number of fields reconstructed
    label nReconstructed_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    pointFieldReconstructor(const pointFieldReconstructor&);
    //- Disallow default bitwise assignment
    void operator=(const pointFieldReconstructor&);
public:
    class pointPatchFieldReconstructor
    :
      public pointPatchFieldMapper
    {
      label size_;
    public:
      // Constructors
        //- Construct given size
        pointPatchFieldReconstructor(const label size)
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
    pointFieldReconstructor
    (
      const pointMesh& mesh,
      const PtrList<pointMesh>& procMeshes,
      const PtrList<labelIOList>& pointProcAddressing,
      const PtrList<labelIOList>& boundaryProcAddressing
    );
  // Member Functions
    //- Return number of fields reconstructed
    label nReconstructed() const
    {
      return nReconstructed_;
    }
    //- Reconstruct field
    template<class Type>
    tmp<GeometricField<Type, pointPatchField, pointMesh> >
    reconstructField(const IOobject& fieldIoObject);
    //- Reconstruct and write all fields
    template<class Type>
    void reconstructFields
    (
      const IOobjectList& objects,
      const HashSet<word>& selectedFields
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "point_field_reconstructor_reconstruct_fields.cpp"
#endif
#endif
