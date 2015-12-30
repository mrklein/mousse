// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "foam_file_surface_writer.hpp"
#include "ofstream.hpp"
#include "os_specific.hpp"
#include "make_surface_writer_methods.hpp"
// Static Data Members
namespace mousse
{
  makeSurfaceWriterType(foamFileSurfaceWriter);
}
// Private Member Functions 
template<class Type>
void mousse::foamFileSurfaceWriter::writeTemplate
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
  fileName surfaceDir(outputDir/surfaceName);
  if (!isDir(surfaceDir))
  {
    mkDir(surfaceDir);
  }
  if (verbose)
  {
    Info<< "Writing field " << fieldName << " to " << surfaceDir << endl;
  }
  // geometry should already have been written
  // Values to separate directory (e.g. "scalarField/p")
  fileName foamName(pTraits<Type>::typeName);
  fileName valuesDir(surfaceDir  / (foamName + Field<Type>::typeName));
  if (!isDir(valuesDir))
  {
    mkDir(valuesDir);
  }
  // values
  OFstream(valuesDir/fieldName)()  << values;
}
// Constructors 
mousse::foamFileSurfaceWriter::foamFileSurfaceWriter()
:
  surfaceWriter()
{}
// Destructor 
mousse::foamFileSurfaceWriter::~foamFileSurfaceWriter()
{}
// Member Functions 
void mousse::foamFileSurfaceWriter::write
(
  const fileName& outputDir,
  const fileName& surfaceName,
  const pointField& points,
  const faceList& faces,
  const bool verbose
) const
{
  fileName surfaceDir(outputDir/surfaceName);
  if (!isDir(surfaceDir))
  {
    mkDir(surfaceDir);
  }
  if (verbose)
  {
    Info<< "Writing geometry to " << surfaceDir << endl;
  }
  // Points
  OFstream(surfaceDir/"points")() << points;
  // Faces
  OFstream(surfaceDir/"faces")() << faces;
  // Face centers. Not really necessary but very handy when reusing as inputs
  // for e.g. timeVaryingMapped bc.
  pointField faceCentres(faces.size(),point::zero);
  forAll(faces, faceI)
  {
    faceCentres[faceI] = faces[faceI].centre(points);
  }
  OFstream(surfaceDir/"faceCentres")() << faceCentres;
}
// create write methods
defineSurfaceWriterWriteFields(mousse::foamFileSurfaceWriter);
