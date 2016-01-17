// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOPtrList
// Description
//   A PtrList of objects of type \<T\> with automated input and output.
// SourceFiles
//   ioptr_list.cpp

#ifndef ioptr_list_hpp_
#define ioptr_list_hpp_

#include "ptr_list.hpp"
#include "reg_ioobject.hpp"

namespace mousse
{
template<class T>
class IOPtrList
:
  public regIOobject,
  public PtrList<T>
{
public:
  //- Runtime type information
  TYPE_NAME("PtrList");

  // Constructors
    //- Construct from IOobject using given Istream constructor class
    template<class INew>
    IOPtrList(const IOobject&, const INew&);
    //- Construct from IOobject
    IOPtrList(const IOobject&);
    //- Construct from IOobject with given size
    IOPtrList(const IOobject&, const label);
    //- Construct from IOobject and a PtrList
    IOPtrList(const IOobject&, const PtrList<T>&);
    //- Construct by transferring the PtrList contents
    IOPtrList(const IOobject&, const Xfer<PtrList<T> >&);

  //- Destructor
  virtual ~IOPtrList();

  // Member functions
    bool writeData(Ostream&) const;

  // Member operators
    void operator=(const IOPtrList<T>&);
};

}  // namespace mousse
#ifdef NoRepository
#   include "ioptr_list.cpp"
#endif
#endif
