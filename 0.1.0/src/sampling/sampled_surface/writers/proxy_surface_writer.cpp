// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "proxy_surface_writer.hpp"
#include "meshed_surface_proxy.hpp"
#include "ofstream.hpp"
#include "os_specific.hpp"
#include "make_surface_writer_methods.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(proxySurfaceWriter, 0);
}
// Constructors 
mousse::proxySurfaceWriter::proxySurfaceWriter(const word& ext)
:
  surfaceWriter(),
  ext_(ext)
{}
// Destructor 
mousse::proxySurfaceWriter::~proxySurfaceWriter()
{}
// Member Functions 
void mousse::proxySurfaceWriter::write
(
  const fileName& outputDir,
  const fileName& surfaceName,
  const pointField& points,
  const faceList& faces,
  const bool verbose
) const
{
  // avoid bad values
  if (ext_.empty())
  {
    return;
  }
  if (!isDir(outputDir))
  {
    mkDir(outputDir);
  }
  fileName outName(outputDir/surfaceName + "." + ext_);
  if (verbose)
  {
    Info<< "Writing geometry to " << outName << endl;
  }
  MeshedSurfaceProxy<face>(points, faces).write(outName);
}
