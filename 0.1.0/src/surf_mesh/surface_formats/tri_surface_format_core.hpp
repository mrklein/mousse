// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::TRIsurfaceFormatCore
// Description
//   Internal class used by the TRIsurfaceFormat
// SourceFiles
//   tri_surface_format_core.cpp
#ifndef tri_surface_format_core_hpp_
#define tri_surface_format_core_hpp_
#include "surface_formats_core.hpp"
#include "tri_face.hpp"
#include "ifstream.hpp"
#include "ostream.hpp"
#include "ofstream.hpp"
namespace mousse
{
namespace fileFormats
{
class TRIsurfaceFormatCore
:
  public surfaceFormatsCore
{
  // Private Data
    bool sorted_;
    //- The points supporting the facets
    pointField points_;
    //- The zones associated with the faces
    List<label> zoneIds_;
    //- The solid count, in the order of their first appearance
    List<label> sizes_;
  // Private Member Functions
    bool read(const fileName&);
public:
  // Constructors
    //- Read from file, filling in the information
    TRIsurfaceFormatCore(const fileName&);
    //- Disallow default bitwise copy construct
    TRIsurfaceFormatCore(const TRIsurfaceFormatCore&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const TRIsurfaceFormatCore&) = delete;
  //- Destructor
  ~TRIsurfaceFormatCore();
  // Member Functions
    //- File read was already sorted
    bool sorted() const
    {
      return sorted_;
    }
    //- Flush all values
    void clear()
    {
      sorted_ = true;
      points_.clear();
      zoneIds_.clear();
      sizes_.clear();
    }
    //- Return full access to the points
    pointField& points()
    {
      return points_;
    }
    //- Return full access to the zones
    List<label>& zoneIds()
    {
      return zoneIds_;
    }
    //- The list of zone sizes in the order of their first appearance
    List<label>& sizes()
    {
      return sizes_;
    }
};
}  // namespace fileFormats
}  // namespace mousse
#endif
