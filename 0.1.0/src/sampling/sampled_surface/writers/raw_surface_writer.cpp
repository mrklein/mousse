// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "raw_surface_writer.hpp"
#include "ofstream.hpp"
#include "os_specific.hpp"
#include "iomanip.hpp"
#include "make_surface_writer_methods.hpp"
// Static Data Members
namespace mousse
{
  makeSurfaceWriterType(rawSurfaceWriter);
}
// Private Member Functions 
inline void mousse::rawSurfaceWriter::writeLocation
(
  Ostream& os,
  const pointField& points,
  const label pointI
)
{
  const point& pt = points[pointI];
  os  << pt.x() << ' ' << pt.y() << ' ' << pt.z() << ' ';
}
inline void mousse::rawSurfaceWriter::writeLocation
(
  Ostream& os,
  const pointField& points,
  const faceList& faces,
  const label faceI
)
{
  const point& ct = faces[faceI].centre(points);
  os  << ct.x() << ' ' << ct.y() << ' ' << ct.z() << ' ';
}
namespace mousse
{
  template<>
  void mousse::rawSurfaceWriter::writeHeader
  (
    Ostream& os,
    const word& fieldName,
    const Field<scalar>& values
  )
  {
    os  << values.size() << nl
      << "#  x  y  z  " << fieldName << nl;
  }
  template<>
  void mousse::rawSurfaceWriter::writeHeader
  (
    Ostream& os,
    const word& fieldName,
    const Field<vector>& values
  )
  {
    os  << values.size() << nl
      << "#  x  y  z  "
      << fieldName << "_x  "
      << fieldName << "_y  "
      << fieldName << "_z  "
      << endl;
  }
  template<>
  void mousse::rawSurfaceWriter::writeHeader
  (
    Ostream& os,
    const word& fieldName,
    const Field<sphericalTensor>& values
  )
  {
    os  << values.size() << nl
      << "#  ii  "
      << fieldName << "_ii" << nl;
  }
  template<>
  void mousse::rawSurfaceWriter::writeHeader
  (
    Ostream& os,
    const word& fieldName,
    const Field<symmTensor>& values
  )
  {
    os  << values.size() << nl
      << "#  xx  xy  xz  yy  yz ";
    for (int i=0; i<6; ++i)
    {
      os  << fieldName << "_" << i << "  ";
    }
    os  << endl;
  }
  template<>
  void mousse::rawSurfaceWriter::writeHeader
  (
    Ostream& os,
    const word& fieldName,
    const Field<tensor>& values
  )
  {
    os  << values.size() << nl
      << "#  xx  xy  xz  yx  yy  yz  zx  zy  zz";
    for (int i=0; i<9; ++i)
    {
      os  << fieldName << "_" << i << "  ";
    }
    os  << nl;
  }
  template<>
  inline void mousse::rawSurfaceWriter::writeData
  (
    Ostream& os,
    const scalar& v
  )
  {
    os  << v << nl;
  }
  template<>
  inline void mousse::rawSurfaceWriter::writeData
  (
    Ostream& os,
    const vector& v
  )
  {
    os  << v[0] << ' ' << v[1] << ' ' << v[2] << nl;
  }
  template<>
  inline void mousse::rawSurfaceWriter::writeData
  (
    Ostream& os,
    const sphericalTensor& v
  )
  {
    os  << v[0] << nl;
  }
  template<>
  inline void mousse::rawSurfaceWriter::writeData
  (
    Ostream& os,
    const symmTensor& v
  )
  {
    os  << v[0] << ' ' << v[1] << ' ' << v[2] << ' '
      << v[3] << ' ' << v[4] << ' ' << v[5] << nl;
  }
  template<>
  inline void mousse::rawSurfaceWriter::writeData
  (
    Ostream& os,
    const tensor& v
  )
  {
    os  << v[0] << ' ' << v[1] << ' ' << v[2] << ' '
      << v[3] << ' ' << v[4] << ' ' << v[5] << ' '
      << v[6] << ' ' << v[7] << ' ' << v[8] << nl;
  }
}
template<class Type>
void mousse::rawSurfaceWriter::writeTemplate
(
  const fileName& outputDir,
  const fileName& surfaceName,
  const pointField& points,
  const faceList& faces,
  const word& fieldName,
  const Field<Type>& values,
  const bool isNodeValues,
  const bool verbose
) const
{
  if (!isDir(outputDir))
  {
    mkDir(outputDir);
  }
  OFstream os(outputDir/fieldName + '_' + surfaceName + ".raw");
  if (verbose)
  {
    Info<< "Writing field " << fieldName << " to " << os.name() << endl;
  }
  // header
  os  << "# " << fieldName;
  if (isNodeValues)
  {
    os  << "  POINT_DATA ";
  }
  else
  {
    os  << "  FACE_DATA ";
  }
  // header
  writeHeader(os, fieldName, values);
  // values
  if (isNodeValues)
  {
    forAll(values, elemI)
    {
      writeLocation(os, points, elemI);
      writeData(os, values[elemI]);
    }
  }
  else
  {
    forAll(values, elemI)
    {
      writeLocation(os, points, faces, elemI);
      writeData(os, values[elemI]);
    }
  }
}
// Constructors 
mousse::rawSurfaceWriter::rawSurfaceWriter()
:
  surfaceWriter()
{}
// Destructor 
mousse::rawSurfaceWriter::~rawSurfaceWriter()
{}
// Member Functions 
void mousse::rawSurfaceWriter::write
(
  const fileName& outputDir,
  const fileName& surfaceName,
  const pointField& points,
  const faceList& faces,
  const bool verbose
) const
{
  if (!isDir(outputDir))
  {
    mkDir(outputDir);
  }
  OFstream os(outputDir/surfaceName + ".raw");
  if (verbose)
  {
    Info<< "Writing geometry to " << os.name() << endl;
  }
  // header
  os  << "# geometry NO_DATA " << faces.size() << nl
    << "#  x  y  z" << nl;
  // Write faces centres
  forAll(faces, elemI)
  {
    writeLocation(os, points, faces, elemI);
    os  << nl;
  }
  os  << nl;
}
// create write methods
defineSurfaceWriterWriteFields(mousse::rawSurfaceWriter);
