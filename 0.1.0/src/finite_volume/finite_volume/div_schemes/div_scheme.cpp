// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv.hpp"
#include "hash_table.hpp"
#include "linear.hpp"
namespace mousse
{
namespace fv
{
// Selectors
template<class Type>
tmp<divScheme<Type> > divScheme<Type>::New
(
  const fvMesh& mesh,
  Istream& schemeData
)
{
  if (fv::debug)
  {
    Info<< "divScheme<Type>::New(const fvMesh&, Istream&) : "
       "constructing divScheme<Type>"
      << endl;
  }
  if (schemeData.eof())
  {
    FatalIOErrorIn
    (
      "divScheme<Type>::New(const fvMesh&, Istream&)",
      schemeData
    )   << "Div scheme not specified" << endl << endl
      << "Valid div schemes are :" << endl
      << IstreamConstructorTablePtr_->sortedToc()
      << exit(FatalIOError);
  }
  const word schemeName(schemeData);
  typename IstreamConstructorTable::iterator cstrIter =
    IstreamConstructorTablePtr_->find(schemeName);
  if (cstrIter == IstreamConstructorTablePtr_->end())
  {
    FatalIOErrorIn
    (
      "divScheme<Type>::New(const fvMesh&, Istream&)",
      schemeData
    )   << "unknown div scheme "
      << schemeName << nl << nl
      << "Valid div schemes are :" << endl
      << IstreamConstructorTablePtr_->sortedToc()
      << exit(FatalIOError);
  }
  return cstrIter()(mesh, schemeData);
}
// Destructor 
template<class Type>
divScheme<Type>::~divScheme()
{}
// Member Functions 
}  // namespace fv
}  // namespace mousse
