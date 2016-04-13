// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vtk_edge_format.hpp"
#include "ofstream.hpp"
#include "clock.hpp"
#include "ifstream.hpp"
#include "vtk_unstructured_reader.hpp"
#include "time.hpp"


// Private Member Functions 
void mousse::fileFormats::VTKedgeFormat::writeHeader
(
  Ostream& os,
  const pointField& pointLst
)
{
  // Write header
  os << "# vtk DataFile Version 2.0" << nl
    << "featureEdgeMesh written " << clock::dateTime().c_str() << nl
    << "ASCII" << nl
    << nl
    << "DATASET POLYDATA" << nl;
  // Write vertex coords
  os << "POINTS " << pointLst.size() << " float" << nl;
  FOR_ALL(pointLst, ptI) {
    const point& pt = pointLst[ptI];
    os << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
  }
}


void mousse::fileFormats::VTKedgeFormat::writeEdges
(
  Ostream& os,
  const UList<edge>& edgeLst
)
{
  os << "LINES " << edgeLst.size() << ' ' << 3*edgeLst.size() << nl;
  FOR_ALL(edgeLst, edgeI) {
    const edge& e = edgeLst[edgeI];
    os << "2 " << e[0] << ' ' << e[1] << nl;
  }
}


// Constructors 
mousse::fileFormats::VTKedgeFormat::VTKedgeFormat
(
  const fileName& filename
)
{
  read(filename);
}


// Member Functions 
bool mousse::fileFormats::VTKedgeFormat::read
(
  const fileName& filename
)
{
  IFstream is{filename};
  if (!is.good()) {
    FATAL_ERROR_IN
    (
      "fileFormats::VTKedgeFormat::read(const fileName&)"
    )
    << "Cannot read file " << filename
    << exit(FatalError);
  }
  // Construct dummy time so we have something to create an objectRegistry
  // from
  Time dummyTime
  {
    "dummyRoot",
    "dummyCase",
    "system",
    "constant",
    false           // enableFunctionObjects
  };
  // Make dummy object registry
  objectRegistry obr
  {
    {
      "dummy",
      dummyTime,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    }
  };
  // Construct reader to read file
  vtkUnstructuredReader reader{obr, is};
  // Extract lines
  storedPoints().transfer(reader.points());
  label nEdges = 0;
  FOR_ALL(reader.lines(), lineI) {
    nEdges += reader.lines()[lineI].size()-1;
  }
  storedEdges().setSize(nEdges);
  nEdges = 0;
  FOR_ALL(reader.lines(), lineI) {
    const labelList& verts = reader.lines()[lineI];
    for (label i = 1; i < verts.size(); i++) {
      storedEdges()[nEdges++] = edge(verts[i-1], verts[i]);
    }
  }
  return true;
}


void mousse::fileFormats::VTKedgeFormat::write
(
  const fileName& filename,
  const edgeMesh& eMesh
)
{
  OFstream os{filename};
  if (!os.good()) {
    FATAL_ERROR_IN
    (
      "fileFormats::VTKedgeFormat::write"
      "(const fileName&, const edgeMesh&)"
    )
    << "Cannot open file for writing " << filename
    << exit(FatalError);
  }
  writeHeader(os, eMesh.points());
  writeEdges(os, eMesh.edges());
}

