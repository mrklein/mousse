// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::STLtriangle
// Description
//   A triangle representation for STL files.
// SourceFiles
//   stl_triangle_i.hpp
#ifndef stl_triangle_hpp_
#define stl_triangle_hpp_
#include "stl_point.hpp"
#include "istream.hpp"
#include "ostream.hpp"
namespace mousse
{
class STLtriangle
{
  // Private data
    //- Attribute is 16-bit
    typedef unsigned short STLattrib;
    //- The face normal, many programs write zore or other junk
    STLpoint normal_;
    //- The three points defining the triangle
    STLpoint a_, b_, c_;
    //- The attribute information could for colour or solid id, etc
    STLattrib attrib_;
public:
  // Constructors
    //- Construct null
    inline STLtriangle();
    //- Construct from components
    inline STLtriangle
    (
      const STLpoint& normal,
      const STLpoint& a,
      const STLpoint& b,
      const STLpoint& c,
      unsigned short attrib
    );
    //- Construct from istream (read binary)
    inline STLtriangle(istream&);
  // Member Functions
    // Access
      inline const STLpoint& normal() const;
      inline const STLpoint& a() const;
      inline const STLpoint& b() const;
      inline const STLpoint& c() const;
      inline unsigned short attrib() const;
    // Read
      //- Read from istream (binary)
      inline void read(istream&);
    // Write
      //- Write to ostream (binary)
      inline void write(ostream&);
  // Ostream operator
    inline friend Ostream& operator<<(Ostream&, const STLtriangle&);
};
}  // namespace mousse
#include "stl_triangle_i.hpp"
#endif
