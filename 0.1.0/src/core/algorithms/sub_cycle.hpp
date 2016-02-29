#ifndef CORE_ALGORITHMS_SUB_CYCLE_HPP_
#define CORE_ALGORITHMS_SUB_CYCLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::subCycle
// Description
//   Perform a subCycleTime on a field


#include "sub_cycle_time.hpp"

namespace mousse
{

template<class GeometricField>
class subCycleField
{
  // Private data

    //- Reference to the field being sub-cycled
    GeometricField& gf_;

    //- Reference to the field old-time field being sub-cycled
    //  Needed to avoid calls to oldTime() which may cause
    //  unexpected updates of the old-time field
    GeometricField& gf0_;

    //- Copy of the "real" old-time value of the field
    GeometricField gf_0_;

public:
  // Constructors

    //- Construct field and number of sub-cycles
    subCycleField(GeometricField& gf)
    :
      gf_{gf},
      gf0_{gf.oldTime()},
      gf_0_{gf0_.name() + "_", gf0_}
    {}

  //- Destructor
  ~subCycleField()
  {
    // Reset the old-time field
    gf0_ = gf_0_;
    // Correct the time index of the field to correspond to the global time
    gf_.timeIndex() = gf_.time().timeIndex();
    gf0_.timeIndex() = gf_.time().timeIndex();
  }

  //- Correct the time index of the field to correspond to
  //  the sub-cycling time.
  //
  //  The time index is incremented to protect the old-time value from
  //  being updated at the beginning of the time-loop in the case of
  //  outer iteration
  void updateTimeIndex()
  {
    gf_.timeIndex() = gf_.time().timeIndex() + 1;
    gf0_.timeIndex() = gf_.time().timeIndex() + 1;
  }
};


template<class GeometricField>
class subCycle
:
  public subCycleField<GeometricField>,
  public subCycleTime
{
public:

  // Constructors

    //- Construct field and number of sub-cycles
    subCycle(GeometricField& gf, const label nSubCycles)
    :
      subCycleField<GeometricField>{gf},
      subCycleTime{const_cast<Time&>(gf.time()), nSubCycles}
    {
      // Update the field time index to correspond to the sub-cycle time
      this->updateTimeIndex();
    }

    //- Disallow default bitwise copy construct
    subCycle(const subCycle<GeometricField>&) = delete;

    //- Disallow default bitwise assignment
    subCycle<GeometricField>& operator=
    (
      const subCycle<GeometricField>&
    ) = delete;

  //- Destructor
  //  End the subCycleTime, which restores the time state
  ~subCycle()
  {
    endSubCycle();
  }

};

}  // namespace mousse
#endif
