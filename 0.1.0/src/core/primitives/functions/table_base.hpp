#ifndef CORE_PRIMITIVES_FUNCTIONS_TABLE_BASE_HPP_
#define CORE_PRIMITIVES_FUNCTIONS_TABLE_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TableBase
// Description
//   Base class for table with bounds handling, interpolation and integration

#include "data_entry.hpp"
#include "tuple2.hpp"
#include "dimension_set.hpp"


namespace mousse {

template<class Type>
class TableBase;
template<class Type>
Ostream& operator<<
(
  Ostream&,
  const TableBase<Type>&
);
class interpolationWeights;


template<class Type>
class TableBase
:
  public DataEntry<Type>
{
public:

  // Public data types

    //- Enumeration for handling out-of-bound values
    enum boundsHandling
    {
      ERROR,          /*!< Exit with a FatalError */
      WARN,           /*!< Issue warning and clamp value (default) */
      CLAMP,          /*!< Clamp value to the start/end value */
      REPEAT          /*!< Treat as a repeating list */
    };

protected:

  // Protected data

    //- Table name
    const word name_;

    //- Enumeration for handling out-of-bound values
    const boundsHandling boundsHandling_;

    //- Interpolation type
    const word interpolationScheme_;

    //- Table data
    List<Tuple2<scalar, Type>> table_;

    //- The dimension set
    dimensionSet dimensions_;

    //- Extracted values
    mutable autoPtr<scalarField> tableSamplesPtr_;

    //- Interpolator method
    mutable autoPtr<interpolationWeights> interpolatorPtr_;

    //- Cached indices and weights
    mutable labelList currentIndices_;
    mutable scalarField currentWeights_;

  // Protected Member Functions

    //- Return (demand driven) interpolator
    const interpolationWeights& interpolator() const;

public:
  // Constructors

    //- Construct from dictionary - note table is not populated
    TableBase(const word& name, const dictionary& dict);

    //- Copy constructor. Note: steals interpolator, tableSamples
    TableBase(const TableBase<Type>& tbl);

    //- Disallow default bitwise assignment
    TableBase<Type>& operator=(const TableBase<Type>&) = delete;

  //- Destructor
  virtual ~TableBase();

  // Member Functions

    //- Return the out-of-bounds handling as a word
    word boundsHandlingToWord(const boundsHandling& bound) const;

    //- Return the out-of-bounds handling as an enumeration
    boundsHandling wordToBoundsHandling(const word& bound) const;

    //- Set the out-of-bounds handling from enum, return previous setting
    boundsHandling outOfBounds(const boundsHandling& bound);

    //- Check the table for size and consistency
    void check() const;

    //- Check minimum table bounds
    bool checkMinBounds(const scalar x, scalar& xDash) const;

    //- Check maximum table bounds
    bool checkMaxBounds(const scalar x, scalar& xDash) const;

    //- Convert time
    virtual void convertTimeBase(const Time& t);

    //- Return Table value
    virtual Type value(const scalar x) const;

    //- Integrate between two (scalar) values
    virtual Type integrate(const scalar x1, const scalar x2) const;

    //- Return dimensioned constant value
    virtual dimensioned<Type> dimValue(const scalar x) const;

    //- Integrate between two values and return dimensioned type
    virtual dimensioned<Type> dimIntegrate
    (
      const scalar x1,
      const scalar x2
    ) const;

    //- Return the reference values
    virtual tmp<scalarField> x() const;

    //- Return the dependent values
    virtual tmp<Field<Type>> y() const;

  // I/O

    //- Ostream Operator
    friend Ostream& operator<< <Type>
    (
      Ostream& os,
      const TableBase<Type>& tbl
    );

    //- Write all table data in dictionary format
    virtual void writeData(Ostream& os) const;

    //- Write keywords only in dictionary format. Used for non-inline
    //  table types
    virtual void writeEntries(Ostream& os) const;
};

}  // namespace mousse

#include "table_base.ipp"

#endif
