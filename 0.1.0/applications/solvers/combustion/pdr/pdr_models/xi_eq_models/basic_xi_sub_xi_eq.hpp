// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiEqModels::basicSubGrid
// Description
//   Basic sub-grid obstacle flame-wrinking enhancement factor model.
//   Details supplied by J Puttock 2/7/06.
//   <b> Sub-grid flame area generation </b>
//   \f$ n = N - \hat{\dwea{\vec{U}}}.n_{s}.\hat{\dwea{\vec{U}}} \f$
//   \f$ n_{r} = \sqrt{n} \f$
//   where:
//     \f$ \hat{\dwea{\vec{U}}} = \dwea{\vec{U}} / \vert \dwea{\vec{U}}
//     \vert \f$
//     \f$ b = \hat{\dwea{\vec{U}}}.B.\hat{\dwea{\vec{U}}} / n_{r} \f$
//   where:
//     \f$ B \f$ is the file "B".
//     \f$ N \f$ is the file "N".
//     \f$  n_{s} \f$ is the file "ns".
//   The flame area enhancement factor \f$ \Xi_{sub} \f$ is expected to
//   approach:
//   \f[
//     \Xi_{{sub}_{eq}} =
//       1 + max(2.2 \sqrt{b}, min(0.34 \frac{\vert \dwea{\vec{U}}
//       \vert}{{\vec{U}}^{'}}, 1.6)) \times min(\frac{n}{4}, 1)
//   \f]
// SourceFiles
//   basic_sub_grid.cpp
#ifndef BASICSUBGRID_H
#define BASICSUBGRID_H
#include "xi_eq_model.hpp"
namespace mousse
{
namespace XiEqModels
{
class basicSubGrid
:
  public XiEqModel
{
  // Private data
    //- tblock
    volSymmTensorField B_;
    //- Equilibrium Xi model due to turbulence
    autoPtr<XiEqModel> XiEqModel_;
public:
  //- Runtime type information
  TYPE_NAME("basicSubGrid");
  // Constructors
    //- Construct from components
    basicSubGrid
    (
      const dictionary& XiEqProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su
    );
    //- Disallow copy construct
    basicSubGrid(const basicSubGrid&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const basicSubGrid&) = delete;
  //- Destructor
  virtual ~basicSubGrid();
  // Member Functions
    //- Return the flame-wrinking XiEq
    virtual tmp<volScalarField> XiEq() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& XiEqProperties);
};
}  // namespace XiEqModels
}  // namespace mousse
#endif
