#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_TEMPLATES_CLOUD_SOLUTION_CLOUD_SOLUTION_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_TEMPLATES_CLOUD_SOLUTION_CLOUD_SOLUTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cloudSolution
// Description
//   Stores all relevant solution info for cloud
// SourceFiles
//   cloud_solution.cpp
#include "fv_mesh.hpp"
#include "switch.hpp"
#include "tuple2.hpp"
namespace mousse
{
class cloudSolution
{
  // Private Data
    //- Reference to the mesh
    const fvMesh& mesh_;
    //- Dictionary used during construction
    dictionary dict_;
    //- Cloud active flag
    const Switch active_;
    //- Transient flag
    Switch transient_;
    //- Calculation frequency - carrier steps per cloud step
    //  NOTE: Steady operation only
    label calcFrequency_;
    //- Maximum particle Courant number
    //  Max fraction of current cell that can be traversed in a single
    //  step
    scalar maxCo_;
    //- Current cloud iteration
    label iter_;
    //- Particle track time
    scalar trackTime_;
    // Run-time options
      //- Flag to indicate whether parcels are coupled to the carrier
      //  phase, i.e. whether or not to generate source terms for
      //  carrier phase
      Switch coupled_;
      //- Flag to correct cell values with latest transfer information
      //  during the lagrangian timestep
      Switch cellValueSourceCorrection_;
      //- Maximum particle track time [s]
      scalar maxTrackTime_;
      //- Flag to indicate whether coupling source terms should be
      //  reset on start-up/first read
      Switch resetSourcesOnStartup_;
      //- List schemes, e.g. U semiImplicit 1
      List<Tuple2<word, Tuple2<bool, scalar> > > schemes_;
public:
  // Constructors
    //- Construct null from mesh reference
    cloudSolution(const fvMesh& mesh);
    //- Construct from mesh and dictionary
    cloudSolution(const fvMesh& mesh, const dictionary& dict);
    //- Construct copy
    cloudSolution(const cloudSolution& cs);
    //- Disallow default bitwise assignment
    cloudSolution& operator=(const cloudSolution&) = delete;
  //- Destructor
  virtual ~cloudSolution();
  // Member functions
    //- Read properties from dictionary
    void read();
    // Access
      //- Return relaxation coefficient for field
      scalar relaxCoeff(const word& fieldName) const;
      //- Return semi-implicit flag coefficient for field
      bool semiImplicit(const word& fieldName) const;
      //- Return reference to the mesh
      inline const fvMesh& mesh() const;
      //- Return const access to the dictionary
      inline const dictionary& dict() const;
      //- Return the active flag
      inline const Switch active() const;
      //- Return const access to the transient flag
      inline const Switch transient() const;
      //- Return const access to the steady flag
      inline const Switch steadyState() const;
      //- Return const access to the calculation frequency
      inline label calcFrequency() const;
      //- Return const access to the max particle Courant number
      inline scalar maxCo() const;
      //- Return const access to the current cloud iteration
      inline label iter() const;
      //- Increment and return iter counter
      inline label nextIter();
      //- Return the particle track time
      inline scalar trackTime() const;
      //- Return const access to the coupled flag
      inline const Switch coupled() const;
      //- Return non-const access to the coupled flag
      inline Switch& coupled();
      //- Return const access to the cell value correction flag
      inline const Switch cellValueSourceCorrection() const;
      //- Return const access to the particle track time
      inline scalar maxTrackTime() const;
      //- Return const access to the reset sources flag
      inline const Switch resetSourcesOnStartup() const;
      //- Source terms dictionary
      inline const dictionary& sourceTermDict() const;
      //- Interpolation schemes dictionary
      inline const dictionary& interpolationSchemes() const;
      //- Integration schemes dictionary
      inline const dictionary& integrationSchemes() const;
  // Helper functions
    //- Returns true if performing a cloud iteration this calc step
    bool solveThisStep() const;
    //- Returns true if possible to evolve the cloud and sets timestep
    //  parameters
    bool canEvolve();
    //- Returns true if writing this step
    bool output() const;
};
}  // namespace mousse

inline const mousse::fvMesh& mousse::cloudSolution::mesh() const
{
  return mesh_;
}
inline const mousse::dictionary& mousse::cloudSolution::dict() const
{
  return dict_;
}
inline const mousse::Switch mousse::cloudSolution::active() const
{
  return active_;
}
inline const mousse::dictionary& mousse::cloudSolution::sourceTermDict() const
{
  return dict_.subDict("sourceTerms");
}
inline const mousse::dictionary& mousse::cloudSolution::interpolationSchemes() const
{
  return dict_.subDict("interpolationSchemes");
}
inline const mousse::dictionary& mousse::cloudSolution::integrationSchemes() const
{
  return dict_.subDict("integrationSchemes");
}
inline const mousse::Switch mousse::cloudSolution::transient() const
{
  return transient_;
}
inline const mousse::Switch mousse::cloudSolution::steadyState() const
{
  return !transient_;
}
inline mousse::label mousse::cloudSolution::calcFrequency() const
{
  return calcFrequency_;
}
inline mousse::scalar mousse::cloudSolution::maxCo() const
{
  return maxCo_;
}
inline mousse::label mousse::cloudSolution::iter() const
{
  return iter_;
}
inline mousse::label mousse::cloudSolution::nextIter()
{
  return ++iter_;
}
inline mousse::scalar mousse::cloudSolution::trackTime() const
{
  return trackTime_;
}
inline mousse::Switch& mousse::cloudSolution::coupled()
{
  return coupled_;
}
inline const mousse::Switch mousse::cloudSolution::coupled() const
{
  return coupled_;
}
inline const mousse::Switch mousse::cloudSolution::cellValueSourceCorrection() const
{
  return cellValueSourceCorrection_;
}
inline mousse::scalar mousse::cloudSolution::maxTrackTime() const
{
  return maxTrackTime_;
}
inline const mousse::Switch mousse::cloudSolution::resetSourcesOnStartup() const
{
  return resetSourcesOnStartup_;
}

#endif
