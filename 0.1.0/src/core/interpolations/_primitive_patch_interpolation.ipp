// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_primitive_patch_interpolation.hpp"
#include "face_list.hpp"
#include "demand_driven_data.hpp"


namespace mousse {

// Private Member Functions 
template<class Patch>
const scalarListList&
PrimitivePatchInterpolation<Patch>::faceToPointWeights() const
{
  if (!faceToPointWeightsPtr_) {
    makeFaceToPointWeights();
  }
  return *faceToPointWeightsPtr_;
}


template<class Patch>
void PrimitivePatchInterpolation<Patch>::makeFaceToPointWeights() const
{
  if (faceToPointWeightsPtr_) {
    FATAL_ERROR_IN
    (
      "PrimitivePatchInterpolation<Patch>::makeFaceToPointWeights() const"
    )
    << "Face-to-edge weights already calculated"
    << abort(FatalError);
  }
  const pointField& points = patch_.localPoints();
  const List<typename Patch::FaceType>& faces = patch_.localFaces();
  faceToPointWeightsPtr_ = new scalarListList(points.size());
  scalarListList& weights = *faceToPointWeightsPtr_;
  // get reference to addressing
  const labelListList& pointFaces = patch_.pointFaces();
  FOR_ALL(pointFaces, pointi) {
    const labelList& curFaces = pointFaces[pointi];
    scalarList& pw = weights[pointi];
    pw.setSize(curFaces.size());
    scalar sumw = 0.0;
    FOR_ALL(curFaces, facei) {
      pw[facei] =
        1.0/mag(faces[curFaces[facei]].centre(points) - points[pointi]);
      sumw += pw[facei];
    }
    FOR_ALL(curFaces, facei) {
      pw[facei] /= sumw;
    }
  }
}


template<class Patch>
const scalarList&
PrimitivePatchInterpolation<Patch>::faceToEdgeWeights() const
{
  if (!faceToEdgeWeightsPtr_) {
    makeFaceToEdgeWeights();
  }
  return *faceToEdgeWeightsPtr_;
}


template<class Patch>
void PrimitivePatchInterpolation<Patch>::makeFaceToEdgeWeights() const
{
  if (faceToEdgeWeightsPtr_) {
    FATAL_ERROR_IN
    (
      "PrimitivePatchInterpolation<Patch>::makeFaceToEdgeWeights() const"
    )
    << "Face-to-edge weights already calculated"
    << abort(FatalError);
  }
  const pointField& points = patch_.localPoints();
  const List<typename Patch::FaceType>& faces = patch_.localFaces();
  const edgeList& edges = patch_.edges();
  const labelListList& edgeFaces = patch_.edgeFaces();
  faceToEdgeWeightsPtr_ = new scalarList(patch_.nInternalEdges());
  scalarList& weights = *faceToEdgeWeightsPtr_;
  FOR_ALL(weights, edgei) {
    vector P = faces[edgeFaces[edgei][0]].centre(points);
    vector N = faces[edgeFaces[edgei][1]].centre(points);
    vector S = points[edges[edgei].start()];
    vector e = edges[edgei].vec(points);
    scalar alpha =
      -(((N - P)^(S - P))&((N - P)^e))/(((N - P)^e )&((N - P)^e));
    vector E = S + alpha*e;
    weights[edgei] = mag(N - E)/(mag(N - E) + mag(E - P));
  }
}


template<class Patch>
void PrimitivePatchInterpolation<Patch>::clearWeights()
{
  deleteDemandDrivenData(faceToPointWeightsPtr_);
  deleteDemandDrivenData(faceToEdgeWeightsPtr_);
}


// Constructors 
template<class Patch>
PrimitivePatchInterpolation<Patch>::PrimitivePatchInterpolation(const Patch& p)
:
  patch_{p},
  faceToPointWeightsPtr_{NULL},
  faceToEdgeWeightsPtr_{NULL}
{}


// Destructor
template<class Patch>
PrimitivePatchInterpolation<Patch>::~PrimitivePatchInterpolation()
{
  clearWeights();
}


// Member Functions 
template<class Patch>
template<class Type>
tmp<Field<Type>> PrimitivePatchInterpolation<Patch>::faceToPointInterpolate
(
  const Field<Type>& ff
) const
{
  // Check size of the given field
  if (ff.size() != patch_.size()) {
    FATAL_ERROR_IN
    (
      "tmp<Field<Type>> PrimitivePatchInterpolation::"
      "faceToPointInterpolate(const Field<Type> ff)"
    )
    << "given field does not correspond to patch. Patch size: "
    << patch_.size() << " field size: " << ff.size()
    << abort(FatalError);
  }
  tmp<Field<Type>> tresult
  {
    new Field<Type>{patch_.nPoints(), pTraits<Type>::zero}
  };
  Field<Type>& result = tresult();
  const labelListList& pointFaces = patch_.pointFaces();
  const scalarListList& weights = faceToPointWeights();
  FOR_ALL(pointFaces, pointi) {
    const labelList& curFaces = pointFaces[pointi];
    const scalarList& w = weights[pointi];
    FOR_ALL(curFaces, facei) {
      result[pointi] += w[facei]*ff[curFaces[facei]];
    }
  }

  return tresult;
}


template<class Patch>
template<class Type>
tmp<Field<Type>> PrimitivePatchInterpolation<Patch>::faceToPointInterpolate
(
  const tmp<Field<Type>>& tff
) const
{
  tmp<Field<Type>> tint = faceToPointInterpolate(tff());
  tff.clear();
  return tint;
}


template<class Patch>
template<class Type>
tmp<Field<Type>> PrimitivePatchInterpolation<Patch>::pointToFaceInterpolate
(
  const Field<Type>& pf
) const
{
  if (pf.size() != patch_.nPoints()) {
    FATAL_ERROR_IN
    (
      "tmp<Field<Type>> PrimitivePatchInterpolation::"
      "pointToFaceInterpolate(const Field<Type> pf)"
    )
    << "given field does not correspond to patch. Patch size: "
    << patch_.nPoints() << " field size: " << pf.size()
    << abort(FatalError);
  }
  tmp<Field<Type>> tresult
  {
    new Field<Type>{patch_.size(), pTraits<Type>::zero}
  };
  Field<Type>& result = tresult();
  const List<typename Patch::FaceType>& localFaces = patch_.localFaces();
  FOR_ALL(result, facei) {
    const labelList& curPoints = localFaces[facei];
    FOR_ALL(curPoints, pointi) {
      result[facei] += pf[curPoints[pointi]];
    }
    result[facei] /= curPoints.size();
  }

  return tresult;
}


template<class Patch>
template<class Type>
tmp<Field<Type>> PrimitivePatchInterpolation<Patch>::pointToFaceInterpolate
(
  const tmp<Field<Type>>& tpf
) const
{
  tmp<Field<Type>> tint = pointToFaceInterpolate(tpf());
  tpf.clear();
  return tint;
}


template<class Patch>
template<class Type>
tmp<Field<Type>> PrimitivePatchInterpolation<Patch>::faceToEdgeInterpolate
(
  const Field<Type>& pf
) const
{
  // Check size of the given field
  if (pf.size() != patch_.size()) {
    FATAL_ERROR_IN
    (
      "tmp<Field<Type>> PrimitivePatchInterpolation::"
      "faceToEdgeInterpolate(const Field<Type> ff)"
    )
    << "given field does not correspond to patch. Patch size: "
    << patch_.size() << " field size: " << pf.size()
    << abort(FatalError);
  }
  tmp<Field<Type>> tresult
  {
    new Field<Type>{patch_.nEdges(), pTraits<Type>::zero}
  };
  Field<Type>& result = tresult();
  const edgeList& edges = patch_.edges();
  const labelListList& edgeFaces = patch_.edgeFaces();
  const scalarList& weights = faceToEdgeWeights();
  for (label edgei = 0; edgei < patch_.nInternalEdges(); edgei++) {
    result[edgei] =
      weights[edgei]*pf[edgeFaces[edgei][0]]
      + (1.0 - weights[edgei])*pf[edgeFaces[edgei][1]];
  }
  for (label edgei = patch_.nInternalEdges(); edgei < edges.size(); edgei++) {
    result[edgei] = pf[edgeFaces[edgei][0]];
  }

  return tresult;
}


template<class Patch>
template<class Type>
tmp<Field<Type>> PrimitivePatchInterpolation<Patch>::faceToEdgeInterpolate
(
  const tmp<Field<Type>>& tpf
) const
{
  tmp<Field<Type>> tint = faceToEdgeInterpolate(tpf());
  tpf.clear();
  return tint;
}


template<class Patch>
bool PrimitivePatchInterpolation<Patch>::movePoints()
{
  clearWeights();
  return true;
}

}  // namespace mousse
