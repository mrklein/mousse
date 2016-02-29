#ifndef SAMPLING_SAMPLED_SURFACE_SAMPLED_TRI_SURFACE_MESH_HPP_
#define SAMPLING_SAMPLED_SURFACE_SAMPLED_TRI_SURFACE_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sampledTriSurfaceMesh
// Description
//   A sampledSurface from a triSurfaceMesh. It samples on the points/triangles
//   of the triSurface.
//   - it either samples cells or (non-coupled) boundary faces
//   - 6 different modes:
//     - source=cells, interpolate=false:
//       finds per triangle centre the nearest cell centre and uses its value
//     - source=cells, interpolate=true
//       finds per triangle centre the nearest cell centre.
//       Per surface point checks if this nearest cell is the one containing
//       point; otherwise projects the point onto the nearest point on
//       the boundary of the cell (to make sure interpolateCellPoint
//       gets a valid location)
//     - source=insideCells, interpolate=false:
//       finds per triangle centre the cell containing it and uses its value.
//       Trims triangles outside mesh.
//     - source=insideCells, interpolate=true
//       Per surface point interpolate cell containing it.
//     - source=boundaryFaces, interpolate=false:
//       finds per triangle centre the nearest point on the boundary
//       (uncoupled faces only) and uses the value (or 0 if the nearest
//       is on an empty boundary)
//     - source=boundaryFaces, interpolate=true:
//       finds per triangle centre the nearest point on the boundary
//       (uncoupled faces only).
//       Per surface point projects the point onto this boundary face
//       (to make sure interpolateCellPoint gets a valid location)
//   - since it finds a nearest per triangle each triangle is guaranteed
//   to be on one processor only. So after stitching (by sampledSurfaces)
//   the original surface should be complete.
// SourceFiles
//   sampled_tri_surface_mesh.cpp
#include "sampled_surface.hpp"
#include "tri_surface_mesh.hpp"
#include "meshed_surface.hpp"
namespace mousse
{
class treeDataFace;
class meshSearch;
class sampledTriSurfaceMesh
:
  public sampledSurface,
  public MeshedSurface<face>
{
public:
    //- Types of communications
    enum samplingSource
    {
      cells,
      insideCells,
      boundaryFaces,
    };
private:
  //- Private typedefs for convenience
    typedef MeshedSurface<face> MeshStorage;
  // Private data
    static const NamedEnum<samplingSource, 3> samplingSourceNames_;
    //- Surface to sample on
    const triSurfaceMesh surface_;
    //- Whether to sample internal cell values or boundary values
    const samplingSource sampleSource_;
    //- Track if the surface needs an update
    mutable bool needsUpdate_;
    //- Search tree for all non-coupled boundary faces
    mutable autoPtr<indexedOctree<treeDataFace> > boundaryTreePtr_;
    //- From local surface triangle to mesh cell/face.
    labelList sampleElements_;
    //- Local points to sample per point
    pointField samplePoints_;
  // Private Member Functions
    //- Get tree of all non-coupled boundary faces
    const indexedOctree<treeDataFace>& nonCoupledboundaryTree() const;
    //- Sample field on faces
    template<class Type>
    tmp<Field<Type> > sampleField
    (
      const GeometricField<Type, fvPatchField, volMesh>& vField
    ) const;
    template<class Type>
    tmp<Field<Type> >
    interpolateField(const interpolation<Type>&) const;
    bool update(const meshSearch& meshSearcher);
public:
  //- Runtime type information
  TYPE_NAME("sampledTriSurfaceMesh");
  // Constructors
    //- Construct from components
    sampledTriSurfaceMesh
    (
      const word& name,
      const polyMesh& mesh,
      const word& surfaceName,
      const samplingSource sampleSource
    );
    //- Construct from dictionary
    sampledTriSurfaceMesh
    (
      const word& name,
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from triSurface
    sampledTriSurfaceMesh
    (
      const word& name,
      const polyMesh& mesh,
      const triSurface& surface,
      const word& sampleSourceName
    );
  //- Destructor
  virtual ~sampledTriSurfaceMesh();
  // Member Functions
    //- Does the surface need an update?
    virtual bool needsUpdate() const;
    //- Mark the surface as needing an update.
    //  May also free up unneeded data.
    //  Return false if surface was already marked as expired.
    virtual bool expire();
    //- Update the surface as required.
    //  Do nothing (and return false) if no update was needed
    virtual bool update();
    //- Update the surface using a bound box to limit the searching.
    //  For direct use, i.e. not through sample.
    //  Do nothing (and return false) if no update was needed
    bool update(const treeBoundBox&);
    //- Points of surface
    virtual const pointField& points() const
    {
      return MeshStorage::points();
    }
    //- Faces of surface
    virtual const faceList& faces() const
    {
      return MeshStorage::faces();
    }
    //- Sample field on surface
    virtual tmp<scalarField> sample
    (
      const volScalarField&
    ) const;
    //- Sample field on surface
    virtual tmp<vectorField> sample
    (
      const volVectorField&
    ) const;
    //- Sample field on surface
    virtual tmp<sphericalTensorField> sample
    (
      const volSphericalTensorField&
    ) const;
    //- Sample field on surface
    virtual tmp<symmTensorField> sample
    (
      const volSymmTensorField&
    ) const;
    //- Sample field on surface
    virtual tmp<tensorField> sample
    (
      const volTensorField&
    ) const;
    //- Interpolate field on surface
    virtual tmp<scalarField> interpolate
    (
      const interpolation<scalar>&
    ) const;
    //- Interpolate field on surface
    virtual tmp<vectorField> interpolate
    (
      const interpolation<vector>&
    ) const;
    //- Interpolate field on surface
    virtual tmp<sphericalTensorField> interpolate
    (
      const interpolation<sphericalTensor>&
    ) const;
    //- Interpolate field on surface
    virtual tmp<symmTensorField> interpolate
    (
      const interpolation<symmTensor>&
    ) const;
    //- Interpolate field on surface
    virtual tmp<tensorField> interpolate
    (
      const interpolation<tensor>&
    ) const;
    //- Write
    virtual void print(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "sampled_tri_surface_mesh_templates.cpp"
#endif
#endif
