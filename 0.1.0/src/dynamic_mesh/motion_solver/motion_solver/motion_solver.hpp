// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::motionSolver
// Description
//   Virtual base class for mesh motion solver.
// SourceFiles
//   motion_solver.cpp
#ifndef motion_solver_hpp_
#define motion_solver_hpp_
#include "iodictionary.hpp"
#include "point_field.hpp"
namespace mousse
{
// Forward class declarations
class polyMesh;
class mapPolyMesh;
class motionSolver
:
  public IOdictionary
{
private:
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
    //- Model coefficients dictionary
    dictionary coeffDict_;
  // Private Member Functions
    //- De-register object if registered and assign to current
    static IOobject stealRegistration(const IOdictionary& dict);
public:
  //- Runtime type information
  TYPE_NAME("motionSolver");
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      motionSolver,
      dictionary,
      (const polyMesh& mesh, const IOdictionary& dict),
      (mesh, dict)
    );
  // Selectors
    //- Select constructed from polyMesh
    static autoPtr<motionSolver> New(const polyMesh&);
    //- Select constructed from polyMesh and dictionary. If dictionary
    //  was registered this will 'steal' that registration.
    static autoPtr<motionSolver> New(const polyMesh&, const IOdictionary&);
  // Constructors
    //- Construct from polyMesh
    motionSolver(const polyMesh& mesh);
    //- Construct from polyMesh and dictionary and type.
    motionSolver
    (
      const polyMesh& mesh,
      const IOdictionary&,
      const word& type
    );
  //- Destructor
  virtual ~motionSolver();
  // Member Functions
    //- Return reference to mesh
    const polyMesh& mesh() const
    {
      return mesh_;
    }
    //- Const access to the coefficients dictionary
    const dictionary& coeffDict() const
    {
      return coeffDict_;
    }
    //- Provide new points for motion.  Solves for motion
    virtual tmp<pointField> newPoints();
    //- Provide current points for motion.  Uses current motion field
    virtual tmp<pointField> curPoints() const = 0;
    virtual void twoDCorrectPoints(pointField&) const;
    //- Solve for motion
    virtual void solve() = 0;
    //- Update local data for geometry changes
    virtual void movePoints(const pointField&) = 0;
    //-  Update local data for topology changes
    virtual void updateMesh(const mapPolyMesh&) = 0;
    //- Write state using given format, version and compression
    virtual bool writeObject
    (
      IOstream::streamFormat fmt,
      IOstream::versionNumber ver,
      IOstream::compressionType cmp
    ) const;
    //- Read dynamicMeshDict dictionary
    virtual bool read();
};
}  // namespace mousse
#endif
