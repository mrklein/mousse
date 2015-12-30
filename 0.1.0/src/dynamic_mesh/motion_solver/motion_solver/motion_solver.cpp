// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "motion_solver.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "dl_library_table.hpp"
#include "twod_point_corrector.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(motionSolver, 0);
  defineRunTimeSelectionTable(motionSolver, dictionary);
}
// Static Member Functions
mousse::IOobject mousse::motionSolver::stealRegistration
(
  const IOdictionary& dict
)
{
  IOobject io(dict);
  if (dict.registerObject())
  {
    // De-register if necessary
    const_cast<IOdictionary&>(dict).checkOut();
    io.registerObject() = true;
  }
  return io;
}
// Constructors 
mousse::motionSolver::motionSolver(const polyMesh& mesh)
:
  IOdictionary
  (
    IOobject
    (
      "dynamicMeshDict",
      mesh.time().constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::AUTO_WRITE
    )
  ),
  mesh_(mesh)
{}
mousse::motionSolver::motionSolver
(
  const polyMesh& mesh,
  const IOdictionary& dict,
  const word& type
)
:
  IOdictionary(stealRegistration(dict), dict),
  mesh_(mesh),
  coeffDict_(dict.subDict(type + "Coeffs"))
{}
// Selectors
mousse::autoPtr<mousse::motionSolver> mousse::motionSolver::New
(
  const polyMesh& mesh,
  const IOdictionary& solverDict
)
{
  const word solverTypeName(solverDict.lookup("solver"));
  Info<< "Selecting motion solver: " << solverTypeName << endl;
  const_cast<Time&>(mesh.time()).libs().open
  (
    solverDict,
    "motionSolverLibs",
    dictionaryConstructorTablePtr_
  );
  if (!dictionaryConstructorTablePtr_)
  {
    FatalErrorIn
    (
      "motionSolver::New(const polyMesh& mesh)"
    )   << "solver table is empty"
      << exit(FatalError);
  }
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(solverTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "motionSolver::New(const polyMesh&)"
    )   << "Unknown solver type "
      << solverTypeName << nl << nl
      << "Valid solver types are:" << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<motionSolver>(cstrIter()(mesh, solverDict));
}
mousse::autoPtr<mousse::motionSolver> mousse::motionSolver::New(const polyMesh& mesh)
{
  IOdictionary solverDict
  (
    IOobject
    (
      "dynamicMeshDict",
      mesh.time().constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::AUTO_WRITE
    )
  );
  return New(mesh, solverDict);
}
// Destructor 
mousse::motionSolver::~motionSolver()
{}
// Member Functions 
mousse::tmp<mousse::pointField> mousse::motionSolver::newPoints()
{
  solve();
  return curPoints();
}
void mousse::motionSolver::twoDCorrectPoints(pointField& p) const
{
  twoDPointCorrector::New(mesh_).correctPoints(p);
}
void mousse::motionSolver::updateMesh(const mapPolyMesh& mpm)
{}
bool mousse::motionSolver::writeObject
(
  IOstream::streamFormat fmt,
  IOstream::versionNumber ver,
  IOstream::compressionType cmp
) const
{
  return true;
}
bool mousse::motionSolver::read()
{
  if (regIOobject::read())
  {
    coeffDict_ = subDict(type() + "Coeffs");
    return true;
  }
  else
  {
    return false;
  }
}
