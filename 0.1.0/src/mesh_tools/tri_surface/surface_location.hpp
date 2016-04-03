#ifndef MESH_TOOLS_TRI_SURFACE_SURFACE_LOCATION_SURFACE_LOCATION_HPP_
#define MESH_TOOLS_TRI_SURFACE_SURFACE_LOCATION_SURFACE_LOCATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceLocation
// Description
//   Contains information about location on a triSurface:
//   - pointIndexHit:
//     - location
//     - bool: hit/miss
//     - index (of triangle/point/edge)
//   - elementType():
//     - what index above relates to. In triangle::proxType
//   - triangle():
//     - last known triangle

#include "point_index_hit.hpp"
#include "tri_point_ref.hpp"
#include "info_proxy.hpp"


namespace mousse {

// Forward declaration of classes
class triSurface;


class surfaceLocation
:
  public pointIndexHit
{
  // Private data
    triPointRef::proxType elementType_;
    label triangle_;
public:
  // Constructors
    //- Construct null
    surfaceLocation()
    :
      pointIndexHit{},
      elementType_{triPointRef::NONE},
      triangle_{-1}
    {}
    //- Construct from components
    surfaceLocation
    (
      const pointIndexHit& pih,
      const triPointRef::proxType elementType,
      const label triangle
    )
    :
      pointIndexHit{pih},
      elementType_{elementType},
      triangle_{triangle}
    {}
    //- Construct from Istream
    surfaceLocation(Istream& is)
    :
      pointIndexHit{is},
      elementType_{triPointRef::proxType(readLabel(is))},
      triangle_{readLabel(is)}
    {}
  // Member Functions
    triPointRef::proxType& elementType()
    {
      return elementType_;
    }
    triPointRef::proxType elementType() const
    {
      return elementType_;
    }
    label& triangle()
    {
      return triangle_;
    }
    label triangle() const
    {
      return triangle_;
    }
    //- Normal. Approximate for points.
    vector normal(const triSurface& s) const;
    //- Return info proxy.
    //  Used to print token information to a stream
    InfoProxy<surfaceLocation> info() const
    {
      return *this;
    }
    //- Write info to os
    void write(Ostream& os, const triSurface& s) const;
  // IOstream Operators
    friend Istream& operator>>(Istream& is, surfaceLocation& sl);
    friend Ostream& operator<<(Ostream& os, const surfaceLocation& sl);
    friend Ostream& operator<<
    (
      Ostream&,
      const InfoProxy<surfaceLocation>&
    );
};
}  // namespace mousse
#endif
