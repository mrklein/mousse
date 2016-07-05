#ifndef SAMPLING_MESH_TO_MESH_MESH_TO_MESH_HPP_
#define SAMPLING_MESH_TO_MESH_MESH_TO_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshToMesh
// Description
//   Class to calculate the cell-addressing between two overlapping meshes
//   Mapping is performed using a run-time selectable interpolation mothod
// SeeAlso
//   meshToMeshMethod

#include "poly_mesh.hpp"
#include "bound_box.hpp"
#include "map_distribute.hpp"
#include "vol_fields_fwd.hpp"
#include "named_enum.hpp"
#include "ami_patch_to_patch_interpolation.hpp"


namespace mousse {

class meshToMesh
{
public:
  // Public data types
    //- Enumeration specifying interpolation method
    enum interpolationMethod
    {
      imDirect,
      imMapNearest,
      imCellVolumeWeight
    };
    static const NamedEnum<interpolationMethod, 3>
      interpolationMethodNames_;
private:
  // Private data
    //- Reference to the source mesh
    const polyMesh& srcRegion_;
    //- Reference to the target mesh
    const polyMesh& tgtRegion_;
    //- List of target patch IDs per source patch (local index)
    List<label> srcPatchID_;
    //- List of source patch IDs per target patch (local index)
    List<label> tgtPatchID_;
    //- List of AMIs between source and target patches
    PtrList<AMIPatchToPatchInterpolation> patchAMIs_;
    //- Cutting patches whose values are set using a zero-gradient condition
    List<label> cuttingPatches_;
    //- Source to target cell addressing
    labelListList srcToTgtCellAddr_;
    //- Target to source cell addressing
    labelListList tgtToSrcCellAddr_;
    //- Source to target cell interplation weights
    scalarListList srcToTgtCellWght_;
    //- Target to source cell interpolation weights
    scalarListList tgtToSrcCellWght_;
    //- Cell total volume in overlap region [m3]
    scalar V_;
    //- Index of processor that holds all of both sides. -1 in all other
    //  cases
    label singleMeshProc_;
    //- Source map pointer - parallel running only
    autoPtr<mapDistribute> srcMapPtr_;
    //- Target map pointer - parallel running only
    autoPtr<mapDistribute> tgtMapPtr_;
  // Private Member Functions
    //- Helper function to add a constant offset to a list
    template<class Type>
    void add(UList<Type>& fld, const label offset) const;
    //- Return src cell IDs for the overlap region
    labelList maskCells(const polyMesh& src, const polyMesh& tgt) const;
    //- Normalise the interpolation weights
    void normaliseWeights
    (
      const word& descriptor,
      const labelListList& addr,
      scalarListList& wght
    ) const;
    //- Calculate the addressing between overlapping regions of src and tgt
    //  meshes
    void calcAddressing
    (
      const word& methodName,
      const polyMesh& src,
      const polyMesh& tgt
    );
    //- Calculate - main driver function
    void calculate(const word& methodName);
    //- Calculate patch overlap
    void calculatePatchAMIs(const word& amiMethodName);
    //- Constructor helper
    void constructNoCuttingPatches
    (
      const word& methodName,
      const word& AMIMethodName,
      const bool interpAllPatches
    );
    //- Constructor helper
    void constructFromCuttingPatches
    (
      const word& methodName,
      const word& AMIMethodName,
      const HashTable<word>& patchMap,
      const wordList& cuttingPatches
    );
    //- Return the list of AMIs between source and target patches
    inline const PtrList<AMIPatchToPatchInterpolation>&
    patchAMIs() const;
    // Parallel operations
      //- Determine whether the meshes are split across multiple pocessors
      label calcDistribution
      (
        const polyMesh& src,
        const polyMesh& tgt
      ) const;
      //- Determine which processor bounding-boxes overlap
      label calcOverlappingProcs
      (
        const List<boundBox>& procBb,
        const boundBox& bb,
        boolList& overlaps
      ) const;
      //- Calculate the mapping between processors
      autoPtr<mapDistribute> calcProcMap
      (
        const polyMesh& src,
        const polyMesh& tgt
      ) const;
      //- Distribute mesh info from 'my' processor to others
      void distributeCells
      (
        const mapDistribute& map,
        const polyMesh& tgtMesh,
        const globalIndex& globalI,
        List<pointField>& points,
        List<label>& nInternalFaces,
        List<faceList>& faces,
        List<labelList>& faceOwner,
        List<labelList>& faceNeighbour,
        List<labelList>& cellIDs,
        List<labelList>& nbrProcIDs,
        List<labelList>& procLocalFaceIDs
      ) const;
      //- Collect pieces of tgt mesh from other procssors and restructure
      void distributeAndMergeCells
      (
        const mapDistribute& map,
        const polyMesh& tgt,
        const globalIndex& globalI,
        pointField& tgtPoints,
        faceList& tgtFaces,
        labelList& tgtFaceOwners,
        labelList& tgtFaceNeighbours,
        labelList& tgtCellIDs
      ) const;
public:
  //- Run-time type information
  TYPE_NAME("meshToMesh");
  // Constructors
    //- Construct from source and target meshes
    meshToMesh
    (
      const polyMesh& src,
      const polyMesh& tgt,
      const interpolationMethod& method,
      const bool interpAllPatches = true
    );
    //- Construct from source and target meshes, generic mapping methods
    meshToMesh
    (
      const polyMesh& src,
      const polyMesh& tgt,
      const word& methodName,     // internal mapping
      const word& AMIMethodName,  // boundary mapping
      const bool interpAllPatches = true
    );
    //- Construct from source and target meshes
    meshToMesh
    (
      const polyMesh& src,
      const polyMesh& tgt,
      const interpolationMethod& method,
      const HashTable<word>& patchMap,
      const wordList& cuttingPatches
    );
    //- Construct from source and target meshes, generic mapping methods
    meshToMesh
    (
      const polyMesh& src,
      const polyMesh& tgt,
      const word& methodName,     // internal mapping
      const word& AMIMethodName,  // boundary mapping
      const HashTable<word>& patchMap,
      const wordList& cuttingPatches
    );
    //- Disallow default bitwise copy construct
    meshToMesh(const meshToMesh&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const meshToMesh&) = delete;
  //- Destructor
  virtual ~meshToMesh();
  // Member Functions
    // Access
      //- Return const access to the source mesh
      inline const polyMesh& srcRegion() const;
      //- Return const access to the target mesh
      inline const polyMesh& tgtRegion() const;
      //- Return const access to the source to target cell addressing
      inline const labelListList& srcToTgtCellAddr() const;
      //- Return const access to the target to source cell addressing
      inline const labelListList& tgtToSrcCellAddr() const;
      //- Return const access to the source to target cell weights
      inline const scalarListList& srcToTgtCellWght() const;
      //- Return const access to the target to source cell weights
      inline const scalarListList& tgtToSrcCellWght() const;
      //- Return const access to the overlap volume
      inline scalar V() const;
      //- Conversion between mesh and patch interpolation methods
      static AMIPatchToPatchInterpolation::interpolationMethod
      interpolationMethodAMI
      (
        const interpolationMethod method
      );
    // Evaluation
      // Source-to-target field mapping
        //- Map field from src to tgt mesh with defined operation
        //  Values passed in via 'result' are used to initialise the
        //  return value
        template<class Type, class CombineOp>
        void mapSrcToTgt
        (
          const UList<Type>& srcFld,
          const CombineOp& cop,
          List<Type>& result
        ) const;
        //- Return the src field mapped to the tgt mesh with a defined
        //  operation.  Initial values of the result are set to zero
        template<class Type, class CombineOp>
        tmp<Field<Type>> mapSrcToTgt
        (
          const Field<Type>& srcFld,
          const CombineOp& cop
        ) const;
        //- Convenience function to map a tmp field to the tgt mesh
        //  with a defined operation
        template<class Type, class CombineOp>
        tmp<Field<Type>> mapSrcToTgt
        (
          const tmp<Field<Type>>& tsrcFld,
          const CombineOp& cop
        ) const;
        //- Convenience function to map a field to the tgt mesh with a
        //  default operation (plusEqOp)
        template<class Type>
        tmp<Field<Type>> mapSrcToTgt
        (
          const Field<Type>& srcFld
        ) const;
        //- Convenience function to map a tmp field to the tgt mesh
        //  with a default operation (plusEqOp)
        template<class Type>
        tmp<Field<Type>> mapSrcToTgt
        (
          const tmp<Field<Type>>& tsrcFld
        ) const;
      // Target-to-source field mapping
        //- Map field from tgt to src mesh with defined operation
        //  Values passed in via 'result' are used to initialise the
        //  return value
        template<class Type, class CombineOp>
        void mapTgtToSrc
        (
          const UList<Type>& tgtFld,
          const CombineOp& cop,
          List<Type>& result
        ) const;
        //- Return the tgt field mapped to the src mesh with a defined
        //  operation.  Initial values of the result are set to zero
        template<class Type, class CombineOp>
        tmp<Field<Type>> mapTgtToSrc
        (
          const Field<Type>& tgtFld,
          const CombineOp& cop
        ) const;
        //- Convenience function to map a tmp field to the src mesh
        //  with a defined operation
        template<class Type, class CombineOp>
        tmp<Field<Type>> mapTgtToSrc
        (
          const tmp<Field<Type>>& ttgtFld,
          const CombineOp& cop
        ) const;
        //- Convenience function to map a field to the src mesh with a
        //  default operation (plusEqOp)
        template<class Type>
        tmp<Field<Type>> mapTgtToSrc
        (
          const Field<Type>& tgtFld
        ) const;
        //- Convenience function to map a tmp field to the src mesh
        //  with a default operation (plusEqOp)
        template<class Type>
        tmp<Field<Type>> mapTgtToSrc
        (
          const tmp<Field<Type>>& ttgtFld
        ) const;
      // Source-to-target volume field mapping
        //- Interpolate a field with a defined operation.  Values
        //  passed in via 'result' are used to initialise the return
        //  value
        template<class Type, class CombineOp>
        void mapSrcToTgt
        (
          const GeometricField<Type, fvPatchField, volMesh>& field,
          const CombineOp& cop,
          GeometricField<Type, fvPatchField, volMesh>& result
        ) const;
        //- Interpolate a field with a defined operation.  The initial
        //  values of the result are set to zero
        template<class Type, class CombineOp>
        tmp<GeometricField<Type, fvPatchField, volMesh>> mapSrcToTgt
        (
          const GeometricField<Type, fvPatchField, volMesh>& field,
          const CombineOp& cop
        ) const;
        //- Interpolate a tmp field with a defined operation.  The
        //  initial values of the result are set to zero
        template<class Type, class CombineOp>
        tmp<GeometricField<Type, fvPatchField, volMesh>> mapSrcToTgt
        (
          const tmp<GeometricField<Type, fvPatchField, volMesh>>&
            tfield,
          const CombineOp& cop
        ) const;
        //- Convenience function to map a field with a default
        //  operation (plusEqOp)
        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh>> mapSrcToTgt
        (
          const GeometricField<Type, fvPatchField, volMesh>& field
        ) const;
        //- Convenience function to map a tmp field with a default
        //  operation (plusEqOp)
        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh>> mapSrcToTgt
        (
          const tmp<GeometricField<Type, fvPatchField, volMesh>>&
            tfield
        ) const;
      // Target-to-source volume field mapping
        //- Interpolate a field with a defined operation.  Values
        //  passed in via 'result' are used to initialise the return
        //  value
        template<class Type, class CombineOp>
        void mapTgtToSrc
        (
          const GeometricField<Type, fvPatchField, volMesh>& field,
          const CombineOp& cop,
          GeometricField<Type, fvPatchField, volMesh>& result
        ) const;
        //- Interpolate a field with a defined operation.  The initial
        //  values of the result are set to zero
        template<class Type, class CombineOp>
        tmp<GeometricField<Type, fvPatchField, volMesh>> mapTgtToSrc
        (
          const GeometricField<Type, fvPatchField, volMesh>& field,
          const CombineOp& cop
        ) const;
        //- Interpolate a tmp field with a defined operation.  The
        //  initial values of the result are set to zero
        template<class Type, class CombineOp>
        tmp<GeometricField<Type, fvPatchField, volMesh>> mapTgtToSrc
        (
          const tmp<GeometricField<Type, fvPatchField, volMesh>>&
            tfield,
          const CombineOp& cop
        ) const;
        //- Convenience function to map a field with a default
        //  operation (plusEqOp)
        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh>> mapTgtToSrc
        (
          const GeometricField<Type, fvPatchField, volMesh>& field
        ) const;
        //- Convenience function to map a tmp field with a default
        //  operation (plusEqOp)
        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh>> mapTgtToSrc
        (
          const tmp<GeometricField<Type, fvPatchField, volMesh>>&
            tfield
        ) const;
};

}  // namespace mousse


// Member Functions 
inline const mousse::polyMesh& mousse::meshToMesh::srcRegion() const
{
  return srcRegion_;
}


inline const mousse::polyMesh& mousse::meshToMesh::tgtRegion() const
{
  return tgtRegion_;
}


inline const mousse::labelListList&
mousse::meshToMesh::srcToTgtCellAddr() const
{
  return srcToTgtCellAddr_;
}


inline const mousse::labelListList&
mousse::meshToMesh::tgtToSrcCellAddr() const
{
  return tgtToSrcCellAddr_;
}


inline const mousse::scalarListList&
mousse::meshToMesh::srcToTgtCellWght() const
{
  return srcToTgtCellWght_;
}


inline const mousse::scalarListList&
mousse::meshToMesh::tgtToSrcCellWght() const
{
  return tgtToSrcCellWght_;
}


inline mousse::scalar mousse::meshToMesh::V() const
{
  return V_;
}


inline const mousse::PtrList<mousse::AMIPatchToPatchInterpolation>&
mousse::meshToMesh::patchAMIs() const
{
  return patchAMIs_;
}

#include "mesh_to_mesh.ipp"

#endif
