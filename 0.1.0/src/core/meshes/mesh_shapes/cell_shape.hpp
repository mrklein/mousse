// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellShape
// Description
//   An analytical geometric cellShape.
//   The optional collapse functionality changes the cellModel to the
//   correct type after removing any duplicate points.
// SourceFiles
//   cell_shape_i.hpp
//   cell_shape.cpp
//   cell_shape_io.cpp
//   cell_shape_equal.cpp
#ifndef cell_shape_hpp_
#define cell_shape_hpp_
#include "point_field.hpp"
#include "label_list.hpp"
#include "cell_model.hpp"
#include "auto_ptr.hpp"
#include "info_proxy.hpp"
namespace mousse
{
// Forward declaration of classes
class cell;
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
#include "cell_shape_i.hpp"
#endif
