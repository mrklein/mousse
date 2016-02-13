// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "channel_index.hpp"
#include "bool_list.hpp"
#include "sync_tools.hpp"
#include "ofstream.hpp"
#include "mesh_tools.hpp"
#include "time.hpp"
#include "sortable_list.hpp"
// Static Member Data 
namespace mousse
{
  template<>
  const char* mousse::NamedEnum
  <
    mousse::vector::components,
    3
  >::names[] =
  {
    "x",
    "y",
    "z"
  };
}
const mousse::NamedEnum<mousse::vector::components, 3>
  mousse::channelIndex::vectorComponentsNames_;
// Private Member Functions 
// Determines face blocking
void mousse::channelIndex::walkOppositeFaces
(
  const polyMesh& mesh,
  const labelList& startFaces,
  boolList& blockedFace
)
{
  const cellList& cells = mesh.cells();
  const faceList& faces = mesh.faces();
  label nBnd = mesh.nFaces() - mesh.nInternalFaces();
  DynamicList<label> frontFaces{startFaces};
  FOR_ALL(frontFaces, i)
  {
    label faceI = frontFaces[i];
    blockedFace[faceI] = true;
  }
  while (returnReduce(frontFaces.size(), sumOp<label>()) > 0)
  {
    // Transfer across.
    boolList isFrontBndFace{nBnd, false};
    FOR_ALL(frontFaces, i)
    {
      label faceI = frontFaces[i];
      if (!mesh.isInternalFace(faceI))
      {
        isFrontBndFace[faceI-mesh.nInternalFaces()] = true;
      }
    }
    syncTools::swapBoundaryFaceList(mesh, isFrontBndFace);
    // Add
    FOR_ALL(isFrontBndFace, i)
    {
      label faceI = mesh.nInternalFaces()+i;
      if (isFrontBndFace[i] && !blockedFace[faceI])
      {
        blockedFace[faceI] = true;
        frontFaces.append(faceI);
      }
    }
    // Transfer across cells
    DynamicList<label> newFrontFaces{frontFaces.size()};
    FOR_ALL(frontFaces, i)
    {
      label faceI = frontFaces[i];
      {
        const cell& ownCell = cells[mesh.faceOwner()[faceI]];
        label oppositeFaceI = ownCell.opposingFaceLabel(faceI, faces);
        if (oppositeFaceI == -1)
        {
          FATAL_ERROR_IN("channelIndex::walkOppositeFaces(..)")
            << "Face:" << faceI << " owner cell:" << ownCell
            << " is not a hex?" << abort(FatalError);
        }
        else
        {
          if (!blockedFace[oppositeFaceI])
          {
            blockedFace[oppositeFaceI] = true;
            newFrontFaces.append(oppositeFaceI);
          }
        }
      }
      if (mesh.isInternalFace(faceI))
      {
        const cell& neiCell = mesh.cells()[mesh.faceNeighbour()[faceI]];
        label oppositeFaceI = neiCell.opposingFaceLabel(faceI, faces);
        if (oppositeFaceI == -1)
        {
          FATAL_ERROR_IN("channelIndex::walkOppositeFaces(..)")
            << "Face:" << faceI << " neighbour cell:" << neiCell
            << " is not a hex?" << abort(FatalError);
        }
        else
        {
          if (!blockedFace[oppositeFaceI])
          {
            blockedFace[oppositeFaceI] = true;
            newFrontFaces.append(oppositeFaceI);
          }
        }
      }
    }
    frontFaces.transfer(newFrontFaces);
  }
}
// Calculate regions.
void mousse::channelIndex::calcLayeredRegions
(
  const polyMesh& mesh,
  const labelList& startFaces
)
{
  boolList blockedFace{mesh.nFaces(), false};
  walkOppositeFaces
  (
    mesh,
    startFaces,
    blockedFace
  );
  if (false)
  {
    OFstream str{mesh.time().path()/"blockedFaces.obj"};
    label vertI = 0;
    FOR_ALL(blockedFace, faceI)
    {
      if (blockedFace[faceI])
      {
        const face& f = mesh.faces()[faceI];
        FOR_ALL(f, fp)
        {
          meshTools::writeOBJ(str, mesh.points()[f[fp]]);
        }
        str<< 'f';
        FOR_ALL(f, fp)
        {
          str << ' ' << vertI+fp+1;
        }
        str << nl;
        vertI += f.size();
      }
    }
  }
  // Do analysis for connected regions
  cellRegion_.reset(new regionSplit(mesh, blockedFace));
  Info<< "Detected " << cellRegion_().nRegions() << " layers." << nl << endl;
  // Sum number of entries per region
  regionCount_ = regionSum(scalarField(mesh.nCells(), 1.0));
  // Average cell centres to determine ordering.
  pointField regionCc{regionSum(mesh.cellCentres())/regionCount_};
  SortableList<scalar> sortComponent{regionCc.component(dir_)};
  sortMap_ = sortComponent.indices();
  y_ = sortComponent;
  if (symmetric_)
  {
    y_.setSize(cellRegion_().nRegions()/2);
  }
}
// Constructors 
mousse::channelIndex::channelIndex
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  symmetric_{readBool(dict.lookup("symmetric"))},
  dir_
  {
    static_cast<direction>
    (
      vectorComponentsNames_.read(dict.lookup("component"))
    )
  }
{
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  const wordList patchNames{dict.lookup("patches")};
  label nFaces = 0;
  FOR_ALL(patchNames, i)
  {
    const label patchI = patches.findPatchID(patchNames[i]);
    if (patchI == -1)
    {
      FATAL_ERROR_IN("channelIndex::channelIndex(const polyMesh&)")
        << "Illegal patch " << patchNames[i]
        << ". Valid patches are " << patches.name()
        << exit(FatalError);
    }
    nFaces += patches[patchI].size();
  }
  labelList startFaces{nFaces};
  nFaces = 0;
  FOR_ALL(patchNames, i)
  {
    const polyPatch& pp = patches[patchNames[i]];
    FOR_ALL(pp, j)
    {
      startFaces[nFaces++] = pp.start()+j;
    }
  }
  // Calculate regions.
  calcLayeredRegions(mesh, startFaces);
}
mousse::channelIndex::channelIndex
(
  const polyMesh& mesh,
  const labelList& startFaces,
  const bool symmetric,
  const direction dir
)
:
  symmetric_{symmetric},
  dir_{dir}
{
  // Calculate regions.
  calcLayeredRegions(mesh, startFaces);
}
