#ifndef MESH_TOOLS_AMI_INTERPOLATION_AMI_INTERPOLATION_AMI_METHOD_AMI_METHOD_AMI_METHOD_HPP_
#define MESH_TOOLS_AMI_INTERPOLATION_AMI_INTERPOLATION_AMI_METHOD_AMI_METHOD_AMI_METHOD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::AMIMethod
// Description
//   Base class for Arbitrary Mesh Interface (AMI) methods
// SourceFiles
//   ami_method.cpp


#include "class_name.hpp"
#include "dynamic_list.hpp"
#include "face_area_intersect.hpp"
#include "indexed_octree.hpp"
#include "tree_data_primitive_patch.hpp"
#include "tree_bound_box_list.hpp"
#include "run_time_selection_tables.hpp"

namespace mousse
{

template<class SourcePatch, class TargetPatch>
class AMIMethod
{
protected:

  //- Local typedef to octree tree-type
  typedef treeDataPrimitivePatch<TargetPatch> treeType;

  // Protected data

    //- Reference to source patch
    const SourcePatch& srcPatch_;

    //- Reference to target patch
    const TargetPatch& tgtPatch_;

    //- Flag to indicate that the two patches are co-directional and
    //  that the orientation of the target patch should be reversed
    const bool reverseTarget_;

    //- Flag to indicate that the two patches must be matched/an overlap
    //  exists between them
    const bool requireMatch_;

    //- Source face areas
    const scalarField& srcMagSf_;

    //- Target face areas
    const scalarField& tgtMagSf_;

    //- Labels of faces that are not overlapped by any target faces
    //  (should be empty for correct functioning)
    labelList srcNonOverlap_;

    //- Octree used to find face seeds
    autoPtr<indexedOctree<treeType> > treePtr_;

    //- Face triangulation mode
    const faceAreaIntersect::triangulationMode triMode_;

  // Protected Member Functions

    // Helper functions

      //- Check AMI patch coupling
      void checkPatches() const;

      //- Initialise and return true if all ok
      bool initialise
      (
        labelListList& srcAddress,
        scalarListList& srcWeights,
        labelListList& tgtAddress,
        scalarListList& tgtWeights,
        label& srcFaceI,
        label& tgtFaceI
      );

      //- Write triangle intersection to OBJ file
      void writeIntersectionOBJ
      (
        const scalar area,
        const face& f1,
        const face& f2,
        const pointField& f1Points,
        const pointField& f2Points
      ) const;

    // Common AMI method functions

      //- Reset the octree for the target patch face search
      void resetTree();

      //- Find face on target patch that overlaps source face
      label findTargetFace(const label srcFaceI) const;

      //- Add faces neighbouring faceI to the ID list
      void appendNbrFaces
      (
        const label faceI,
        const TargetPatch& patch,
        const DynamicList<label>& visitedFaces,
        DynamicList<label>& faceIDs
      ) const;

public:

  //- Runtime type information
  TYPE_NAME("AMIMethod");

  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    AMIMethod,
    components,
    (
      const SourcePatch& srcPatch,
      const TargetPatch& tgtPatch,
      const scalarField& srcMagSf,
      const scalarField& tgtMagSf,
      const faceAreaIntersect::triangulationMode& triMode,
      const bool reverseTarget,
      const bool requireMatch
    ),
    (
      srcPatch,
      tgtPatch,
      srcMagSf,
      tgtMagSf,
      triMode,
      reverseTarget,
      requireMatch
    )
  );

  //- Construct from components
  AMIMethod
  (
    const SourcePatch& srcPatch,
    const TargetPatch& tgtPatch,
    const scalarField& srcMagSf,
    const scalarField& tgtMagSf,
    const faceAreaIntersect::triangulationMode& triMode,
    const bool reverseTarget,
    const bool requireMatch
  );

  //- Disallow default bitwise copy construct
  AMIMethod(const AMIMethod&) = delete;

  //- Disallow default bitwise assignment
  AMIMethod& operator=(const AMIMethod&) = delete;

  //- Selector
  static autoPtr<AMIMethod> New
  (
    const word& methodName,
    const SourcePatch& srcPatch,
    const TargetPatch& tgtPatch,
    const scalarField& srcMagSf,
    const scalarField& tgtMagSf,
    const faceAreaIntersect::triangulationMode& triMode,
    const bool reverseTarget,
    const bool requireMatch
  );

  //- Destructor
  virtual ~AMIMethod();

  // Member Functions

    // Access

      //- Labels of faces that are not overlapped by any target faces
      //  Note: this should be empty for correct functioning
      inline const labelList& srcNonOverlap() const;

      //- Flag to indicate that interpolation patches are conformal
      virtual bool conformal() const;

    // Manipulation

      //- Update addressing and weights
      virtual void calculate
      (
        labelListList& srcAddress,
        scalarListList& srcWeights,
        labelListList& tgtAddress,
        scalarListList& tgtWeights,
        label srcFaceI = -1,
        label tgtFaceI = -1
      ) = 0;
};

}  // namespace mousse

#define MAKE_AMI_METHOD(AMIType)                                              \
                                                                              \
  typedef AMIMethod<AMIType::sourcePatchType,AMIType::targetPatchType>        \
    AMIMethod##AMIType;                                                       \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(AMIMethod##AMIType, 0);           \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(AMIMethod##AMIType, components);

#define MAKE_AMI_METHOD_TYPE(AMIType, Method)                                 \
                                                                              \
  typedef Method<AMIType::sourcePatchType,AMIType::targetPatchType>           \
    Method##AMIType;                                                          \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(Method##AMIType, 0);              \
                                                                              \
  AMIMethod<AMIType::sourcePatchType,AMIType::targetPatchType>::              \
    addcomponentsConstructorToTable<Method##AMIType>                          \
    add##Method##AMIType##ConstructorToTable_;

template<class SourcePatch, class TargetPatch>
inline const mousse::labelList&
mousse::AMIMethod<SourcePatch, TargetPatch>::srcNonOverlap() const
{
  return srcNonOverlap_;
}

#ifdef NoRepository
#   include "ami_method.cpp"
#   include "ami_method_new.cpp"
#endif
#endif
