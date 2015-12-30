// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::curvedEdge
// Description
//   Define a curved edge that is parameterized for 0<lambda<1
//   between the start and end point.
// SourceFiles
//   curved_edge.cpp
#ifndef curvedEdges_H
#define curvedEdges_H
#include "edge.hpp"
#include "point_field.hpp"
#include "type_info.hpp"
#include "hash_table.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class curvedEdge
{
protected:
  // Protected data
    const pointField& points_;
    const label start_;
    const label end_;
  // Protected Member Functions
    //- Return a complete point field by appending the start/end points
    //  to the given list
    static pointField appendEndPoints
    (
      const pointField&,
      const label start,
      const label end,
      const pointField& otherKnots
    );
public:
  //- Runtime type information
  TypeName("curvedEdge");
  // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
      autoPtr,
      curvedEdge,
      Istream,
      (
        const pointField& points,
        Istream& is
      ),
      (points, is)
    );
  // Constructors
    //- Construct from components
    curvedEdge
    (
      const pointField& points,
      const label start,
      const label end
    );
    //- Construct from Istream setting pointsList
    curvedEdge(const pointField&, Istream&);
    //- Copy construct
    curvedEdge(const curvedEdge&);
    //- Clone function
    virtual autoPtr<curvedEdge> clone() const;
    //- New function which constructs and returns pointer to a curvedEdge
    static autoPtr<curvedEdge> New(const pointField&, Istream&);
  //- Destructor
  virtual ~curvedEdge(){}
  // Member Functions
    //- Return label of start point
    inline label start() const;
    //- Return label of end point
    inline label end() const;
    //- Compare the given start and end points with this curve
    //  Return:
    //  -  0: different
    //  - +1: identical
    //  - -1: same edge, but different orientation
    inline int compare(const curvedEdge&) const;
    //- Compare the given start and end points with this curve
    //  Return:
    //  -  0: different
    //  - +1: identical
    //  - -1: same edge, but different orientation
    inline int compare(const edge&) const;
    //- Compare the given start and end points with this curve
    //  Return:
    //  -  0: different
    //  - +1: identical
    //  - -1: same edge, but different orientation
    inline int compare(const label start, const label end) const;
    //- Return the point position corresponding to the curve parameter
    //  0 <= lambda <= 1
    virtual point position(const scalar) const = 0;
    //- Return the length of the curve
    virtual scalar length() const = 0;
  // Member operators
    void operator=(const curvedEdge&);
  // Ostream operator
    friend Ostream& operator<<(Ostream&, const curvedEdge&);
};
}  // namespace mousse
#include "curved_edge_i.hpp"
#endif
