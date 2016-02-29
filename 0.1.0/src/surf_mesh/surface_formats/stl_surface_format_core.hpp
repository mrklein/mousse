#ifndef SURF_MESH_SURFACE_FORMATS_STL_SURFACE_FORMAT_CORE_HPP_
#define SURF_MESH_SURFACE_FORMATS_STL_SURFACE_FORMAT_CORE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::STLsurfaceFormatCore
// Description
//   Internal class used by the STLsurfaceFormat
// SourceFiles
//   stl_surface_format_core.cpp
//   stl_surface_format_ascii.l
#include "stl_triangle.hpp"
#include "tri_face.hpp"
#include "ifstream.hpp"
#include "ostream.hpp"
namespace mousse
{
namespace fileFormats
{
class STLsurfaceFormatCore
{
  // Private Data
    bool sorted_;
    //- The points supporting the facets
    pointField points_;
    //- The zones associated with the faces
    List<label> zoneIds_;
    //- The solid names, in the order of their first appearance
    List<word> names_;
    //- The solid count, in the order of their first appearance
    List<label> sizes_;
  // Private Member Functions
    //- Determine the file type
    static int detectBINARY(const fileName&);
    //- Read ASCII
    bool readASCII(istream&, const off_t);
    //- Read BINARY
    bool readBINARY(istream&, const off_t);
public:
  // Static Data
    //- The number of bytes in the STL binary header
    static const unsigned int headerSize = 80;
  // Static Member Functions
    //- Write "STL binary file" and number of triangles to stream
    static void writeHeaderBINARY(ostream&, unsigned int);
  // Constructors
    //- Read from file, filling in the information
    STLsurfaceFormatCore(const fileName&);
    //- Disallow default bitwise copy construct
    STLsurfaceFormatCore(const STLsurfaceFormatCore&) = delete;
    //- Disallow default bitwise assignment
    STLsurfaceFormatCore& operator=(const STLsurfaceFormatCore&) = delete;
  //- Destructor
  ~STLsurfaceFormatCore();
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
      names_.clear();
      sizes_.clear();
    }
    //- Return full access to the points
    pointField& points()
    {
      return points_;
    }
    //- Return full access to the zoneIds
    List<label>& zoneIds()
    {
      return zoneIds_;
    }
    //- The list of solid names in the order of their first appearance
    List<word>& names()
    {
      return names_;
    }
    //- The list of solid sizes in the order of their first appearance
    List<label>& sizes()
    {
      return sizes_;
    }
};
}  // namespace fileFormats
}  // namespace mousse
#endif
