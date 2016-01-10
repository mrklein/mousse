// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfMesh
// Description
//   A surface mesh consisting of general polygon faces.
// SourceFiles
//   surf_mesh.cpp
//   surf_mesh_clear.cpp
//   surf_mesh_io.cpp
#ifndef surf_mesh_hpp_
#define surf_mesh_hpp_
#include "surface_registry.hpp"
#include "meshed_surface_io_allocator.hpp"
#include "primitive_patch.hpp"
#include "sub_field.hpp"
namespace mousse
{
template<class Face> class MeshedSurface;
class surfMesh
:
  public  surfaceRegistry,
  private MeshedSurfaceIOAllocator,
  public  PrimitivePatch<face, ::mousse::UList, ::mousse::SubField<point>, point>
{
  // friends
  template<class Face> friend class MeshedSurface;
  template<class Face> friend class UnsortedMeshedSurface;
public:
  // Public data types
    //- Enumeration defining the state of the mesh after a read update.
    //  Used for post-processing applications, where the mesh
    //  needs to update based on the files written in time directores
    enum readUpdateState
    {
      UNCHANGED,
      POINTS_MOVED,
      TOPO_CHANGE,
      TOPO_PATCH_CHANGE
    };
private:
  // Private typedefs
    typedef MeshedSurfaceIOAllocator Allocator;
    typedef PrimitivePatch
    <
      face,
      ::mousse::UList,
      ::mousse::SubField<point>,
      point
    >
    MeshReference;
  // Private Member Functions
    //- Disallow construct as copy
    surfMesh(const surfMesh&);
    //- Disallow default bitwise assignment
    void operator=(const surfMesh&);
protected:
  // Protected Member Functions
    //- Non-const access to global points
    pointField& storedPoints()
    {
      return Allocator::storedIOPoints();
    }
    //- Non-const access to the faces
    faceList& storedFaces()
    {
      return Allocator::storedIOFaces();
    }
    //- Non-const access to the zones
    surfZoneList& storedZones()
    {
      return Allocator::storedIOZones();
    }
    //- Update references to storedFaces
    virtual void updateFacesRef();
    //- Update references to storedPoints
    virtual void updatePointsRef();
    //- Update references to storedPoints/storedFaces
    virtual void updateRefs();
public:
  // Public typedefs
  typedef surfMesh Mesh;
  //- Placeholder only, but do not remove - it is needed for GeoMesh
  typedef bool BoundaryMesh;
  //- Runtime type information
  TYPE_NAME("surfMesh");
  //- Return the mesh sub-directory name (normally "surfMesh")
  static word meshSubDir;
  // Constructors
    //- Construct from IOobject, with alternative surface name
    explicit surfMesh(const IOobject&, const word& surfName="");
    //- Construct by transferring components (points, faces) without zones.
    //  surfZones are added using addZones() member function
    surfMesh
    (
      const IOobject&,
      const Xfer<pointField>&,
      const Xfer<faceList>&,
      const word& surfName=""
    );
    //- Construct copy/move from MeshedSurface
    surfMesh
    (
      const IOobject&,
      const Xfer<MeshedSurface<face> >& surf,
      const word& surfName=""
    );
  //- Destructor
  virtual ~surfMesh();
  // Member Functions
    // Database
      //- Return the local mesh directory (dbDir()/meshSubDir)
      fileName meshDir() const;
      //- Return the current instance directory for points
      //  Used in the consruction of geometric mesh data dependent
      //  on points
      const fileName& pointsInstance() const;
      //- Return the current instance directory for faces
      const fileName& facesInstance() const;
      //- Set the instance for mesh files
      void setInstance(const fileName&);
    // Access
      //- Return the number of raw points
      virtual label nPoints() const;
      //- Return the number of raw faces
      virtual label nFaces() const;
      //- Return number of faces
      virtual label size() const
      {
        return nFaces();
      }
      //- Return points
      virtual const pointField& points() const;
      //- Return faces
      virtual const faceList& faces() const;
      //- Return surface zones
      virtual const surfZoneList& surfZones() const
      {
        return Allocator::storedIOZones();
      }
      //- Check the surface zone definitions
      void checkZones();
      //- Add surface zones
      void addZones
      (
        const List<surfZone>&,
        const bool validate = true
      );
      //- Update the mesh based on the mesh files saved in
      //  time directories
      virtual readUpdateState readUpdate();
      //- Update the mesh corresponding to given map
      //- Remove surface zones
      void removeZones();
      //- Reset mesh primitive data.
      void resetPrimitives
      (
        const Xfer<pointField>& points,
        const Xfer<faceList>& faces,
        const Xfer<surfZoneList>& zones,
        const bool validate = true
      );
      //- Transfer the contents of the argument and annul the argument
      void transfer(MeshedSurface<face>&);
    //- Avoid masking the normal objectRegistry write
    using surfaceRegistry::write;
    //- Write to file
    static void write(const fileName&, const surfMesh&);
    //- Write to file
    void write(const fileName&);
    //  Storage management
      //- Transfer contents to the Xfer container as a MeshedSurface
      Xfer<MeshedSurface<face> > xfer();
      //- Clear geometry
      void clearGeom();
      //- Clear addressing
      void clearAddressing();
      //- Clear all geometry and addressing unnecessary for CFD
      void clearOut();
      //- Clear primitive data (points, faces and cells)
      void clearPrimitives();
      //- Remove all files from mesh instance
      void removeFiles(const fileName& instanceDir) const;
      //- Remove all files from mesh instance()
      void removeFiles() const;
};
}  // namespace mousse
#endif
