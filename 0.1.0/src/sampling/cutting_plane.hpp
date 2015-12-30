// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cuttingPlane
// Description
//   Constructs plane through mesh.
//   No attempt at resolving degenerate cases. Since the cut faces are
//   usually quite ugly, they will always be triangulated.
// Note
//   When the cutting plane coincides with a mesh face, the cell edge on the
//   positive side of the plane is taken.
// SourceFiles
//   cutting_plane.cpp
#ifndef cutting_plane_hpp_
#define cutting_plane_hpp_
#include "plane.hpp"
#include "point_field.hpp"
#include "face_list.hpp"
#include "meshed_surface.hpp"
namespace mousse
{
class primitiveMesh;
class cuttingPlane
:
  public plane,
  public MeshedSurface<face>
{
  //- Private typedef for convenience
  typedef MeshedSurface<face> MeshStorage;
  // Private data
    //- List of cells cut by the plane
    labelList cutCells_;
  // Private Member Functions
    //- Determine cut cells, possibly restricted to a list of cells
    void calcCutCells
    (
      const primitiveMesh&,
      const scalarField& dotProducts,
      const labelUList& cellIdLabels = labelUList::null()
    );
    //- Determine intersection points (cutPoints).
    void intersectEdges
    (
      const primitiveMesh&,
      const scalarField& dotProducts,
      List<label>& edgePoint
    );
    //- Walk circumference of cell, starting from startEdgeI crossing
    //  only cut edges. Record cutPoint labels in faceVerts.
    static bool walkCell
    (
      const primitiveMesh&,
      const labelUList& edgePoint,
      const label cellI,
      const label startEdgeI,
      DynamicList<label>& faceVerts
    );
    //- Determine cuts for all cut cells.
    void walkCellCuts
    (
      const primitiveMesh& mesh,
      const bool triangulate,
      const labelUList& edgePoint
    );
protected:
  // Constructors
    //- Construct plane description without cutting
    cuttingPlane(const plane&);
  // Protected Member Functions
    //- Recut mesh with existing planeDesc, restricted to a list of cells
    void reCut
    (
      const primitiveMesh&,
      const bool triangulate,
      const labelUList& cellIdLabels = labelUList::null()
    );
    //- Remap action on triangulation or cleanup
    virtual void remapFaces(const labelUList& faceMap);
public:
  // Constructors
    //- Construct from plane and mesh reference,
    //  possibly restricted to a list of cells
    cuttingPlane
    (
      const plane&,
      const primitiveMesh&,
      const bool triangulate,
      const labelUList& cellIdLabels = labelUList::null()
    );
  // Member Functions
    //- Return plane used
    const plane& planeDesc() const
    {
      return static_cast<const plane&>(*this);
    }
    //- Return List of cells cut by the plane
    const labelList& cutCells() const
    {
      return cutCells_;
    }
    //- Return true or false to question: have any cells been cut?
    bool cut() const
    {
      return cutCells_.size();
    }
    //- Sample the cell field
    template<class Type>
    tmp<Field<Type> > sample(const Field<Type>&) const;
    template<class Type>
    tmp<Field<Type> > sample(const tmp<Field<Type> >&) const;
  // Member Operators
    void operator=(const cuttingPlane&);
};
}  // namespace mousse
#ifdef NoRepository
#   include "cutting_plane_templates.cpp"
#endif
#endif
