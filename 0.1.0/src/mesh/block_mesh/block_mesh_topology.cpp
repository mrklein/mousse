// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "block_mesh.hpp"
#include "time.hpp"
#include "preserve_patch_types.hpp"
#include "empty_poly_patch.hpp"
#include "cyclic_poly_patch.hpp"


bool mousse::blockMesh::readPatches
(
  const dictionary& meshDescription,
  faceListList& tmpBlocksPatches,
  wordList& patchNames,
  wordList& patchTypes,
  wordList& nbrPatchNames
)
{
  bool topologyOK = true;
  ITstream& patchStream{meshDescription.lookup("patches")};
  // read number of patches in mesh
  label nPatches = 0;
  token firstToken{patchStream};
  if (firstToken.isLabel()) {
    nPatches = firstToken.labelToken();
    tmpBlocksPatches.setSize(nPatches);
    patchNames.setSize(nPatches);
    patchTypes.setSize(nPatches);
    nbrPatchNames.setSize(nPatches);
  } else {
    patchStream.putBack(firstToken);
  }
  // Read beginning of blocks
  patchStream.readBegin("patches");
  nPatches = 0;
  token lastToken{patchStream};
  while (!(lastToken.isPunctuation() && lastToken.pToken() == token::END_LIST)) {
    if (tmpBlocksPatches.size() <= nPatches) {
      tmpBlocksPatches.setSize(nPatches + 1);
      patchNames.setSize(nPatches + 1);
      patchTypes.setSize(nPatches + 1);
      nbrPatchNames.setSize(nPatches + 1);
    }
    patchStream.putBack(lastToken);
    patchStream >> patchTypes[nPatches] >> patchNames[nPatches];
    // Read patch faces
    patchStream >> tmpBlocksPatches[nPatches];
    // Catch multiple patches asap.
    for (label i = 0; i < nPatches; i++) {
      if (patchNames[nPatches] == patchNames[i]) {
        FATAL_ERROR_IN
        (
          "blockMesh::createTopology(IOdictionary&)"
        )
        << "Duplicate patch " << patchNames[nPatches]
        << " at line " << patchStream.lineNumber()
        << ". Exiting !" << nl
        << exit(FatalError);
      }
    }
    topologyOK = topologyOK && patchLabelsOK(nPatches, blockPointField_,
                                             tmpBlocksPatches[nPatches]);
    nPatches++;
    // Split old style cyclics
    if (patchTypes[nPatches-1] == cyclicPolyPatch::typeName) {
      word halfA = patchNames[nPatches-1] + "_half0";
      word halfB = patchNames[nPatches-1] + "_half1";
      WARNING_IN("blockMesh::createTopology(IOdictionary&)")
        << "Old-style cyclic definition."
        << " Splitting patch "
        << patchNames[nPatches-1] << " into two halves "
        << halfA << " and " << halfB << endl
        << "    Alternatively use new 'boundary' dictionary syntax."
        << endl;
      // Add extra patch
      if (tmpBlocksPatches.size() <= nPatches) {
        tmpBlocksPatches.setSize(nPatches + 1);
        patchNames.setSize(nPatches + 1);
        patchTypes.setSize(nPatches + 1);
        nbrPatchNames.setSize(nPatches + 1);
      }
      // Update halfA info
      patchNames[nPatches-1] = halfA;
      nbrPatchNames[nPatches-1] = halfB;
      // Update halfB info
      patchTypes[nPatches] = patchTypes[nPatches-1];
      patchNames[nPatches] = halfB;
      nbrPatchNames[nPatches] = halfA;
      // Split faces
      if ((tmpBlocksPatches[nPatches-1].size() % 2) != 0) {
        FATAL_ERROR_IN
        (
          "blockMesh::createTopology(IOdictionary&)"
        )
        << "Size of cyclic faces is not a multiple of 2 :"
        << tmpBlocksPatches[nPatches-1]
        << exit(FatalError);
      }
      label sz = tmpBlocksPatches[nPatches-1].size()/2;
      faceList unsplitFaces{tmpBlocksPatches[nPatches-1], true};
      tmpBlocksPatches[nPatches-1] = faceList{SubList<face>{unsplitFaces, sz}};
      tmpBlocksPatches[nPatches] =
        faceList{SubList<face>{unsplitFaces, sz, sz}};
      nPatches++;
    }
    patchStream >> lastToken;
  }
  patchStream.putBack(lastToken);
  // Read end of blocks
  patchStream.readEnd("patches");
  return topologyOK;
}


