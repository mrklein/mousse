#ifndef PARALLEL_DECOMPOSE_DECOMPOSE_FV_FIELD_DECOMPOSER_HPP_
#define PARALLEL_DECOMPOSE_DECOMPOSE_FV_FIELD_DECOMPOSER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvFieldDecomposer
// Description
//   Finite Volume volume and surface field decomposer.

#include "fv_mesh.hpp"
#include "fv_patch_field_mapper.hpp"
#include "surface_fields.hpp"


namespace mousse {

class IOobjectList;


class fvFieldDecomposer
{
public:
    //- Patch field decomposer class
    class patchFieldDecomposer
    :
      public fvPatchFieldMapper
    {
      // Private data
        labelList directAddressing_;
    public:
      // Constructors
        //- Construct given addressing
        patchFieldDecomposer
        (
          const labelUList& addressingSlice,
          const label addressingOffset
        );
      // Member functions
        label size() const { return directAddressing_.size(); }
        bool direct() const { return true; }
        //- Are there unmapped values
        bool hasUnmapped() const { return false; }
        const labelUList& directAddressing() const { return directAddressing_; }
    };
    //- Processor patch field decomposer class. Maps either owner or
    //  neighbour data (no interpolate anymore - processorFvPatchField
    //  holds neighbour data)
    class processorVolPatchFieldDecomposer
    :
      public fvPatchFieldMapper
    {
      // Private data
        labelList directAddressing_;
    public:
      //- Construct given addressing
      processorVolPatchFieldDecomposer
      (
        const fvMesh& mesh,
        const labelUList& addressingSlice
      );
      // Member functions
        label size() const { return directAddressing_.size(); }
        bool direct() const { return true; }
        //- Are there unmapped values
        bool hasUnmapped() const { return false; }
        const labelUList& directAddressing() const { return directAddressing_; }
    };
    //- Processor patch field decomposer class. Surface field is assumed
    //  to have direction (so manipulates sign when mapping)
    class processorSurfacePatchFieldDecomposer
    :
      public fvPatchFieldMapper
    {
      labelListList addressing_;
      scalarListList weights_;
    public:
      //- Construct given addressing
      processorSurfacePatchFieldDecomposer
      (
        const labelUList& addressingSlice
      );
      // Member functions
        label size() const { return addressing_.size(); }
        bool direct() const { return false; }
        //- Are there unmapped values
        bool hasUnmapped() const { return false; }
        const labelListList& addressing() const { return addressing_; }
        const scalarListList& weights() const { return weights_; }
    };
private:
  // Private data
    //- Reference to complete mesh
    const fvMesh& completeMesh_;
    //- Reference to processor mesh
    const fvMesh& procMesh_;
    //- Reference to face addressing
    const labelList& faceAddressing_;
    //- Reference to cell addressing
    const labelList& cellAddressing_;
    //- Reference to boundary addressing
    const labelList& boundaryAddressing_;
    //- List of patch field decomposers
    List<patchFieldDecomposer*> patchFieldDecomposerPtrs_;
    List<processorVolPatchFieldDecomposer*>
      processorVolPatchFieldDecomposerPtrs_;
    List<processorSurfacePatchFieldDecomposer*>
      processorSurfacePatchFieldDecomposerPtrs_;
public:
  // Constructors
    //- Construct from components
    fvFieldDecomposer
    (
      const fvMesh& completeMesh,
      const fvMesh& procMesh,
      const labelList& faceAddressing,
      const labelList& cellAddressing,
      const labelList& boundaryAddressing
    );
    //- Disallow default bitwise copy construct
    fvFieldDecomposer(const fvFieldDecomposer&) = delete;
    //- Disallow default bitwise assignment
    fvFieldDecomposer& operator=(const fvFieldDecomposer&) = delete;
  //- Destructor
  ~fvFieldDecomposer();
  // Member Functions
    //- Decompose volume field
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> >
    decomposeField
    (
      const GeometricField<Type, fvPatchField, volMesh>& field,
      const bool allowUnknownPatchFields = false
    ) const;
    //- Decompose surface field
    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    decomposeField
    (
      const GeometricField<Type, fvsPatchField, surfaceMesh>& field
    ) const;
    template<class GeoField>
    void decomposeFields(const PtrList<GeoField>& fields) const;
};
}  // namespace mousse

#include "fv_field_decomposer_decompose_fields.ipp"

#endif
