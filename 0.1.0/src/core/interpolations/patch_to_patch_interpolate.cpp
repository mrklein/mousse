// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse
{

// Member Functions 
//- Interpolate point field
template<class FromPatch, class ToPatch>
template<class Type>
tmp<Field<Type> >
PatchToPatchInterpolation<FromPatch, ToPatch>::pointInterpolate
(
  const Field<Type>& pf
) const
{
  if (pf.size() != fromPatch_.nPoints())
  {
    FATAL_ERROR_IN
    (
      "PatchToPatchInterpolation::pointInterpolate"
      "(const Field<Type> pf)"
    )
    << "given field does not correspond to patch. Patch size: "
    << fromPatch_.nPoints() << " field size: " << pf.size()
    << abort(FatalError);
  }

  tmp<Field<Type> > tresult
  {
    new Field<Type>
    {
      toPatch_.nPoints(),
      pTraits<Type>::zero
    }
  };

  Field<Type>& result = tresult();
  const List<typename FromPatch::FaceType>& fromPatchLocalFaces =
    fromPatch_.localFaces();
  const FieldField<Field, scalar>& weights = pointWeights();
  const labelList& addr = pointAddr();

  FOR_ALL(result, pointI)
  {
    const scalarField& curWeights = weights[pointI];
    if (addr[pointI] > -1)
    {
      const labelList& hitFacePoints =
        fromPatchLocalFaces[addr[pointI]];
      FOR_ALL(curWeights, wI)
      {
        result[pointI] += curWeights[wI]*pf[hitFacePoints[wI]];
      }
    }
  }
  return tresult;
}


template<class FromPatch, class ToPatch>
template<class Type>
tmp<Field<Type> >
PatchToPatchInterpolation<FromPatch, ToPatch>::pointInterpolate
(
  const tmp<Field<Type> >& tpf
) const
{
  tmp<Field<Type> > tint = pointInterpolate<Type>(tpf());
  tpf.clear();
  return tint;
}


//- Interpolate face field
template<class FromPatch, class ToPatch>
template<class Type>
tmp<Field<Type> >
PatchToPatchInterpolation<FromPatch, ToPatch>::faceInterpolate
(
  const Field<Type>& ff
) const
{
  if (ff.size() != fromPatch_.size())
  {
    FATAL_ERROR_IN
    (
      "PatchToPatchInterpolation::faceInterpolate"
      "(const Field<Type> ff)"
    )
    << "given field does not correspond to patch. Patch size: "
    << fromPatch_.size() << " field size: " << ff.size()
    << abort(FatalError);
  }

  tmp<Field<Type> > tresult
  {
    new Field<Type>
    {
      toPatch_.size(),
      pTraits<Type>::zero
    }
  };
  Field<Type>& result = tresult();
  const labelListList& fromPatchFaceFaces = fromPatch_.faceFaces();
  const FieldField<Field, scalar>& weights = faceWeights();
  const labelList& addr = faceAddr();
  FOR_ALL(result, faceI)
  {
    const scalarField& curWeights = weights[faceI];
    if (addr[faceI] > -1)
    {
      const labelList& hitFaceFaces =
        fromPatchFaceFaces[addr[faceI]];
      // first add the hit face
      result[faceI] += ff[addr[faceI]]*curWeights[0];
      for (label wI = 1; wI < curWeights.size(); wI++)
      {
        result[faceI] += ff[hitFaceFaces[wI - 1]]*curWeights[wI];
      }
    }
  }

  return tresult;
}


template<class FromPatch, class ToPatch>
template<class Type>
tmp<Field<Type> >
PatchToPatchInterpolation<FromPatch, ToPatch>::faceInterpolate
(
  const tmp<Field<Type> >& tff
) const
{
  tmp<Field<Type> > tint = faceInterpolate(tff());
  tff.clear();
  return tint;
}

}  // namespace mousse
