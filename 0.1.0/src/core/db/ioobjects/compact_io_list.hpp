// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CompactIOList
// Description
//   A List of objects of type \<T\> with automated input and output using
//   a compact storage. Behaves like IOList except when binary output in
//   case it writes a CompactListList.
//   Useful for lists of small sublists e.g. faceList, cellList.
// SourceFiles
//   compact_io_list.cpp
#ifndef compact_io_list_hpp_
#define compact_io_list_hpp_
#include "iolist.hpp"
#include "reg_ioobject.hpp"
namespace mousse
{
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
template<class T, class BaseType> class CompactIOList;
template<class T, class BaseType> Istream& operator>>
(
  Istream&,
  CompactIOList<T, BaseType>&
);
template<class T, class BaseType> Ostream& operator<<
(
  Ostream&,
  const CompactIOList<T, BaseType>&
);
template<class T, class BaseType>
class CompactIOList
:
  public regIOobject,
  public List<T>
{
  // Private Member Functions
    //- Read according to header type
    void readFromStream();
public:
  //- Runtime type information
  TYPE_NAME("CompactList");
  // Constructors
    //- Construct from IOobject
    CompactIOList(const IOobject&);
    //- Construct from IOobject and size of CompactIOList
    CompactIOList(const IOobject&, const label);
    //- Construct from IOobject and a List
    CompactIOList(const IOobject&, const List<T>&);
    //- Construct by transferring the List contents
    CompactIOList(const IOobject&, const Xfer<List<T> >&);
  // Destructor
    virtual ~CompactIOList();
  // Member functions
    virtual bool writeObject
    (
      IOstream::streamFormat,
      IOstream::versionNumber,
      IOstream::compressionType
    ) const;
    virtual bool writeData(Ostream&) const;
  // Member operators
    void operator=(const CompactIOList<T, BaseType>&);
    void operator=(const List<T>&);
  // IOstream operators
    //- Read List from Istream, discarding contents of existing List.
    friend Istream& operator>> <T, BaseType>
    (
      Istream&,
      CompactIOList<T, BaseType>&
    );
    // Write List to Ostream.
    friend Ostream& operator<< <T, BaseType>
    (
      Ostream&,
      const CompactIOList<T, BaseType>&
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "compact_io_list.cpp"
#endif
#endif
