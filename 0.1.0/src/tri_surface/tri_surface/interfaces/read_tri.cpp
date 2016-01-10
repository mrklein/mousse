// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
#include "stl_point.hpp"
#include "sl_list.hpp"
#include "ifstream.hpp"
#include "read_hex_label.hpp"
#include "string_list.hpp"
#include "istring_stream.hpp"
#include "ostring_stream.hpp"

// Member Functions
bool mousse::triSurface::readTRI(const fileName& TRIfileName)
{
  IFstream TRIfile(TRIfileName);
  if (!TRIfile.good())
  {
    FATAL_ERROR_IN("triSurface::readTRI(const fileName&)")
      << "Cannot read file " << TRIfileName
      << exit(FatalError);
  }
  SLList<STLpoint> STLpoints;
  SLList<label> STLlabels;
  HashTable<label, string> STLsolidNames;
  // Max region number so far
  label maxRegion = 0;
  while (TRIfile)
  {
    string line = getLineNoComment(TRIfile);
    if (line.empty())
    {
      break;
    }
    IStringStream lineStream(line);
    STLpoint p
    (
      readScalar(lineStream),
      readScalar(lineStream),
      readScalar(lineStream)
    );
    if (!lineStream) break;
    STLpoints.append(p);
    STLpoints.append
    (
      STLpoint
      (
        readScalar(lineStream),
        readScalar(lineStream),
        readScalar(lineStream)
      )
    );
    STLpoints.append
    (
      STLpoint
      (
        readScalar(lineStream),
        readScalar(lineStream),
        readScalar(lineStream)
      )
    );
    // Region/colour in .tri file starts with 0x. Skip.
    char zero;
    lineStream >> zero;
    word rawSolidName(lineStream);
    word solidName("patch" + rawSolidName(1, rawSolidName.size()-1));
    label region  = -1;
    HashTable<label, string>::const_iterator fnd =
      STLsolidNames.find(solidName);
    if (fnd != STLsolidNames.end())
    {
      region = fnd();
    }
    else
    {
      Pout<< "Mapping triangle colour 0" << rawSolidName
        << " to region " << maxRegion << " name " << solidName
        << endl;
      region = maxRegion++;
      STLsolidNames.insert(solidName, region);
    }
    STLlabels.append(region);
  }
  pointField rawPoints(STLpoints.size());
  label pointI = 0;
  FOR_ALL_CONST_ITER(SLList<STLpoint>, STLpoints, iter)
  {
    rawPoints[pointI++] = *iter;
  }
  setSize(STLlabels.size());
  // Assign triangles
  pointI = 0;
  SLList<label>::const_iterator iter = STLlabels.begin();
  FOR_ALL(*this, i)
  {
    operator[](i)[0] = pointI++;
    operator[](i)[1] = pointI++;
    operator[](i)[2] = pointI++;
    operator[](i).region() = *iter;
    ++iter;
  }
  // Assign coordinates
  storedPoints().transfer(rawPoints);
  // Merge duplicate points
  stitchTriangles();
  // Convert solidNames into regionNames
  stringList names(STLsolidNames.toc());
  patches_.setSize(names.size());
  FOR_ALL(names, nameI)
  {
    patches_[nameI].name() = names[nameI];
    patches_[nameI].geometricType() = "empty";
  }
  return true;
}
