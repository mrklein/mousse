// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#define MAKE_BASE_TURBULENCE_MODEL(Alpha, Rho, baseModel, BaseModel, Transport)\
                                                                              \
  namespace mousse {                                                          \
    typedef TurbulenceModel                                                   \
    <                                                                         \
      Alpha,                                                                  \
      Rho,                                                                    \
      baseModel,                                                              \
      Transport                                                               \
    > Transport##baseModel;                                                   \
                                                                              \
    DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                  \
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
    DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(Laminar##Transport##BaseModel, 0);\
                                                                              \
    ADD_TO_RUN_TIME_SELECTION_TABLE                                           \
    (                                                                         \
      Transport##baseModel,                                                   \
      Laminar##Transport##BaseModel,                                          \
      dictionary                                                              \
    );                                                                        \
                                                                              \
                                                                              \
    typedef RASModel<Transport##BaseModel> RAS##Transport##BaseModel;         \
                                                                              \
    DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(RAS##Transport##BaseModel, 0);  \
                                                                              \
    DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                  \
    (RAS##Transport##BaseModel, dictionary);                                  \
                                                                              \
    ADD_TO_RUN_TIME_SELECTION_TABLE                                           \
    (                                                                         \
      Transport##baseModel,                                                   \
      RAS##Transport##BaseModel,                                              \
      dictionary                                                              \
    );                                                                        \
                                                                              \
                                                                              \
    typedef LESModel<Transport##BaseModel> LES##Transport##BaseModel;         \
                                                                              \
    DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(LES##Transport##BaseModel, 0);  \
                                                                              \
    DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                  \
    (LES##Transport##BaseModel, dictionary);                                  \
                                                                              \
    ADD_TO_RUN_TIME_SELECTION_TABLE                                           \
    (                                                                         \
      Transport##baseModel,                                                   \
      LES##Transport##BaseModel,                                              \
      dictionary                                                              \
    );                                                                        \
  }


#define MAKE_TEMPLATED_TURBULENCE_MODEL(BaseModel, SType, Type)               \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
    (mousse::SType##Models::Type<mousse::BaseModel>, 0);                      \
                                                                              \
  namespace mousse {                                                          \
  namespace SType##Models {                                                   \
    typedef Type<BaseModel> Type##SType##BaseModel;                           \
                                                                              \
    ADD_TO_RUN_TIME_SELECTION_TABLE                                           \
    (                                                                         \
      SType##BaseModel,                                                       \
      Type##SType##BaseModel,                                                 \
      dictionary                                                              \
    );                                                                        \
  }                                                                           \
  }


#define MAKE_TURBULENCE_MODEL(BaseModel, SType, Type)                         \
  namespace mousse {                                                          \
  namespace SType##Models {                                                   \
    DEFINE_TYPE_NAME_AND_DEBUG(Type, 0);                                      \
                                                                              \
    ADD_TO_RUN_TIME_SELECTION_TABLE                                           \
    (                                                                         \
      SType##BaseModel,                                                       \
      Type,                                                                   \
      dictionary                                                              \
    );                                                                        \
  }                                                                           \
  }
