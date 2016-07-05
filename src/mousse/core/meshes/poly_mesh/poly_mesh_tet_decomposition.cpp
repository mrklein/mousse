// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh_tet_decomposition.hpp"
#include "pstream_reduce_ops.hpp"


// Static Data Members
// Note: the use of this tolerance is ad-hoc, there may be extreme
// cases where the resulting tetrahedra still have particle tracking
// problems, or tets with lower quality may track OK.
const mousse::scalar mousse::polyMeshTetDecomposition::minTetQuality = sqr(SMALL);


// Static Member Functions
mousse::label mousse::polyMeshTetDecomposition::findSharedBasePoint
(
  const polyMesh& mesh,
  label fI,
  const point& nCc,
  scalar tol,
  bool /*report*/
)
{
  const faceList& pFaces = mesh.faces();
  const pointField& pPts = mesh.points();
  const vectorField& pC = mesh.cellCentres();
  const labelList& pOwner = mesh.faceOwner();
  const face& f = pFaces[fI];
  label oCI = pOwner[fI];
  const point& oCc = pC[oCI];
  List<scalar> tetQualities{2, 0.0};
  FOR_ALL(f, faceBasePtI) {
    scalar thisBaseMinTetQuality = VGREAT;
    const point& tetBasePt = pPts[f[faceBasePtI]];
    for (label tetPtI = 1; tetPtI < f.size() - 1; tetPtI++) {
      label facePtI = (tetPtI + faceBasePtI) % f.size();
      label otherFacePtI = f.fcIndex(facePtI);

      {
        // owner cell tet
        label ptAI = f[facePtI];
        label ptBI = f[otherFacePtI];
        const point& pA = pPts[ptAI];
        const point& pB = pPts[ptBI];
        tetPointRef tet{oCc, tetBasePt, pA, pB};
        tetQualities[0] = tet.quality();
      }

      {
        // neighbour cell tet
        label ptAI = f[otherFacePtI];
        label ptBI = f[facePtI];
        const point& pA = pPts[ptAI];
        const point& pB = pPts[ptBI];
        tetPointRef tet(nCc, tetBasePt, pA, pB);
        tetQualities[1] = tet.quality();
      }

      if (min(tetQualities) < thisBaseMinTetQuality) {
        thisBaseMinTetQuality = min(tetQualities);
      }
    }
    if (thisBaseMinTetQuality > tol) {
      return faceBasePtI;
    }
  }
  // If a base point hasn't triggered a return by now, then there is
  // none that can produce a good decomposition
  return -1;
}


mousse::label mousse::polyMeshTetDecomposition::findSharedBasePoint
(
  const polyMesh& mesh,
  label fI,
  scalar tol,
  bool report
)
{
  return findSharedBasePoint
  (
    mesh,
    fI,
    mesh.cellCentres()[mesh.faceNeighbour()[fI]],
    tol,
    report
  );
}


mousse::label mousse::polyMeshTetDecomposition::findBasePoint
(
  const polyMesh& mesh,
  label fI,
  scalar tol,
  bool /*report*/
)
{
  const faceList& pFaces = mesh.faces();
  const pointField& pPts = mesh.points();
  const vectorField& pC = mesh.cellCentres();
  const labelList& pOwner = mesh.faceOwner();
  const face& f = pFaces[fI];
  label cI = pOwner[fI];
  bool own = (pOwner[fI] == cI);
  const point& cC = pC[cI];
  FOR_ALL(f, faceBasePtI) {
    scalar thisBaseMinTetQuality = VGREAT;
    const point& tetBasePt = pPts[f[faceBasePtI]];
    for (label tetPtI = 1; tetPtI < f.size() - 1; tetPtI++) {
      label facePtI = (tetPtI + faceBasePtI) % f.size();
      label otherFacePtI = f.fcIndex(facePtI);
      label ptAI = -1;
      label ptBI = -1;
      if (own) {
        ptAI = f[facePtI];
        ptBI = f[otherFacePtI];
      } else {
        ptAI = f[otherFacePtI];
        ptBI = f[facePtI];
      }
      const point& pA = pPts[ptAI];
      const point& pB = pPts[ptBI];
      tetPointRef tet{cC, tetBasePt, pA, pB};
      scalar tetQuality = tet.quality();
      if (tetQuality < thisBaseMinTetQuality) {
        thisBaseMinTetQuality = tetQuality;
      }
    }
    if (thisBaseMinTetQuality > tol) {
      return faceBasePtI;
    }
  }
  // If a base point hasn't triggered a return by now, then there is
  // none that can produce a good decomposition
  return -1;
}


