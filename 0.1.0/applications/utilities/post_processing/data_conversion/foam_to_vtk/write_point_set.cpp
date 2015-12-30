// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_point_set.hpp"
#include "ofstream.hpp"
#include "write_funs.hpp"
namespace mousse
{
// Global Functions 
void writePointSet
(
  const bool binary,
  const vtkMesh& vMesh,
  const pointSet& set,
  const fileName& fileName
)
{
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
  // Write points
  ostr<< "POINTS " << set.size() << " float" << std::endl;
  DynamicList<floatScalar> ptField(3*set.size());
  writeFuns::insert
  (
    UIndirectList<point>(vMesh.mesh().points(), set.toc())(),
    ptField
  );
  writeFuns::write(ostr, binary, ptField);
  //-----------------------------------------------------------------
  //
  // Write data
  //
  //-----------------------------------------------------------------
  // Write faceID
  ostr
    << "POINT_DATA " << set.size() << std::endl
    << "FIELD attributes 1" << std::endl;
  // Cell ids first
  ostr<< "pointID 1 " << set.size() << " int" << std::endl;
  labelList pointIDs(set.toc());
  writeFuns::write(ostr, binary, pointIDs);
}
}  // namespace mousse
