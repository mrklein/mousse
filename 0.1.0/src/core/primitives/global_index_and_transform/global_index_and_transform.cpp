// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "global_index_and_transform.hpp"
#include "cyclic_poly_patch.hpp"
// Private Static Data Members
namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(globalIndexAndTransform, 0);
const label globalIndexAndTransform::base_ = 32;

}
// Private Member Functions
mousse::label mousse::globalIndexAndTransform::matchTransform
(
  const List<vectorTensorTransform>& refTransforms,
  label& matchedRefTransformI,
  const vectorTensorTransform& testTransform,
  scalar tolerance,
  bool checkBothSigns
) const
{
  matchedRefTransformI = -1;
  FOR_ALL(refTransforms, i)
  {
    const vectorTensorTransform& refTransform = refTransforms[i];
    scalar maxVectorMag = sqrt
    (
      max(magSqr(testTransform.t()), magSqr(refTransform.t()))
    );
    // Test the difference between vector parts to see if it is
    // less than tolerance times the larger vector part magnitude.
    scalar vectorDiff =
      mag(refTransform.t() - testTransform.t())
     /(maxVectorMag + VSMALL)
     /tolerance;
    // Test the difference between tensor parts to see if it is
    // less than the tolerance.  sqrt(3.0) factor used to scale
    // differnces as this is magnitude of a rotation tensor.  If
    // neither transform has a rotation, then the test is not
    // necessary.
    scalar tensorDiff = 0;
    if (refTransform.hasR() || testTransform.hasR())
    {
      tensorDiff =
        mag(refTransform.R() - testTransform.R())
       /sqrt(3.0)
       /tolerance;
    }
    // ...Diff result is < 1 if the test part matches the ref part
    // within tolerance
    if (vectorDiff < 1 && tensorDiff < 1)
    {
      matchedRefTransformI = i;
      return +1;
    }
    if (checkBothSigns)
    {
      // Test the inverse transform differences too
      vectorDiff =
        mag(refTransform.t() + testTransform.t())
       /(maxVectorMag + VSMALL)
       /tolerance;
      tensorDiff = 0;
      if (refTransform.hasR() || testTransform.hasR())
      {
        tensorDiff =
          mag(refTransform.R() - testTransform.R().T())
         /sqrt(3.0)
         /tolerance;
      }
      if (vectorDiff < 1 && tensorDiff < 1)
      {
        matchedRefTransformI = i;
        return -1;
      }
    }
  }
  return 0;
}
void mousse::globalIndexAndTransform::determineTransforms()
{
  const polyBoundaryMesh& patches = mesh_.boundaryMesh();
  transforms_ = List<vectorTensorTransform>(6);
  scalarField maxTol(6);
  label nextTrans = 0;
  label dummyMatch = -1;
  FOR_ALL(patches, patchI)
  {
    const polyPatch& pp = patches[patchI];
    // Note: special check for unordered cyclics. These are in fact
    // transform bcs and should probably be split off.
    if
    (
      isA<coupledPolyPatch>(pp)
    && !(
        isA<cyclicPolyPatch>(pp)
      && (
          refCast<const cyclicPolyPatch>(pp).transform()
        == cyclicPolyPatch::NOORDERING
        )
      )
    )
    {
      const coupledPolyPatch& cpp = refCast<const coupledPolyPatch>(pp);
      if (cpp.separated())
      {
        const vectorField& sepVecs = cpp.separation();
        FOR_ALL(sepVecs, sVI)
        {
          const vector& sepVec = sepVecs[sVI];
          if (mag(sepVec) > SMALL)
          {
            vectorTensorTransform transform(sepVec);
            if
            (
              matchTransform
              (
                transforms_,
                dummyMatch,
                transform,
                cpp.matchTolerance(),
                false
              ) == 0
            )
            {
              if (nextTrans == 6)
              {
                FATAL_ERROR_IN
                (
                  "void mousse::globalIndexAndTransform::"
                  "determineTransforms()"
                )   << "More than six unsigned transforms"
                  << " detected:" << nl << transforms_
                  << exit(FatalError);
              }
              transforms_[nextTrans] = transform;
              maxTol[nextTrans++] = cpp.matchTolerance();
            }
          }
        }
      }
      else if (!cpp.parallel())
      {
        const tensorField& transTensors = cpp.reverseT();
        FOR_ALL(transTensors, tTI)
        {
          const tensor& transT = transTensors[tTI];
          if (mag(transT - I) > SMALL)
          {
            vectorTensorTransform transform(transT);
            if
            (
              matchTransform
              (
                transforms_,
                dummyMatch,
                transform,
                cpp.matchTolerance(),
                false
              ) == 0
            )
            {
              if (nextTrans == 6)
              {
                FATAL_ERROR_IN
                (
                  "void mousse::globalIndexAndTransform::"
                  "determineTransforms()"
                )   << "More than six unsigned transforms"
                  << " detected:" << nl << transforms_
                  << exit(FatalError);
              }
              transforms_[nextTrans] = transform;
              maxTol[nextTrans++] = cpp.matchTolerance();
            }
          }
        }
      }
    }
  }
  // Collect transforms on master
  List<List<vectorTensorTransform> > allTransforms(Pstream::nProcs());
  allTransforms[Pstream::myProcNo()] = transforms_;
  Pstream::gatherList(allTransforms);
  // Collect matching tolerance on master
  List<scalarField> allTols(Pstream::nProcs());
  allTols[Pstream::myProcNo()] = maxTol;
  Pstream::gatherList(allTols);
  if (Pstream::master())
  {
    transforms_ = List<vectorTensorTransform>(3);
    label nextTrans = 0;
    FOR_ALL(allTransforms, procI)
    {
      const List<vectorTensorTransform>& procTransVecs =
        allTransforms[procI];
      FOR_ALL(procTransVecs, pSVI)
      {
        const vectorTensorTransform& transform = procTransVecs[pSVI];
        if (mag(transform.t()) > SMALL || transform.hasR())
        {
          if
          (
            matchTransform
            (
              transforms_,
              dummyMatch,
              transform,
              allTols[procI][pSVI],
              true
            ) ==  0
          )
          {
            transforms_[nextTrans++] = transform;
          }
          if (nextTrans > 3)
          {
            FATAL_ERROR_IN
            (
              "void mousse::globalIndexAndTransform::"
              "determineTransforms()"
            )
              << "More than three independent basic "
              << "transforms detected:" << nl
              << allTransforms
              << transforms_
              << exit(FatalError);
          }
        }
      }
    }
    transforms_.setSize(nextTrans);
  }
  Pstream::scatter(transforms_);
  if (transforms_.size() > 3)
  {
    WARNING_IN
    (
      "void globalIndexAndTransform::determineTransforms()"
    )   << "More than three independent basic "
      << "transforms detected:" << nl
      << transforms_ << nl
      << "This is not a space filling tiling and will probably"
      << " give problems for e.g. lagrangian tracking or interpolation"
      << endl;
  }
}
void mousse::globalIndexAndTransform::determineTransformPermutations()
{
  label nTransformPermutations = pow(label(3), transforms_.size());
  transformPermutations_.setSize(nTransformPermutations);
  FOR_ALL(transformPermutations_, tPI)
  {
    vectorTensorTransform transform;
    label transformIndex = tPI;
    // Invert the ternary index encoding using repeated division by
    // three
    FOR_ALL(transforms_, b)
    {
      const label w = (transformIndex % 3) - 1;
      transformIndex /= 3;
      if (w > 0)
      {
        transform &= transforms_[b];
      }
      else if (w < 0)
      {
        transform &= inv(transforms_[b]);
      }
    }
    transformPermutations_[tPI] = transform;
  }
  // Encode index for 0 sign
  labelList permutationIndices(nIndependentTransforms(), 0);
  nullTransformIndex_ = encodeTransformIndex(permutationIndices);
}
void mousse::globalIndexAndTransform::determinePatchTransformSign()
{
  const polyBoundaryMesh& patches = mesh_.boundaryMesh();
  patchTransformSign_.setSize(patches.size(), Pair<label>(-1, 0));
  label matchTransI = -1;
  FOR_ALL(patches, patchI)
  {
    const polyPatch& pp = patches[patchI];
    // Pout<< nl << patchI << " " << pp.name() << endl;
    // Note: special check for unordered cyclics. These are in fact
    // transform bcs and should probably be split off.
    if
    (
      isA<coupledPolyPatch>(pp)
    && !(
        isA<cyclicPolyPatch>(pp)
      && (
          refCast<const cyclicPolyPatch>(pp).transform()
        == cyclicPolyPatch::NOORDERING
        )
      )
    )
    {
      const coupledPolyPatch& cpp =
      refCast<const coupledPolyPatch>(pp);
      if (cpp.separated())
      {
        const vectorField& sepVecs = cpp.separation();
        // Pout<< "sepVecs " << sepVecs << endl;
        // This loop is implicitly expecting only a single
        // value for separation()
        FOR_ALL(sepVecs, sVI)
        {
          const vector& sepVec = sepVecs[sVI];
          if (mag(sepVec) > SMALL)
          {
            vectorTensorTransform t(sepVec);
            label sign = matchTransform
            (
              transforms_,
              matchTransI,
              t,
              cpp.matchTolerance(),
              true
            );
            // Pout<< sign << " " << matchTransI << endl;
            // List<label> permutation(transforms_.size(), 0);
            // permutation[matchTransI] = sign;
            // Pout<< encodeTransformIndex(permutation) << nl
            //     << transformPermutations_
            //        [
            //            encodeTransformIndex(permutation)
            //        ]
            //     << endl;
            patchTransformSign_[patchI] =
              Pair<label>(matchTransI, sign);
          }
        }
      }
      else if (!cpp.parallel())
      {
        const tensorField& transTensors = cpp.reverseT();
        // Pout<< "transTensors " << transTensors << endl;
        // This loop is implicitly expecting only a single
        // value for reverseT()
        FOR_ALL(transTensors, tTI)
        {
          const tensor& transT = transTensors[tTI];
          if (mag(transT - I) > SMALL)
          {
            vectorTensorTransform t(transT);
            label sign = matchTransform
            (
              transforms_,
              matchTransI,
              t,
              cpp.matchTolerance(),
              true
            );
            // Pout<< sign << " " << matchTransI << endl;
            // List<label> permutation(transforms_.size(), 0);
            // permutation[matchTransI] = sign;
            // Pout<< encodeTransformIndex(permutation) << nl
            //     << transformPermutations_
            //        [
            //            encodeTransformIndex(permutation)
            //        ]
            //     << endl;
            patchTransformSign_[patchI] =
              Pair<label>(matchTransI, sign);
          }
        }
      }
    }
  }
  // Pout<< patchTransformSign_ << endl;
}
// Constructors
mousse::globalIndexAndTransform::globalIndexAndTransform
(
  const polyMesh& mesh
)
:
  mesh_(mesh),
  transforms_(),
  transformPermutations_(),
  patchTransformSign_()
{
  determineTransforms();
  determineTransformPermutations();
  determinePatchTransformSign();
  if (debug && transforms_.size() > 0)
  {
    const polyBoundaryMesh& patches = mesh_.boundaryMesh();
    Info<< "Determined global transforms :" << endl;
    Info<< "\t\ttranslation\trotation" << endl;
    FOR_ALL(transforms_, i)
    {
      Info<< '\t' << i << '\t';
      const vectorTensorTransform& trafo = transforms_[i];
      if (trafo.hasR())
      {
        Info<< trafo.t() << '\t' << trafo.R();
      }
      else
      {
        Info<< trafo.t() << '\t' << "---";
      }
      Info<< endl;
    }
    Info<< endl;
    Info<< "\tpatch\ttransform\tsign" << endl;
    FOR_ALL(patchTransformSign_, patchI)
    {
      if (patchTransformSign_[patchI].first() != -1)
      {
        Info
          << '\t' << patches[patchI].name()
          << '\t' << patchTransformSign_[patchI].first()
          << '\t' << patchTransformSign_[patchI].second()
          << endl;
      }
    }
    Info<< endl;
    Info<< "Permutations of transformations:" << endl
      << "\t\ttranslation\trotation" << endl;
    FOR_ALL(transformPermutations_, i)
    {
      Info<< '\t' << i << '\t';
      const vectorTensorTransform& trafo = transformPermutations_[i];
      if (trafo.hasR())
      {
        Info<< trafo.t() << '\t' << trafo.R();
      }
      else
      {
        Info<< trafo.t() << '\t' << "---";
      }
      Info<< endl;
    }
    Info<< "nullTransformIndex:" << nullTransformIndex() << endl
      << endl;
  }
}

// Destructor
mousse::globalIndexAndTransform::~globalIndexAndTransform()
{}
