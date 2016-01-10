// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::AMIInterpolation
// Description
//   Interpolation class dealing with transfer of data between two
//   primitive patches with an arbitrary mesh interface (AMI).
//   Based on the algorithm given in:
//     Conservative interpolation between volume meshes by local Galerkin
//     projection, Farrell PE and Maddison JR, 2011, Comput. Methods Appl.
//     Mech Engrg, Volume 200, Issues 1-4, pp 89-100
//   Interpolation requires that the two patches should have opposite
//   orientations (opposite normals).  The 'reverseTarget' flag can be used to
//   reverse the orientation of the target patch.
// SourceFiles
//   ami_interpolation.cpp
//   ami_interpolation_name.cpp
//   ami_interpolation_parallel_ops.cpp

#ifndef ami_interpolation_hpp_
#define ami_interpolation_hpp_

#include "class_name.hpp"
#include "searchable_surface.hpp"
#include "tree_bound_box_list.hpp"
#include "bool_list.hpp"
#include "primitive_patch.hpp"
#include "face_area_intersect.hpp"
#include "global_index.hpp"
#include "ops.hpp"

namespace mousse
{
TEMPLATE_NAME(AMIInterpolation);
template<class SourcePatch, class TargetPatch>
class AMIInterpolation
:
  public AMIInterpolationName
{
public:
  // Public data types

    //- Enumeration specifying interpolation method
    enum interpolationMethod
    {
      imDirect,
      imMapNearest,
      imFaceAreaWeight,
      imPartialFaceAreaWeight
    };

    //- Convert interpolationMethod to word representation
    static word interpolationMethodToWord
    (
      const interpolationMethod& method
    );

    //- Convert word to interpolationMethod
    static interpolationMethod wordTointerpolationMethod
    (
      const word& method
    );

private:

  // Private data

    //- Interpolation method
    const word methodName_;

    //- Flag to indicate that the two patches are co-directional and
    //  that the orientation of the target patch should be reversed
    const bool reverseTarget_;

    //- Flag to indicate that the two patches must be matched/an overlap
    //  exists between them
    const bool requireMatch_;

    //- Index of processor that holds all of both sides. -1 in all other
    //  cases
    label singlePatchProc_;

    //- Threshold weight below which interpolation is deactivated
    scalar lowWeightCorrection_;

    // Source patch

      //- Source face areas
      scalarField srcMagSf_;

      //- Addresses of target faces per source face
      labelListList srcAddress_;

      //- Weights of target faces per source face
      scalarListList srcWeights_;

      //- Sum of weights of target faces per source face
      scalarField srcWeightsSum_;

    // Target patch

      //- Target face areas
      scalarField tgtMagSf_;

      //- Addresses of source faces per target face
      labelListList tgtAddress_;

      //- Weights of source faces per target face
      scalarListList tgtWeights_;

      //- Sum of weights of source faces per target face
      scalarField tgtWeightsSum_;

    //- Face triangulation mode
    const faceAreaIntersect::triangulationMode triMode_;

    //- Source map pointer - parallel running only
    autoPtr<mapDistribute> srcMapPtr_;

    //- Target map pointer - parallel running only
    autoPtr<mapDistribute> tgtMapPtr_;

  // Private Member Functions

    // Parallel functionality

      //- Calculate if patches are on multiple processors
      label calcDistribution
      (
        const SourcePatch& srcPatch,
        const TargetPatch& tgtPatch
      ) const;

      label calcOverlappingProcs
      (
        const List<treeBoundBoxList>& procBb,
        const treeBoundBox& bb,
        boolList& overlaps
      ) const;

      void distributePatches
      (
        const mapDistribute& map,
        const TargetPatch& pp,
        const globalIndex& gi,
        List<faceList>& faces,
        List<pointField>& points,
        List<labelList>& tgtFaceIDs
      ) const;

      void distributeAndMergePatches
      (
        const mapDistribute& map,
        const TargetPatch& tgtPatch,
        const globalIndex& gi,
        faceList& tgtFaces,
        pointField& tgtPoints,
        labelList& tgtFaceIDs
      ) const;

      autoPtr<mapDistribute> calcProcMap
      (
        const SourcePatch& srcPatch,
        const TargetPatch& tgtPatch
      ) const;

    // Initialisation

      //- Project points to surface
      void projectPointsToSurface
      (
        const searchableSurface& surf,
        pointField& pts
      ) const;

    // Evaluation

      //- Normalise the (area) weights - suppresses numerical error in
      //  weights calculation
      //  NOTE: if area weights are incorrect by 'a significant amount'
      //     normalisation may stabilise the solution, but will introduce
      //     numerical error!
      static void normaliseWeights
      (
        const scalarField& patchAreas,
        const word& patchName,
        const labelListList& addr,
        scalarListList& wght,
        scalarField& wghtSum,
        const bool conformal,
        const bool output,
        const scalar lowWeightTol
      );

    // Constructor helpers
      static void agglomerate
      (
        const autoPtr<mapDistribute>& targetMap,
        const scalarField& fineSrcMagSf,
        const labelListList& fineSrcAddress,
        const scalarListList& fineSrcWeights,
        const labelList& sourceRestrictAddressing,
        const labelList& targetRestrictAddressing,
        scalarField& srcMagSf,
        labelListList& srcAddress,
        scalarListList& srcWeights,
        scalarField& srcWeightsSum,
        autoPtr<mapDistribute>& tgtMap
      );

      void constructFromSurface
      (
        const SourcePatch& srcPatch,
        const TargetPatch& tgtPatch,
        const autoPtr<searchableSurface>& surfPtr
      );

public:
  // Constructors

    //- Construct from components
    AMIInterpolation
    (
      const SourcePatch& srcPatch,
      const TargetPatch& tgtPatch,
      const faceAreaIntersect::triangulationMode& triMode,
      const bool requireMatch = true,
      const interpolationMethod& method = imFaceAreaWeight,
      const scalar lowWeightCorrection = -1,
      const bool reverseTarget = false
    );

    //- Construct from components
    AMIInterpolation
    (
      const SourcePatch& srcPatch,
      const TargetPatch& tgtPatch,
      const faceAreaIntersect::triangulationMode& triMode,
      const bool requireMatch = true,
      const word& methodName =
        interpolationMethodToWord(imFaceAreaWeight),
      const scalar lowWeightCorrection = -1,
      const bool reverseTarget = false
    );

    //- Construct from components, with projection surface
    AMIInterpolation
    (
      const SourcePatch& srcPatch,
      const TargetPatch& tgtPatch,
      const autoPtr<searchableSurface>& surf,
      const faceAreaIntersect::triangulationMode& triMode,
      const bool requireMatch = true,
      const interpolationMethod& method = imFaceAreaWeight,
      const scalar lowWeightCorrection = -1,
      const bool reverseTarget = false
    );

    //- Construct from components, with projection surface
    AMIInterpolation
    (
      const SourcePatch& srcPatch,
      const TargetPatch& tgtPatch,
      const autoPtr<searchableSurface>& surf,
      const faceAreaIntersect::triangulationMode& triMode,
      const bool requireMatch = true,
      const word& methodName =
        interpolationMethodToWord(imFaceAreaWeight),
      const scalar lowWeightCorrection = -1,
      const bool reverseTarget = false
    );

    //- Construct from agglomeration of AMIInterpolation. Agglomeration
    //  passed in as new coarse size and addressing from fine from coarse
    AMIInterpolation
    (
      const AMIInterpolation<SourcePatch, TargetPatch>& fineAMI,
      const labelList& sourceRestrictAddressing,
      const labelList& neighbourRestrictAddressing
    );

    //- Disallow default bitwise copy construct
    AMIInterpolation(const AMIInterpolation&) = delete;

    //- Disallow default bitwise assignment
    AMIInterpolation& operator=(const AMIInterpolation&) = delete;

  //- Destructor
  ~AMIInterpolation();

  // Typedef to SourcePatch type this AMIInterplation is instantiated on
  typedef SourcePatch sourcePatchType;

  // Typedef to TargetPatch type this AMIInterplation is instantiated on
  typedef TargetPatch targetPatchType;

  // Member Functions

    // Access

      //- Set to -1, or the processor holding all faces (both sides) of
      //  the AMI
      inline label singlePatchProc() const;

      //- Threshold weight below which interpolation is deactivated
      inline scalar lowWeightCorrection() const;

      //- Return true if employing a 'lowWeightCorrection'
      inline bool applyLowWeightCorrection() const;

      // Source patch

        //- Return const access to source patch face areas
        inline const scalarField& srcMagSf() const;

        //- Return const access to source patch addressing
        inline const labelListList& srcAddress() const;

        //- Return const access to source patch weights
        inline const scalarListList& srcWeights() const;

        //- Return const access to normalisation factor of source
        //  patch weights (i.e. the sum before normalisation)
        inline const scalarField& srcWeightsSum() const;

        //- Source map pointer - valid only if singlePatchProc = -1
        //  This gets source data into a form to be consumed by
        //  tgtAddress, tgtWeights
        inline const mapDistribute& srcMap() const;

      // Target patch

        //- Return const access to target patch face areas
        inline const scalarField& tgtMagSf() const;

        //- Return const access to target patch addressing
        inline const labelListList& tgtAddress() const;

        //- Return const access to target patch weights
        inline const scalarListList& tgtWeights() const;

        //- Return const access to normalisation factor of target
        //  patch weights (i.e. the sum before normalisation)
        inline const scalarField& tgtWeightsSum() const;

        //- Target map pointer -  valid only if singlePatchProc=-1.
        //  This gets target data into a form to be consumed by
        //  srcAddress, srcWeights
        inline const mapDistribute& tgtMap() const;

    // Manipulation

      //- Update addressing and weights
      void update
      (
        const SourcePatch& srcPatch,
        const TargetPatch& tgtPatch
      );

    // Evaluation

      // Low-level

        //- Interpolate from target to source with supplied op
        //  to combine existing value with remote value and weight
        template<class Type, class CombineOp>
        void interpolateToSource
        (
          const UList<Type>& fld,
          const CombineOp& cop,
          List<Type>& result,
          const UList<Type>& defaultValues = UList<Type>::null()
        ) const;

        //- Interpolate from source to target with supplied op
        //  to combine existing value with remote value and weight
        template<class Type, class CombineOp>
        void interpolateToTarget
        (
          const UList<Type>& fld,
          const CombineOp& cop,
          List<Type>& result,
          const UList<Type>& defaultValues = UList<Type>::null()
        ) const;

      //- Interpolate from target to source with supplied op
      template<class Type, class CombineOp>
      tmp<Field<Type>> interpolateToSource
      (
        const Field<Type>& fld,
        const CombineOp& cop,
        const UList<Type>& defaultValues = UList<Type>::null()
      ) const;

      //- Interpolate from target tmp field to source with supplied op
      template<class Type, class CombineOp>
      tmp<Field<Type>> interpolateToSource
      (
        const tmp<Field<Type>>& tFld,
        const CombineOp& cop,
        const UList<Type>& defaultValues = UList<Type>::null()
      ) const;

      //- Interpolate from source to target with supplied op
      template<class Type, class CombineOp>
      tmp<Field<Type>> interpolateToTarget
      (
        const Field<Type>& fld,
        const CombineOp& cop,
        const UList<Type>& defaultValues = UList<Type>::null()
      ) const;

      //- Interpolate from source tmp field to target with supplied op
      template<class Type, class CombineOp>
      tmp<Field<Type>> interpolateToTarget
      (
        const tmp<Field<Type>>& tFld,
        const CombineOp& cop,
        const UList<Type>& defaultValues = UList<Type>::null()
      ) const;

      //- Interpolate from target to source
      template<class Type>
      tmp<Field<Type>> interpolateToSource
      (
        const Field<Type>& fld,
        const UList<Type>& defaultValues = UList<Type>::null()
      ) const;

      //- Interpolate from target tmp field
      template<class Type>
      tmp<Field<Type>> interpolateToSource
      (
        const tmp<Field<Type>>& tFld,
        const UList<Type>& defaultValues = UList<Type>::null()
      ) const;

      //- Interpolate from source to target
      template<class Type>
      tmp<Field<Type>> interpolateToTarget
      (
        const Field<Type>& fld,
        const UList<Type>& defaultValues = UList<Type>::null()
      ) const;

      //- Interpolate from source tmp field
      template<class Type>
      tmp<Field<Type>> interpolateToTarget
      (
        const tmp<Field<Type>>& tFld,
        const UList<Type>& defaultValues = UList<Type>::null()
      ) const;

    // Point intersections

      //- Return source patch face index of point on target patch face
      label srcPointFace
      (
        const SourcePatch& srcPatch,
        const TargetPatch& tgtPatch,
        const vector& n,
        const label tgtFaceI,
        point& tgtPoint
      )
      const;

      //- Return target patch face index of point on source patch face
      label tgtPointFace
      (
        const SourcePatch& srcPatch,
        const TargetPatch& tgtPatch,
        const vector& n,
        const label srcFaceI,
        point& srcPoint
      )
      const;

    // Checks

      //- Write face connectivity as OBJ file
      void writeFaceConnectivity
      (
        const SourcePatch& srcPatch,
        const TargetPatch& tgtPatch,
        const labelListList& srcAddress
      ) const;

};

}  // namespace mousse