mousse::labelList mousse::polyMeshTetDecomposition::findFaceBasePts
(
  const polyMesh& mesh,
  scalar tol,
  bool report
)
{
  const labelList& pOwner = mesh.faceOwner();
  const vectorField& pC = mesh.cellCentres();
  // Find a suitable base point for each face, considering both
  // cells for interface faces or those on coupled patches
  labelList tetBasePtIs(mesh.nFaces(), -1);
  label nInternalFaces = mesh.nInternalFaces();
  for (label fI = 0; fI < nInternalFaces; fI++) {
    tetBasePtIs[fI] = findSharedBasePoint(mesh, fI, tol, report);
  }
  pointField neighbourCellCentres(mesh.nFaces() - nInternalFaces);
  for(label faceI = nInternalFaces; faceI < mesh.nFaces(); faceI++) {
    neighbourCellCentres[faceI - nInternalFaces] = pC[pOwner[faceI]];
  }
  syncTools::swapBoundaryFacePositions(mesh, neighbourCellCentres);
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  SubList<label> boundaryFaceTetBasePtIs
  {
    tetBasePtIs,
    mesh.nFaces() - nInternalFaces,
    nInternalFaces
  };
  for
  (
    label fI = nInternalFaces, bFI = 0;
    fI < mesh.nFaces();
    fI++, bFI++
  ) {
    label patchI = mesh.boundaryMesh().patchID()[bFI];
    if (patches[patchI].coupled()) {
      const coupledPolyPatch& pp =
        refCast<const coupledPolyPatch>(patches[patchI]);
      if (pp.owner()) {
        boundaryFaceTetBasePtIs[bFI] = findSharedBasePoint
        (
          mesh,
          fI,
          neighbourCellCentres[bFI],
          tol,
          report
        );
      } else {
        // Assign -2, to distinguish from a failed base point
        // find, which returns -1.
        boundaryFaceTetBasePtIs[bFI] = -2;
      }
    } else {
      boundaryFaceTetBasePtIs[bFI] = findBasePoint
      (
        mesh,
        fI,
        tol,
        report
      );
    }
  }
  // maxEqOp will replace the -2 values on the neighbour patches
  // with the result from the owner base point find.
  syncTools::syncBoundaryFaceList
  (
    mesh,
    boundaryFaceTetBasePtIs,
    maxEqOp<label>()
  );
  for
  (
    label fI = nInternalFaces, bFI = 0;
    fI < mesh.nFaces();
    fI++, bFI++
  ) {
    label& bFTetBasePtI = boundaryFaceTetBasePtIs[bFI];
    if (bFTetBasePtI == -2) {
      FATAL_ERROR_IN
      (
        "labelList"
        "polyMeshTetDecomposition::findFaceBasePts"
        "("
          "const polyMesh& mesh, "
          "scalar tol, "
          "bool report"
        ")"
      )
      << "Coupled face base point exchange failure for face "
      << fI
      << abort(FatalError);
    }
    if (bFTetBasePtI < 1) {
      // If the base point is -1, it should be left as such to
      // indicate a problem, if it is 0, then no action is required.
      continue;
    }
    label patchI = mesh.boundaryMesh().patchID()[bFI];
    if (patches[patchI].coupled()) {
      const coupledPolyPatch& pp =
        refCast<const coupledPolyPatch>(patches[patchI]);
      // Calculated base points on coupled faces are those of
      // the owner patch face. They need to be reindexed to for
      // the non-owner face, which has the opposite order.
      // So, for fPtI_o != 0, fPtI_n = f.size() - fPtI_o
      // i.e.:
      // owner coupledPolyPatch face
      // face    (a b c d e f)
      // fPtI     0 1 2 3 4 5
      //            +
      //              #
      // neighbour coupledPolyPatch face
      // face    (a f e d c b)
      // fPtI     0 1 2 3 4 5
      //                    +
      //                  #
      // +: 6 - 1 = 5
      // #: 6 - 2 = 4
      if (!pp.owner()) {
        bFTetBasePtI = mesh.faces()[fI].size() - bFTetBasePtI;
      }
    }
  }
  return tetBasePtIs;
}


