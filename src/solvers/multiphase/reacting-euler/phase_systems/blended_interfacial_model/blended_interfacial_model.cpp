// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "blended_interfacial_model.hpp"
#include "fixed_value_fvs_patch_fields.hpp"
#include "surface_interpolate.hpp"
// Private Member Functions 
template<class ModelType>
template<class GeometricField>
void mousse::BlendedInterfacialModel<ModelType>::correctFixedFluxBCs
(
  GeometricField& field
) const
{
  FOR_ALL(phase1_.phi()->boundaryField(), patchI)
  {
    if
    (
      isA<fixedValueFvsPatchScalarField>
      (
        phase1_.phi()->boundaryField()[patchI]
      )
    )
    {
      field.boundaryField()[patchI]
       = pTraits<typename GeometricField::value_type>::zero;
    }
  }
}
// Constructors 
template<class ModelType>
mousse::BlendedInterfacialModel<ModelType>::BlendedInterfacialModel
(
  const phaseModel& phase1,
  const phaseModel& phase2,
  const blendingMethod& blending,
  autoPtr<ModelType> model,
  autoPtr<ModelType> model1In2,
  autoPtr<ModelType> model2In1,
  const bool correctFixedFluxBCs
)
:
  phase1_{phase1},
  phase2_{phase2},
  blending_{blending},
  model_{model},
  model1In2_{model1In2},
  model2In1_{model2In1},
  correctFixedFluxBCs_{correctFixedFluxBCs}
{}
template<class ModelType>
mousse::BlendedInterfacialModel<ModelType>::BlendedInterfacialModel
(
  const phasePair::dictTable& modelTable,
  const blendingMethod& blending,
  const phasePair& pair,
  const orderedPhasePair& pair1In2,
  const orderedPhasePair& pair2In1,
  const bool correctFixedFluxBCs
)
:
  phase1_{pair.phase1()},
  phase2_{pair.phase2()},
  blending_{blending},
  correctFixedFluxBCs_{correctFixedFluxBCs}
{
  if (modelTable.found(pair))
  {
    model_.set
    (
      ModelType::New
      (
        modelTable[pair],
        pair
      ).ptr()
    );
  }
  if (modelTable.found(pair1In2))
  {
    model1In2_.set
    (
      ModelType::New
      (
        modelTable[pair1In2],
        pair1In2
      ).ptr()
    );
  }
  if (modelTable.found(pair2In1))
  {
    model2In1_.set
    (
      ModelType::New
      (
        modelTable[pair2In1],
        pair2In1
      ).ptr()
    );
  }
}
// Destructor 
template<class ModelType>
mousse::BlendedInterfacialModel<ModelType>::~BlendedInterfacialModel()
{}
// Member Functions 
template<class ModelType>
bool mousse::BlendedInterfacialModel<ModelType>::hasModel
(
  const class phaseModel& phase
) const
{
  return
   &phase == &(phase1_)
   ? model1In2_.valid()
   : model2In1_.valid();
}
template<class ModelType>
const ModelType& mousse::BlendedInterfacialModel<ModelType>::model
(
  const class phaseModel& phase
) const
{
  return &phase == &(phase1_) ? model1In2_ : model2In1_;
}
template<class ModelType>
mousse::tmp<mousse::volScalarField>
mousse::BlendedInterfacialModel<ModelType>::K() const
{
  tmp<volScalarField> f1, f2;
  if (model_.valid() || model1In2_.valid())
  {
    f1 = blending_.f1(phase1_, phase2_);
  }
  if (model_.valid() || model2In1_.valid())
  {
    f2 = blending_.f2(phase1_, phase2_);
  }
  tmp<volScalarField> x
  {
    new volScalarField
    {
      {
        ModelType::typeName + ":K",
        phase1_.mesh().time().timeName(),
        phase1_.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      phase1_.mesh(),
      {"zero", ModelType::dimK, 0}
    }
  };
  if (model_.valid())
  {
    x() += model_->K()*(scalar(1) - f1() - f2());
  }
  if (model1In2_.valid())
  {
    x() += model1In2_->K()*f1;
  }
  if (model2In1_.valid())
  {
    x() += model2In1_->K()*f2;
  }
  if (correctFixedFluxBCs_
      && (model_.valid() || model1In2_.valid() || model2In1_.valid()))
  {
    correctFixedFluxBCs(x());
  }
  return x;
}
template<class ModelType>
mousse::tmp<mousse::volScalarField>
mousse::BlendedInterfacialModel<ModelType>::K(const scalar residualAlpha) const
{
  tmp<volScalarField> f1, f2;
  if (model_.valid() || model1In2_.valid())
  {
    f1 = blending_.f1(phase1_, phase2_);
  }
  if (model_.valid() || model2In1_.valid())
  {
    f2 = blending_.f2(phase1_, phase2_);
  }
  tmp<volScalarField> x
  {
    new volScalarField
    {
      {
        ModelType::typeName + ":K",
        phase1_.mesh().time().timeName(),
        phase1_.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      phase1_.mesh(),
      {"zero", ModelType::dimK, 0}
    }
  };
  if (model_.valid())
  {
    x() += model_->K(residualAlpha)*(scalar(1) - f1() - f2());
  }
  if (model1In2_.valid())
  {
    x() += model1In2_->K(residualAlpha)*f1;
  }
  if (model2In1_.valid())
  {
    x() += model2In1_->K(residualAlpha)*f2;
  }
  if (correctFixedFluxBCs_
      && (model_.valid() || model1In2_.valid() || model2In1_.valid()))
  {
    correctFixedFluxBCs(x());
  }
  return x;
}
template<class ModelType>
mousse::tmp<mousse::surfaceScalarField>
mousse::BlendedInterfacialModel<ModelType>::Kf() const
{
  tmp<surfaceScalarField> f1, f2;
  if (model_.valid() || model1In2_.valid())
  {
    f1 = fvc::interpolate
    (
      blending_.f1(phase1_, phase2_)
    );
  }
  if (model_.valid() || model2In1_.valid())
  {
    f2 = fvc::interpolate
    (
      blending_.f2(phase1_, phase2_)
    );
  }
  tmp<surfaceScalarField> x
  {
    new surfaceScalarField
    {
      {
        ModelType::typeName + ":Kf",
        phase1_.mesh().time().timeName(),
        phase1_.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      phase1_.mesh(),
      {"zero", ModelType::dimK, 0}
    }
  };
  if (model_.valid())
  {
    x() += model_->Kf()*(scalar(1) - f1() - f2());
  }
  if (model1In2_.valid())
  {
    x() += model1In2_->Kf()*f1;
  }
  if (model2In1_.valid())
  {
    x() += model2In1_->Kf()*f2;
  }
  if (correctFixedFluxBCs_
      && (model_.valid() || model1In2_.valid() || model2In1_.valid()))
  {
    correctFixedFluxBCs(x());
  }
  return x;
}
template<class ModelType>
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::BlendedInterfacialModel<ModelType>::F() const
{
  tmp<volScalarField> f1, f2;
  if (model_.valid() || model1In2_.valid())
  {
    f1 = blending_.f1(phase1_, phase2_);
  }
  if (model_.valid() || model2In1_.valid())
  {
    f2 = blending_.f2(phase1_, phase2_);
  }
  tmp<GeometricField<Type, fvPatchField, volMesh> > x
  {
    new GeometricField<Type, fvPatchField, volMesh>
    {
      {
        ModelType::typeName + ":F",
        phase1_.mesh().time().timeName(),
        phase1_.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      phase1_.mesh(),
      dimensioned<Type>{"zero", ModelType::dimF, pTraits<Type>::zero}
    }
  };
  if (model_.valid())
  {
    x() += model_->F()*(scalar(1) - f1() - f2());
  }
  if (model1In2_.valid())
  {
    x() += model1In2_->F()*f1;
  }
  if (model2In1_.valid())
  {
    x() -= model2In1_->F()*f2; // note : subtraction
  }
  if (correctFixedFluxBCs_
      && (model_.valid() || model1In2_.valid() || model2In1_.valid()))
  {
    correctFixedFluxBCs(x());
  }
  return x;
}
template<class ModelType>
mousse::tmp<mousse::surfaceScalarField>
mousse::BlendedInterfacialModel<ModelType>::Ff() const
{
  tmp<surfaceScalarField> f1, f2;
  if (model_.valid() || model1In2_.valid())
  {
    f1 = fvc::interpolate
    (
      blending_.f1(phase1_, phase2_)
    );
  }
  if (model_.valid() || model2In1_.valid())
  {
    f2 = fvc::interpolate
    (
      blending_.f2(phase1_, phase2_)
    );
  }
  tmp<surfaceScalarField> x
  {
    new surfaceScalarField
    {
      {
        ModelType::typeName + ":Ff",
        phase1_.mesh().time().timeName(),
        phase1_.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      phase1_.mesh(),
      {"zero", ModelType::dimF*dimArea, 0}
    }
  };
  if (model_.valid())
  {
    x() += model_->Ff()*(scalar(1) - f1() - f2());
  }
  if (model1In2_.valid())
  {
    x() += model1In2_->Ff()*f1;
  }
  if (model2In1_.valid())
  {
    x() -= model2In1_->Ff()*f2; // note : subtraction
  }
  if (correctFixedFluxBCs_
      && (model_.valid() || model1In2_.valid() || model2In1_.valid()))
  {
    correctFixedFluxBCs(x());
  }
  return x;
}
template<class ModelType>
mousse::tmp<mousse::volScalarField>
mousse::BlendedInterfacialModel<ModelType>::D() const
{
  tmp<volScalarField> f1, f2;
  if (model_.valid() || model1In2_.valid())
  {
    f1 = blending_.f1(phase1_, phase2_);
  }
  if (model_.valid() || model2In1_.valid())
  {
    f2 = blending_.f2(phase1_, phase2_);
  }
  tmp<volScalarField> x
  {
    new volScalarField
    {
      {
        ModelType::typeName + ":D",
        phase1_.mesh().time().timeName(),
        phase1_.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      phase1_.mesh(),
      {"zero", ModelType::dimD, 0}
    }
  };
  if (model_.valid())
  {
    x() += model_->D()*(scalar(1) - f1() - f2());
  }
  if (model1In2_.valid())
  {
    x() += model1In2_->D()*f1;
  }
  if (model2In1_.valid())
  {
    x() += model2In1_->D()*f2;
  }
  if (correctFixedFluxBCs_
      && (model_.valid() || model1In2_.valid() || model2In1_.valid()))
  {
    correctFixedFluxBCs(x());
  }
  return x;
}
