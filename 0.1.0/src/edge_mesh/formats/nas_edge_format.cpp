// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nas_edge_format.hpp"
#include "ifstream.hpp"
#include "istring_stream.hpp"
#include "packed_bool_list.hpp"


// Constructors 
mousse::fileFormats::NASedgeFormat::NASedgeFormat
(
  const fileName& filename
)
{
  read(filename);
}


// Member Functions 
bool mousse::fileFormats::NASedgeFormat::read
(
  const fileName& filename
)
{
  clear();
  IFstream is{filename};
  if (!is.good()) {
    FATAL_ERROR_IN
    (
      "fileFormats::NASedgeFormat::read(const fileName&)"
    )
    << "Cannot read file " << filename
    << exit(FatalError);
  }
  DynamicList<point> dynPoints;
  DynamicList<edge> dynEdges;
  DynamicList<label> pointId;     // Nastran index of points
  while (is.good()) {
    string line;
    is.getLine(line);
    // Skip empty or comment
    if (line.empty() || line[0] == '$') {
      continue;
    }
    // Check if character 72 is continuation
    if (line.size() > 72 && line[72] == '+') {
      line = line.substr(0, 72);
      while (true) {
        string buf;
        is.getLine(buf);
        if (buf.size() > 72 && buf[72] == '+') {
          line += buf.substr(8, 64);
        } else {
          line += buf.substr(8, buf.size()-8);
          break;
        }
      }
    }
    // Read first word
    IStringStream lineStream{line};
    word cmd;
    lineStream >> cmd;
    if (cmd == "CBEAM" || cmd == "CROD") {
      edge e;
      // label groupId = readLabel(IStringStream(line.substr(16,8))());
      e[0] = readLabel(IStringStream(line.substr(24,8))());
      e[1] = readLabel(IStringStream(line.substr(32,8))());
      // discard groupID
      dynEdges.append(e);
    } else if (cmd == "PLOTEL") {
      edge e;
      // label groupId = readLabel(IStringStream(line.substr(16,8))());
      e[0] = readLabel(IStringStream(line.substr(16,8))());
      e[1] = readLabel(IStringStream(line.substr(24,8))());
      // discard groupID
      dynEdges.append(e);
    } else if (cmd == "GRID") {
      label index = readLabel(IStringStream(line.substr(8,8))());
      scalar x = parseNASCoord(line.substr(24, 8));
      scalar y = parseNASCoord(line.substr(32, 8));
      scalar z = parseNASCoord(line.substr(40, 8));
      pointId.append(index);
      dynPoints.append(point(x, y, z));
    } else if (cmd == "GRID*") {
      // Long format is on two lines with '*' continuation symbol
      // on start of second line.
      // Typical line (spaces compacted)
      // GRID*      126   0 -5.55999875E+02 -5.68730474E+02
      // *         2.14897901E+02
      label index = readLabel(IStringStream(line.substr(8,16))());
      scalar x = parseNASCoord(line.substr(40, 16));
      scalar y = parseNASCoord(line.substr(56, 16));
      is.getLine(line);
      if (line[0] != '*') {
        FATAL_ERROR_IN
        (
          "fileFormats::NASedgeFormat::read(const fileName&)"
        )
        << "Expected continuation symbol '*' when reading GRID*"
        << " (double precision coordinate) format" << nl
        << "Read:" << line << nl
        << "File:" << is.name() << " line:" << is.lineNumber()
        << exit(FatalError);
      }
      scalar z = parseNASCoord(line.substr(8, 16));
      pointId.append(index);
      dynPoints.append(point(x, y, z));
    }
  }
  // transfer to normal lists
  storedPoints().transfer(dynPoints);
  pointId.shrink();
  dynEdges.shrink();
  // Build inverse mapping (NASTRAN pointId -> index)
  Map<label> mapPointId{2*pointId.size()};
  FOR_ALL(pointId, i) {
    mapPointId.insert(pointId[i], i);
  }
  // note which points were really used and which can be culled
  PackedBoolList usedPoints{points().size()};
  // Pass1: relabel edges
  // ~~~~~~~~~~~~~~~~~~~~
  FOR_ALL(dynEdges, i) {
    edge& e = dynEdges[i];
    e[0] = mapPointId[e[0]];
    e[1] = mapPointId[e[1]];
    usedPoints.set(e[0]);
    usedPoints.set(e[1]);
  }
  pointId.clearStorage();
  mapPointId.clear();
  // not all the points were used, cull them accordingly
  if (unsigned(points().size()) != usedPoints.count()) {
    label nUsed = 0;
    pointField& pts = storedPoints();
    FOR_ALL(pts, pointI) {
      if (usedPoints.get(pointI)) {
        if (nUsed != pointI) {
          pts[nUsed] = pts[pointI];
        }
        // map prev -> new id
        mapPointId[pointI] = nUsed;
        ++nUsed;
      }
    }
    pts.setSize(nUsed);
    // renumber edge vertices
    FOR_ALL(dynEdges, edgeI) {
      edge& e = dynEdges[edgeI];
      e[0] = mapPointId[e[0]];
      e[1] = mapPointId[e[1]];
    }
  }
  // transfer to normal lists
  storedEdges().transfer(dynEdges);
  return true;
}

