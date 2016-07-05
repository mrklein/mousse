#ifndef FILE_FORMATS_COORD_SET_HPP_
#define FILE_FORMATS_COORD_SET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coordSet
// Description
//   Holds list of sampling positions

#include "point_field.hpp"
#include "word.hpp"


namespace mousse {

class coordSet
:
  public pointField
{
public:
  // Public data types
    //- Enumeration defining the output format for coordinates
    enum coordFormat
    {
      XYZ,
      X,
      Y,
      Z,
      DISTANCE
    };
private:
    //- String representation of coordFormat enums
    static const NamedEnum<coordFormat, 5> coordFormatNames_;
protected:
    //- Name
    const word name_;
    //- Axis write type
    const coordFormat axis_;
    //- Cumulative distance "distance" write specifier.
    scalarList curveDist_;
public:
  // Constructors
    //- Construct from components
    coordSet
    (
      const word& name,
      const word& axis
    );
    //- Construct from components
    coordSet
    (
      const word& name,
      const word& axis,
      const List<point>& points,
      const scalarList& curveDist
    );
  // Member functions
    const word& name() const
    {
      return name_;
    }
    word axis() const
    {
      return coordFormatNames_[axis_];
    }
    //- Cumulative distance
    const scalarList& curveDist() const
    {
      return curveDist_;
    }
    //- Is axis specification a vector
    bool hasVectorAxis() const;
    //- Get coordinate of point according to axis specification.
    //  If axis="distance" is the curveDist[index]
    scalar scalarCoord(const label index) const;
    //- Get point according to axis="xyz" specification
    vector vectorCoord(const label index) const;
    Ostream& write(Ostream& os) const;
};

}  // namespace mousse
#endif
