// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::AC3DsurfaceFormatCore
// Description
//   Internal class used by the AC3DsurfaceFormat
// SourceFiles
//   ac3d_surface_format_core.cpp
#ifndef ac3d_surface_format_core_hpp_
#define ac3d_surface_format_core_hpp_
#include "ifstream.hpp"
#include "ostream.hpp"
#include "ofstream.hpp"
#include "meshed_surface.hpp"
namespace mousse
{
namespace fileFormats
{
class AC3DsurfaceFormatCore
{
protected:
  // Protected Member Functions
    //- Read a type via IStringStream
    template<class Type>
    static Type parse(const string&);
    //- Read cmd, args from IFstream
    static bool readCmd(IFstream&, string& cmd, string& args);
    //- Cue up to cmd, reading args
    static bool cueTo(IFstream&, const string& cmd, string& args);
    //- Cue up to cmd, reading args or exit with a FatalError
    //  returns the command args
    static string cueToOrDie
    (
      IFstream&,
      const string& cmd,
      const string& errorMsg=string::null
    );
    //- Write header with materials
    static void writeHeader(Ostream&, const UList<surfZone>&);
};
}  // namespace fileFormats
}  // namespace mousse
#ifdef NoRepository
#   include "ac3d_surface_format_core_templates.cpp"
#endif
#endif
