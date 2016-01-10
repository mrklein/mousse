// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceAreaWeightAMI
// Description
//   Face area weighted Arbitrary Mesh Interface (AMI) method
// SourceFiles
//   face_area_weight_ami.cpp
//
#ifndef face_area_weight_ami_hpp_
#define face_area_weight_ami_hpp_

#include "ami_method.hpp"

namespace mousse
{

template<class SourcePatch, class TargetPatch>
class faceAreaWeightAMI
:
  public AMIMethod<SourcePatch, TargetPatch>
{
private:
  // Private data

    //- Flag to restart uncovered source faces
    const bool restartUncoveredSourceFace_;

protected:
  // Protected Member Functions

    // Marching front
      //- Calculate addressing and weights using temporary storage
      virtual void calcAddressing
      (
        List<DynamicList<label> >& srcAddress,
        List<DynamicList<scalar> >& srcWeights,
        List<DynamicList<label> >& tgtAddress,
        List<DynamicList<scalar> >& tgtWeights,
        label srcFaceI,
        label tgtFaceI
      );

      //- Determine overlap contributions for source face srcFaceI
      virtual bool processSourceFace
      (
        const label srcFaceI,
        const label tgtStartFaceI,
        DynamicList<label>& nbrFaces,
        DynamicList<label>& visitedFaces,
        List<DynamicList<label> >& srcAddr,
        List<DynamicList<scalar> >& srcWght,
        List<DynamicList<label> >& tgtAddr,
        List<DynamicList<scalar> >& tgtWght
      );

      //- Attempt to re-evaluate source faces that have not been included
      virtual void restartUncoveredSourceFace
      (
        List<DynamicList<label> >& srcAddr,
        List<DynamicList<scalar> >& srcWght,
        List<DynamicList<label> >& tgtAddr,
        List<DynamicList<scalar> >& tgtWght
      );

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

    // Evaluation

      //- Area of intersection between source and target faces
      virtual scalar interArea
      (
        const label srcFaceI,
        const label tgtFaceI
      ) const;

public:

  //- Runtime type information
  TYPE_NAME("faceAreaWeightAMI");

  // Constructors

    //- Construct from components
    faceAreaWeightAMI
    (
      const SourcePatch& srcPatch,
      const TargetPatch& tgtPatch,
      const scalarField& srcMagSf,
      const scalarField& tgtMagSf,
      const faceAreaIntersect::triangulationMode& triMode,
      const bool reverseTarget = false,
      const bool requireMatch = true,
      const bool restartUncoveredSourceFace = true
    );

    //- Disallow default bitwise copy construct
    faceAreaWeightAMI(const faceAreaWeightAMI&) = delete;

    //- Disallow default bitwise assignment
    faceAreaWeightAMI& operator=(const faceAreaWeightAMI&) = delete;

  //- Destructor
  virtual ~faceAreaWeightAMI();

  // Member Functions

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

#ifdef NoRepository
#include "face_area_weight_ami.cpp"
#endif
#endif
