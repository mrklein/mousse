#ifndef CORE_MESHES_MESH_SHAPES_CELL_SHAPE_HPP_
#define CORE_MESHES_MESH_SHAPES_CELL_SHAPE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellShape
// Description
//   An analytical geometric cellShape.
//   The optional collapse functionality changes the cellModel to the
//   correct type after removing any duplicate points.

#include "point_field.hpp"
#include "label_list.hpp"
#include "cell_model.hpp"
#include "auto_ptr.hpp"
#include "info_proxy.hpp"
#include "istream.hpp"
#include "cell.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
class cellShape;
bool operator==(const cellShape&, const cellShape&);
Istream& operator>>(Istream&, cellShape&);
Ostream& operator<<(Ostream&, const cellShape&);


class cellShape
:
  public labelList
{
  // Private data
    //- Access to the cellShape's model
    const cellModel *m;
public:
  // Constructors
    //- Construct null
    inline cellShape();
    //- Construct from components
    inline cellShape
    (
      const cellModel&,
      const labelList&,
      const bool doCollapse = false
    );
    //- Construct from Istream
    inline cellShape(Istream& is);
    //- Clone
    inline autoPtr<cellShape> clone() const;
  // Member Functions
    //- Return the points corresponding to this cellShape
    inline pointField points(const pointField& meshPoints) const;
    //- Model reference
    inline const cellModel& model() const;
    //- Mesh face labels of this cell (in order of model)
    inline labelList meshFaces(const faceList& allFaces, const cell&) const;
    //- Mesh edge labels of this cell (in order of model)
    inline labelList meshEdges
    (
      const edgeList& allEdges,
      const labelList&
    ) const;
    //- Faces of this cell
    inline faceList faces() const;
    //- Collapsed faces of this cell
    inline faceList collapsedFaces() const;
    //- Number of faces
    inline label nFaces() const;
    //- Edges of this cellShape
    inline edgeList edges() const;
    //- Number of edges
    inline label nEdges() const;
    //- Number of points
    inline label nPoints() const;
    //- Centroid of the cell
    inline point centre(const pointField&) const;
    //- Return info proxy.
    //  Used to print token information to a stream
    mousse::InfoProxy<cellShape> info() const
    {
      return *this;
    }
    //- Scalar magnitude
    inline scalar mag(const pointField&) const;
    //- Collapse shape to correct one after removing duplicate vertices
    void collapse();
  // Friend Operators
    friend bool operator==(const cellShape&, const cellShape&);
  // IOstream operators
    friend Istream& operator>>(Istream&, cellShape&);
    friend Ostream& operator<<(Ostream&, const cellShape&);
};

template<>
Ostream& operator<<(Ostream& os, const InfoProxy<cellShape>& ip);

}  // namespace mousse


// Constructors 
inline mousse::cellShape::cellShape()
:
  m{nullptr}
{}


inline mousse::cellShape::cellShape
(
  const cellModel& M,
  const labelList& l,
  const bool doCollapse
)
:
  labelList{l},
  m{&M}
{
  if (doCollapse) {
    collapse();
  }
}


inline mousse::cellShape::cellShape(Istream& is)
{
  is >> *this;
}


inline mousse::autoPtr<mousse::cellShape> mousse::cellShape::clone() const
{
  return autoPtr<cellShape>{new cellShape{*this}};
}


// Member Functions 
inline mousse::pointField mousse::cellShape::points
(
  const pointField& meshPoints
) const
{
  // There are as many points as there labels for them
  pointField p{size()};
  // For each point in list, set it to the point in 'pnts' addressed
  // by 'labs'
  FOR_ALL(p, i) {
    p[i] = meshPoints[operator[](i)];
  }
  // Return list
  return p;
}


inline const mousse::cellModel& mousse::cellShape::model() const
{
  return *m;
}


inline mousse::labelList mousse::cellShape::meshFaces
(
  const faceList& allFaces,
  const cell& cFaces
) const
{
  // Faces in model order
  faceList localFaces{faces()};
  // Do linear match (usually cell shape is low complexity)
  labelList modelToMesh{localFaces.size(), -1};
  FOR_ALL(localFaces, i) {
    const face& localF = localFaces[i];
    FOR_ALL(cFaces, j) {
      label meshFaceI = cFaces[j];
      if (allFaces[meshFaceI] == localF) {
        modelToMesh[i] = meshFaceI;
        break;
      }
    }
  }
  return modelToMesh;
}


inline mousse::labelList mousse::cellShape::meshEdges
(
  const edgeList& allEdges,
  const labelList& cEdges
) const
{
  // Edges in model order
  edgeList localEdges{edges()};
  // Do linear match (usually cell shape is low complexity)
  labelList modelToMesh{localEdges.size(), -1};
  FOR_ALL(localEdges, i) {
    const edge& e = localEdges[i];
    FOR_ALL(cEdges, j) {
      label edgeI = cEdges[j];
      if (allEdges[edgeI] == e) {
        modelToMesh[i] = edgeI;
        break;
      }
    }
  }
  return modelToMesh;
}


inline mousse::faceList mousse::cellShape::faces() const
{
  return m->faces(*this);
}


inline mousse::faceList mousse::cellShape::collapsedFaces() const
{
  faceList oldFaces{faces()};
  faceList newFaces{oldFaces.size()};
  label newFaceI = 0;
  FOR_ALL(oldFaces, oldFaceI) {
    const face& f = oldFaces[oldFaceI];
    face& newF = newFaces[newFaceI];
    newF.setSize(f.size());
    label newFp = 0;
    label prevVertI = -1;
    FOR_ALL(f, fp) {
      label vertI = f[fp];
      if (vertI != prevVertI) {
        newF[newFp++] = vertI;
        prevVertI = vertI;
      }
    }
    if ((newFp > 1) && (newF[newFp-1] == newF[0])) {
      --newFp;
    }
    if (newFp > 2) {
      // Size face and go to next one
      newF.setSize(newFp);
      newFaceI++;
    }
  }
  newFaces.setSize(newFaceI);
  return newFaces;
}


inline mousse::label mousse::cellShape::nFaces() const
{
  return m->nFaces();
}


inline mousse::edgeList mousse::cellShape::edges() const
{
  return m->edges(*this);
}


inline mousse::label mousse::cellShape::nEdges() const
{
  return m->nEdges();
}


inline mousse::label mousse::cellShape::nPoints() const
{
  return size();
}


inline mousse::point mousse::cellShape::centre(const pointField& points) const
{
  return m->centre(*this, points);
}


inline mousse::scalar mousse::cellShape::mag(const pointField& points) const
{
  return m->mag(*this, points);
}

#endif
