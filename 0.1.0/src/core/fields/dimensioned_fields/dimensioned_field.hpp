// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DimensionedField
// Description
//   Field with dimensions and associated with geometry type GeoMesh which is
//   used to size the field and a reference to it is maintained.
// SourceFiles
//   dimensioned_field.cpp
//   dimensioned_field_io.cpp

#ifndef dimensioned_field_hpp_
#define dimensioned_field_hpp_

#include "reg_ioobject.hpp"
#include "field.hpp"
#include "dimensioned_type.hpp"

namespace mousse
{
// Forward declaration of friend functions and operators
template<class Type, class GeoMesh> class DimensionedField;
template<class Type, class GeoMesh> Ostream& operator<<
(
  Ostream&,
  const DimensionedField<Type, GeoMesh>&
);
template<class Type, class GeoMesh> Ostream& operator<<
(
  Ostream&,
  const tmp<DimensionedField<Type, GeoMesh> >&
);
template<class Type, class GeoMesh>
class DimensionedField
:
  public regIOobject,
  public Field<Type>
{
public:
  // Public typedefs
    typedef typename GeoMesh::Mesh Mesh;
    typedef typename Field<Type>::cmptType cmptType;
private:
  // Private data
    //- Reference to mesh
    const Mesh& mesh_;
    //- Dimension set for this field
    dimensionSet dimensions_;
  // Private Member Functions
    void readIfPresent(const word& fieldDictEntry = "value");
public:
  //- Runtime type information
  TYPE_NAME("DimensionedField");
  // Static Member Functions
    //- Return a null DimensionedField
    inline static const DimensionedField<Type, GeoMesh>& null();
  // Constructors
    //- Construct from components
    DimensionedField
    (
      const IOobject&,
      const Mesh& mesh,
      const dimensionSet&,
      const Field<Type>&
    );
    //- Construct from components
    //  Used for temporary fields which are initialised after construction
    DimensionedField
    (
      const IOobject&,
      const Mesh& mesh,
      const dimensionSet&,
      const bool checkIOFlags = true
    );
    //- Construct from components
    DimensionedField
    (
      const IOobject&,
      const Mesh& mesh,
      const dimensioned<Type>&,
      const bool checkIOFlags = true
    );
    //- Construct from Istream
    DimensionedField
    (
      const IOobject&,
      const Mesh& mesh,
      const word& fieldDictEntry="value"
    );
    //- Construct as copy
    DimensionedField
    (
      const DimensionedField<Type, GeoMesh>&
    );
    //- Construct as copy or re-use as specified.
    DimensionedField
    (
      DimensionedField<Type, GeoMesh>&,
      bool reUse
    );
    //- Construct by transferring the DimensionedField
    DimensionedField
    (
      const Xfer<DimensionedField<Type, GeoMesh> >&
    );
    //- Construct as copy of tmp<DimensionedField> deleting argument
    #ifndef NoConstructFromTmp
    DimensionedField
    (
      const tmp<DimensionedField<Type, GeoMesh> >&
    );
    #endif
    //- Construct as copy resetting IO parameters
    DimensionedField
    (
      const IOobject&,
      const DimensionedField<Type, GeoMesh>&
    );
    //- Construct as copy resetting IO parameters and re-use as specified.
    DimensionedField
    (
      const IOobject&,
      DimensionedField<Type, GeoMesh>&,
      bool reUse
    );
    //- Construct as copy resetting name
    DimensionedField
    (
      const word& newName,
      const DimensionedField<Type, GeoMesh>&
    );
    //- Construct as copy resetting name and re-use as specified.
    DimensionedField
    (
      const word& newName,
      DimensionedField<Type, GeoMesh>&,
      bool reUse
    );
    //- Construct by transferring the DimensionedField with a new name
    DimensionedField
    (
      const word& newName,
      const Xfer<DimensionedField<Type, GeoMesh> >&
    );
    //- Construct as copy resetting name
    #ifndef NoConstructFromTmp
    DimensionedField
    (
      const word& newName,
      const tmp<DimensionedField<Type, GeoMesh> >&
    );
    #endif
    //- Clone
    tmp<DimensionedField<Type, GeoMesh> > clone() const;
  //- Destructor
  virtual ~DimensionedField();
  // Member Functions
    void readField
    (
      const dictionary& fieldDict,
      const word& fieldDictEntry = "value"
    );
    //- Return mesh
    inline const Mesh& mesh() const;
    //- Return dimensions
    inline const dimensionSet& dimensions() const;
    //- Return non-const access to dimensions
    inline dimensionSet& dimensions();
    inline const Field<Type>& field() const;
    inline Field<Type>& field();
    //- Return a component field of the field
    tmp<DimensionedField<cmptType, GeoMesh> > component
    (
      const direction
    ) const;
    //- Replace a component field of the field
    void replace
    (
      const direction,
      const DimensionedField<cmptType, GeoMesh>&
    );
    //- Replace a component field of the field
    void replace
    (
      const direction,
      const tmp<DimensionedField<cmptType, GeoMesh> >&
    );
    //- Return the field transpose (only defined for second rank tensors)
    tmp<DimensionedField<Type, GeoMesh> > T() const;
    //- Calculate and return arithmetic average
    dimensioned<Type> average() const;
    //- Calculate and return weighted average
    dimensioned<Type> weightedAverage
    (
      const DimensionedField<scalar, GeoMesh>&
    ) const;
    //- Calculate and return weighted average
    dimensioned<Type> weightedAverage
    (
      const tmp<DimensionedField<scalar, GeoMesh> >&
    ) const;
    // Write
      bool writeData(Ostream&, const word& fieldDictEntry) const;
      bool writeData(Ostream&) const;
  // Member Operators
    void operator=(const DimensionedField<Type, GeoMesh>&);
    void operator=(const tmp<DimensionedField<Type, GeoMesh> >&);
    void operator=(const dimensioned<Type>&);
    void operator+=(const DimensionedField<Type, GeoMesh>&);
    void operator+=(const tmp<DimensionedField<Type, GeoMesh> >&);
    void operator-=(const DimensionedField<Type, GeoMesh>&);
    void operator-=(const tmp<DimensionedField<Type, GeoMesh> >&);
    void operator*=(const DimensionedField<scalar, GeoMesh>&);
    void operator*=(const tmp<DimensionedField<scalar, GeoMesh> >&);
    void operator/=(const DimensionedField<scalar, GeoMesh>&);
    void operator/=(const tmp<DimensionedField<scalar, GeoMesh> >&);
    void operator+=(const dimensioned<Type>&);
    void operator-=(const dimensioned<Type>&);
    void operator*=(const dimensioned<scalar>&);
    void operator/=(const dimensioned<scalar>&);
  // Ostream Operators
    friend Ostream& operator<< <Type, GeoMesh>
    (
      Ostream&,
      const DimensionedField<Type, GeoMesh>&
    );
    friend Ostream& operator<< <Type, GeoMesh>
    (
      Ostream&,
      const tmp<DimensionedField<Type, GeoMesh> >&
    );
};
}  // namespace mousse

// Member Functions 
template<class Type, class GeoMesh>
inline const mousse::DimensionedField<Type, GeoMesh>&
mousse::DimensionedField<Type, GeoMesh>::null()
{
  return NullObjectRef<DimensionedField<Type, GeoMesh> >();
}
template<class Type, class GeoMesh>
inline const typename GeoMesh::Mesh&
mousse::DimensionedField<Type, GeoMesh>::mesh() const
{
  return mesh_;
}
template<class Type, class GeoMesh>
inline const mousse::dimensionSet&
mousse::DimensionedField<Type, GeoMesh>::dimensions() const
{
  return dimensions_;
}
template<class Type, class GeoMesh>
inline mousse::dimensionSet&
mousse::DimensionedField<Type, GeoMesh>::dimensions()
{
  return dimensions_;
}
template<class Type, class GeoMesh>
inline const mousse::Field<Type>&
mousse::DimensionedField<Type, GeoMesh>::field() const
{
  return *this;
}
template<class Type, class GeoMesh>
inline mousse::Field<Type>&
mousse::DimensionedField<Type, GeoMesh>::field()
{
  return *this;
}

#include "dimensioned_field_functions.hpp"

#ifdef NoRepository
#   include "dimensioned_field.cpp"
#endif
#endif
