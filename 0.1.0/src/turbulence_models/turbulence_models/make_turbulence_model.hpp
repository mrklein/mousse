// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#define makeBaseTurbulenceModel(Alpha, Rho, baseModel, BaseModel, Transport)  \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    typedef TurbulenceModel                                                   \
    <                                                                         \
      Alpha,                                                                  \
      Rho,                                                                    \
      baseModel,                                                              \
      Transport                                                               \
    > Transport##baseModel;                                                   \
                                                                              \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
      Transport##baseModel,                                                   \
      dictionary                                                              \
    );                                                                        \
                                                                              \
    typedef BaseModel<Transport> Transport##BaseModel;                        \
                                                                              \
                                                                              \
    typedef laminar<Transport##BaseModel> Laminar##Transport##BaseModel;      \
                                                                              \
    defineNamedTemplateTypeNameAndDebug(Laminar##Transport##BaseModel, 0);    \
                                                                              \
    addToRunTimeSelectionTable                                                \
    (                                                                         \
      Transport##baseModel,                                                   \
      Laminar##Transport##BaseModel,                                          \
      dictionary                                                              \
    );                                                                        \
                                                                              \
                                                                              \
    typedef RASModel<Transport##BaseModel> RAS##Transport##BaseModel;         \
                                                                              \
    defineNamedTemplateTypeNameAndDebug(RAS##Transport##BaseModel, 0);        \
                                                                              \
    defineTemplateRunTimeSelectionTable                                       \
    (RAS##Transport##BaseModel, dictionary);                                  \
                                                                              \
    addToRunTimeSelectionTable                                                \
    (                                                                         \
      Transport##baseModel,                                                   \
      RAS##Transport##BaseModel,                                              \
      dictionary                                                              \
    );                                                                        \
                                                                              \
                                                                              \
    typedef LESModel<Transport##BaseModel> LES##Transport##BaseModel;         \
                                                                              \
    defineNamedTemplateTypeNameAndDebug(LES##Transport##BaseModel, 0);        \
                                                                              \
    defineTemplateRunTimeSelectionTable                                       \
    (LES##Transport##BaseModel, dictionary);                                  \
                                                                              \
    addToRunTimeSelectionTable                                                \
    (                                                                         \
      Transport##baseModel,                                                   \
      LES##Transport##BaseModel,                                              \
      dictionary                                                              \
    );                                                                        \
  }

#define makeTemplatedTurbulenceModel(BaseModel, SType, Type)                  \
  defineNamedTemplateTypeNameAndDebug                                         \
    (mousse::SType##Models::Type<mousse::BaseModel>, 0);                      \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    namespace SType##Models                                                   \
    {                                                                         \
      typedef Type<BaseModel> Type##SType##BaseModel;                         \
                                                                              \
      addToRunTimeSelectionTable                                              \
      (                                                                       \
        SType##BaseModel,                                                     \
        Type##SType##BaseModel,                                               \
        dictionary                                                            \
      );                                                                      \
    }                                                                         \
  }

#define makeTurbulenceModel(BaseModel, SType, Type)                           \
  namespace mousse                                                            \
  {                                                                           \
    namespace SType##Models                                                   \
    {                                                                         \
      defineTypeNameAndDebug(Type, 0);                                        \
                                                                              \
      addToRunTimeSelectionTable                                              \
      (                                                                       \
        SType##BaseModel,                                                     \
        Type,                                                                 \
        dictionary                                                            \
      );                                                                      \
    }                                                                         \
  }
