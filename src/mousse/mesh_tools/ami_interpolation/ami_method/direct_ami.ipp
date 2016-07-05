// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "direct_ami.hpp"


// Private Member Functions 
template<class SourcePatch, class TargetPatch>
void mousse::directAMI<SourcePatch, TargetPatch>::appendToDirectSeeds
(
  labelList& mapFlag,
  labelList& srcTgtSeed,
  DynamicList<label>& srcSeeds,
  DynamicList<label>& nonOverlapFaces,
  label& srcFaceI,
  label& tgtFaceI
) const
{
  const labelList& srcNbr = this->srcPatch_.faceFaces()[srcFaceI];
  const labelList& tgtNbr = this->tgtPatch_.faceFaces()[tgtFaceI];
  const pointField& srcPoints = this->srcPatch_.points();
  const pointField& tgtPoints = this->tgtPatch_.points();
  const vectorField& srcCf = this->srcPatch_.faceCentres();
  FOR_ALL(srcNbr, i) {
    label srcI = srcNbr[i];
    if ((mapFlag[srcI] == 0) && (srcTgtSeed[srcI] == -1)) {
      // first attempt: match by comparing face centres
      const face& srcF = this->srcPatch_[srcI];
      const point& srcC = srcCf[srcI];
      scalar tol = GREAT;
      FOR_ALL(srcF, fpI) {
        const point& p = srcPoints[srcF[fpI]];
        scalar d2 = magSqr(p - srcC);
        if (d2 < tol) {
          tol = d2;
        }
      }
      tol = max(SMALL, 0.0001*sqrt(tol));
      bool found = false;
      FOR_ALL(tgtNbr, j) {
        label tgtI = tgtNbr[j];
        const face& tgtF = this->tgtPatch_[tgtI];
        const point tgtC = tgtF.centre(tgtPoints);
        if (mag(srcC - tgtC) < tol) {
          // new match - append to lists
          found = true;
          srcTgtSeed[srcI] = tgtI;
          srcSeeds.append(srcI);
          break;
        }
      }
      // second attempt: match by shooting a ray into the tgt face
      if (!found) {
        const vector srcN = srcF.normal(srcPoints);
        FOR_ALL(tgtNbr, j) {
          label tgtI = tgtNbr[j];
          const face& tgtF = this->tgtPatch_[tgtI];
          pointHit ray = tgtF.ray(srcCf[srcI], srcN, tgtPoints);
          if (ray.hit()) {
            // new match - append to lists
            found = true;
            srcTgtSeed[srcI] = tgtI;
            srcSeeds.append(srcI);
            break;
          }
        }
      }
      // no match available for source face srcI
      if (!found) {
        mapFlag[srcI] = -1;
        nonOverlapFaces.append(srcI);
        if (debug) {
          Pout << "source face not found: id=" << srcI
            << " centre=" << srcCf[srcI]
            << " normal=" << srcF.normal(srcPoints)
            << " points=" << srcF.points(srcPoints)
            << endl;
          Pout << "target neighbours:" << nl;
          FOR_ALL(tgtNbr, j) {
            label tgtI = tgtNbr[j];
            const face& tgtF = this->tgtPatch_[tgtI];
            Pout << "face id: " << tgtI
              << " centre=" << tgtF.centre(tgtPoints)
              << " normal=" << tgtF.normal(tgtPoints)
              << " points=" << tgtF.points(tgtPoints)
              << endl;
          }
        }
      }
    }
  }
  if (srcSeeds.size()) {
    srcFaceI = srcSeeds.remove();
    tgtFaceI = srcTgtSeed[srcFaceI];
  } else {
    srcFaceI = -1;
    tgtFaceI = -1;
  }
}


