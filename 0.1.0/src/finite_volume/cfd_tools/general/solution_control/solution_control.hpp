// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solutionControl
// Description
//   Base class for solution control classes
#ifndef solution_control_hpp_
#define solution_control_hpp_
#include "fv_mesh.hpp"
namespace mousse
{
class solutionControl
:
  public IOobject
{
public:
  struct fieldData
  {
    wordRe name;
    scalar absTol;
    scalar relTol;
    scalar initialResidual;
  };
protected:
  // Protected data
    //- Reference to the mesh database
    fvMesh& mesh_;
    //- List of residual data per field
    List<fieldData> residualControl_;
    //- The dictionary name, e.g. SIMPLE, PIMPLE
    const word algorithmName_;
    // Solution controls
      //- Maximum number of non-orthogonal correctors
      label nNonOrthCorr_;
      //- Flag to indicate to solve for momentum
      bool momentumPredictor_;
      //- Flag to indicate to solve using transonic algorithm
      bool transonic_;
      //- Flag to indicate to relax pressure using the
      //  "consistent" approach of SIMPLEC
      bool consistent_;
    // Evolution
      //- Current corrector loop index
      label corr_;
      //- Current non-orthogonal corrector loop index
      label corrNonOrtho_;
  // Protected Member Functions
    //- Read controls from fvSolution dictionary
    virtual void read(const bool absTolOnly);
    //- Read controls from fvSolution dictionary
    virtual void read();
    //- Return index of field in residualControl_ if present
    virtual label applyToField
    (
      const word& fieldName,
      const bool useRegEx = true
    ) const;
    //- Return true if all convergence checks are satisfied
    virtual bool criteriaSatisfied() = 0;
    //- Store previous iteration fields
    virtual void storePrevIterFields() const;
    //- Store previous iteration field for vol<Type>Fields
    template<class Type>
    void storePrevIter() const;
public:
  // Static Data Members
    //- Run-time type information
    TYPE_NAME("solutionControl");
  // Constructors
    //- Construct from mesh
    solutionControl(fvMesh& mesh, const word& algorithmName);
    //- Disallow default bitwise copy construct
    solutionControl(const solutionControl&) = delete;
    //- Disallow default bitwise assignment
    solutionControl& operator=(const solutionControl&) = delete;
  //- Destructor
  virtual ~solutionControl();
  // Member Functions
    // Access
      //- Return the solution dictionary
      inline const dictionary& dict() const;
      //- Current corrector loop index
      inline label corr() const;
      //- Current non-orthogonal corrector index
      inline label corrNonOrtho() const;
    // Solution control
      //- Maximum number of non-orthogonal correctors
      inline label nNonOrthCorr() const;
      //- Helper function to identify final non-orthogonal iteration
      inline bool finalNonOrthogonalIter() const;
      //- Flag to indicate to solve for momentum
      inline bool momentumPredictor() const;
      //- Flag to indicate to solve using transonic algorithm
      inline bool transonic() const;
      //- Flag to indicate to relax pressure using the
      //  "consistent" approach of SIMPLEC
      inline bool consistent() const;
    // Evolution
      //- Main control loop
      virtual bool loop() = 0;
      //- Non-orthogonal corrector loop
      inline bool correctNonOrthogonal();
};
}  // namespace mousse

// Member Functions 
inline const mousse::dictionary& mousse::solutionControl::dict() const
{
  return mesh_.solutionDict().subDict(algorithmName_);
}
inline mousse::label mousse::solutionControl::corr() const
{
  return corr_;
}
inline mousse::label mousse::solutionControl::corrNonOrtho() const
{
  return corrNonOrtho_;
}
inline mousse::label mousse::solutionControl::nNonOrthCorr() const
{
  return nNonOrthCorr_;
}
inline bool mousse::solutionControl::finalNonOrthogonalIter() const
{
  return corrNonOrtho_ == nNonOrthCorr_ + 1;
}
inline bool mousse::solutionControl::momentumPredictor() const
{
  return momentumPredictor_;
}
inline bool mousse::solutionControl::transonic() const
{
  return transonic_;
}
inline bool mousse::solutionControl::consistent() const
{
  return consistent_;
}
inline bool mousse::solutionControl::correctNonOrthogonal()
{
  corrNonOrtho_++;
  if (debug)
  {
    Info<< algorithmName_ << " correctNonOrthogonal: corrNonOrtho = "
      << corrNonOrtho_ << endl;
  }
  if (corrNonOrtho_ <= nNonOrthCorr_ + 1)
  {
    return true;
  }
  else
  {
    corrNonOrtho_ = 0;
    return false;
  }
}

#ifdef NoRepository
  #include "solution_control_templates.cpp"
#endif
#endif
