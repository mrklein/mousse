#ifndef MESH_TOOLS_AMI_INTERPOLATION_AMI_INTERPOLATION_AMI_METHOD_PARTIAL_FACE_AREA_WEIGHT_AMI_PARTIAL_FACE_AREA_WEIGHT_AMI_HPP_
#define MESH_TOOLS_AMI_INTERPOLATION_AMI_INTERPOLATION_AMI_METHOD_PARTIAL_FACE_AREA_WEIGHT_AMI_PARTIAL_FACE_AREA_WEIGHT_AMI_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::partialFaceAreaWeightAMI
// Description
//   Partial face area weighted Arbitrary Mesh Interface (AMI) method

#include "face_area_weight_ami.hpp"


namespace mousse {

template<class SourcePatch, class TargetPatch>
class partialFaceAreaWeightAMI
:
  public faceAreaWeightAMI<SourcePatch, TargetPatch>
{
private:
  // Private Member Functions

    // Marching front

      //- Set the source and target seed faces
      virtual void setNextFaces
      (
        label& startSeedI,
        label& srcFaceI,
        label& tgtFaceI,
        const boolList& mapFlag,
        labelList& seedFaces,
        const DynamicList<label>& visitedFaces,
        bool errorOnNotFound = true
      ) const;

public:
  //- Runtime type information
  TYPE_NAME("partialFaceAreaWeightAMI");

  // Constructors

    //- Construct from components
    partialFaceAreaWeightAMI
    (
      const SourcePatch& srcPatch,
      const TargetPatch& tgtPatch,
      const scalarField& srcMagSf,
      const scalarField& tgtMagSf,
      const faceAreaIntersect::triangulationMode& triMode,
      const bool reverseTarget = false,
      const bool requireMatch = true
    );

    //- Disallow default bitwise copy construct
    partialFaceAreaWeightAMI(const partialFaceAreaWeightAMI&) = delete;

    //- Disallow default bitwise assignment
    partialFaceAreaWeightAMI& operator=(const partialFaceAreaWeightAMI&) = delete;

  //- Destructor
  virtual ~partialFaceAreaWeightAMI();

  // Member Functions

    // Access

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
      );
};

}  // namespace mousse

#include "partial_face_area_weight_ami.ipp"

#endif
