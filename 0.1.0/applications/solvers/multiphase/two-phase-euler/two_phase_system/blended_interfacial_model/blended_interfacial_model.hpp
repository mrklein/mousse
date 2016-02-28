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
template<class modelType>
class BlendedInterfacialModel
{
  // Private data
    //- Unordered phase pair
    const phasePair& pair_;
    //- Ordered phase pair for dispersed phase 1 in continuous phase 2
    const orderedPhasePair& pair1In2_;
    //- Ordered phase pair for dispersed phase 2 in continuous phase 1
    const orderedPhasePair& pair2In1_;
    //- Model for region with no obvious dispersed phase
    autoPtr<modelType> model_;
    //- Model for dispersed phase 1 in continuous phase 2
    autoPtr<modelType> model1In2_;
    //- Model for dispersed phase 2 in continuous phase 1
    autoPtr<modelType> model2In1_;
    //- Blending model
    const blendingMethod& blending_;
    //- If true set coefficients and forces to 0 at fixed-flux BCs
    bool correctFixedFluxBCs_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    BlendedInterfacialModel(const BlendedInterfacialModel<modelType>&);
    //- Disallow default bitwise assignment
    void operator=(const BlendedInterfacialModel<modelType>&);
    //- Correct coeff/value on fixed flux boundary conditions
    template<class GeometricField>
    void correctFixedFluxBCs(GeometricField& field) const;
public:
  // Constructors
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
    const modelType& phaseModel(const phaseModel& phase) const;
    //- Return the blended force coefficient
    tmp<volScalarField> K() const;
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
