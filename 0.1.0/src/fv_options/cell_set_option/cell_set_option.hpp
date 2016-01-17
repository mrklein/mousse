// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::cellSetOption
// Description
//   Cell-set options abtract base class.  Provides a base set of controls,
//   e.g.
//     type            scalarExplicitSource    // Source type
//     active          on;                     // on/off switch
//     scalarExplicitSourceCoeffs
//     {
//       timeStart       0.0;        // Start time
//       duration        1000.0;     // Duration
//       selectionMode   cellSet;    // cellSet, points, cellZone
//       .
//       .
//       .
//     }
// Note:
//   Source/sink options are to be added to the equation R.H.S.
// SourceFiles
//   cell_set_option.cpp
//   cell_set_option_io.cpp
#ifndef cell_set_option_hpp_
#define cell_set_option_hpp_
#include "fv_option.hpp"
#include "cell_set.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
namespace mousse
{
namespace fv
{
class cellSetOption
:
  public option
{
public:
  // Public data
    //- Enumeration for selection mode types
    enum selectionModeType
    {
      smPoints,
      smCellSet,
      smCellZone,
      smAll
    };
    //- Word list of selection mode type names
    static const NamedEnum<selectionModeType, 4>
      selectionModeTypeNames_;
protected:
  // Protected data
    //- Time start
    scalar timeStart_;
    //- Duration
    scalar duration_;
    //- Cell selection mode
    selectionModeType selectionMode_;
    //- Name of cell set for "cellSet" and "cellZone" selectionMode
    word cellSetName_;
    //- List of points for "points" selectionMode
    List<point> points_;
    //- Set of cells to apply source to
    labelList cells_;
    //- Sum of cell volumes
    scalar V_;
  // Protected functions
    //- Set the cellSet or points selection
    void setSelection(const dictionary& dict);
    //- Set the cell set based on the user input selection mode
    void setCellSet();
public:
  //- Runtime type information
  TYPE_NAME("cellSetOption");
  // Constructors
    //- Construct from components
    cellSetOption
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~cellSetOption();
  // Member Functions
    // Access
      //- Return const access to the time start
      inline scalar timeStart() const;
      //- Return const access to the duration
      inline scalar duration() const;
      //- Return true if within time limits
      inline bool inTimeLimits(const scalar time) const;
      //- Return const access to the cell selection mode
      inline const selectionModeType& selectionMode() const;
      //- Return const access to the name of cell set for "cellSet"
      //  selectionMode
      inline const word& cellSetName() const;
      //- Return const access to the total cell volume
      inline scalar V() const;
      //- Return const access to the cell set
      inline const labelList& cells() const;
    // Edit
      //- Return access to the time start
      inline scalar& timeStart();
      //- Return access to the duration
      inline scalar& duration();
    // Checks
      //- Is the source active?
      virtual bool isActive();
    // IO
      //- Read source dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse

// Member Functions 
inline mousse::scalar mousse::fv::cellSetOption::timeStart() const
{
  return timeStart_;
}
inline mousse::scalar mousse::fv::cellSetOption::duration() const
{
  return duration_;
}
inline bool mousse::fv::cellSetOption::inTimeLimits(const scalar /*time*/) const
{
  return ((timeStart_ < 0)
          || ((mesh_.time().value() >= timeStart_)
              && (mesh_.time().value() <= (timeStart_ + duration_))));
}
inline const mousse::fv::cellSetOption::selectionModeType&
mousse::fv::cellSetOption::selectionMode() const
{
  return selectionMode_;
}
inline const mousse::word& mousse::fv::cellSetOption::cellSetName() const
{
  return cellSetName_;
}
inline mousse::scalar mousse::fv::cellSetOption::V() const
{
  return V_;
}
inline const mousse::labelList& mousse::fv::cellSetOption::cells() const
{
  return cells_;
}
inline mousse::scalar& mousse::fv::cellSetOption::timeStart()
{
  return timeStart_;
}
inline mousse::scalar& mousse::fv::cellSetOption::duration()
{
  return duration_;
}
#endif
