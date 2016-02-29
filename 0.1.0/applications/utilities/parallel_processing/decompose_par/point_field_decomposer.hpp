#ifndef UTILITIES_PARALLEL_PROCESSING_DECOMPOSE_PAR_POINT_FIELD_DECOMPOSER_HPP_
#define UTILITIES_PARALLEL_PROCESSING_DECOMPOSE_PAR_POINT_FIELD_DECOMPOSER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointFieldDecomposer
// Description
//   Point field decomposer.
// SourceFiles
//   point_field_decomposer.cpp
//   point_field_decomposer_decompose_fields.cpp
#include "point_mesh.hpp"
#include "point_patch_field_mapper_patch_ref.hpp"
#include "point_fields.hpp"
namespace mousse
{
class pointFieldDecomposer
{
public:
    //- Point patch field decomposer class
    class patchFieldDecomposer
    :
      public pointPatchFieldMapperPatchRef
    {
      // Private data
        labelList directAddressing_;
        //- Does map contain any unmapped values
        bool hasUnmapped_;
    public:
      // Constructors
        //- Construct given addressing
        patchFieldDecomposer
        (
          const pointPatch& completeMeshPatch,
          const pointPatch& procMeshPatch,
          const labelList& directAddr
        );
      // Member functions
        label size() const
        {
          return directAddressing_.size();
        }
        bool direct() const
        {
          return true;
        }
        bool hasUnmapped() const
        {
          return hasUnmapped_;
        }
        const labelUList& directAddressing() const
        {
          return directAddressing_;
        }
    };
private:
  // Private data
    //- Reference to complete mesh
    const pointMesh& completeMesh_;
    //- Reference to processor mesh
    const pointMesh& procMesh_;
    //- Reference to point addressing
    const labelList& pointAddressing_;
    //- Reference to boundary addressing
    const labelList& boundaryAddressing_;
    //- List of patch field decomposers
    List<patchFieldDecomposer*> patchFieldDecomposerPtrs_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    pointFieldDecomposer(const pointFieldDecomposer&);
    //- Disallow default bitwise assignment
    void operator=(const pointFieldDecomposer&);
public:
  // Constructors
    //- Construct from components
    pointFieldDecomposer
    (
      const pointMesh& completeMesh,
      const pointMesh& procMesh,
      const labelList& pointAddressing,
      const labelList& boundaryAddressing
    );
  //- Destructor
  ~pointFieldDecomposer();
  // Member Functions
    //- Decompose point field
    template<class Type>
    tmp<GeometricField<Type, pointPatchField, pointMesh> >
    decomposeField
    (
      const GeometricField<Type, pointPatchField, pointMesh>&
    ) const;
    template<class GeoField>
    void decomposeFields(const PtrList<GeoField>& fields) const;
};
}  // namespace mousse
#ifdef NoRepository
  #include "point_field_decomposer_decompose_fields.cpp"
#endif
#endif
