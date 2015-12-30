// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::moleculeCloud
// Description
// SourceFiles
//   molecule_cloud_i.hpp
//   molecule_cloud.cpp
#ifndef molecule_cloud_hpp_
#define molecule_cloud_hpp_
#include "_cloud.hpp"
#include "molecule.hpp"
#include "iodictionary.hpp"
#include "potential.hpp"
#include "_interaction_lists.hpp"
#include "label_vector.hpp"
#include "random.hpp"
#include "file_name.hpp"
namespace mousse
{
class moleculeCloud
:
  public Cloud<molecule>
{
private:
  // Private data
    const polyMesh& mesh_;
    const potential& pot_;
    List<DynamicList<molecule*> > cellOccupancy_;
    InteractionLists<molecule> il_;
    List<molecule::constantProperties> constPropList_;
    Random rndGen_;
  // Private Member Functions
    void buildConstProps();
    void setSiteSizesAndPositions();
    //- Determine which molecules are in which cells
    void buildCellOccupancy();
    void calculatePairForce();
    inline void evaluatePair
    (
      molecule& molI,
      molecule& molJ
    );
    inline bool evaluatePotentialLimit
    (
      molecule& molI,
      molecule& molJ
    ) const;
    void calculateTetherForce();
    void calculateExternalForce();
    void removeHighEnergyOverlaps();
    void initialiseMolecules
    (
      const IOdictionary& mdInitialiseDict
    );
    void createMolecule
    (
      const point& position,
      label cell,
      label tetFace,
      label tetPt,
      label id,
      bool tethered,
      scalar temperature,
      const vector& bulkVelocity
    );
    label nSites() const;
    inline vector equipartitionLinearVelocity
    (
      scalar temperature,
      scalar mass
    );
    inline vector equipartitionAngularMomentum
    (
      scalar temperature,
      const molecule::constantProperties& cP
    );
    //- Disallow default bitwise copy construct
    moleculeCloud(const moleculeCloud&);
    //- Disallow default bitwise assignment
    void operator=(const moleculeCloud&);
public:
  // Constructors
    //- Construct given mesh and potential references
    moleculeCloud
    (
      const polyMesh& mesh,
      const potential& pot,
      bool readFields = true
    );
    //- Construct given mesh, potential and mdInitialiseDict
    moleculeCloud
    (
      const polyMesh& mesh,
      const potential& pot,
      const IOdictionary& mdInitialiseDict,
      bool readFields = true
    );
  // Member Functions
    //- Evolve the molecules (move, calculate forces, control state etc)
    void evolve();
    void calculateForce();
    void applyConstraintsAndThermostats
    (
      const scalar targetTemperature,
      const scalar measuredTemperature
    );
    // Access
      inline const polyMesh& mesh() const;
      inline const potential& pot() const;
      inline const List<DynamicList<molecule*> >& cellOccupancy() const;
      inline const InteractionLists<molecule>& il() const;
      inline const List<molecule::constantProperties> constProps() const;
      inline const molecule::constantProperties&
        constProps(label id) const;
      inline Random& rndGen();
  // Member Operators
    //- Write molecule sites in XYZ format
    void writeXYZ(const fileName& fName) const;
};
}  // namespace mousse
#include "molecule_cloud_i.hpp"
#endif
