// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_quality.hpp"
#include "unit_conversion.hpp"
#include "sub_field.hpp"


// Constructors 
mousse::cellQuality::cellQuality(const polyMesh& mesh)
:
  mesh_{mesh}
{}


// Member Functions 
mousse::tmp<mousse::scalarField> mousse::cellQuality::nonOrthogonality() const
{
  tmp<scalarField> tresult
  {
    new scalarField{mesh_.nCells(), 0.0}
  };
  scalarField& result = tresult();
  scalarField sumArea{mesh_.nCells(), 0.0};
  const vectorField& centres = mesh_.cellCentres();
  const vectorField& areas = mesh_.faceAreas();
  const labelList& own = mesh_.faceOwner();
  const labelList& nei = mesh_.faceNeighbour();
  FOR_ALL(nei, faceI) {
    vector d = centres[nei[faceI]] - centres[own[faceI]];
    vector s = areas[faceI];
    scalar magS = mag(s);
    scalar cosDDotS =
      radToDeg(mousse::acos(min(1.0, (d & s)/(mag(d)*magS + VSMALL))));
    result[own[faceI]] = max(cosDDotS, result[own[faceI]]);
    result[nei[faceI]] = max(cosDDotS, result[nei[faceI]]);
  }
  FOR_ALL(mesh_.boundaryMesh(), patchI) {
    const labelUList& faceCells =
      mesh_.boundaryMesh()[patchI].faceCells();
    const vectorField::subField faceCentres =
      mesh_.boundaryMesh()[patchI].faceCentres();
    const vectorField::subField faceAreas =
      mesh_.boundaryMesh()[patchI].faceAreas();
    FOR_ALL(faceCentres, faceI) {
      vector d = faceCentres[faceI] - centres[faceCells[faceI]];
      vector s = faceAreas[faceI];
      scalar magS = mag(s);
      scalar cosDDotS =
        radToDeg(mousse::acos(min(1.0, (d & s)/(mag(d)*magS + VSMALL))));
      result[faceCells[faceI]] = max(cosDDotS, result[faceCells[faceI]]);
    }
  }
  return tresult;
}


mousse::tmp<mousse::scalarField> mousse::cellQuality::skewness() const
{
  tmp<scalarField> tresult
  {
    new scalarField{mesh_.nCells(), 0.0}
  };
  scalarField& result = tresult();
  scalarField sumArea{mesh_.nCells(), 0.0};
  const vectorField& cellCtrs = mesh_.cellCentres();
  const vectorField& faceCtrs = mesh_.faceCentres();
  const vectorField& areas = mesh_.faceAreas();
  const labelList& own = mesh_.faceOwner();
  const labelList& nei = mesh_.faceNeighbour();
  FOR_ALL(nei, faceI) {
    scalar dOwn = mag
    (
      (faceCtrs[faceI] - cellCtrs[own[faceI]]) & areas[faceI]
    )/mag(areas[faceI]);
    scalar dNei = mag
    (
      (cellCtrs[nei[faceI]] - faceCtrs[faceI]) & areas[faceI]
    )/mag(areas[faceI]);
    point faceIntersection = cellCtrs[own[faceI]]
      + (dOwn/(dOwn+dNei))*(cellCtrs[nei[faceI]] - cellCtrs[own[faceI]]);
    scalar skewness = mag(faceCtrs[faceI] - faceIntersection)
      /(mag(cellCtrs[nei[faceI]] - cellCtrs[own[faceI]]) + VSMALL);
    result[own[faceI]] = max(skewness, result[own[faceI]]);
    result[nei[faceI]] = max(skewness, result[nei[faceI]]);
  }
  FOR_ALL(mesh_.boundaryMesh(), patchI) {
    const labelUList& faceCells =
      mesh_.boundaryMesh()[patchI].faceCells();
    const vectorField::subField faceCentres =
      mesh_.boundaryMesh()[patchI].faceCentres();
    const vectorField::subField faceAreas =
      mesh_.boundaryMesh()[patchI].faceAreas();
    FOR_ALL(faceCentres, faceI) {
      vector n = faceAreas[faceI]/mag(faceAreas[faceI]);
      point faceIntersection = cellCtrs[faceCells[faceI]]
        + ((faceCentres[faceI] - cellCtrs[faceCells[faceI]])&n)*n;
      scalar skewness =
        mag(faceCentres[faceI] - faceIntersection)
        /(
          mag(faceCentres[faceI] - cellCtrs[faceCells[faceI]])
          + VSMALL
         );
      result[faceCells[faceI]] = max(skewness, result[faceCells[faceI]]);
    }
  }
  return tresult;
}


