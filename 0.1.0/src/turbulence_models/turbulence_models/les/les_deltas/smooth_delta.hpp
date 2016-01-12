// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::smoothDelta
// Description
//   Smoothed delta which takes a given simple geometric delta and applies
//   smoothing to it such that the ratio of deltas between two cells is no
//   larger than a specified amount, typically 1.15.
// SourceFiles
//   smooth_delta.cpp
#ifndef smooth_delta_hpp_
#define smooth_delta_hpp_
#include "les_delta.hpp"
namespace mousse
{
namespace LESModels
{
class smoothDelta
:
  public LESdelta
{
public:
  //- Public member class used by mesh-wave to propagate the delta-ratio
  class deltaData
  {
    scalar delta_;
    // Private Member Functions
      //- Update. Gets information from neighbouring face/cell and
      //  uses this to update itself (if necessary) and return true.
      template<class TrackingData>
      inline bool update
      (
        const deltaData& w2,
        const scalar scale,
        const scalar tol,
        TrackingData& td
      );
  public:
    // Constructors
      //- Construct null
      inline deltaData();
      //- Construct from delta value
      inline deltaData(const scalar delta);
    // Member Functions
      // Access
      scalar delta() const
      {
        return delta_;
      }
      // Needed by FaceCellWave
        //- Check whether origin has been changed at all or
        //  still contains original (invalid) value.
        template<class TrackingData>
        inline bool valid(TrackingData& td) const;
        //- Check for identical geometrical data.
        //  Used for cyclics checking.
        template<class TrackingData>
        inline bool sameGeometry
        (
          const polyMesh&,
          const deltaData&,
          const scalar,
          TrackingData& td
        ) const;
        //- Convert any absolute coordinates into relative to
        //  (patch)face centre
        template<class TrackingData>
        inline void leaveDomain
        (
          const polyMesh&,
          const polyPatch&,
          const label patchFaceI,
          const point& faceCentre,
          TrackingData& td
        );
        //- Reverse of leaveDomain
        template<class TrackingData>
        inline void enterDomain
        (
          const polyMesh&,
          const polyPatch&,
          const label patchFaceI,
          const point& faceCentre,
          TrackingData& td
        );
        //- Apply rotation matrix to any coordinates
        template<class TrackingData>
        inline void transform
        (
          const polyMesh&,
          const tensor&,
          TrackingData& td
        );
        //- Influence of neighbouring face.
        template<class TrackingData>
        inline bool updateCell
        (
          const polyMesh&,
          const label thisCellI,
          const label neighbourFaceI,
          const deltaData& neighbourInfo,
          const scalar tol,
          TrackingData& td
        );
        //- Influence of neighbouring cell.
        template<class TrackingData>
        inline bool updateFace
        (
          const polyMesh&,
          const label thisFaceI,
          const label neighbourCellI,
          const deltaData& neighbourInfo,
          const scalar tol,
          TrackingData& td
        );
        //- Influence of different value on same face.
        template<class TrackingData>
        inline bool updateFace
        (
          const polyMesh&,
          const label thisFaceI,
          const deltaData& neighbourInfo,
          const scalar tol,
          TrackingData& td
        );
        //- Same (like operator==)
        template<class TrackingData>
        inline bool equal(const deltaData&, TrackingData& td) const;
      // Member Operators
        // Needed for List IO
        inline bool operator==(const deltaData&) const;
        inline bool operator!=(const deltaData&) const;
      // IOstream Operators
        friend Ostream& operator<<
        (
          Ostream& os,
          const deltaData& wDist
        )
        {
          return os << wDist.delta_;
        }
        friend Istream& operator>>(Istream& is, deltaData& wDist)
        {
          return is >> wDist.delta_;
        }
  };
private:
  // Private data
    autoPtr<LESdelta> geometricDelta_;
    scalar maxDeltaRatio_;
  // Private Member Functions
    // Calculate the delta values
    void calcDelta();
    //- Fill changedFaces (with face labels) and changedFacesInfo
    //  (with delta).
    //  This is the initial set of faces from which to start the waves.
    //  Since there might be lots of places with delta jumps we can follow
    //  various strategies for this initial 'seed'.
    //  - start from single cell/face and let FaceCellWave pick up all
    //    others from there. might be quite a few waves before everything
    //    settles.
    //  - start from all faces. Lots of initial transfers.
    //  We do something in between:
    //  - start from all faces where there is a jump. Since we cannot easily
    //    determine this across coupled patches (cyclic, processor)
    //    introduce all faces of these and let FaceCellWave sort it out.
    void setChangedFaces
    (
      const polyMesh& mesh,
      const volScalarField& delta,
      DynamicList<label>& changedFaces,
      DynamicList<deltaData>& changedFacesInfo
    );
public:
  //- Runtime type information
  TYPE_NAME("smooth");
  // Constructors
    //- Construct from name, turbulenceModel and dictionary
    smoothDelta
    (
      const word& name,
      const turbulenceModel& turbulence,
      const dictionary&
    );
    //- Disallow default bitwise copy construct and assignment
    smoothDelta(const smoothDelta&) = delete;
    smoothDelta& operator=(const smoothDelta&) = delete;
  //- Destructor
  virtual ~smoothDelta()
  {}
  // Member Functions
    //- Read the LESdelta dictionary
    virtual void read(const dictionary&);
    // Correct values
    virtual void correct();
};
}  // namespace LESModels
//- Data associated with deltaData type are contiguous
template<>
inline bool contiguous<LESModels::smoothDelta::deltaData>()
{
  return true;
}
}  // namespace mousse
#include "smooth_delta_delta_data_i.hpp"
#endif
