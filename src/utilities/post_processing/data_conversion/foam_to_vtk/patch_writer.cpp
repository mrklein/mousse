// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_writer.hpp"
#include "write_funs.hpp"


// Constructors 
mousse::patchWriter::patchWriter
(
  const vtkMesh& vMesh,
  const bool binary,
  const bool nearCellValue,
  const fileName& fName,
  const labelList& patchIDs
)
:
  vMesh_{vMesh},
  binary_{binary},
  nearCellValue_{nearCellValue},
  fName_{fName},
  patchIDs_{patchIDs},
  os_{fName.c_str()}
{
  const fvMesh& mesh = vMesh_.mesh();
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  // Write header
  if (patchIDs_.size() == 1) {
    writeFuns::writeHeader(os_, binary_, patches[patchIDs_[0]].name());
  } else {
    writeFuns::writeHeader(os_, binary_, "patches");
  }
  os_ << "DATASET POLYDATA" << std::endl;
  // Write topology
  nPoints_ = 0;
  nFaces_ = 0;
  label nFaceVerts = 0;
  FOR_ALL(patchIDs_, i) {
    const polyPatch& pp = patches[patchIDs_[i]];
    nPoints_ += pp.nPoints();
    nFaces_ += pp.size();
    FOR_ALL(pp, faceI) {
      nFaceVerts += pp[faceI].size() + 1;
    }
  }
  os_ << "POINTS " << nPoints_ << " float" << std::endl;
  DynamicList<floatScalar> ptField{3*nPoints_};
  FOR_ALL(patchIDs_, i) {
    const polyPatch& pp = patches[patchIDs_[i]];
    writeFuns::insert(pp.localPoints(), ptField);
  }
  writeFuns::write(os_, binary_, ptField);
  os_ << "POLYGONS " << nFaces_ << ' ' << nFaceVerts << std::endl;
  DynamicList<label> vertLabels{nFaceVerts};
  label offset = 0;
  FOR_ALL(patchIDs_, i) {
    const polyPatch& pp = patches[patchIDs_[i]];
    FOR_ALL(pp, faceI) {
      const face& f = pp.localFaces()[faceI];
      vertLabels.append(f.size());
      writeFuns::insert(f + offset, vertLabels);
    }
    offset += pp.nPoints();
  }
  writeFuns::write(os_, binary_, vertLabels);
}


// Member Functions 
void mousse::patchWriter::writePatchIDs()
{
  const fvMesh& mesh = vMesh_.mesh();
  DynamicList<floatScalar> fField{nFaces_};
  os_ << "patchID 1 " << nFaces_ << " float" << std::endl;
  FOR_ALL(patchIDs_, i) {
    label patchI = patchIDs_[i];
    const polyPatch& pp = mesh.boundaryMesh()[patchI];
    if (!isA<emptyPolyPatch>(pp)) {
      writeFuns::insert(scalarField(pp.size(), patchI), fField);
    }
  }
  writeFuns::write(os_, binary_, fField);
}