template<class SourcePatch, class TargetPatch>
inline mousse::label
mousse::AMIInterpolation<SourcePatch, TargetPatch>::singlePatchProc() const
{
  return singlePatchProc_;
}


template<class SourcePatch, class TargetPatch>
inline mousse::scalar
mousse::AMIInterpolation<SourcePatch, TargetPatch>::lowWeightCorrection() const
{
  return lowWeightCorrection_;
}


template<class SourcePatch, class TargetPatch>
inline bool
mousse::AMIInterpolation<SourcePatch, TargetPatch>::
applyLowWeightCorrection() const
{
  return lowWeightCorrection_ > 0;
}


template<class SourcePatch, class TargetPatch>
inline const mousse::scalarField&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::srcMagSf() const
{
  return srcMagSf_;
}


template<class SourcePatch, class TargetPatch>
inline const mousse::labelListList&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::srcAddress() const
{
  return srcAddress_;
}


template<class SourcePatch, class TargetPatch>
inline const mousse::scalarListList&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::srcWeights() const
{
  return srcWeights_;
}


template<class SourcePatch, class TargetPatch>
inline const mousse::scalarField&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::srcWeightsSum() const
{
  return srcWeightsSum_;
}


template<class SourcePatch, class TargetPatch>
inline const mousse::mapDistribute&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::srcMap() const
{
  return srcMapPtr_();
}


template<class SourcePatch, class TargetPatch>
inline const mousse::scalarField&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::tgtMagSf() const
{
  return tgtMagSf_;
}


template<class SourcePatch, class TargetPatch>
inline const mousse::labelListList&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::tgtAddress() const
{
  return tgtAddress_;
}


template<class SourcePatch, class TargetPatch>
inline const mousse::scalarListList&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::tgtWeights() const
{
  return tgtWeights_;
}


template<class SourcePatch, class TargetPatch>
inline const mousse::scalarField&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::tgtWeightsSum() const
{
  return tgtWeightsSum_;
}


template<class SourcePatch, class TargetPatch>
inline const mousse::mapDistribute&
mousse::AMIInterpolation<SourcePatch, TargetPatch>::tgtMap() const
{
  return tgtMapPtr_();
}

#ifdef NoRepository
#   include "ami_interpolation.cpp"
#   include "ami_interpolation_parallel_ops.cpp"
#endif
#endif
