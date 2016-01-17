// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "starcd_surface_writer.hpp"
#include "meshed_surface_proxy.hpp"
#include "ofstream.hpp"
#include "os_specific.hpp"
#include "make_surface_writer_methods.hpp"
// Static Data Members
namespace mousse
{
  makeSurfaceWriterType(starcdSurfaceWriter);
}
// Private Member Functions 
namespace mousse
{
  template<>
  inline void mousse::starcdSurfaceWriter::writeData
  (
    Ostream& os,
    const scalar& v
  )
  {
    os  << v << nl;
  }
  template<>
  inline void mousse::starcdSurfaceWriter::writeData
  (
    Ostream& os,
    const vector& v
  )
  {
    os  << v[0] << ' ' << v[1] << ' ' << v[2] << nl;
  }
  template<>
  inline void mousse::starcdSurfaceWriter::writeData
  (
    Ostream& os,
    const sphericalTensor& v
  )
  {
    os  << v[0] << nl;
  }
}
template<class Type>
inline void mousse::starcdSurfaceWriter::writeData
(
  Ostream&,
  const Type& /*v*/
)
{}
template<class Type>
void mousse::starcdSurfaceWriter::writeTemplate
(
  const fileName& outputDir,
  const fileName& surfaceName,
  const pointField& /*points*/,
  const faceList& /*faces*/,
  const word& fieldName,
  const Field<Type>& values,
  const bool /*isNodeValues*/,
  const bool verbose
) const
{
  if (!isDir(outputDir))
  {
    mkDir(outputDir);
  }
  OFstream os(outputDir/fieldName + '_' + surfaceName + ".usr");
  if (verbose)
  {
    Info<< "Writing field " << fieldName << " to " << os.name() << endl;
  }
  // no header, just write values
  FOR_ALL(values, elemI)
  {
    os  << elemI+1 << ' ';
    writeData(os, values[elemI]);
  }
}
// Constructors 
mousse::starcdSurfaceWriter::starcdSurfaceWriter()
:
  surfaceWriter()
{}
// Destructor 
mousse::starcdSurfaceWriter::~starcdSurfaceWriter()
{}
// Member Functions 
void mousse::starcdSurfaceWriter::write
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
  fileName outName(outputDir/surfaceName + ".inp");
  if (verbose)
  {
    Info<< "Writing geometry to " << outName << endl;
  }
  MeshedSurfaceProxy<face>(points, faces).write(outName);
}
// create write methods
defineSurfaceWriterWriteField(mousse::starcdSurfaceWriter, scalar);
defineSurfaceWriterWriteField(mousse::starcdSurfaceWriter, vector);
defineSurfaceWriterWriteField(mousse::starcdSurfaceWriter, sphericalTensor);
