#ifndef CORE_INTERPOLATIONS_TPATCH_TO_PATCH_INTERPOLATION_HPP_
#define CORE_INTERPOLATIONS_TPATCH_TO_PATCH_INTERPOLATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PatchToPatchInterpolation
// Description
//   Interpolation class dealing with transfer of data between two
//   primitivePatches

#include "class_name.hpp"
#include "label_list.hpp"
#include "scalar_field.hpp"
#include "point_field.hpp"
#include "field_fields.hpp"
#include "face_list.hpp"
#include "intersection.hpp"


namespace mousse {

TEMPLATE_NAME(PatchToPatchInterpolation);


template<class FromPatch, class ToPatch>
class PatchToPatchInterpolation
:
  public PatchToPatchInterpolationName
{

  // Private data

    //- Reference to the source patch
    const FromPatch& fromPatch_;

    //- Reference to the target patch
    const ToPatch& toPatch_;

    //- Type of intersection algorithm to use in projection
    intersection::algorithm alg_;

    //- Direction projection to use in projection
    intersection::direction dir_;

  // Static data

    //- Relative merge tolerance for projected points missing the target
    //  Expressed as the fraction of min involved edge size
    static scalar projectionTol_;

    // Point addressing

      //- Face into which each point of target patch is projected
      mutable labelList* pointAddressingPtr_;

      //- Weighting factors
      mutable FieldField<Field, scalar>* pointWeightsPtr_;

      //- Distance to intersection for patch points
      mutable scalarField* pointDistancePtr_;

    // Face addressing

      //- Face into which each face centre of target patch is projected
      mutable labelList* faceAddressingPtr_;

      //- Weighting factors
      mutable FieldField<Field, scalar>* faceWeightsPtr_;

      //- Distance to intersection for patch face centres
      mutable scalarField* faceDistancePtr_;

  // Private Member Functions

    //- Calculate point weights
    void calcPointAddressing() const;

    //- Calculate face weights
    void calcFaceAddressing() const;

    //- Clear all geometry and addressing
    void clearOut();

    //- Return reference to point addressing
    const labelList& pointAddr() const;

    //- Return reference to point weights
    const FieldField<Field, scalar>& pointWeights() const;

    //- Return reference to face addressing
    const labelList& faceAddr() const;

    //- Return reference to face weights
    const FieldField<Field, scalar>& faceWeights() const;

  // Private static data members

    //- Direct hit tolerance
    static const scalar directHitTol;

public:

  // Constructors

    //- Construct from components
    PatchToPatchInterpolation
    (
      const FromPatch& fromPatch,
      const ToPatch& toPatch,
      const intersection::algorithm alg = intersection::FULL_RAY,
      const intersection::direction dir = intersection::VECTOR
    );

    //- Disallow default bitwise copy construct
    PatchToPatchInterpolation(const PatchToPatchInterpolation&) = delete;

    //- Disallow default bitwise assignment
    PatchToPatchInterpolation& operator=
    (
      const PatchToPatchInterpolation&
    ) = delete;

  //- Destructor
  ~PatchToPatchInterpolation();

  // Member Functions

    //- Set the projection tolerance, returning the previous value
    static scalar setProjectionTol(const scalar t)
    {
      if (t < -VSMALL) {
        FATAL_ERROR_IN
        (
          "scalar PatchToPatchInterpolation::"
          "setProjectionTol(const scalar t)"
        )
        << "Negative projection tolerance.  This is not allowed."
        << abort(FatalError);
      }
      scalar oldTol = projectionTol_;
      projectionTol_ = t;
      return oldTol;
    }

    //- Return ype of intersection algorithm to use in projection
    intersection::algorithm projectionAlgo() const
    {
      return alg_;
    }

    //- Return direction projection to use in projection
    intersection::direction projectionDir() const
    {
      return dir_;
    }

    //- Return distance to intersection for patch points
    const scalarField& pointDistanceToIntersection() const;

    //- Return distance to intersection for patch face centres
    const scalarField& faceDistanceToIntersection() const;

    //- Correct weighting factors for moving mesh.
    bool movePoints();

    //- Interpolate point field
    template<class Type>
    tmp<Field<Type> > pointInterpolate(const Field<Type>& pf) const;

    template<class Type>
    tmp<Field<Type> > pointInterpolate(const tmp<Field<Type> >& tpf) const;

    //- Interpolate face field
    template<class Type>
    tmp<Field<Type> > faceInterpolate(const Field<Type>& pf) const;

    template<class Type>
    tmp<Field<Type> > faceInterpolate(const tmp<Field<Type> >& tpf) const;
};

}  // namespace mousse

#include "_patch_to_patch_interpolation.ipp"

#endif
