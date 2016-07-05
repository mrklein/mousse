// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_injection_base.hpp"
#include "poly_mesh.hpp"
#include "sub_field.hpp"
#include "cached_random.hpp"
#include "tri_point_ref.hpp"


// Constructors 
mousse::patchInjectionBase::patchInjectionBase
(
  const polyMesh& mesh,
  const word& patchName
)
:
  patchName_{patchName},
  patchId_{mesh.boundaryMesh().findPatchID(patchName_)},
  patchArea_{0.0},
  patchNormal_{},
  cellOwners_{},
  triFace_{},
  triToFace_{},
  triCumulativeMagSf_{},
  sumTriMagSf_{Pstream::nProcs() + 1, 0.0}
{
  if (patchId_ < 0) {
    FATAL_ERROR_IN
    (
      "patchInjectionBase::patchInjectionBase"
      "("
      "  const polyMesh& mesh, "
      "  const word& patchName"
      ")"
    )
    << "Requested patch " << patchName_ << " not found" << nl
    << "Available patches are: " << mesh.boundaryMesh().names() << nl
    << exit(FatalError);
  }
  updateMesh(mesh);
}


mousse::patchInjectionBase::patchInjectionBase(const patchInjectionBase& pib)
:
  patchName_{pib.patchName_},
  patchId_{pib.patchId_},
  patchArea_{pib.patchArea_},
  patchNormal_{pib.patchNormal_},
  cellOwners_{pib.cellOwners_},
  triFace_{pib.triFace_},
  triToFace_{pib.triToFace_},
  triCumulativeMagSf_{pib.triCumulativeMagSf_},
  sumTriMagSf_{pib.sumTriMagSf_}
{}


// Destructor 
mousse::patchInjectionBase::~patchInjectionBase()
{}


// Member Functions 
void mousse::patchInjectionBase::updateMesh(const polyMesh& mesh)
{
  // Set/cache the injector cells
  const polyPatch& patch = mesh.boundaryMesh()[patchId_];
  const pointField& points = patch.points();
  cellOwners_ = patch.faceCells();
  // Triangulate the patch faces and create addressing
  DynamicList<label> triToFace{2*patch.size()};
  DynamicList<scalar> triMagSf{2*patch.size()};
  DynamicList<face> triFace{2*patch.size()};
  DynamicList<face> tris{5};
  // Set zero value at the start of the tri area list
  triMagSf.append(0.0);
  FOR_ALL(patch, faceI) {
    const face& f = patch[faceI];
    tris.clear();
    f.triangles(points, tris);
    FOR_ALL(tris, i) {
      triToFace.append(faceI);
      triFace.append(tris[i]);
      triMagSf.append(tris[i].mag(points));
    }
  }
  FOR_ALL(sumTriMagSf_, i) {
    sumTriMagSf_[i] = 0.0;
  }
  sumTriMagSf_[Pstream::myProcNo() + 1] = sum(triMagSf);
  Pstream::listCombineGather(sumTriMagSf_, maxEqOp<scalar>());
  Pstream::listCombineScatter(sumTriMagSf_);
  for (label i = 1; i < triMagSf.size(); i++) {
    triMagSf[i] += triMagSf[i-1];
  }
  // Transfer to persistent storage
  triFace_.transfer(triFace);
  triToFace_.transfer(triToFace);
  triCumulativeMagSf_.transfer(triMagSf);
  // Convert sumTriMagSf_ into cumulative sum of areas per proc
  for (label i = 1; i < sumTriMagSf_.size(); i++) {
    sumTriMagSf_[i] += sumTriMagSf_[i-1];
  }
  const scalarField magSf(mag(patch.faceAreas()));
  patchArea_ = sum(magSf);
  patchNormal_ = patch.faceAreas()/magSf;
  reduce(patchArea_, sumOp<scalar>());
}


void mousse::patchInjectionBase::setPositionAndCell
(
  const polyMesh& mesh,
  cachedRandom& rnd,
  vector& position,
  label& cellOwner,
  label& tetFaceI,
  label& tetPtI
)
{
  scalar areaFraction = 0;
  if (Pstream::master()) {
    areaFraction = rnd.position<scalar>(0, patchArea_);
  }
  Pstream::scatter(areaFraction);
  if (cellOwners_.size() > 0) {
    // Determine which processor to inject from
    label procI = 0;
    FOR_ALL_REVERSE(sumTriMagSf_, i) {
      if (areaFraction >= sumTriMagSf_[i]) {
        procI = i;
        break;
      }
    }
    if (Pstream::myProcNo() == procI) {
      // Find corresponding decomposed face triangle
      label triI = 0;
      scalar offset = sumTriMagSf_[procI];
      FOR_ALL_REVERSE(triCumulativeMagSf_, i) {
        if (areaFraction > triCumulativeMagSf_[i] + offset) {
          triI = i;
          break;
        }
      }
      // Set cellOwner
      label faceI = triToFace_[triI];
      cellOwner = cellOwners_[faceI];
      // Find random point in triangle
      const polyPatch& patch = mesh.boundaryMesh()[patchId_];
      const pointField& points = patch.points();
      const face& tf = triFace_[triI];
      const triPointRef tri{points[tf[0]], points[tf[1]], points[tf[2]]};
      const point pf{tri.randomPoint(rnd)};
      // Position perturbed away from face (into domain)
      const scalar a = rnd.position(scalar(0.1), scalar(0.5));
      const vector& pc = mesh.cellCentres()[cellOwner];
      const vector d = mag(pf - pc)*patchNormal_[faceI];
      position = pf - a*d;
      // The position is between the face and cell centre, which could
      // be in any tet of the decomposed cell, so arbitrarily choose the
      // first face of the cell as the tetFace and the first point after
      // the base point on the face as the tetPt.  The tracking will pick
      // the cell consistent with the motion in the first tracking step
      tetFaceI = mesh.cells()[cellOwner][0];
      tetPtI = 1;
    } else {
      cellOwner = -1;
      tetFaceI = -1;
      tetPtI = -1;
      // Dummy position
      position = pTraits<vector>::max;
    }
  } else {
    cellOwner = -1;
    tetFaceI = -1;
    tetPtI = -1;
    // Dummy position
    position = pTraits<vector>::max;
  }
}

