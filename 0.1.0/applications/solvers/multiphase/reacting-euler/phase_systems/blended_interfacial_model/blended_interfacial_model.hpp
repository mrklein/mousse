// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::BlendedInterfacialModel
// Description
// SourceFiles
//   blended_interfacial_model.cpp
#ifndef blended_interfacial_model_hpp_
#define blended_interfacial_model_hpp_
#include "blending_method.hpp"
#include "phase_pair.hpp"
#include "ordered_phase_pair.hpp"
#include "geometric_zero_field.hpp"
namespace mousse
{
template<class ModelType>
class BlendedInterfacialModel
{
  // Private data
    //- Reference to phase 1
    const phaseModel& phase1_;
    //- Reference to phase 2
    const phaseModel& phase2_;
    //- Blending model
    const blendingMethod& blending_;
    //- Model for region with no obvious dispersed phase
    autoPtr<ModelType> model_;
    //- Model for dispersed phase 1 in continuous phase 2
    autoPtr<ModelType> model1In2_;
    //- Model for dispersed phase 2 in continuous phase 1
    autoPtr<ModelType> model2In1_;
    //- If true set coefficients and forces to 0 at fixed-flux BCs
    bool correctFixedFluxBCs_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    BlendedInterfacialModel(const BlendedInterfacialModel<ModelType>&);
    //- Disallow default bitwise assignment
    void operator=(const BlendedInterfacialModel<ModelType>&);
    //- Correct coeff/value on fixed flux boundary conditions
    template<class GeometricField>
    void correctFixedFluxBCs(GeometricField& field) const;
public:
  // Constructors
    //- Construct from two phases, blending method and three models
    BlendedInterfacialModel
    (
      const phaseModel& phase1,
      const phaseModel& phase2,
      const blendingMethod& blending,
      autoPtr<ModelType> model,
      autoPtr<ModelType> model1In2,
      autoPtr<ModelType> model2In1,
      const bool correctFixedFluxBCs = true
    );
    //- Construct from the model table, dictionary and pairs
    BlendedInterfacialModel
    (
      const phasePair::dictTable& modelTable,
      const blendingMethod& blending,
      const phasePair& pair,
      const orderedPhasePair& pair1In2,
      const orderedPhasePair& pair2In1,
      const bool correctFixedFluxBCs = true
    );
  //- Destructor
  ~BlendedInterfacialModel();
  // Member Functions
    //- Return true if a model is specified for the supplied phase
    bool hasModel(const phaseModel& phase) const;
    //- Return the model for the supplied phase
    const ModelType& model(const phaseModel& phase) const;
    //- Return the sign of the explicit value for the supplied phase
    scalar sign(const phaseModel& phase) const;
    //- Return the blended force coefficient
    tmp<volScalarField> K() const;
    //- Return the blended force coefficient with a specified residual alpha
    tmp<volScalarField> K(const scalar residualAlpha) const;
    //- Return the face blended force coefficient
    tmp<surfaceScalarField> Kf() const;
    //- Return the blended force
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > F() const;
    //- Return the face blended force
    tmp<surfaceScalarField> Ff() const;
    //- Return the blended diffusivity
    tmp<volScalarField> D() const;
};
}  // namespace mousse
#ifdef NoRepository
  #include "blended_interfacial_model.cpp"
#endif
#endif
