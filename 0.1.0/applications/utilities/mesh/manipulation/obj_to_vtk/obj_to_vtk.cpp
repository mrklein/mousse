// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "ofstream.hpp"
#include <fstream>
#include <sstream>
#include "istring_stream.hpp"
#include "point.hpp"
#include "dynamic_list.hpp"


using namespace mousse;


string getLine(std::ifstream& is)
{
  string line;
  do {
    std::getline(is, line);
  } while (line.size() && line[0] == '#');
  return line;
}


// Read space-separated vertices (with optional '/' arguments)
labelList parseVertices(const string& line)
{
  DynamicList<label> verts;
  // Assume 'l' is followed by space.
  string::size_type endNum = 1;
  do {
    string::size_type startNum = line.find_first_not_of(' ', endNum);
    if (startNum == string::npos) {
      break;
    }
    endNum = line.find(' ', startNum);
    string vertexSpec;
    if (endNum != string::npos) {
      vertexSpec = line.substr(startNum, endNum - startNum);
    } else {
      vertexSpec = line.substr(startNum, line.size() - startNum);
    }
    string::size_type slashPos = vertexSpec.find('/');
    label vertI = 0;
    if (slashPos != string::npos) {
      IStringStream intStream{vertexSpec.substr(0, slashPos)};
      intStream >> vertI;
    } else {
      IStringStream intStream{vertexSpec};
      intStream >> vertI;
    }
    verts.append(vertI - 1);
  } while (true);
  return verts.shrink();
}


int main(int argc, char *argv[])
{
  argList::noParallel();
  argList::validArgs.append("OBJ file");
  argList::validArgs.append("output VTK file");
  argList args{argc, argv};
  const fileName objName = args[1];
  const fileName outName = args[2];
  std::ifstream OBJfile{objName.c_str()};
  if (!OBJfile.good()) {
    FATAL_ERROR_IN(args.executable())
      << "Cannot read file " << objName << exit(FatalError);
  }
  // Points and lines
  DynamicList<point> points;
  DynamicList<vector> pointNormals;
  DynamicList<labelList> polyLines;
  DynamicList<labelList> polygons;
  bool hasWarned = false;
  label lineNo = 0;
  while (OBJfile.good()) {
    string line = getLine(OBJfile);
    lineNo++;
    // Read first word
    IStringStream lineStream{line};
    word cmd;
    lineStream >> cmd;
    if (cmd == "v") {
      scalar x, y, z;
      lineStream >> x >> y >> z;
      points.append(point(x, y, z));
    } else if (cmd == "vn") {
      scalar x, y, z;
      lineStream >> x >> y >> z;
      pointNormals.append(vector(x, y, z));
    } else if (cmd == "l") {
      polyLines.append(parseVertices(line));
    } else if (cmd == "f") {
      polygons.append(parseVertices(line));
    } else if (cmd != "") {
      if (!hasWarned) {
        hasWarned = true;
        WARNING_IN(args.executable())
          << "Unrecognized OBJ command " << cmd << nl
          << "In line " << lineStream.str()
          << " at linenumber " << lineNo << nl
          << "Only recognized commands are 'v' and 'l'.\n"
          << "If this is a surface command use surfaceConvert instead"
          << " to convert to a file format that can be read by VTK"
          << endl;
      }
    }
  }
  // Write as vtk 'polydata' file
  OFstream outFile{outName};
  outFile
    << "# vtk DataFile Version 2.0\n"
    << objName << nl
    << "ASCII\n"
    << "DATASET POLYDATA\n"
    << "POINTS " << points.size() << " float\n";
  FOR_ALL(points, i) {
    const point& pt = points[i];
    outFile << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
  }
  outFile
    << "VERTICES " << points.size() << ' ' << (2 * points.size()) << nl;
  FOR_ALL(points, i) {
    outFile << 1 << ' ' << i << nl;
  }
  label nItems = 0;
  FOR_ALL(polyLines, polyI) {
    nItems += polyLines[polyI].size() + 1;
  }
  outFile
    << "LINES " << polyLines.size() << ' ' << nItems << nl;
  FOR_ALL(polyLines, polyI) {
    const labelList& line = polyLines[polyI];
    outFile << line.size();
    FOR_ALL(line, i) {
      outFile << ' ' << line[i];
    }
    outFile << nl;
  }
  nItems = 0;
  FOR_ALL(polygons, polyI) {
    nItems += polygons[polyI].size() + 1;
  }
  outFile
    << "POLYGONS " << polygons.size() << ' ' << nItems << nl;
  FOR_ALL(polygons, polyI) {
    const labelList& line = polygons[polyI];
    outFile << line.size();
    FOR_ALL(line, i) {
      outFile << ' ' << line[i];
    }
    outFile << nl;
  }
  outFile
    << "POINT_DATA " << points.size() << nl
    << "SCALARS pointID float 1\n"
    << "LOOKUP_TABLE default\n";
  FOR_ALL(points, i) {
    outFile << i;
    if ((i % 10) == 1) {
      outFile << nl;
    } else {
      outFile << ' ';
    }
  }
  if (!pointNormals.empty()) {
    outFile << nl << "NORMALS pointNormals float\n";
    FOR_ALL(pointNormals, i) {
      const vector& n = pointNormals[i];
      outFile << n.x() << ' ' << n.y() << ' ' << n.z() << nl;
    }
  }
  Info << "End\n" << endl;
  return 0;
}