bool mousse::blockMesh::readBoundary
(
  const dictionary& meshDescription,
  wordList& patchNames,
  faceListList& tmpBlocksPatches,
  PtrList<dictionary>& patchDicts
)
{
  bool topologyOK = true;
  // Read like boundary file
  const PtrList<entry> patchesInfo{meshDescription.lookup("boundary")};
  patchNames.setSize(patchesInfo.size());
  tmpBlocksPatches.setSize(patchesInfo.size());
  patchDicts.setSize(patchesInfo.size());
  FOR_ALL(tmpBlocksPatches, patchI) {
    const entry& patchInfo = patchesInfo[patchI];
    if (!patchInfo.isDict()) {
      FATAL_IO_ERROR_IN("blockMesh::readBoundary(..)", meshDescription)
        << "Entry " << patchInfo << " in boundary section is not a"
        << " valid dictionary." << exit(FatalIOError);
    }
    patchNames[patchI] = patchInfo.keyword();
    // Construct dictionary
    patchDicts.set(patchI, new dictionary{patchInfo.dict()});
    // Read block faces
    patchDicts[patchI].lookup("faces") >> tmpBlocksPatches[patchI];
    topologyOK = topologyOK && patchLabelsOK(patchI, blockPointField_,
                                             tmpBlocksPatches[patchI]);
  }
  return topologyOK;
}


void mousse::blockMesh::createCellShapes
(
  cellShapeList& tmpBlockCells
)
{
  const blockMesh& blocks = *this;
  tmpBlockCells.setSize(blocks.size());
  FOR_ALL(blocks, blockI) {
    tmpBlockCells[blockI] = cellShape(blocks[blockI].blockShape());
    if (tmpBlockCells[blockI].mag(blockPointField_) < 0.0) {
      WARNING_IN
      (
        "blockMesh::createTopology(IOdictionary&)"
      )
      << "negative volume block : " << blockI
      << ", probably defined inside-out" << endl;
    }
  }
}


