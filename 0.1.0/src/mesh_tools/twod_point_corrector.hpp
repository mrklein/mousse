#ifndef MESH_TOOLS_TWOD_POINT_CORRECTOR_TWOD_POINT_CORRECTOR_HPP_
#define MESH_TOOLS_TWOD_POINT_CORRECTOR_TWOD_POINT_CORRECTOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::twoDPointCorrector
// Description
//   Class applies a two-dimensional correction to mesh motion point field.
//   The correction guarantees that the mesh does not get twisted during motion
//   and thus introduce a third dimension into a 2-D problem.
//   The operation is performed by looping through all edges approximately
//   normal to the plane and enforcing their orthogonality onto the plane by
//   adjusting points on their ends.

#include "_mesh_object.hpp"
#include "point_field.hpp"
#include "label_list.hpp"
#include "vector.hpp"


namespace mousse {

// Forward class declarations
class polyMesh;


class twoDPointCorrector
:
  public MeshObject<polyMesh, UpdateableMeshObject, twoDPointCorrector>
{
  // Private data
    //- Is 2D correction required, i.e. is the mesh
    bool required_;
    //- 2-D plane unit normal
    mutable vector* planeNormalPtr_;
    //- Indices of edges normal to plane
    mutable labelList* normalEdgeIndicesPtr_;
    //- Flag to indicate a wedge geometry
    mutable bool isWedge_;
    //- Wedge axis (if wedge geometry)
    mutable vector wedgeAxis_;
    //- Wedge angle (if wedge geometry)
    mutable scalar wedgeAngle_;
  // Private Member Functions
    //- Calculate addressing
    void calcAddressing() const;
    //- Clear addressing
    void clearAddressing() const;
    //- Snap a point to the wedge patch(es)
    void snapToWedge(const vector& n, const point& A, point& p) const;
  // Static data members
    //- Edge orthogonality tolerance
    static const scalar edgeOrthogonalityTol;
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("twoDPointCorrector");
  // Constructors
    //- Construct from components
    twoDPointCorrector(const polyMesh& mesh);
    //- Disallow default bitwise copy construct
    twoDPointCorrector(const twoDPointCorrector&) = delete;
    //- Disallow default bitwise assignment
    twoDPointCorrector& operator=(const twoDPointCorrector&) = delete;
  //- Destructor
  ~twoDPointCorrector();
  // Member Functions
    //- Is 2D correction required, i.e. is the mesh a wedge or slab
    bool required() const
    {
      return required_;
    }
    //- Return plane normal
    const vector& planeNormal() const;
    //- Return indices of normal edges.
    const labelList& normalEdgeIndices() const;
    //- Return direction normal to plane
    direction normalDir() const;
    //- Correct motion points
    void correctPoints(pointField& p) const;
    //- Correct motion displacements
    void correctDisplacement(const pointField& p, vectorField& disp) const;
    //- Update topology
    void updateMesh(const mapPolyMesh&);
    //- Correct weighting factors for moving mesh.
    bool movePoints();
};
}  // namespace mousse
#endif
