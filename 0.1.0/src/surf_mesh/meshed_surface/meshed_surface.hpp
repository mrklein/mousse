// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MeshedSurface
// Description
//   A surface geometry mesh with zone information, not to be confused with
//   the similarly named surfaceMesh, which actually refers to the cell faces
//   of a volume mesh.
//   A MeshedSurface can have zero or more surface zones (roughly equivalent
//   to faceZones for a polyMesh). If surface zones are defined, they must
//   be contiguous and cover all of the faces.
//   The MeshedSurface is intended for surfaces from a variety of sources.
//   - A set of points and faces without any surface zone information.
//   - A set of points and faces with randomly ordered zone information.
//    This could arise, for example, from reading external file formats
//    such as STL, etc.
// SourceFiles
//   meshed_surface.cpp
#ifndef meshed_surface_hpp_
#define meshed_surface_hpp_
#include "primitive_patch.hpp"
#include "patch_tools.hpp"
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
class Time;
class surfMesh;
class polyBoundaryMesh;
template<class Face> class MeshedSurface;
template<class Face> class MeshedSurfaceProxy;
template<class Face> class UnsortedMeshedSurface;
template<class Face>
class MeshedSurface
:
  public PrimitivePatch<Face, ::mousse::List, pointField, point>,
  public fileFormats::surfaceFormatsCore
{
  // friends - despite different face representationsx
  template<class Face2> friend class MeshedSurface;
  template<class Face2> friend class UnsortedMeshedSurface;
  friend class surfMesh;
private:
  // Private typedefs for convenience
    typedef PrimitivePatch
    <
      Face,
      ::mousse::List,
      pointField,
      point
    >
    ParentType;
    typedef UnsortedMeshedSurface<Face>  FriendType;
    typedef MeshedSurfaceProxy<Face>     ProxyType;
  // Private Member Data
    //- Zone information
    // (face ordering nFaces/startFace only used during reading/writing)
    List<surfZone> zones_;
protected:
  // Protected Member functions
    //- Transfer points/zones and transcribe face -> triFace
    void transcribe(MeshedSurface<face>&);
    //- Basic sanity check on zones
    void checkZones();
    //- Non-const access to global points
    pointField& storedPoints()
    {
      return const_cast<pointField&>(ParentType::points());
    }
    //- Non-const access to the faces
    List<Face>& storedFaces()
    {
      return static_cast<List<Face> &>(*this);
    }
    //- Non-const access to the zones
    surfZoneList& storedZones()
    {
      return zones_;
    }
    //- Sort faces by zones and store sorted faces
    void sortFacesAndStore
    (
      const Xfer<List<Face> >& unsortedFaces,
      const Xfer<List<label> >& zoneIds,
      const bool sorted
    );
    //- Set new zones from faceMap
    virtual void remapFaces(const labelUList& faceMap);
public:
  // Public typedefs
    //- Face type used
    typedef Face FaceType;
    //- Runtime type information
    ClassName("MeshedSurface");
  // Static
    //- Face storage only handles triangulated faces
    inline static bool isTri();
    //- Can we read this file format?
    static bool canRead(const fileName&, const bool verbose=false);
    //- Can we read this file format?
    static bool canReadType(const word& ext, const bool verbose=false);
    //- Can we write this file format?
    static bool canWriteType(const word& ext, const bool verbose=false);
    static wordHashSet readTypes();
    static wordHashSet writeTypes();
  // Constructors
    //- Construct null
    MeshedSurface();
    //- Construct by transferring components (points, faces, zones).
    MeshedSurface
    (
      const Xfer<pointField>&,
      const Xfer<List<Face> >&,
      const Xfer<surfZoneList>&
    );
    //- Construct by transferring components (points, faces).
    //  Use zone information if available
    MeshedSurface
    (
      const Xfer<pointField>&,
      const Xfer<List<Face> >&,
      const labelUList& zoneSizes = labelUList(),
      const UList<word>& zoneNames = UList<word>()
    );
    //- Construct as copy
    MeshedSurface(const MeshedSurface&);
    //- Construct from a UnsortedMeshedSurface
    MeshedSurface(const UnsortedMeshedSurface<Face>&);
    //- Construct from a boundary mesh with local points/faces
    MeshedSurface
    (
      const polyBoundaryMesh&,
      const bool globalPoints=false
    );
    //- Construct from a surfMesh
    MeshedSurface(const surfMesh&);
    //- Construct by transferring the contents from a UnsortedMeshedSurface
    MeshedSurface(const Xfer<UnsortedMeshedSurface<Face> >&);
    //- Construct by transferring the contents from a MeshedSurface
    MeshedSurface(const Xfer<MeshedSurface<Face> >&);
    //- Construct from file name (uses extension to determine type)
    MeshedSurface(const fileName&);
    //- Construct from file name (uses extension to determine type)
    MeshedSurface(const fileName&, const word& ext);
    //- Construct from database
    MeshedSurface(const Time&, const word& surfName="");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      MeshedSurface,
      fileExtension,
      (
        const fileName& name
      ),
      (name)
    );
  // Selectors
    //- Select constructed from filename (explicit extension)
    static autoPtr<MeshedSurface> New
    (
      const fileName&,
      const word& ext
    );
    //- Select constructed from filename (implicit extension)
    static autoPtr<MeshedSurface> New(const fileName&);
  //- Destructor
  virtual ~MeshedSurface();
  // Member Function Selectors
    declareMemberFunctionSelectionTable
    (
      void,
      UnsortedMeshedSurface,
      write,
      fileExtension,
      (
        const fileName& name,
        const MeshedSurface<Face>& surf
      ),
      (name, surf)
    );
    //- Write to file
    static void write(const fileName&, const MeshedSurface<Face>&);
  // Member Functions
    // Access
      //- The surface size is the number of faces
      label size() const
      {
        return ParentType::size();
      }
      //- Return const access to the faces
      inline const List<Face>& faces() const
      {
        return static_cast<const List<Face> &>(*this);
      }
      //- Const access to the surface zones.
      //  If zones are defined, they must be contiguous and cover the
      //  entire surface
      const List<surfZone>& surfZones() const
      {
        return zones_;
      }
      //- Add surface zones
      virtual void addZones
      (
        const UList<surfZone>&,
        const bool cullEmpty=false
      );
      //- Add surface zones
      virtual void addZones
      (
        const labelUList& sizes,
        const UList<word>& names,
        const bool cullEmpty=false
      );
      //- Add surface zones
      virtual void addZones
      (
        const labelUList& sizes,
        const bool cullEmpty=false
      );
      //- Remove surface zones
      virtual void removeZones();
    // Edit
      //- Clear all storage
      virtual void clear();
      //- Move points
      virtual void movePoints(const pointField&);
      //- Scale points. A non-positive factor is ignored
      virtual void scalePoints(const scalar);
      //- Reset primitive data (points, faces and zones)
      //  Note, optimized to avoid overwriting data (with Xfer::null)
      virtual void reset
      (
        const Xfer<pointField >& points,
        const Xfer<List<Face> >& faces,
        const Xfer<surfZoneList>& zones
      );
      //- Reset primitive data (points, faces and zones)
      //  Note, optimized to avoid overwriting data (with Xfer::null)
      virtual void reset
      (
        const Xfer<List<point> >& points,
        const Xfer<List<Face> >& faces,
        const Xfer<surfZoneList >& zones
      );
      //- Remove invalid faces
      virtual void cleanup(const bool verbose);
      virtual bool stitchFaces
      (
        const scalar tol=SMALL,
        const bool verbose=false
      );
      virtual bool checkFaces
      (
        const bool verbose=false
      );
      //- Triangulate in-place, returning the number of triangles added
      virtual label triangulate();
      //- Triangulate in-place, returning the number of triangles added
      //  and setting a map of original face Ids.
      //  The faceMap is zero-sized when no triangulation was done.
      virtual label triangulate(List<label>& faceMap);
      //- Return new surface.
      //  Returns return pointMap, faceMap from subsetMeshMap
      MeshedSurface subsetMesh
      (
        const labelHashSet& include,
        labelList& pointMap,
        labelList& faceMap
      ) const;
      //- Return new surface.
      MeshedSurface subsetMesh
      (
        const labelHashSet& include
      ) const;
      //- Transfer the contents of the argument and annul the argument
      void transfer(MeshedSurface<Face>&);
      //- Transfer the contents of the argument and annul the argument
      void transfer(UnsortedMeshedSurface<Face>&);
      //- Transfer contents to the Xfer container
      Xfer<MeshedSurface<Face> > xfer();
    // Read
      //- Read from file. Chooses reader based on explicit extension
      bool read(const fileName&, const word& ext);
      //- Read from file. Chooses reader based on detected extension
      virtual bool read(const fileName&);
    // Write
      void writeStats(Ostream& os) const;
      //- Generic write routine. Chooses writer based on extension.
      virtual void write(const fileName& name) const
      {
        write(name, *this);
      }
      //- Write to database
      void write(const Time&, const word& surfName="") const;
  // Member operators
    void operator=(const MeshedSurface<Face>&);
    //- Conversion operator to MeshedSurfaceProxy
    operator MeshedSurfaceProxy<Face>() const;
};
//- Specialization for holding triangulated information
template<>
inline bool MeshedSurface<triFace>::isTri()
{
  return true;
}
//- Specialization for holding triangulated information
template<>
inline label MeshedSurface<triFace>::triangulate()
{
  return 0;
}
//- Specialization for holding triangulated information
template<>
inline label MeshedSurface<triFace>::triangulate(List<label>& faceMap)
{
  if (notNull(faceMap))
  {
    faceMap.clear();
  }
  return 0;
}
}  // namespace mousse
#ifdef NoRepository
#   include "meshed_surface.cpp"
#endif
#endif