bool mousse::polyMeshTetDecomposition::checkFaceTets
(
  const polyMesh& mesh,
  scalar tol,
  const bool report,
  labelHashSet* setPtr
)
{
  const labelList& own = mesh.faceOwner();
  const labelList& nei = mesh.faceNeighbour();
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  const vectorField& cc = mesh.cellCentres();
  const vectorField& fc = mesh.faceCentres();
  // Calculate coupled cell centre
  pointField neiCc(mesh.nFaces() - mesh.nInternalFaces());
  for (label faceI = mesh.nInternalFaces(); faceI < mesh.nFaces(); faceI++) {
    neiCc[faceI - mesh.nInternalFaces()] = cc[own[faceI]];
  }
  syncTools::swapBoundaryFacePositions(mesh, neiCc);
  const faceList& fcs = mesh.faces();
  const pointField& p = mesh.points();
  label nErrorTets = 0;
  FOR_ALL(fcs, faceI) {
    const face& f = fcs[faceI];
    FOR_ALL(f, fPtI) {
      scalar tetQual = tetPointRef
      (
        p[f[fPtI]],
        p[f.nextLabel(fPtI)],
        fc[faceI],
        cc[own[faceI]]
      ).quality();
      if (tetQual > -tol) {
        if (setPtr) {
          setPtr->insert(faceI);
        }
        nErrorTets++;
        break;              // no need to check other tets
      }
    }
    if (mesh.isInternalFace(faceI)) {
      // Create the neighbour tet - it will have positive volume
      const face& f = fcs[faceI];
      FOR_ALL(f, fPtI) {
        scalar tetQual = tetPointRef
        (
          p[f[fPtI]],
          p[f.nextLabel(fPtI)],
          fc[faceI],
          cc[nei[faceI]]
        ).quality();
        if (tetQual < tol) {
          if (setPtr) {
            setPtr->insert(faceI);
          }
          nErrorTets++;
          break;
        }
      }
      if (findSharedBasePoint(mesh, faceI, tol, report) == -1) {
        if (setPtr) {
          setPtr->insert(faceI);
        }
        nErrorTets++;
      }
    } else {
      label patchI = patches.patchID()[faceI - mesh.nInternalFaces()];
      if (patches[patchI].coupled()) {
        if
        (
          findSharedBasePoint
          (
            mesh,
            faceI,
            neiCc[faceI - mesh.nInternalFaces()],
            tol,
            report
          ) == -1
        ) {
          if (setPtr) {
            setPtr->insert(faceI);
          }
          nErrorTets++;
        }
      } else {
        if (findBasePoint(mesh, faceI, tol, report) == -1) {
          if (setPtr) {
            setPtr->insert(faceI);
          }
          nErrorTets++;
        }
      }
    }
  }
  reduce(nErrorTets, sumOp<label>());
  if (nErrorTets > 0) {
    if (report) {
      Info << " ***Error in face tets: "
        << nErrorTets << " faces with low quality or negative volume "
        << "decomposition tets." << endl;
    }
    return true;
  } else {
    if (report) {
      Info << "    Face tets OK." << endl;
    }
    return false;
  }
}


mousse::List<mousse::tetIndices> mousse::polyMeshTetDecomposition::faceTetIndices
(
  const polyMesh& mesh,
  label fI,
  label cI
)
{
  static label nWarnings = 0;
  static const label maxWarnings = 100;
  const faceList& pFaces = mesh.faces();
  const labelList& pOwner = mesh.faceOwner();
  const face& f = pFaces[fI];
  label nTets = f.size() - 2;
  List<tetIndices> faceTets{nTets};
  bool own = (pOwner[fI] == cI);
  label tetBasePtI = mesh.tetBasePtIs()[fI];
  if (tetBasePtI == -1) {
    if (nWarnings < maxWarnings) {
      WARNING_IN
      (
        "List<tetIndices> "
        "polyMeshTetDecomposition::faceTetIndices"
        "("
          "const polyMesh&, "
          "label, "
          "label"
        ")"
      )
      << "No base point for face " << fI << ", " << f
      << ", produces a valid tet decomposition."
      << endl;
      nWarnings++;
    }
    if (nWarnings == maxWarnings) {
      Warning<< "Suppressing any further warnings." << endl;
      nWarnings++;
    }
    tetBasePtI = 0;
  }
  for (label tetPtI = 1; tetPtI < f.size() - 1; tetPtI++) {
    tetIndices& faceTetIs = faceTets[tetPtI - 1];
    label facePtI = (tetPtI + tetBasePtI) % f.size();
    label otherFacePtI = f.fcIndex(facePtI);
    faceTetIs.cell() = cI;
    faceTetIs.face() = fI;
    faceTetIs.faceBasePt() = tetBasePtI;
    if (own) {
      faceTetIs.facePtA() = facePtI;
      faceTetIs.facePtB() = otherFacePtI;
    } else {
      faceTetIs.facePtA() = otherFacePtI;
      faceTetIs.facePtB() = facePtI;
    }
    faceTetIs.tetPt() = tetPtI;
  }
  return faceTets;
}


