// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "obj_edge_format.hpp"
#include "clock.hpp"
#include "ifstream.hpp"
#include "istring_stream.hpp"
#include "ostream.hpp"
#include "ofstream.hpp"
#include "list_ops.hpp"
// Constructors 
mousse::fileFormats::OBJedgeFormat::OBJedgeFormat
(
  const fileName& filename
)
{
  read(filename);
}
// Member Functions 
void mousse::fileFormats::OBJedgeFormat::readVertices
(
  const string& line,
  string::size_type& endNum,
  DynamicList<label>& dynVertices
)
{
  dynVertices.clear();
  while (true)
  {
    string::size_type startNum =
      line.find_first_not_of(' ', endNum);
    if (startNum == string::npos)
    {
      break;
    }
    endNum = line.find(' ', startNum);
    string vertexSpec;
    if (endNum != string::npos)
    {
      vertexSpec = line.substr(startNum, endNum-startNum);
    }
    else
    {
      vertexSpec = line.substr(startNum, line.size() - startNum);
    }
    string::size_type slashPos = vertexSpec.find('/');
    label vertI = 0;
    if (slashPos != string::npos)
    {
      IStringStream intStream(vertexSpec.substr(0, slashPos));
      intStream >> vertI;
    }
    else
    {
      IStringStream intStream(vertexSpec);
      intStream >> vertI;
    }
    dynVertices.append(vertI - 1);
  }
}
bool mousse::fileFormats::OBJedgeFormat::read(const fileName& filename)
{
  clear();
  IFstream is(filename);
  if (!is.good())
  {
    FatalErrorIn
    (
      "fileFormats::OBJedgeFormat::read(const fileName&)"
    )
      << "Cannot read file " << filename
      << exit(FatalError);
  }
  DynamicList<point> dynPoints;
  DynamicList<edge> dynEdges;
  DynamicList<label> dynUsedPoints;
  DynamicList<label> dynVertices;
  while (is.good())
  {
    string line = this->getLineNoComment(is);
    // handle continuations
    if (line[line.size()-1] == '\\')
    {
      line.substr(0, line.size()-1);
      line += this->getLineNoComment(is);
    }
    // Read first word
    IStringStream lineStream(line);
    word cmd;
    lineStream >> cmd;
    if (cmd == "v")
    {
      scalar x, y, z;
      lineStream >> x >> y >> z;
      dynPoints.append(point(x, y, z));
      dynUsedPoints.append(-1);
    }
    else if (cmd == "l")
    {
      // Assume 'l' is followed by space.
      string::size_type endNum = 1;
      readVertices
      (
        line,
        endNum,
        dynVertices
      );
      for (label i = 1; i < dynVertices.size(); i++)
      {
        edge edgeRead(dynVertices[i-1], dynVertices[i]);
        dynUsedPoints[edgeRead[0]] = edgeRead[0];
        dynUsedPoints[edgeRead[1]] = edgeRead[1];
        dynEdges.append(edgeRead);
      }
    }
    else if (cmd == "f")
    {
      // Support for faces with 2 vertices
      // Assume 'f' is followed by space.
      string::size_type endNum = 1;
      readVertices
      (
        line,
        endNum,
        dynVertices
      );
      if (dynVertices.size() == 2)
      {
        for (label i = 1; i < dynVertices.size(); i++)
        {
          edge edgeRead(dynVertices[i-1], dynVertices[i]);
          dynUsedPoints[edgeRead[0]] = edgeRead[0];
          dynUsedPoints[edgeRead[1]] = edgeRead[1];
          dynEdges.append(edgeRead);
        }
      }
    }
  }
  // cull unused points
  label nUsed = 0;
  forAll(dynPoints, pointI)
  {
    if (dynUsedPoints[pointI] >= 0)
    {
      if (nUsed != pointI)
      {
        dynPoints[nUsed] = dynPoints[pointI];
        dynUsedPoints[pointI] = nUsed;   // new position
      }
      ++nUsed;
    }
  }
  dynPoints.setSize(nUsed);
  // transfer to normal lists
  storedPoints().transfer(dynPoints);
  // renumber edge vertices
  if (nUsed != dynUsedPoints.size())
  {
    forAll(dynEdges, edgeI)
    {
      edge& e = dynEdges[edgeI];
      e[0] = dynUsedPoints[e[0]];
      e[1] = dynUsedPoints[e[1]];
    }
  }
  storedEdges().transfer(dynEdges);
  return true;
}
void mousse::fileFormats::OBJedgeFormat::write
(
  const fileName& filename,
  const edgeMesh& mesh
)
{
  const pointField& pointLst = mesh.points();
  const edgeList& edgeLst = mesh.edges();
  OFstream os(filename);
  if (!os.good())
  {
    FatalErrorIn
    (
      "fileFormats::OBJedgeFormat::write"
      "(const fileName&, const edgeMesh&)"
    )
      << "Cannot open file for writing " << filename
      << exit(FatalError);
  }
  os  << "# Wavefront OBJ file written " << clock::dateTime().c_str() << nl
    << "o " << os.name().lessExt().name() << nl
    << nl
    << "# points : " << pointLst.size() << nl
    << "# lines  : " << edgeLst.size() << nl;
  os  << nl
    << "# <points count=\"" << pointLst.size() << "\">" << nl;
  // Write vertex coords
  forAll(pointLst, ptI)
  {
    const point& p = pointLst[ptI];
    os  << "v " << p.x() << ' ' << p.y() << ' ' << p.z() << nl;
  }
  os  << "# </points>" << nl
    << nl
    << "# <edges count=\"" << edgeLst.size() << "\">" << endl;
  // Write line connectivity
  forAll(edgeLst, edgeI)
  {
    const edge& e = edgeLst[edgeI];
    os << "l " << (e[0] + 1) << " " << (e[1] + 1) << nl;
  }
  os << "# </edges>" << endl;
}