mousse::tmp<mousse::scalarField> mousse::cellQuality::faceNonOrthogonality() const
{
  tmp<scalarField> tresult
  {
    new scalarField{mesh_.nFaces(), 0.0}
  };
  scalarField& result = tresult();
  const vectorField& centres = mesh_.cellCentres();
  const vectorField& areas = mesh_.faceAreas();
  const labelList& own = mesh_.faceOwner();
  const labelList& nei = mesh_.faceNeighbour();
  FOR_ALL(nei, faceI) {
    vector d = centres[nei[faceI]] - centres[own[faceI]];
    vector s = areas[faceI];
    scalar magS = mag(s);
    scalar cosDDotS =
      radToDeg(mousse::acos(min(1.0, (d & s)/(mag(d)*magS + VSMALL))));
    result[faceI] = cosDDotS;
  }
  label globalFaceI = mesh_.nInternalFaces();
  FOR_ALL(mesh_.boundaryMesh(), patchI) {
    const labelUList& faceCells =
      mesh_.boundaryMesh()[patchI].faceCells();
    const vectorField::subField faceCentres =
      mesh_.boundaryMesh()[patchI].faceCentres();
    const vectorField::subField faceAreas =
      mesh_.boundaryMesh()[patchI].faceAreas();
    FOR_ALL(faceCentres, faceI) {
      vector d = faceCentres[faceI] - centres[faceCells[faceI]];
      vector s = faceAreas[faceI];
      scalar magS = mag(s);
      scalar cosDDotS =
        radToDeg(mousse::acos(min(1.0, (d & s)/(mag(d)*magS + VSMALL))));
      result[globalFaceI++] = cosDDotS;
    }
  }
  return tresult;
}


mousse::tmp<mousse::scalarField> mousse::cellQuality::faceSkewness() const
{
  tmp<scalarField> tresult
  {
    new scalarField{mesh_.nFaces(), 0.0}
  };
  scalarField& result = tresult();
  const vectorField& cellCtrs = mesh_.cellCentres();
  const vectorField& faceCtrs = mesh_.faceCentres();
  const vectorField& areas = mesh_.faceAreas();
  const labelList& own = mesh_.faceOwner();
  const labelList& nei = mesh_.faceNeighbour();
  FOR_ALL(nei, faceI) {
    scalar dOwn = mag
    (
      (faceCtrs[faceI] - cellCtrs[own[faceI]]) & areas[faceI]
    )/mag(areas[faceI]);
    scalar dNei = mag
    (
      (cellCtrs[nei[faceI]] - faceCtrs[faceI]) & areas[faceI]
    )/mag(areas[faceI]);
    point faceIntersection = cellCtrs[own[faceI]]
      + (dOwn/(dOwn+dNei))*(cellCtrs[nei[faceI]] - cellCtrs[own[faceI]]);
    result[faceI] = mag(faceCtrs[faceI] - faceIntersection)
      /(mag(cellCtrs[nei[faceI]] - cellCtrs[own[faceI]]) + VSMALL);
  }
  label globalFaceI = mesh_.nInternalFaces();
  FOR_ALL(mesh_.boundaryMesh(), patchI) {
    const labelUList& faceCells =
      mesh_.boundaryMesh()[patchI].faceCells();
    const vectorField::subField faceCentres =
      mesh_.boundaryMesh()[patchI].faceCentres();
    const vectorField::subField faceAreas =
      mesh_.boundaryMesh()[patchI].faceAreas();
    FOR_ALL(faceCentres, faceI) {
      vector n = faceAreas[faceI]/mag(faceAreas[faceI]);
      point faceIntersection = cellCtrs[faceCells[faceI]]
        + ((faceCentres[faceI] - cellCtrs[faceCells[faceI]])&n)*n;
      result[globalFaceI++] =
        mag(faceCentres[faceI] - faceIntersection)
        /(
          mag(faceCentres[faceI] - cellCtrs[faceCells[faceI]])
          + VSMALL
         );
    }
  }
  return tresult;
}