mousse::tetIndices mousse::polyMeshTetDecomposition::triangleTetIndices
(
  const polyMesh& mesh,
  const label fI,
  const label cI,
  const label tetPtI
)
{
  static label nWarnings = 0;
  static const label maxWarnings = 100;
  const face& f = mesh.faces()[fI];
  bool own = (mesh.faceOwner()[fI] == cI);
  label tetBasePtI = mesh.tetBasePtIs()[fI];
  if (tetBasePtI == -1) {
    if (nWarnings < maxWarnings) {
      WARNING_IN
      (
        "tetIndices "
        "polyMeshTetDecomposition::triangleTetIndices"
        "("
          "const polyMesh&, "
          "label, "
          "label, "
          "label"
        ")"
      )
      << "No base point for face " << fI << ", " << f
      << ", produces a valid tet decomposition."
      << endl;
      nWarnings++;
    }
    if (nWarnings == maxWarnings) {
      Warning << "Suppressing any further warnings." << endl;
      nWarnings++;
    }
    tetBasePtI = 0;
  }
  tetIndices faceTetIs;
  label facePtI = (tetPtI + tetBasePtI) % f.size();
  label otherFacePtI = f.fcIndex(facePtI);
  faceTetIs.cell() = cI;
  faceTetIs.face() = fI;
  faceTetIs.faceBasePt() = tetBasePtI;
  if (own) {
    faceTetIs.facePtA() = facePtI;
    faceTetIs.facePtB() = otherFacePtI;
  } else {
    faceTetIs.facePtA() = otherFacePtI;
    faceTetIs.facePtB() = facePtI;
  }
  faceTetIs.tetPt() = tetPtI;
  return faceTetIs;
}


mousse::List<mousse::tetIndices> mousse::polyMeshTetDecomposition::cellTetIndices
(
  const polyMesh& mesh,
  label cI
)
{
  const faceList& pFaces = mesh.faces();
  const cellList& pCells = mesh.cells();
  const cell& thisCell = pCells[cI];
  label nTets = 0;
  FOR_ALL(thisCell, cFI) {
    nTets += pFaces[thisCell[cFI]].size() - 2;
  }
  DynamicList<tetIndices> cellTets{nTets};
  FOR_ALL(thisCell, cFI) {
    label fI = thisCell[cFI];
    cellTets.append(faceTetIndices(mesh, fI, cI));
  }
  return cellTets;
}


mousse::tetIndices mousse::polyMeshTetDecomposition::findTet
(
  const polyMesh& mesh,
  label cI,
  const point& pt
)
{
  const faceList& pFaces = mesh.faces();
  const cellList& pCells = mesh.cells();
  const cell& thisCell = pCells[cI];
  tetIndices tetContainingPt;
  FOR_ALL(thisCell, cFI) {
    label fI = thisCell[cFI];
    const face& f = pFaces[fI];
    for (label tetPtI = 1; tetPtI < f.size() - 1; tetPtI++) {
      // Get tetIndices of face triangle
      tetIndices faceTetIs
      {
        triangleTetIndices
        (
          mesh,
          fI,
          cI,
          tetPtI
        )
      };
      // Check if inside
      if (faceTetIs.tet(mesh).inside(pt)) {
        tetContainingPt = faceTetIs;
        break;
      }
    }
    if (tetContainingPt.cell() != -1) {
      break;
    }
  }
  return tetContainingPt;
}
