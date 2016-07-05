// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_smooth.hpp"
#include "vol_fields.hpp"
#include "face_cell_wave.hpp"
#include "smooth_data.hpp"
#include "sweep_data.hpp"
#include "global_mesh_data.hpp"

void mousse::fvc::smooth
(
  volScalarField& field,
  const scalar coeff
)
{
  const fvMesh& mesh = field.mesh();
  scalar maxRatio = 1 + coeff;
  DynamicList<label> changedFaces(mesh.nFaces()/100 + 100);
  DynamicList<smoothData> changedFacesInfo(changedFaces.size());
  const labelUList& owner = mesh.owner();
  const labelUList& neighbour = mesh.neighbour();
  FOR_ALL(owner, facei)
  {
    const label own = owner[facei];
    const label nbr = neighbour[facei];
    // Check if owner value much larger than neighbour value or vice versa
    if (field[own] > maxRatio*field[nbr])
    {
      changedFaces.append(facei);
      changedFacesInfo.append(smoothData(field[own]));
    }
    else if (field[nbr] > maxRatio*field[own])
    {
      changedFaces.append(facei);
      changedFacesInfo.append(smoothData(field[nbr]));
    }
  }
  // Insert all faces of coupled patches - FaceCellWave will correct them
  FOR_ALL(mesh.boundaryMesh(), patchi)
  {
    const polyPatch& patch = mesh.boundaryMesh()[patchi];
    if (patch.coupled())
    {
      FOR_ALL(patch, patchFacei)
      {
        label facei = patch.start() + patchFacei;
        label own = mesh.faceOwner()[facei];
        changedFaces.append(facei);
        changedFacesInfo.append(smoothData(field[own]));
      }
    }
  }
  changedFaces.shrink();
  changedFacesInfo.shrink();
  // Set initial field on cells
  List<smoothData> cellData(mesh.nCells());
  FOR_ALL(field, celli)
  {
    cellData[celli] = field[celli];
  }
  // Set initial field on faces
  List<smoothData> faceData(mesh.nFaces());
  smoothData::trackData td;
  td.maxRatio = maxRatio;
  // Propagate information over whole domain
  FaceCellWave<smoothData, smoothData::trackData> smoothData
  (
    mesh,
    changedFaces,
    changedFacesInfo,
    faceData,
    cellData,
    mesh.globalData().nTotalCells(),   // max iterations
    td
  );
  FOR_ALL(field, celli)
  {
    field[celli] = cellData[celli].value();
  }
  field.correctBoundaryConditions();
}


void mousse::fvc::spread
(
  volScalarField& field,
  const volScalarField& alpha,
  const label nLayers,
  const scalar alphaDiff,
  const scalar /*alphaMax*/,
  const scalar /*alphaMin*/
)
{
  const fvMesh& mesh = field.mesh();
  DynamicList<label> changedFaces{mesh.nFaces()/100 + 100};
  DynamicList<smoothData> changedFacesInfo{changedFaces.size()};

  // Set initial field on cells
  List<smoothData> cellData{mesh.nCells()};
  FOR_ALL(field, celli)
  {
    cellData[celli] = field[celli];
  }

  // Set initial field on faces
  List<smoothData> faceData{mesh.nFaces()};
  const labelUList& owner = mesh.owner();
  const labelUList& neighbour = mesh.neighbour();
  FOR_ALL(owner, facei)
  {
    const label own = owner[facei];
    const label nbr = neighbour[facei];
    if (mag(alpha[own] - alpha[nbr]) > alphaDiff)
    {
      changedFaces.append(facei);
      changedFacesInfo.append
      (
        smoothData(max(field[own], field[nbr]))
      );
    }
  }

  // Insert all faces of coupled patches - FaceCellWave will correct them
  FOR_ALL(mesh.boundaryMesh(), patchi)
  {
    const polyPatch& patch = mesh.boundaryMesh()[patchi];
    if (patch.coupled())
    {
      FOR_ALL(patch, patchFacei)
      {
        label facei = patch.start() + patchFacei;
        label own = mesh.faceOwner()[facei];
        const scalarField alphapn
        (
          alpha.boundaryField()[patchi].patchNeighbourField()
        );
        if (mag(alpha[own] - alphapn[patchFacei]) > alphaDiff)
        {
          changedFaces.append(facei);
          changedFacesInfo.append(smoothData(field[own]));
        }
      }
    }
  }
  changedFaces.shrink();
  changedFacesInfo.shrink();
  smoothData::trackData td;
  td.maxRatio = 1.0;

  // Propagate information over whole domain
  FaceCellWave<smoothData, smoothData::trackData> smoothData
  {
    mesh,
    faceData,
    cellData,
    td
  };
  smoothData.setFaceInfo(changedFaces, changedFacesInfo);
  smoothData.iterate(nLayers);
  FOR_ALL(field, celli)
  {
    field[celli] = cellData[celli].value();
  }
  field.correctBoundaryConditions();
}


void mousse::fvc::sweep
(
  volScalarField& field,
  const volScalarField& alpha,
  const label nLayers,
  const scalar alphaDiff
)
{
  const fvMesh& mesh = field.mesh();
  DynamicList<label> changedFaces{mesh.nFaces()/100 + 100};
  DynamicList<sweepData> changedFacesInfo{changedFaces.size()};

  // Set initial field on cells
  List<sweepData> cellData{mesh.nCells()};

  // Set initial field on faces
  List<sweepData> faceData{mesh.nFaces()};
  const labelUList& owner = mesh.owner();
  const labelUList& neighbour = mesh.neighbour();
  const vectorField& Cf = mesh.faceCentres();
  FOR_ALL(owner, facei)
  {
    const label own = owner[facei];
    const label nbr = neighbour[facei];
    if (mag(alpha[own] - alpha[nbr]) > alphaDiff)
    {
      changedFaces.append(facei);
      changedFacesInfo.append
      (
        sweepData(max(field[own], field[nbr]), Cf[facei])
      );
    }
  }

  // Insert all faces of coupled patches - FaceCellWave will correct them
  FOR_ALL(mesh.boundaryMesh(), patchi)
  {
    const polyPatch& patch = mesh.boundaryMesh()[patchi];
    if (patch.coupled())
    {
      FOR_ALL(patch, patchFacei)
      {
        label facei = patch.start() + patchFacei;
        label own = mesh.faceOwner()[facei];
        const scalarField alphapn
        (
          alpha.boundaryField()[patchi].patchNeighbourField()
        );
        if (mag(alpha[own] - alphapn[patchFacei]) > alphaDiff)
        {
          changedFaces.append(facei);
          changedFacesInfo.append
          (
            sweepData(field[own], Cf[facei])
          );
        }
      }
    }
  }
  changedFaces.shrink();
  changedFacesInfo.shrink();

  // Propagate information over whole domain
  FaceCellWave<sweepData> sweepData
  {
    mesh,
    faceData,
    cellData
  };
  sweepData.setFaceInfo(changedFaces, changedFacesInfo);
  sweepData.iterate(nLayers);
  FOR_ALL(field, celli)
  {
    if (cellData[celli].valid(sweepData.data()))
    {
      field[celli] = max(field[celli], cellData[celli].value());
    }
  }
  field.correctBoundaryConditions();
}
