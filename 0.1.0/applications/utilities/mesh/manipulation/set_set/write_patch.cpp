// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_patch.hpp"
#include "ofstream.hpp"
#include "write_funs.hpp"
#include "primitive_face_patch.hpp"

namespace mousse
{
// Global Functions 
void writePatch
(
  const bool binary,
  const word& setName,
  const primitiveFacePatch& fp,
  const word& fieldName,
  labelList& fieldValues,
  const fileName& fileName
)
{
  std::ofstream pStream(fileName.c_str());
  pStream
    << "# vtk DataFile Version 2.0" << std::endl
    << setName << std::endl;
  if (binary)
  {
    pStream << "BINARY" << std::endl;
  }
  else
  {
    pStream << "ASCII" << std::endl;
  }
  pStream << "DATASET POLYDATA" << std::endl;
  //------------------------------------------------------------------
  //
  // Write topology
  //
  //------------------------------------------------------------------
  // Write points and faces as polygons
  pStream << "POINTS " << fp.nPoints() << " float" << std::endl;
  DynamicList<floatScalar> ptField{3*fp.nPoints()};
  writeFuns::insert(fp.localPoints(), ptField);
  writeFuns::write(pStream, binary, ptField);
  label nFaceVerts = 0;
  FOR_ALL(fp.localFaces(), faceI)
  {
    nFaceVerts += fp.localFaces()[faceI].size() + 1;
  }
  pStream << "POLYGONS " << fp.size() << ' ' << nFaceVerts
    << std::endl;
  DynamicList<label> vertLabels{nFaceVerts};
  FOR_ALL(fp.localFaces(), faceI)
  {
    const face& f = fp.localFaces()[faceI];
    vertLabels.append(f.size());
    writeFuns::insert(f, vertLabels);
  }
  writeFuns::write(pStream, binary, vertLabels);
  //-----------------------------------------------------------------
  //
  // Write data
  //
  //-----------------------------------------------------------------
  // Write faceID
  pStream
    << "CELL_DATA " << fp.size() << std::endl
    << "FIELD attributes 1" << std::endl;
  // Cell ids first
  pStream << fieldName << " 1 " << fp.size() << " int" << std::endl;
  writeFuns::write(pStream, binary, fieldValues);
}
}  // namespace mousse
