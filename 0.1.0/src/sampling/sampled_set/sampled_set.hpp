// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sampledSet
// Group
//   grpFunctionObjects
// Description
//   Holds list of sampling points which is filled at construction time.
//   Various implementations of this base class to e.g. get sampling points
//   at uniform distance along a line (uniformSet) or directly specified
//   (cloudSet)
//   Each 'sampledSet' has a name and a specifier of how the axis should be
//   write (x/y/z component or all 3 components)
// SourceFiles
//   sampled_set.cpp
#ifndef sampled_set_hpp_
#define sampled_set_hpp_
#include "coord_set.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class meshSearch;
class sampledSet
:
  public coordSet
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
    //- Reference to mesh searching class
    const meshSearch& searchEngine_;
protected:
    //- Segment numbers
    labelList segments_;
    //- Cell numbers
    labelList cells_;
    //- Face numbers (-1 if not known)
    labelList faces_;
  // Protected Member Functions
    //- Returns cell next to boundary face
    label getBoundaryCell(const label) const;
    //- Returns cell using face and containing sample
    label getCell
    (
      const label faceI,
      const point& sample
    ) const;
    //- Calculates inproduct of face normal and vector sample-face centre
    //  <0 if sample inside.
    scalar calcSign(const label faceI, const point& sample) const;
    //- Returns face label (or -1) of face which is close to sample
    label findNearFace
    (
      const label cellI,
      const point& sample,
      const scalar smallDist
    ) const;
    //- Moves sample in direction of -n to it is 'inside' of faceI
    point pushIn
    (
      const point& sample,
      const label faceI
    ) const;
    //- Calculates start of tracking given samplePt and first boundary
    //  intersection
    //  (bPoint, bFaceI) (bFaceI == -1 if no boundary intersection)
    //  Returns true if trackPt is valid sampling point. Sets trackPt,
    //  trackFaceI, trackCellI (-1 if no tracking point found)
    bool getTrackingPoint
    (
      const vector& offset,
      const point& samplePt,
      const point& bPoint,
      const label bFaceI,
      point& trackPt,
      label& trackCellI,
      label& trackFaceI
    ) const;
    //- Sets sample data
    void setSamples
    (
      const List<point>& samplingPts,
      const labelList& samplingCells,
      const labelList& samplingFaces,
      const labelList& samplingSegments,
      const scalarList& samplingCurveDist
    );
public:
  //- Runtime type information
  TypeName("sampledSet");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      sampledSet,
      word,
      (
        const word& name,
        const polyMesh& mesh,
        const meshSearch& searchEngine,
        const dictionary& dict
      ),
      (name, mesh, searchEngine, dict)
    );
    //- Class used for the read-construction of
    //  PtrLists of sampledSet
    class iNew
    {
      const polyMesh& mesh_;
      const meshSearch& searchEngine_;
    public:
      iNew(const polyMesh& mesh, const meshSearch& searchEngine)
      :
        mesh_(mesh),
        searchEngine_(searchEngine)
      {}
      autoPtr<sampledSet> operator()(Istream& is) const
      {
        word name(is);
        dictionary dict(is);
        return sampledSet::New(name, mesh_, searchEngine_, dict);
      }
    };
  // Static data
    //- Tolerance when comparing points. Usually relative to difference
    //  between start_ and end_
    static const scalar tol;
  // Constructors
    //- Construct from components
    sampledSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const word& axis
    );
    //- Construct from dictionary
    sampledSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
    //- Clone
    autoPtr<sampledSet> clone() const
    {
      notImplemented("autoPtr<sampledSet> clone() const");
      return autoPtr<sampledSet>(NULL);
    }
  // Selectors
    //- Return a reference to the selected sampledSet
    static autoPtr<sampledSet> New
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
  //- Destructor
  virtual ~sampledSet();
  // Member Functions
    const polyMesh& mesh() const
    {
      return mesh_;
    }
    const meshSearch& searchEngine() const
    {
      return searchEngine_;
    }
    const labelList& segments() const
    {
      return segments_;
    }
    const labelList& cells() const
    {
      return cells_;
    }
    const labelList& faces() const
    {
      return faces_;
    }
    //- Output for debugging
    Ostream& write(Ostream&) const;
};
}  // namespace mousse
#endif
