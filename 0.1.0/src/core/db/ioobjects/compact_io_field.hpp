// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CompactIOField
// Description
//   A Field of objects of type \<T\> with automated input and output using
//   a compact storage. Behaves like IOField except when binary output in
//   case it writes a CompactListList.
//   Useful for fields of small subfields e.g. in lagrangian
// SourceFiles
//   compact_io_field.cpp
#ifndef compact_io_field_hpp_
#define compact_io_field_hpp_
#include "iofield.hpp"
#include "reg_ioobject.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class T, class BaseType> class CompactIOField;
template<class T, class BaseType> Istream& operator>>
(
  Istream&,
  CompactIOField<T, BaseType>&
);
template<class T, class BaseType> Ostream& operator<<
(
  Ostream&,
  const CompactIOField<T, BaseType>&
);
template<class T, class BaseType>
class CompactIOField
:
  public regIOobject,
  public Field<T>
{
  // Private Member Functions
    //- Read according to header type
    void readFromStream();
public:
  //- Runtime type information
  TYPE_NAME("FieldField");
  // Constructors
    //- Construct from IOobject
    CompactIOField(const IOobject&);
    //- Construct from IOobject and size
    CompactIOField(const IOobject&, const label);
    //- Construct from IOobject and a Field
    CompactIOField(const IOobject&, const Field<T>&);
    //- Construct by transferring the Field contents
    CompactIOField(const IOobject&, const Xfer<Field<T> >&);
  // Destructor
    virtual ~CompactIOField();
  // Member functions
    virtual bool writeObject
    (
      IOstream::streamFormat,
      IOstream::versionNumber,
      IOstream::compressionType
    ) const;
    virtual bool writeData(Ostream&) const;
  // Member operators
    void operator=(const CompactIOField<T, BaseType>&);
    void operator=(const Field<T>&);
};
}  // namespace mousse
#ifdef NoRepository
#   include "compact_io_field.cpp"
#endif
#endif
