// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_point_set.hpp"
#include "ofstream.hpp"
#include "write_funs.hpp"


namespace mousse {

// Global Functions 
void writePointSet
(
  const bool binary,
  const primitiveMesh& mesh,
  const topoSet& set,
  const fileName& fileName
)
{
  std::ofstream pStream{fileName.c_str()};
  pStream
    << "# vtk DataFile Version 2.0" << std::endl
    << set.name() << std::endl;
  if (binary) {
    pStream << "BINARY" << std::endl;
  } else {
    pStream << "ASCII" << std::endl;
  }
  pStream << "DATASET POLYDATA" << std::endl;
  //------------------------------------------------------------------
  //
  // Write topology
  //
  //------------------------------------------------------------------
  labelList pointLabels{set.toc()};
  pointField setPoints{mesh.points(), pointLabels};
  // Write points
  pStream << "POINTS " << pointLabels.size() << " float" << std::endl;
  DynamicList<floatScalar> ptField{3*pointLabels.size()};
  writeFuns::insert(setPoints, ptField);
  writeFuns::write(pStream, binary, ptField);
  //-----------------------------------------------------------------
  //
  // Write data
  //
  //-----------------------------------------------------------------
  // Write pointID
  pStream
    << "POINT_DATA " << pointLabels.size() << std::endl
    << "FIELD attributes 1" << std::endl;
  // Cell ids first
  pStream << "pointID 1 " << pointLabels.size() << " int" << std::endl;
  writeFuns::write(pStream, binary, pointLabels);
}

}  // namespace mousse