// Private Member Functions 
mousse::polyMesh* mousse::blockMesh::createTopology
(
  const IOdictionary& meshDescription,
  const word& regionName
)
{
  bool topologyOK = true;
  blockList& blocks = *this;
  word defaultPatchName = "defaultFaces";
  word defaultPatchType = emptyPolyPatch::typeName;
  // get names/types for the unassigned patch faces
  // this is a bit heavy handed (and ugly), but there is currently
  // no easy way to rename polyMesh patches subsequently
  if (const dictionary* dictPtr = meshDescription.subDictPtr("defaultPatch")) {
    dictPtr->readIfPresent("name", defaultPatchName);
    dictPtr->readIfPresent("type", defaultPatchType);
  }
  // optional 'convertToMeters' or 'scale'  scaling factor
  if (!meshDescription.readIfPresent("convertToMeters", scaleFactor_)) {
    meshDescription.readIfPresent("scale", scaleFactor_);
  }
  //
  // get the non-linear edges in mesh
  //
  if (meshDescription.found("edges")) {
    if (verboseOutput) {
      Info << "Creating curved edges" << endl;
    }
    ITstream& is{meshDescription.lookup("edges")};
    // read number of edges in mesh
    label nEdges = 0;
    token firstToken{is};
    if (firstToken.isLabel()) {
      nEdges = firstToken.labelToken();
      edges_.setSize(nEdges);
    } else {
      is.putBack(firstToken);
    }
    // Read beginning of edges
    is.readBegin("edges");
    nEdges = 0;
    token lastToken{is};
    while (!(lastToken.isPunctuation()
             && lastToken.pToken() == token::END_LIST)) {
      if (edges_.size() <= nEdges) {
        edges_.setSize(nEdges + 1);
      }
      is.putBack(lastToken);
      edges_.set(nEdges, curvedEdge::New(blockPointField_, is));
      nEdges++;
      is >> lastToken;
    }
    is.putBack(lastToken);
    // Read end of edges
    is.readEnd("edges");
  } else if (verboseOutput) {
    Info << "No non-linear edges defined" << endl;
  }
  //
  // Create the blocks
  //
  if (verboseOutput) {
    Info << "Creating topology blocks" << endl;
  }
  {
    ITstream& is{meshDescription.lookup("blocks")};
    // read number of blocks in mesh
    label nBlocks = 0;
    token firstToken{is};
    if (firstToken.isLabel()) {
      nBlocks = firstToken.labelToken();
      blocks.setSize(nBlocks);
    } else {
      is.putBack(firstToken);
    }
    // Read beginning of blocks
    is.readBegin("blocks");
    nBlocks = 0;
    token lastToken{is};
    while (!(lastToken.isPunctuation()
             && lastToken.pToken() == token::END_LIST)) {
      if (blocks.size() <= nBlocks) {
        blocks.setSize(nBlocks + 1);
      }
      is.putBack(lastToken);
      blocks.set
      (
        nBlocks,
        new block
        {
          blockPointField_,
          edges_,
          is
        }
      );
      topologyOK = topologyOK && blockLabelsOK(nBlocks, blockPointField_,
                                               blocks[nBlocks].blockShape());
      nBlocks++;
      is >> lastToken;
    }
    is.putBack(lastToken);
    // Read end of blocks
    is.readEnd("blocks");
  }
  polyMesh* blockMeshPtr = nullptr;
  //
  // Create the patches
  //
  if (verboseOutput) {
    Info << "Creating topology patches" << endl;
  }
  if (meshDescription.found("patches")) {
    Info << nl << "Reading patches section" << endl;
    faceListList tmpBlocksPatches;
    wordList patchNames;
    wordList patchTypes;
    wordList nbrPatchNames;
    topologyOK = topologyOK && readPatches(meshDescription, tmpBlocksPatches,
                                           patchNames, patchTypes,
                                           nbrPatchNames);
    if (!topologyOK) {
      FATAL_ERROR_IN("blockMesh::createTopology(IOdictionary&)")
        << "Cannot create mesh due to errors in topology, exiting !"
        << nl << exit(FatalError);
    }
    Info << nl << "Creating block mesh topology" << endl;
    cellShapeList tmpBlockCells{blocks.size()};
    createCellShapes(tmpBlockCells);
    Info << nl << "Reading physicalType from existing boundary file" << endl;
    PtrList<dictionary> patchDicts{patchNames.size()};
    word defaultFacesType;
    preservePatchTypes
      (
        meshDescription.time(),
        meshDescription.time().constant(),
        polyMesh::meshSubDir,
        patchNames,
        patchDicts,
        defaultPatchName,
        defaultPatchType
      );
    // Add cyclic info (might not be present from older file)
    FOR_ALL(patchDicts, patchI) {
      if (!patchDicts.set(patchI)) {
        patchDicts.set(patchI, new dictionary{});
      }
      dictionary& dict = patchDicts[patchI];
      // Add but not override type
      if (!dict.found("type")) {
        dict.add("type", patchTypes[patchI], false);
      } else if (word(dict.lookup("type")) != patchTypes[patchI]) {
        IO_WARNING_IN
        (
          "blockMesh::createTopology(IOdictionary&)",
          meshDescription
        )
        << "For patch " << patchNames[patchI]
        << " overriding type '" << patchTypes[patchI]
        << "' with '" << word(dict.lookup("type"))
        << "' (read from boundary file)"
        << endl;
      }
      // Override neighbourpatch name
      if (nbrPatchNames[patchI] != word::null) {
        dict.set("neighbourPatch", nbrPatchNames[patchI]);
      }
    }
    blockMeshPtr =
      new polyMesh
      {
        IOobject
        {
          regionName,
          meshDescription.time().constant(),
          meshDescription.time(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        xferCopy(blockPointField_),   // copy these points, do NOT move
        tmpBlockCells,
        tmpBlocksPatches,
        patchNames,
        patchDicts,
        defaultPatchName,
        defaultPatchType
      };
  } else if (meshDescription.found("boundary")) {
    wordList patchNames;
    faceListList tmpBlocksPatches;
    PtrList<dictionary> patchDicts;
    topologyOK = topologyOK && readBoundary(meshDescription, patchNames,
                                            tmpBlocksPatches, patchDicts);
    if (!topologyOK) {
      FATAL_ERROR_IN("blockMesh::createTopology(IOdictionary&)")
        << "Cannot create mesh due to errors in topology, exiting !"
        << nl << exit(FatalError);
    }
    Info << nl << "Creating block mesh topology" << endl;
    cellShapeList tmpBlockCells{blocks.size()};
    createCellShapes(tmpBlockCells);
    // Extract
    blockMeshPtr =
      new polyMesh
      {
        IOobject
        {
          regionName,
          meshDescription.time().constant(),
          meshDescription.time(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        xferCopy(blockPointField_),   // copy these points, do NOT move
        tmpBlockCells,
        tmpBlocksPatches,
        patchNames,
        patchDicts,
        defaultPatchName,
        defaultPatchType
      };
  }
  checkBlockMesh(*blockMeshPtr);
  return blockMeshPtr;
}

