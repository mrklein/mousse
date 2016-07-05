#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_MOLECULAR_MEASUREMENTS_DISTRIBUTION_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_MOLECULAR_MEASUREMENTS_DISTRIBUTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::distribution
// Description
//   Accumulating histogram of values.  Specified bin resolution
//   automatic generation of bins.

#include "map.hpp"
#include "pair.hpp"


namespace mousse {

class distribution
:
  public Map<label>
{
  // Private data
    scalar binWidth_;
public:
  //- Runtime type information
    TYPE_NAME("distribution");
  // Static functions
    //- Write to file
      static void write
      (
        const fileName& file,
        const List<Pair<scalar>>& pairs
      );
  // Constructors
    //- Construct null
    distribution();
    //- Construct from binWidth
    distribution(const scalar binWidth);
    //- Construct as copy
    distribution(const distribution&);
  //- Destructor
  virtual ~distribution();
  // Member Functions
    label totalEntries() const;
    scalar approxTotalEntries() const;
    scalar mean() const;
    scalar median();
    //- Add a value to the appropriate bin of the distribution.
    void add(const scalar valueToAdd);
    void add(const label valueToAdd);
    void insertMissingKeys();
    List<Pair<scalar>> normalised();
    List<Pair<scalar>> normalisedMinusMean();
    List<Pair<scalar>> normalisedShifted(scalar shiftValue);
    List<Pair<scalar>> raw();
    // Access
      inline scalar binWidth() const { return binWidth_; }
  // Member Operators
    void operator=(const distribution&);
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const distribution&);
};

}  // namespace mousse

#endif
