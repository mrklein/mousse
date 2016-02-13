// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "delaunay_mesh_tools.hpp"
#include "mesh_tools.hpp"
#include "ofstream.hpp"
#include "point_conversion.hpp"
// Member Functions 
void mousse::DelaunayMeshTools::writeOBJ
(
  const fileName& fName,
  const List<mousse::point>& points
)
{
  if (points.size())
  {
    OFstream str{fName};
    Pout << nl << "Writing " << points.size() << " points from pointList to "
      << str.name() << endl;
    FOR_ALL(points, p)
    {
      meshTools::writeOBJ(str, points[p]);
    }
  }
}
void mousse::DelaunayMeshTools::writeOBJ
(
  const fileName& fName,
  const List<Vb>& points
)
{
  if (points.size())
  {
    OFstream str{fName};
    Pout << nl
      << "Writing " << points.size() << " points from pointList to "
      << str.name() << endl;
    FOR_ALL(points, p)
    {
      meshTools::writeOBJ(str, topoint(points[p].point()));
    }
  }
}
void mousse::DelaunayMeshTools::writeObjMesh
(
  const fileName& fName,
  const pointField& points,
  const faceList& faces
)
{
  OFstream str{fName};
  Pout << nl << "Writing points and faces to " << str.name() << endl;
  FOR_ALL(points, p)
  {
    meshTools::writeOBJ(str, points[p]);
  }
  FOR_ALL(faces, f)
  {
    str<< 'f';
    const face& fP = faces[f];
    FOR_ALL(fP, p)
    {
      str<< ' ' << fP[p] + 1;
    }
    str<< nl;
  }
}
