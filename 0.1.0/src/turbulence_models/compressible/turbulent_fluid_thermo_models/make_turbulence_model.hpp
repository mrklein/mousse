// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "eddy_diffusivity.hpp"
#define makeBaseTurbulenceModel(                                               \
  Alpha, Rho, baseModel, BaseModel, TDModel, Transport)                      \
                                       \
  namespace mousse                                                             \
  {                                                                          \
    typedef TurbulenceModel                                                \
    <                                                                      \
      Alpha,                                                             \
      Rho,                                                               \
      baseModel,                                                         \
      Transport                                                          \
    > Transport##baseModel;                                                \
                                       \
    defineTemplateRunTimeSelectionTable                                    \
    (                                                                      \
      Transport##baseModel,                                              \
      dictionary                                                         \
    );                                                                     \
                                       \
    typedef TDModel<BaseModel<Transport> >                                 \
      Transport##BaseModel;                                              \
                                       \
                                       \
    typedef laminar<Transport##BaseModel> Laminar##Transport##BaseModel;   \
                                       \
    defineNamedTemplateTypeNameAndDebug(Laminar##Transport##BaseModel, 0); \
                                       \
    addToRunTimeSelectionTable                                             \
    (                                                                      \
      Transport##baseModel,                                              \
      Laminar##Transport##BaseModel,                                     \
      dictionary                                                         \
    );                                                                     \
                                       \
                                       \
    typedef RASModel<EddyDiffusivity<Transport##BaseModel> >               \
      RAS##Transport##BaseModel;                                         \
                                       \
    defineNamedTemplateTypeNameAndDebug(RAS##Transport##BaseModel, 0);     \
                                       \
    defineTemplateRunTimeSelectionTable                                    \
    (RAS##Transport##BaseModel, dictionary);                               \
                                       \
    addToRunTimeSelectionTable                                             \
    (                                                                      \
      Transport##baseModel,                                              \
      RAS##Transport##BaseModel,                                         \
      dictionary                                                         \
    );                                                                     \
                                       \
                                       \
    typedef LESModel<EddyDiffusivity<Transport##BaseModel> >               \
      LES##Transport##BaseModel;                                         \
                                       \
    defineNamedTemplateTypeNameAndDebug(LES##Transport##BaseModel, 0);     \
                                       \
    defineTemplateRunTimeSelectionTable                                    \
    (LES##Transport##BaseModel, dictionary);                               \
                                       \
    addToRunTimeSelectionTable                                             \
    (                                                                      \
      Transport##baseModel,                                              \
      LES##Transport##BaseModel,                                         \
      dictionary                                                         \
    );                                                                     \
  }
#define makeTemplatedTurbulenceModel(BaseModel, SType, Type)                   \
  typedef mousse::SType##Models::Type<mousse::EddyDiffusivity<mousse::BaseModel> > \
    Type##SType##BaseModel;                                                \
  defineNamedTemplateTypeNameAndDebug(Type##SType##BaseModel, 0);            \
                                       \
  namespace mousse                                                             \
  {                                                                          \
    namespace SType##Models                                                \
    {                                                                      \
      typedef Type<EddyDiffusivity<BaseModel> > Type##SType##BaseModel;  \
                                       \
      addToRunTimeSelectionTable                                         \
      (                                                                  \
        SType##BaseModel,                                              \
        Type##SType##BaseModel,                                        \
        dictionary                                                     \
      );                                                                 \
    }                                                                      \
  }
#define makeTurbulenceModel(BaseModel, SType, Type)                            \
  namespace mousse                                                             \
  {                                                                          \
    namespace SType##Models                                                \
    {                                                                      \
      defineTypeNameAndDebug(Type, 0);                                   \
                                       \
      addToRunTimeSelectionTable                                         \
      (                                                                  \
        SType##BaseModel,                                              \
        Type,                                                          \
        dictionary                                                     \
      );                                                                 \
    }                                                                      \
  }
