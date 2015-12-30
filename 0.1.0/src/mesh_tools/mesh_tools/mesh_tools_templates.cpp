// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class FaceType>
void mousse::meshTools::writeOBJ
(
  Ostream& os,
  const UList<FaceType>& faces,
  const pointField& points,
  const labelList& faceLabels
)
{
  Map<label> foamToObj(4*faceLabels.size());
  label vertI = 0;
  forAll(faceLabels, i)
  {
    const FaceType& f = faces[faceLabels[i]];
    forAll(f, fp)
    {
      if (foamToObj.insert(f[fp], vertI))
      {
        writeOBJ(os, points[f[fp]]);
        vertI++;
      }
    }
    os << 'l';
    forAll(f, fp)
    {
      os << ' ' << foamToObj[f[fp]]+1;
    }
    os << ' ' << foamToObj[f[0]]+1 << endl;
  }
}
template<class FaceType>
void mousse::meshTools::writeOBJ
(
  Ostream& os,
  const UList<FaceType>& faces,
  const pointField& points
)
{
  labelList allFaces(faces.size());
  forAll(allFaces, i)
  {
    allFaces[i] = i;
  }
  writeOBJ(os, faces, points, allFaces);
}
