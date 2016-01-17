// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MeshedSurfaceProxy
// Description
//   A proxy for writing MeshedSurface, UnsortedMeshedSurface and surfMesh
//   to various file formats.
// SourceFiles
//   meshed_surface_proxy.cpp
//   meshed_surface_proxy_core.cpp
#ifndef meshed_surface_proxy_hpp_
#define meshed_surface_proxy_hpp_
#include "point_field.hpp"
#include "face.hpp"
#include "tri_face.hpp"
#include "surf_zone_list.hpp"
#include "surface_formats_core.hpp"
#include "run_time_selection_tables.hpp"
#include "member_function_selection_tables.hpp"
#include "hash_set.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Face> class MeshedSurface;
template<class Face>
class MeshedSurfaceProxy
:
  public fileFormats::surfaceFormatsCore
{
  //- Private data
    const pointField& points_;
    const List<Face>& faces_;
    const List<surfZone>& zones_;
    const List<label>& faceMap_;
public:
  // Public typedefs
    //- Face type used
    typedef Face FaceType;
  // Static
    //- Runtime type information
    CLASS_NAME("MeshedSurfaceProxy");
    //- The file format types that can be written via MeshedSurfaceProxy
    static wordHashSet writeTypes();
    //- Can this file format type be written via MeshedSurfaceProxy?
    static bool canWriteType(const word& ext, const bool verbose=false);
  // Constructors
    //- Construct from component references
    MeshedSurfaceProxy
    (
      const pointField&,
      const List<Face>&,
      const List<surfZone>& = List<surfZone>(),
      const List<label>& faceMap = List<label>()
    );
  //- Destructor
  virtual ~MeshedSurfaceProxy();
  // Member Function Selectors
    DECLARE_MEMBER_FUNCTION_SELECTION_TABLE
    (
      void,
      MeshedSurfaceProxy,
      write,
      fileExtension,
      (
        const fileName& name,
        const MeshedSurfaceProxy<Face>& surf
      ),
      (name, surf)
    );
    //- Write to file
    static void write(const fileName&, const MeshedSurfaceProxy<Face>&);
  // Member Functions
    // Access
      //- Return const access to the points
      inline const pointField& points() const
      {
        return points_;
      }
      //- Return const access to the faces
      inline const List<Face>& faces() const
      {
        return faces_;
      }
      //- Const access to the surface zones.
      //  If zones are defined, they must be contiguous and cover the
      //  entire surface
      inline const List<surfZone>& surfZones() const
      {
        return zones_;
      }
      //- Const access to the faceMap, zero-sized when unused
      inline const List<label>& faceMap() const
      {
        return faceMap_;
      }
      //- Use faceMap?
      inline bool useFaceMap() const
      {
        return faceMap_.size() == faces_.size();
      }
    // Write
      //- Generic write routine. Chooses writer based on extension.
      virtual void write(const fileName& name) const
      {
        write(name, *this);
      }
      //- Write to database
      virtual void write(const Time&, const word& surfName = "") const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "meshed_surface_proxy.cpp"
#endif
#endif
