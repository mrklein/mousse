// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv.hpp"
#include "hash_table.hpp"


namespace mousse {
namespace fv {

// Selectors
template<class Type>
tmp<d2dt2Scheme<Type> > d2dt2Scheme<Type>::New
(
  const fvMesh& mesh,
  Istream& schemeData
)
{
  if (fv::debug) {
    Info << "d2dt2Scheme<Type>::New(const fvMesh&, Istream&) : "
            "constructing d2dt2Scheme<Type>"
      << endl;
  }
  if (schemeData.eof()) {
    FATAL_IO_ERROR_IN
    (
      "d2dt2Scheme<Type>::New(const fvMesh&, Istream&)",
      schemeData
    )
    << "D2dt2 scheme not specified" << endl << endl
    << "Valid d2dt2 schemes are :" << endl
    << IstreamConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  const word schemeName(schemeData);
  typename IstreamConstructorTable::iterator cstrIter =
    IstreamConstructorTablePtr_->find(schemeName);
  if (cstrIter == IstreamConstructorTablePtr_->end()) {
    FATAL_IO_ERROR_IN
    (
      "d2dt2Scheme<Type>::New(const fvMesh&, Istream&)",
      schemeData
    )
    << "Unknown d2dt2 scheme " << schemeName << nl << nl
    << "Valid d2dt2 schemes are :" << endl
    << IstreamConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  return cstrIter()(mesh, schemeData);
}


// Destructor 
template<class Type>
d2dt2Scheme<Type>::~d2dt2Scheme()
{}

}  // namespace fv
}  // namespace mousse
