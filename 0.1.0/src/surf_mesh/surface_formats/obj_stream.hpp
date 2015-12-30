// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::OBJstream
// Description
//   OFstream which keeps track of vertices
// SourceFiles
//   obj_stream.cpp
#ifndef obj_stream_hpp_
#define obj_stream_hpp_
#include "ofstream.hpp"
#include "point.hpp"
#include "edge.hpp"
#include "face.hpp"
#include "tri_point_ref.hpp"
#include "line_point_ref.hpp"
namespace mousse
{
class OBJstream
:
  public OFstream
{
  // Private data
    bool startOfLine_;
    label nVertices_;
  // Private Member Functions
    void writeAndCheck(const char);
public:
  // Declare name of the class and its debug switch
  ClassName("OBJstream");
  // Constructors
    //- Construct from pathname
    OBJstream
    (
      const fileName& pathname,
      streamFormat format=ASCII,
      versionNumber version=currentVersion,
      compressionType compression=UNCOMPRESSED
    );
  //- Destructor
  ~OBJstream();
  // Member functions
    // Access
      //- Return the number of vertices written
      label nVertices() const
      {
        return nVertices_;
      }
    // Ostream implementation
      //- Inherit write from Ostream
      using Ostream::write;
      //- Write character
      virtual Ostream& write(const char);
      //- Write character string
      virtual Ostream& write(const char*);
      //- Write word
      virtual Ostream& write(const word&);
      virtual Ostream& write(const string&);
      //- Write std::string surrounded by quotes.
      //  Optional write without quotes.
      virtual Ostream& writeQuoted
      (
        const std::string&,
        const bool quoted=true
      );
    // Direct write functionality
      //- Write point
      Ostream& write(const point&);
      //- Write point and vector normal ('vn')
      Ostream& write(const point&, const vector&);
      //- Write edge as points with line
      Ostream& write(const edge&, const UList<point>&);
      //- Write line
      Ostream& write(const linePointRef&);
      //- Write line with points and vector normals ('vn')
      Ostream& write
      (
        const linePointRef&,
        const vector& n0,
        const vector& n1
      );
      //- Write triangle as points with lines or filled polygon
      Ostream& write(const triPointRef&, const bool lines = true);
      //- Write face as points with lines or filled polygon
      Ostream& write
      (
        const face&,
        const UList<point>&,
        const bool lines = true
      );
      //- Write patch as points and faces with lines or filled polygons
      Ostream& write
      (
        const faceList&,
        const pointField&,
        const bool lines = true
      );
};
}  // namespace mousse
#endif
