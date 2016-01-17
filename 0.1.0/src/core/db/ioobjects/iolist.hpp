// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOList
// Description
//   A List of objects of type \<T\> with automated input and output.
// SourceFiles
//   iolist.cpp
#ifndef iolist_hpp_
#define iolist_hpp_
#include "list.hpp"
#include "reg_ioobject.hpp"
namespace mousse
{
template<class T>
class IOList
:
  public regIOobject,
  public List<T>
{
public:
  //- Runtime type information
  TYPE_NAME("List");
  // Constructors
    //- Construct from IOobject
    IOList(const IOobject&);
    //- Construct from IOobject and size of IOList
    IOList(const IOobject&, const label);
    //- Construct from IOobject and a List
    IOList(const IOobject&, const List<T>&);
    //- Construct by transferring the List contents
    IOList(const IOobject&, const Xfer<List<T> >&);
  //- Destructor
  virtual ~IOList();
  // Member functions
    bool writeData(Ostream&) const;
  // Member operators
    void operator=(const IOList<T>&);
    void operator=(const List<T>&);
};
}  // namespace mousse
#ifdef NoRepository
#   include "iolist.cpp"
#endif
#endif
