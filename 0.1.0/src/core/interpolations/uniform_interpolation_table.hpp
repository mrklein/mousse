// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformInterpolationTable
// Description
//   Table with uniform interval in independant variable, with linear
//   interpolation
//   Example usage (scalar): values specified within a dictionary:
//   \verbatim
//   {
//     x0          0;          // lower limit
//     dx          0.2;        // fixed interval
//     log10       true;       // take log(10) when interpolating?
//     data                    // list of dependent data values
//     (
//       7870                // value at x0
//       7870                // value at x0 + dx
//       ...
//       7870                // value at x0 + n*dx
//     );
//   }
//   \endverbatim
// SourceFiles
//   uniform_interpolation_table.cpp

#ifndef uniform_interpolation_table_hpp_
#define uniform_interpolation_table_hpp_

#include "list.hpp"
#include "switch.hpp"
#include "ioobject.hpp"
#include "object_registry.hpp"

namespace mousse
{

template<class Type>
class uniformInterpolationTable
:
  public IOobject,
  public List<Type>
{
  // Private data

    // Control parameters

      //- Lower limit
      scalar x0_;

      //- Fixed interval
      scalar dx_;

      //- Flag to indicate that x data are given in log10(x) form
      Switch log10_;

      //- Bound x values
      Switch bound_;

  // Private Member Functions

    //- Check that the table is valid
    void checkTable() const;

public:
  // Constructors

    //- Construct from IOobject and readFields flag.
    //  Creates a null object if readFields = false
    uniformInterpolationTable(const IOobject&, const bool readFields);

    //- Construct from name, objectRegistry and dictionary.
    //  If initialiseOnly flag is set, control parameters are read from
    //  the dictionary, but not the data table
    uniformInterpolationTable
    (
      const word& tableName,
      const objectRegistry&,
      const dictionary&,
      const bool initialiseOnly = false
    );

    //- Construct as copy
    uniformInterpolationTable(const uniformInterpolationTable&);

    //- Disallow default bitwise assignment
    uniformInterpolationTable& operator=
    (
      const uniformInterpolationTable&
    ) = delete;

  //- Destructor
  ~uniformInterpolationTable();

  // Member Functions

    // Access

      //- Return the lower limit
      inline scalar x0() const;

      //- Return the fixed interval
      inline scalar dx() const;

      //- Return the log10(x) flag
      inline const Switch& log10() const;

      //- Return the bound flag
      inline const Switch& bound() const;

    // Edit

      //- Return the lower limit
      inline scalar& x0();

      //- Return the fixed interval
      inline scalar& dx();

      //- Return the log10(x) flag
      inline Switch& log10();

      //- Return the bound flag
      inline Switch& bound();

    // Evaluation

      //- Return the minimum x value
      inline scalar xMin() const;

      //- Return the maximum x value
      inline scalar xMax() const;

      //- Interpolate
      Type interpolate(scalar x) const;

      //- Interpolate - takes log10 flag into account
      Type interpolateLog10(scalar x) const;

    // Override ancestor size() function and [] operator

      //- Return the size of the table
      using List<Type>::size;

      //- Use List[] operator for read/write access
      using List<Type>::operator[];

    // I-O

      //- Write
      void write() const;
};
}  // namespace mousse

template<class Type>
mousse::scalar mousse::uniformInterpolationTable<Type>::x0() const
{
  return x0_;
}
template<class Type>
mousse::scalar mousse::uniformInterpolationTable<Type>::dx() const
{
  return dx_;
}
template<class Type>
const mousse::Switch& mousse::uniformInterpolationTable<Type>::log10() const
{
  return log10_;
}
template<class Type>
const mousse::Switch& mousse::uniformInterpolationTable<Type>::bound() const
{
  return bound_;
}
template<class Type>
mousse::scalar& mousse::uniformInterpolationTable<Type>::x0()
{
  return x0_;
}
template<class Type>
mousse::scalar& mousse::uniformInterpolationTable<Type>::dx()
{
  return dx_;
}
template<class Type>
mousse::Switch& mousse::uniformInterpolationTable<Type>::log10()
{
  return log10_;
}
template<class Type>
mousse::Switch& mousse::uniformInterpolationTable<Type>::bound()
{
  return bound_;
}
template<class Type>
mousse::scalar mousse::uniformInterpolationTable<Type>::xMin() const
{
  return x0_;
}
template<class Type>
mousse::scalar mousse::uniformInterpolationTable<Type>::xMax() const
{
  return x0_ + dx_*(size() - 1);
}

#ifdef NoRepository
  #include "uniform_interpolation_table.cpp"
#endif
#endif
