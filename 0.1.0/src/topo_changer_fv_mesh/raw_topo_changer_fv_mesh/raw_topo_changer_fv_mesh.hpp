// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rawTopoChangerFvMesh
// Description
//   topoChangerFvMesh without any added functionality.
//   Note: run without MOUSSE_SETNAN. Temporary has unitialised patch
//   data when faces get created out of nothing.
// SourceFiles
//   raw_topo_changer_fv_mesh.cpp
#ifndef raw_topo_changer_fv_mesh_hpp_
#define raw_topo_changer_fv_mesh_hpp_
#include "topo_changer_fv_mesh.hpp"
#include "packed_bool_list.hpp"
namespace mousse
{
// Forward declaration of classes
class rawTopoChangerFvMesh
:
  public topoChangerFvMesh
{
  // Private Member Functions
    //- Set unmapped values
    template<class Type, template<class> class PatchField, class GeoMesh>
    static void setUnmappedValues
    (
      GeometricField<Type, PatchField, GeoMesh>& fld,
      const PackedBoolList& mappedFace,
      const GeometricField<Type, PatchField, GeoMesh>& baseFld
    );
    template<class Type, template<class> class PatchField, class GeoMesh>
    void zeroUnmappedValues(const PackedBoolList&) const;
public:
  //- Runtime type information
  TYPE_NAME("rawTopoChangerFvMesh");
  // Constructors
    //- Construct from database
    explicit rawTopoChangerFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    rawTopoChangerFvMesh(const rawTopoChangerFvMesh&) = delete;
    //- Disallow default bitwise assignment
    rawTopoChangerFvMesh& operator=(const rawTopoChangerFvMesh&) = delete;
  //- Destructor
  virtual ~rawTopoChangerFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};
}  // namespace mousse
#ifdef NoRepository
#   include "raw_topo_changer_fv_mesh_templates.cpp"
#endif
#endif
