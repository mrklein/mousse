#ifndef CORE_INTERPOLATIONS_INTERPOLATION_LOOK_UP_TABLE_HPP_
#define CORE_INTERPOLATIONS_INTERPOLATION_LOOK_UP_TABLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationLookUpTable
// Description
//   A list of lists. Interpolates based on the first dimension.
//   The values must be positive and monotonically increasing in each dimension
// Note
//   - Accessing an empty list results in an error.
//   - Accessing a list with a single element always returns the same value.
// SourceFiles
//   interpolation_look_up_table.cpp
#include "list.hpp"
#include "list_ops.hpp"
#include "scalar_field.hpp"
#include "hash_table.hpp"
#include "iodictionary.hpp"
namespace mousse
{
// Forward declaration of classes
class fvMesh;
template<class Type>
class interpolationLookUpTable
:
  public List<scalarField>
{
private:
  // Private data
    //- File name
    fileName fileName_;
    //- Table dimensions
    List<label> dim_;
    //- Min on each dimension
    List<scalar> min_;
    //- Deltas on each dimension
    List<scalar> delta_;
    //- Maximum on each dimension
    List<scalar> max_;
    //- Dictionary entries
    List<dictionary> entries_;
    //- Output dictionaries
    List<dictionary> output_;
    //- Input indices from the lookup table
    List<label> entryIndices_;
    //- Output indices from the lookup Table
    List<label> outputIndices_;
    //- Field names and indices
    HashTable<label> fieldIndices_;
    //- Output list containing input and interpolation values of outputs
    List<scalar> interpOutput_;
  // Private Member Functions
    //- Read the table of data from file
    void readTable(const word& instance, const objectRegistry&);
    //- Dimension table from dictionaries input and output
    void dimensionTable();
    //- Find table index by scalarList
    label index(const List<scalar>&, const bool lastDim=true) const;
    //- Find table index by scalar
    label index(const scalar) const;
    //- Check range of lookup value
    bool checkRange(const scalar, const label) const;
    //- Interpolate function returning a scalar
    scalar interpolate
    (
      const label lo,
      const label hi,
      const scalar lookUpValue,
      const label ofield,
      const label interfield
    ) const;
    // Check list is monotonically increasing
    void check() const;
    // find hi index, interpolate and populate interpOutput_
    void findHi(const label lo, const scalar retvals);
public:
  // Constructors
    //- Construct null
    interpolationLookUpTable();
    //- Construct given the name of the file containing the table of data
    interpolationLookUpTable
    (
      const fileName&,
      const word& instance,
      const objectRegistry&
    );
    //- Construct from dictionary
    interpolationLookUpTable(const dictionary&);
    //- Construct copy
    interpolationLookUpTable(const interpolationLookUpTable&);
  // Member Functions
    //- Return true if the field exists in the table
    bool found(const word& fieldName) const;
    //- Return the output list given a single input scalar
    const List<scalar>& lookUp(const scalar);
    //- Write lookup table to filename.
    void write
    (
      Ostream&,
      const fileName&,
      const word& instance,
      const objectRegistry&
    ) const;
  // Access
    //- Return the index of a field by name
    inline label findFieldIndex(const word& fieldName) const;
    //- Return const access to the output dictionaries
    inline const List<dictionary>& output() const;
    //- Return const access tp the dictionary entries
    inline const List<dictionary>& entries() const;
    //- Return const access to the list of min dimensions
    inline const List<scalar>& min() const;
    //- Return const access to the list of dimensions
    inline const List<label>& dim() const;
    //- Return const access to the deltas in each dimension
    inline const List<scalar>& delta() const;
    //- Return const access to the list of max dimensions
    inline const List<scalar>& max() const;
    //- Return const access to the table name
    inline word tableName() const;
  // Member Operators
    //- Return an element of constant List<scalar, Type>
    const scalarField& operator[](const label) const;
    //- Return an element of List<scalar, Type>
    scalarField& operator[](const label);
};
}  // namespace mousse

template<class Type>
inline mousse::label
mousse::interpolationLookUpTable<Type>::findFieldIndex
(
  const word& fieldName
) const
{
  return fieldIndices_[fieldName];
}
template<class Type>
inline const mousse::List<mousse::dictionary>&
mousse::interpolationLookUpTable<Type>::output() const
{
  return output_;
}
template<class Type>
inline const mousse::List<mousse::dictionary>&
mousse::interpolationLookUpTable<Type>::entries() const
{
  return entries_;
}
template<class Type>
inline const mousse::List<mousse::scalar>&
mousse::interpolationLookUpTable<Type>::min() const
{
  return min_;
}
template<class Type>
inline const mousse::List<mousse::label>&
mousse::interpolationLookUpTable<Type>::dim() const
{
  return dim_;
}
template<class Type>
inline const mousse::List<mousse::scalar>&
mousse::interpolationLookUpTable<Type>::delta() const
{
  return delta_;
}
template<class Type>
inline const mousse::List<mousse::scalar>&
mousse::interpolationLookUpTable<Type>::max() const
{
  return max_;
}
template<class Type>
inline mousse::word mousse::interpolationLookUpTable<Type>::tableName() const
{
  return fileName_.name();
}
#ifdef NoRepository
#   include "interpolation_look_up_table.cpp"
#endif
#endif
