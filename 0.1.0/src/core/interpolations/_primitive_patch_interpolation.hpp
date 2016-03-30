#ifndef CORE_INTERPOLATIONS_PRIMITIVE_PATCH_INTERPOLATION_HPP_
#define CORE_INTERPOLATIONS_PRIMITIVE_PATCH_INTERPOLATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PrimitivePatchInterpolation
// Description
//   Interpolation class within a primitive patch. Allows interpolation from
//   points to faces and vice versa

#include "scalar_list.hpp"
#include "field.hpp"


namespace mousse {

template<class Patch>
class PrimitivePatchInterpolation
{
  // Private data

    //- Reference to patch
    const Patch& patch_;

  // Demand-driven data

    //- Face-to-point weights
    mutable scalarListList* faceToPointWeightsPtr_;

    //- Face-to-edge weights
    mutable scalarList* faceToEdgeWeightsPtr_;

  // Private Member Functions

    //- Face-to-point weights
    const scalarListList& faceToPointWeights() const;

    //- Make face-to-point weights
    void makeFaceToPointWeights() const;

    //- Face-to-edge weights
    const scalarList& faceToEdgeWeights() const;

    //- Make face-to-edge weights
    void makeFaceToEdgeWeights() const;

    //- Clear weights
    void clearWeights();

public:
  // Constructors

    //- Construct from PrimitivePatch
    PrimitivePatchInterpolation(const Patch& p);

    //- Disallow default bitwise copy construct
    PrimitivePatchInterpolation(const PrimitivePatchInterpolation&) = delete;

    //- Disallow default bitwise assignment
    PrimitivePatchInterpolation& operator=
    (
      const PrimitivePatchInterpolation&
    ) = delete;

  //- Destructor
  ~PrimitivePatchInterpolation();

  // Member Functions

    //- Interpolate from faces to points
    template<class Type>
    tmp<Field<Type> > faceToPointInterpolate
    (
      const Field<Type>& ff
    ) const;

    template<class Type>
    tmp<Field<Type> > faceToPointInterpolate
    (
      const tmp<Field<Type> >& tff
    ) const;

    //- Interpolate from points to faces
    template<class Type>
    tmp<Field<Type> > pointToFaceInterpolate
    (
      const Field<Type>& pf
    ) const;

    template<class Type>
    tmp<Field<Type> > pointToFaceInterpolate
    (
      const tmp<Field<Type> >& tpf
    ) const;

    //- Interpolate from faces to edges
    template<class Type>
    tmp<Field<Type> > faceToEdgeInterpolate
    (
      const Field<Type>& ff
    ) const;

    template<class Type>
    tmp<Field<Type> > faceToEdgeInterpolate
    (
      const tmp<Field<Type> >& tff
    ) const;

    //- Do what is neccessary if the mesh has moved
    bool movePoints();

};

}  // namespace mousse

#include "_primitive_patch_interpolation.ipp"

#endif
