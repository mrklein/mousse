// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "obj_stream.hpp"
#include "primitive_patch.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(OBJstream, 0);

}


// Private Member Functions 
void mousse::OBJstream::writeAndCheck(const char c)
{
  if (c == '\n') {
    startOfLine_ = true;
  } else if (startOfLine_) {
    startOfLine_ = false;
    if (c == 'v') {
      nVertices_++;
    }
  }
  OFstream::write(c);
}


// Constructors 
mousse::OBJstream::OBJstream
(
  const fileName& pathname,
  streamFormat format,
  versionNumber version,
  compressionType compression
)
:
  OFstream{pathname, format, version, compression},
  startOfLine_{true},
  nVertices_{0}
{}


// Destructor 
mousse::OBJstream::~OBJstream()
{}


// Member Functions 
mousse::Ostream& mousse::OBJstream::write(const char c)
{
  writeAndCheck(c);
  return *this;
}


mousse::Ostream& mousse::OBJstream::write(const char* str)
{
  for (const char* p = str; *p != '\0'; ++p) {
    writeAndCheck(*p);
  }
  return *this;
}


mousse::Ostream& mousse::OBJstream::write(const word& str)
{
  write(str.c_str());
  return *this;
}


mousse::Ostream& mousse::OBJstream::write(const string& str)
{
  OFstream::write(token::BEGIN_STRING);
  int backslash = 0;
  for (string::const_iterator iter = str.begin(); iter != str.end(); ++iter) {
    char c = *iter;
    if (c == '\\') {
      backslash++;
      // suppress output until we know if other characters follow
      continue;
    } else if (c == token::NL) {
      lineNumber_++;
      backslash++;    // backslash escape for newline
    } else if (c == token::END_STRING) {
      backslash++;    // backslash escape for quote
    }
    // output pending backslashes
    while (backslash) {
      OFstream::write('\\');
      backslash--;
    }
    writeAndCheck(c);
  }
  // silently drop any trailing backslashes
  // they would otherwise appear like an escaped end-quote
  OFstream::write(token::END_STRING);
  return *this;
}


mousse::Ostream& mousse::OBJstream::writeQuoted
(
  const std::string& str,
  const bool quoted
)
{
  if (quoted) {
    OFstream::write(token::BEGIN_STRING);
    int backslash = 0;
    for
    (
      string::const_iterator iter = str.begin();
      iter != str.end();
      ++iter
    ) {
      char c = *iter;
      if (c == '\\') {
        backslash++;
        // suppress output until we know if other characters follow
        continue;
      } else if (c == token::NL) {
        lineNumber_++;
        backslash++;    // backslash escape for newline
      } else if (c == token::END_STRING) {
        backslash++;    // backslash escape for quote
      }
      // output pending backslashes
      while (backslash) {
        OFstream::write('\\');
        backslash--;
      }
      writeAndCheck(c);
    }
    // silently drop any trailing backslashes
    // they would otherwise appear like an escaped end-quote
    OFstream::write(token::END_STRING);
  } else {
    // output unquoted string, only advance line number on newline
    write(str.c_str());
  }
  return *this;
}


mousse::Ostream& mousse::OBJstream::write(const point& pt)
{
  write("v ") << pt.x() << ' ' << pt.y() << ' ' << pt.z()
    << nl;
  return *this;
}


mousse::Ostream& mousse::OBJstream::write(const point& pt, const vector& n)
{
  write(pt);
  OFstream::write("vn ") << n.x() << ' ' << n.y()
    << ' ' << n.z() << nl;
  return *this;
}


mousse::Ostream& mousse::OBJstream::write(const edge& e, const UList<point>& points)
{
  write(points[e[0]]);
  write(points[e[1]]);
  write("l ") << nVertices_-1 << ' ' << nVertices_ << nl;
  return *this;
}


mousse::Ostream& mousse::OBJstream::write(const linePointRef& ln)
{
  write(ln.start());
  write(ln.end());
  write("l ") << nVertices_-1 << ' ' << nVertices_ << nl;
  return *this;
}


mousse::Ostream& mousse::OBJstream::write
(
  const linePointRef& ln,
  const vector& n0,
  const vector& n1
)
{
  write(ln.start(), n0);
  write(ln.end(), n1);
  write("l ") << nVertices_-1 << ' ' << nVertices_ << nl;
  return *this;
}


mousse::Ostream& mousse::OBJstream::write
(
  const triPointRef& f,
  const bool lines
)
{
  label start = nVertices_;
  write(f.a());
  write(f.b());
  write(f.c());
  if (lines) {
    write('l');
    for (int i = 0; i < 3; i++) {
      write(' ') << start+1+i;
    }
    write(' ') << start+1 << '\n';
  } else {
    write('f');
    for (int i = 0; i < 3; i++) {
      write(' ') << start+1+i;
    }
    write('\n');
  }
  return *this;
}


mousse::Ostream& mousse::OBJstream::write
(
  const face& f,
  const UList<point>& points,
  const bool lines
)
{
  label start = nVertices_;
  FOR_ALL(f, i) {
    write(points[f[i]]);
  }
  if (lines) {
    write('l');
    FOR_ALL(f, i) {
      write(' ') << start+1+i;
    }
    write(' ') << start+1 << '\n';
  } else {
    write('f');
    FOR_ALL(f, i) {
      write(' ') << start+1+i;
    }
    write('\n');
  }
  return *this;
}


mousse::Ostream& mousse::OBJstream::write
(
  const faceList& fcs,
  const pointField& points,
  const bool lines
)
{
  SubList<face> allFcs{fcs, fcs.size()};
  primitivePatch pp{allFcs, points};
  const pointField& localPoints = pp.localPoints();
  const faceList& localFaces = pp.localFaces();
  label start = nVertices_;
  FOR_ALL(localPoints, i) {
    write(localPoints[i]);
  }
  if (lines) {
    const edgeList& edges = pp.edges();
    FOR_ALL(edges, edgeI) {
      const edge& e = edges[edgeI];
      write("l ") << start+e[0]+1 << ' ' << start+e[1]+1 << nl;
    }
  } else {
    FOR_ALL(localFaces, faceI) {
      const face& f = localFaces[faceI];
      write('f');
      FOR_ALL(f, i) {
        write(' ') << start+f[i]+1;
      }
      write('\n');
    }
  }
  return *this;
}

