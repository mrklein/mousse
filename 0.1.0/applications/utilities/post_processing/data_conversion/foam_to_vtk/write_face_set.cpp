// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_face_set.hpp"
#include "ofstream.hpp"
#include "write_funs.hpp"
// Global Functions 
void mousse::writeFaceSet
(
  const bool binary,
  const vtkMesh& vMesh,
  const faceSet& set,
  const fileName& fileName
)
{
  const faceList& faces = vMesh.mesh().faces();
  std::ofstream ostr(fileName.c_str());
  writeFuns::writeHeader
  (
    ostr,
    binary,
    set.name()
  );
  ostr<< "DATASET POLYDATA" << std::endl;
  //------------------------------------------------------------------
  //
  // Write topology
  //
  //------------------------------------------------------------------
  // Construct primitivePatch of faces in faceSet.
  faceList setFaces(set.size());
  labelList setFaceLabels(set.size());
  label setFaceI = 0;
  forAllConstIter(faceSet, set, iter)
  {
    setFaceLabels[setFaceI] = iter.key();
    setFaces[setFaceI] = faces[iter.key()];
    setFaceI++;
  }
  primitiveFacePatch fp(setFaces, vMesh.mesh().points());
  // Write points and faces as polygons
  ostr<< "POINTS " << fp.nPoints() << " float" << std::endl;
  DynamicList<floatScalar> ptField(3*fp.nPoints());
  writeFuns::insert(fp.localPoints(), ptField);
  writeFuns::write(ostr, binary, ptField);
  label nFaceVerts = 0;
  forAll(fp.localFaces(), faceI)
  {
    nFaceVerts += fp.localFaces()[faceI].size() + 1;
  }
  ostr<< "POLYGONS " << fp.size() << ' ' << nFaceVerts << std::endl;
  DynamicList<label> vertLabels(nFaceVerts);
  forAll(fp.localFaces(), faceI)
  {
    const face& f = fp.localFaces()[faceI];
    vertLabels.append(f.size());
    writeFuns::insert(f, vertLabels);
  }
  writeFuns::write(ostr, binary, vertLabels);
  //-----------------------------------------------------------------
  //
  // Write data
  //
  //-----------------------------------------------------------------
  // Write faceID
  ostr
    << "CELL_DATA " << fp.size() << std::endl
    << "FIELD attributes 1" << std::endl;
  // Cell ids first
  ostr<< "faceID 1 " << fp.size() << " int" << std::endl;
  writeFuns::write(ostr, binary, setFaceLabels);
}
