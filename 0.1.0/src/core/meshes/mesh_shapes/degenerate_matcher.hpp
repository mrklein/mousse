// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::degenerateMatcher
// Description
//   Collection of all hex degenerate matchers (hex, wedge, prism etc.)
//   Has static member function to match a shape.
// See Also
//   cellMatcher
// SourceFiles
//   degenerate_matcher.cpp
#ifndef degenerate_matcher_hpp_
#define degenerate_matcher_hpp_
#include "hex_matcher.hpp"
#include "wedge_matcher.hpp"
#include "prism_matcher.hpp"
#include "tet_wedge_matcher.hpp"
#include "pyr_matcher.hpp"
#include "tet_matcher.hpp"
namespace mousse
{
class degenerateMatcher
{
  // Static data members
    //- Matchers for all degenerate hex shapes
    static hexMatcher hex;
    static wedgeMatcher wedge;
    static prismMatcher prism;
    static tetWedgeMatcher tetWedge;
    static pyrMatcher pyr;
    static tetMatcher tet;
  // Static functions
    //- Recognize basic shape
    static cellShape match
    (
      const faceList& faces,
      const labelList& faceOwner,
      const label cellI,
      const labelList& cellFaces
    );
public:
  // Static data members
    //- Recognize shape given faces of a cell
    static cellShape match(const faceList& faces);
    //- Recognize given uncollapsed shape (usually hex) with duplicate
    //  vertices. cellShape just used to extract faces.
    static cellShape match(const cellShape& shape);
    //- Recognize shape given mesh and cellI
    static cellShape match(const primitiveMesh& mesh, const label cellI);
};
}  // namespace mousse
#endif
