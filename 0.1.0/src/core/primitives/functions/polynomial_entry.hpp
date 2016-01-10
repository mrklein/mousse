// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PolynomialEntry
// Description
//   PolynomialEntry container data entry for scalars. Items are stored in a
//   list of Tuple2's. Data is input in the form,
//   e.g. for an entry \<entryName\> that describes y = x^2 + 2x^3
//   \verbatim
//     <entryName>  polynomial  [0 0 1 0 0] // optional dimensions
//     (
//       (1    2)
//       (2    3)
//     );
//   \endverbatim
// SourceFiles
//   polynomial_entry.cpp

#ifndef polynomial_entry_hpp_
#define polynomial_entry_hpp_

#include "data_entry.hpp"
#include "tuple2.hpp"
#include "dimension_set.hpp"
#include "data_entry_fwd.hpp"

namespace mousse
{

// Forward declaration of classes
template<class Type>
class PolynomialEntry;

// Forward declaration of friend functions
template<class Type>
Ostream& operator<<(Ostream&, const PolynomialEntry<Type>&);

template<class Type>
class PolynomialEntry
:
  public DataEntry<Type>
{
  // Private data

    //- PolynomialEntry coefficients - list of prefactor, exponent
    List<Tuple2<Type, Type>> coeffs_;

    //- Flag to indicate whether poly can be integrated
    bool canIntegrate_;

    //- The dimension set
    dimensionSet dimensions_;

public:

  //- Runtime type information
  TYPE_NAME("polynomial");

  // Constructors

    PolynomialEntry(const word& entryName, const dictionary& dict);

    //- Construct from components
    PolynomialEntry
    (
      const word& entryName,
      const List<Tuple2<Type, Type> >&
    );

    //- Copy constructor
    PolynomialEntry(const PolynomialEntry& poly);

    //- Disallow default bitwise assignment
    PolynomialEntry<Type>& operator=(const PolynomialEntry<Type>&) = delete;

    //- Construct and return a clone
    virtual tmp<DataEntry<Type> > clone() const
    {
      return tmp<DataEntry<Type> >(new PolynomialEntry(*this));
    }

  //- Destructor
  virtual ~PolynomialEntry();

  // Member Functions

    // Manipulation

      //- Convert time
      virtual void convertTimeBase(const Time& t);

    // Evaluation

      //- Return PolynomialEntry value
      Type value(const scalar x) const;

      //- Integrate between two (scalar) values
      Type integrate(const scalar x1, const scalar x2) const;

      //- Return dimensioned constant value
      dimensioned<Type> dimValue(const scalar) const;

      //- Integrate between two values and return dimensioned type
      dimensioned<Type> dimIntegrate
      (
        const scalar x1,
        const scalar x2
      ) const;

  // I/O

    //- Ostream Operator
    friend Ostream& operator<< <Type>
    (
      Ostream& os,
      const PolynomialEntry<Type>& cnst
    );

    //- Write in dictionary format
    virtual void writeData(Ostream& os) const;
};

}  // namespace mousse
#ifdef NoRepository
#   include "polynomial_entry.cpp"
#endif
#endif
