#ifndef CORE_GRAPH_CURVE_HPP_
#define CORE_GRAPH_CURVE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::curve
// Description
//   A single curve in a graph.

#include "string.hpp"
#include "primitive_fields.hpp"
#include "auto_ptr.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
class curve;
Ostream& operator<<(Ostream&, const curve&);


class curve
:
  public scalarField
{
public:
  //- The style (line, symbol, etc) of a curve
  class curveStyle
  {
  public:
    //- Enumeration definitions
    enum curveStyleNo
    {
      CONTINUOUS,
      SYMBOL,
      SYMBOL_CURVE,
      SYMBOL_WITH_ERROR_BARS,
      SYMBOL_WITH_VARIABLE_SIZE
    };
  private:
    //- Private data
    curveStyleNo CurveStyleNo;
  public:
    // Constructors
      //- Construct given a curveStyleNo
      curveStyle(const curveStyleNo csn)
      :
        CurveStyleNo{csn}
      {}
      //- Construct from Istream
      curveStyle(Istream& is)
      :
        CurveStyleNo{curveStyleNo(readInt(is))}
      {}
    // Ostream operator
      friend Ostream& operator<<(Ostream& os, const curveStyle& cs)
      {
        os << int(cs.CurveStyleNo);
        return os;
      }
  };
private:
  // private data
    string name_;
    curveStyle style_;
public:
  // Constructors
    //- Construct as interpolation of an existing curve
    //curve(const curve&, const label);
    //- Construct from name, style and size
    curve
    (
      const string& name,
      const curveStyle& style,
      const label l
    );
    //- Construct from the components
    curve
    (
      const string&,
      const curveStyle&,
      const scalarField& y
    );
    autoPtr<curve> clone() const
    {
      return autoPtr<curve>(new curve(*this));
    }
  // Member functions
    // Access
      const string& name() const
      {
        return name_;
      }
      const curveStyle& style() const
      {
        return style_;
      }
  // Friend functions
    //- Gradient of the curve
    //friend curve grad(const curve&);
  // Ostream operator
    friend Ostream& operator<<(Ostream&, const curve&);
};
}  // namespace mousse
#endif
