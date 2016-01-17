// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_mesh_writer.hpp"
#include "write_funs.hpp"
// Constructors 
mousse::surfaceMeshWriter::surfaceMeshWriter
(
  const bool binary,
  const indirectPrimitivePatch& pp,
  const word& name,
  const fileName& fName
)
:
  binary_{binary},
  pp_{pp},
  fName_{fName},
  os_{fName.c_str()}
{
  // Write header
  writeFuns::writeHeader(os_, binary_, name);
  os_ << "DATASET POLYDATA" << std::endl;
  // Write topology
  label nFaceVerts = 0;
  FOR_ALL(pp, faceI)
  {
    nFaceVerts += pp[faceI].size() + 1;
  }
  os_ << "POINTS " << pp.nPoints() << " float" << std::endl;
  DynamicList<floatScalar> ptField(3*pp.nPoints());
  writeFuns::insert(pp.localPoints(), ptField);
  writeFuns::write(os_, binary, ptField);
  os_ << "POLYGONS " << pp.size() << ' ' << nFaceVerts << std::endl;
  DynamicList<label> vertLabels{nFaceVerts};
  FOR_ALL(pp, faceI)
  {
    const face& f = pp.localFaces()[faceI];
    vertLabels.append(f.size());
    writeFuns::insert(f, vertLabels);
  }
  writeFuns::write(os_, binary_, vertLabels);
}
