#ifndef SAMPLING_MESH_TO_MESH0_MESH_TO_MESH0_HPP_
#define SAMPLING_MESH_TO_MESH0_MESH_TO_MESH0_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshToMesh0
// Description
//   mesh to mesh interpolation class.
// Note
//   This class is due to be deprecated in favour of meshToMesh0New

#include "fv_mesh.hpp"
#include "hash_table.hpp"
#include "fv_patch_mapper.hpp"
#include "scalar_list.hpp"
#include "class_name.hpp"


namespace mousse {

template<class Type>
class indexedOctree;
class treeDataCell;


class meshToMesh0
{
  // Private data
    // mesh references
    const fvMesh& fromMesh_;
    const fvMesh& toMesh_;
    //- fromMesh patch labels
    HashTable<label> fromMeshPatches_;
    //- toMesh patch labels
    HashTable<label> toMeshPatches_;
    //- Patch map
    HashTable<word> patchMap_;
    //- toMesh patch labels which cut the from-mesh
    HashTable<label> cuttingPatches_;
    //- Cell addressing
    labelList cellAddressing_;
    //- Boundary addressing
    labelListList boundaryAddressing_;
    //- Inverse-distance interpolation weights
    mutable scalarListList* inverseDistanceWeightsPtr_;
    //- Inverse-volume interpolation weights
    mutable scalarListList* inverseVolumeWeightsPtr_;
    //- Cell to cell overlap addressing
    mutable labelListList* cellToCellAddressingPtr_;
    //- Overlap volume
    mutable scalar V_;
  // Private Member Functions
    void calcAddressing();
    void cellAddresses
    (
      labelList& cells,
      const pointField& points,
      const fvMesh& fromMesh,
      const List<bool>& boundaryCell,
      const indexedOctree<treeDataCell>& oc
    ) const;
    void calculateInverseDistanceWeights() const;
    void calculateInverseVolumeWeights() const;
    void calculateCellToCellAddressing() const;
    const scalarListList& inverseDistanceWeights() const;
    const scalarListList& inverseVolumeWeights() const;
    const labelListList& cellToCellAddressing() const;
  // Private static data members
    //- Direct hit tolerance
    static const scalar directHitTol;
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("meshToMesh0");
  //- Enumeration specifying required accuracy
  enum order
  {
    MAP,
    INTERPOLATE,
    CELL_POINT_INTERPOLATE,
    CELL_VOLUME_WEIGHT
  };
  // Constructors
    //- Construct from the two meshes, the patch name map for the patches
    //  to be interpolated and the names of the toMesh-patches which
    //  cut the fromMesh
    meshToMesh0
    (
      const fvMesh& fromMesh,
      const fvMesh& toMesh,
      const HashTable<word>& patchMap,
      const wordList& cuttingPatchNames
    );
    //- Construct from the two meshes assuming there is an exact mapping
    //  between the patches
    meshToMesh0
    (
      const fvMesh& fromMesh,
      const fvMesh& toMesh
    );
  //- Destructor
  ~meshToMesh0();
  //- Patch-field interpolation class
  class patchFieldInterpolator
  :
    public fvPatchFieldMapper
  {
    const labelList& directAddressing_;
  public:
    // Constructors
      //- Construct given addressing
      patchFieldInterpolator(const labelList& addr)
      :
        directAddressing_{addr}
      {}
    //- Destructor
    virtual ~patchFieldInterpolator()
    {}
    // Member Functions
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
        return false;
      }
      const labelList& directAddressing() const
      {
        return directAddressing_;
      }
  };
  // Member Functions
    // Access
      const fvMesh& fromMesh() const
      {
        return fromMesh_;
      }
      const fvMesh& toMesh() const
      {
        return toMesh_;
      }
      //- From toMesh cells to fromMesh cells
      const labelList& cellAddressing() const
      {
        return cellAddressing_;
      }
      //- Overlap volume
      scalar V() const
      {
        return V_;
      }
    // Interpolation
      //- Map field
      template<class Type, class CombineOp>
      void mapField
      (
        Field<Type>&,
        const Field<Type>&,
        const labelList& adr,
        const CombineOp& cop
      ) const;
      //- Interpolate field using inverse-distance weights
      template<class Type, class CombineOp>
      void interpolateField
      (
        Field<Type>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const labelList& adr,
        const scalarListList& weights,
        const CombineOp& cop
      ) const;
      //- Interpolate field using inverse-volume weights
      template<class Type, class CombineOp>
      void interpolateField
      (
        Field<Type>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const labelListList& adr,
        const scalarListList& weights,
        const CombineOp& cop
      ) const;
      //- Interpolate field using cell-point interpolation
      template<class Type, class CombineOp>
      void interpolateField
      (
        Field<Type>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const labelList& adr,
        const vectorField& centres,
        const CombineOp& cop
      )const;
      //- Interpolate internal volume field
      template<class Type, class CombineOp>
      void interpolateInternalField
      (
        Field<Type>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        order=INTERPOLATE,
        const CombineOp& cop = eqOp<Type>()
      ) const;
      template<class Type, class CombineOp>
      void interpolateInternalField
      (
        Field<Type>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        order=INTERPOLATE,
        const CombineOp& cop = eqOp<Type>()
      ) const;
      //- Interpolate volume field
      template<class Type, class CombineOp>
      void interpolate
      (
        GeometricField<Type, fvPatchField, volMesh>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        order=INTERPOLATE,
        const CombineOp& cop = eqOp<Type>()
      ) const;
      template<class Type, class CombineOp>
      void interpolate
      (
        GeometricField<Type, fvPatchField, volMesh>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        order=INTERPOLATE,
        const CombineOp& cop = eqOp<Type>()
      ) const;
      //- Interpolate volume field
      template<class Type, class CombineOp>
      tmp<GeometricField<Type, fvPatchField, volMesh> > interpolate
      (
        const GeometricField<Type, fvPatchField, volMesh>&,
        order=INTERPOLATE,
        const CombineOp& cop = eqOp<Type>()
      ) const;
      template<class Type, class CombineOp>
      tmp<GeometricField<Type, fvPatchField, volMesh> > interpolate
      (
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        order=INTERPOLATE,
        const CombineOp& cop = eqOp<Type>()
      ) const;
};
}  // namespace mousse

#include "mesh_to_mesh0.ipp"

#endif