template<class SourcePatch, class TargetPatch>
void mousse::directAMI<SourcePatch, TargetPatch>::restartAdvancingFront
(
  labelList& mapFlag,
  DynamicList<label>& nonOverlapFaces,
  label& srcFaceI,
  label& tgtFaceI
) const
{
  FOR_ALL(mapFlag, faceI) {
    if (mapFlag[faceI] == 0) {
      tgtFaceI = this->findTargetFace(faceI);
      if (tgtFaceI < 0) {
        mapFlag[faceI] = -1;
        nonOverlapFaces.append(faceI);
      } else {
        srcFaceI = faceI;
        break;
      }
    }
  }
}


// Constructors 
template<class SourcePatch, class TargetPatch>
mousse::directAMI<SourcePatch, TargetPatch>::directAMI
(
  const SourcePatch& srcPatch,
  const TargetPatch& tgtPatch,
  const scalarField& srcMagSf,
  const scalarField& tgtMagSf,
  const faceAreaIntersect::triangulationMode& triMode,
  const bool reverseTarget,
  const bool requireMatch
)
:
  AMIMethod<SourcePatch, TargetPatch>
  {
    srcPatch,
    tgtPatch,
    srcMagSf,
    tgtMagSf,
    triMode,
    reverseTarget,
    requireMatch
  }
{}


// Destructor
template<class SourcePatch, class TargetPatch>
mousse::directAMI<SourcePatch, TargetPatch>::~directAMI()
{}


// Member Functions 
template<class SourcePatch, class TargetPatch>
void mousse::directAMI<SourcePatch, TargetPatch>::calculate
(
  labelListList& srcAddress,
  scalarListList& srcWeights,
  labelListList& tgtAddress,
  scalarListList& tgtWeights,
  label srcFaceI,
  label tgtFaceI
)
{
  bool ok =
    this->initialise
    (
      srcAddress,
      srcWeights,
      tgtAddress,
      tgtWeights,
      srcFaceI,
      tgtFaceI
    );
  if (!ok) {
    return;
  }
  // temporary storage for addressing and weights
  List<DynamicList<label>> srcAddr{this->srcPatch_.size()};
  List<DynamicList<label>> tgtAddr{this->tgtPatch_.size()};
  // construct weights and addressing
  // list of faces currently visited for srcFaceI to avoid multiple hits
  DynamicList<label> srcSeeds{10};
  // list to keep track of tgt faces used to seed src faces
  labelList srcTgtSeed{srcAddr.size(), -1};
  srcTgtSeed[srcFaceI] = tgtFaceI;
  // list to keep track of whether src face can be mapped
  // 1 = mapped, 0 = untested, -1 = cannot map
  labelList mapFlag{srcAddr.size(), 0};
  label nTested = 0;
  DynamicList<label> nonOverlapFaces;
  do {
    srcAddr[srcFaceI].append(tgtFaceI);
    tgtAddr[tgtFaceI].append(srcFaceI);
    mapFlag[srcFaceI] = 1;
    nTested++;
    // Do advancing front starting from srcFaceI, tgtFaceI
    appendToDirectSeeds
    (
      mapFlag,
      srcTgtSeed,
      srcSeeds,
      nonOverlapFaces,
      srcFaceI,
      tgtFaceI
    );
    if (srcFaceI < 0 && nTested < this->srcPatch_.size()) {
      restartAdvancingFront(mapFlag, nonOverlapFaces, srcFaceI, tgtFaceI);
    }
  } while (srcFaceI >= 0);
  if (nonOverlapFaces.size() != 0) {
    Pout << "    AMI: " << nonOverlapFaces.size()
      << " non-overlap faces identified"
      << endl;
    this->srcNonOverlap_.transfer(nonOverlapFaces);
  }
  // transfer data to persistent storage
  FOR_ALL(srcAddr, i) {
    scalar magSf = this->srcMagSf_[i];
    srcWeights[i] = scalarList(1, magSf);
    srcAddress[i].transfer(srcAddr[i]);
  }
  FOR_ALL(tgtAddr, i) {
    scalar magSf = this->tgtMagSf_[i];
    tgtWeights[i] = scalarList(1, magSf);
    tgtAddress[i].transfer(tgtAddr[i]);
  }
}

