#ifndef CORE_FIELDS_GEOMETRIC_FIELDS_GEOMETRIC_FIELD_REUSE_FUNCTIONS_HPP_
#define CORE_FIELDS_GEOMETRIC_FIELDS_GEOMETRIC_FIELD_REUSE_FUNCTIONS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


namespace mousse
{

template
<
  class TypeR,
  class Type1,
  template<class> class PatchField,
  class GeoMesh
>
class reuseTmpGeometricField
{
public:

  static tmp<GeometricField<TypeR, PatchField, GeoMesh>> New
  (
    const tmp<GeometricField<Type1, PatchField, GeoMesh>>& tdf1,
    const word& name,
    const dimensionSet& dimensions
  )
  {
    const GeometricField<Type1, PatchField, GeoMesh>& df1 = tdf1();
    return tmp<GeometricField<TypeR, PatchField, GeoMesh>>
    {
      new GeometricField<TypeR, PatchField, GeoMesh>
      {
        IOobject
        {
          name,
          df1.instance(),
          df1.db()
        },
        df1.mesh(),
        dimensions
      }
    };
  }

  static void clear
  (
    const tmp<GeometricField<Type1, PatchField, GeoMesh>>& tdf1
  )
  {
    tdf1.clear();
  }
};


template<class TypeR, template<class> class PatchField, class GeoMesh>
class reuseTmpGeometricField<TypeR, TypeR, PatchField, GeoMesh>
{
public:

  static tmp<GeometricField<TypeR, PatchField, GeoMesh>> New
  (
    const tmp<GeometricField<TypeR, PatchField, GeoMesh>>& tdf1,
    const word& name,
    const dimensionSet& dimensions
  )
  {
    GeometricField<TypeR, PatchField, GeoMesh>& df1 =
      const_cast<GeometricField<TypeR, PatchField, GeoMesh>& >(tdf1());
    if (tdf1.isTmp())
    {
      df1.rename(name);
      df1.dimensions().reset(dimensions);
      return tdf1;
    }
    else
    {
      return tmp<GeometricField<TypeR, PatchField, GeoMesh>>
      {
        new GeometricField<TypeR, PatchField, GeoMesh>
        {
          IOobject
          {
            name,
            df1.instance(),
            df1.db()
          },
          df1.mesh(),
          dimensions
        }
      };
    }
  }

  static void clear
  (
    const tmp<GeometricField<TypeR, PatchField, GeoMesh>>& tdf1
  )
  {
    if (tdf1.isTmp())
    {
      tdf1.ptr();
    }
  }
};


template
<
  class TypeR,
  class Type1,
  class Type12,
  class Type2,
  template<class> class PatchField,
  class GeoMesh
>
class reuseTmpTmpGeometricField
{
public:
  static tmp<GeometricField<TypeR, PatchField, GeoMesh>> New
  (
    const tmp<GeometricField<Type1, PatchField, GeoMesh>>& tdf1,
    const tmp<GeometricField<Type2, PatchField, GeoMesh>>& /*tdf2*/,
    const word& name,
    const dimensionSet& dimensions
  )
  {
    const GeometricField<Type1, PatchField, GeoMesh>& df1 = tdf1();
    return tmp<GeometricField<TypeR, PatchField, GeoMesh>>
    {
      new GeometricField<TypeR, PatchField, GeoMesh>
      {
        IOobject
        {
          name,
          df1.instance(),
          df1.db()
        },
        df1.mesh(),
        dimensions
      }
    };
  }

  static void clear
  (
    const tmp<GeometricField<Type1, PatchField, GeoMesh>>& tdf1,
    const tmp<GeometricField<Type2, PatchField, GeoMesh>>& tdf2
  )
  {
    tdf1.clear();
    tdf2.clear();
  }
};


template
<
  class TypeR,
  class Type1,
  class Type12,
  template<class> class PatchField,
  class GeoMesh
>
class reuseTmpTmpGeometricField
  <TypeR, Type1, Type12, TypeR, PatchField, GeoMesh>
{
public:

  static tmp<GeometricField<TypeR, PatchField, GeoMesh>> New
  (
    const tmp<GeometricField<Type1, PatchField, GeoMesh>>& tdf1,
    const tmp<GeometricField<TypeR, PatchField, GeoMesh>>& tdf2,
    const word& name,
    const dimensionSet& dimensions
  )
  {
    const GeometricField<Type1, PatchField, GeoMesh>& df1 = tdf1();
    GeometricField<TypeR, PatchField, GeoMesh>& df2 =
      const_cast<GeometricField<TypeR, PatchField, GeoMesh>& >(tdf2());
    if (tdf2.isTmp())
    {
      df2.rename(name);
      df2.dimensions().reset(dimensions);
      return tdf2;
    }
    else
    {
      return tmp<GeometricField<TypeR, PatchField, GeoMesh>>
      {
        new GeometricField<TypeR, PatchField, GeoMesh>
        {
          IOobject
          {
            name,
            df1.instance(),
            df1.db()
          },
          df1.mesh(),
          dimensions
        }
      };
    }
  }

  static void clear
  (
    const tmp<GeometricField<Type1, PatchField, GeoMesh>>& tdf1,
    const tmp<GeometricField<TypeR, PatchField, GeoMesh>>& tdf2
  )
  {
    tdf1.clear();
    if (tdf2.isTmp())
    {
      tdf2.ptr();
    }
  }
};


template
<
  class TypeR,
  class Type2,
  template<class> class PatchField,
  class GeoMesh
>
class reuseTmpTmpGeometricField<TypeR, TypeR, TypeR, Type2, PatchField, GeoMesh>
{
public:
  static tmp<GeometricField<TypeR, PatchField, GeoMesh>> New
  (
    const tmp<GeometricField<TypeR, PatchField, GeoMesh>>& tdf1,
    const tmp<GeometricField<Type2, PatchField, GeoMesh>>& /*tdf2*/,
    const word& name,
    const dimensionSet& dimensions
  )
  {
    GeometricField<TypeR, PatchField, GeoMesh>& df1 =
      const_cast<GeometricField<TypeR, PatchField, GeoMesh>& >(tdf1());
    if (tdf1.isTmp())
    {
      df1.rename(name);
      df1.dimensions().reset(dimensions);
      return tdf1;
    }
    else
    {
      return tmp<GeometricField<TypeR, PatchField, GeoMesh>>
      {
        new GeometricField<TypeR, PatchField, GeoMesh>
        {
          IOobject
          {
            name,
            df1.instance(),
            df1.db()
          },
          df1.mesh(),
          dimensions
        }
      };
    }
  }

  static void clear
  (
    const tmp<GeometricField<TypeR, PatchField, GeoMesh>>& tdf1,
    const tmp<GeometricField<Type2, PatchField, GeoMesh>>& tdf2
  )
  {
    if (tdf1.isTmp())
    {
      tdf1.ptr();
    }
    tdf2.clear();
  }
};


template<class TypeR, template<class> class PatchField, class GeoMesh>
class reuseTmpTmpGeometricField<TypeR, TypeR, TypeR, TypeR, PatchField, GeoMesh>
{
public:

  static tmp<GeometricField<TypeR, PatchField, GeoMesh>> New
  (
    const tmp<GeometricField<TypeR, PatchField, GeoMesh>>& tdf1,
    const tmp<GeometricField<TypeR, PatchField, GeoMesh>>& tdf2,
    const word& name,
    const dimensionSet& dimensions
  )
  {
    GeometricField<TypeR, PatchField, GeoMesh>& df1 =
      const_cast<GeometricField<TypeR, PatchField, GeoMesh>& >(tdf1());
    GeometricField<TypeR, PatchField, GeoMesh>& df2 =
      const_cast<GeometricField<TypeR, PatchField, GeoMesh>& >(tdf2());
    if (tdf1.isTmp())
    {
      df1.rename(name);
      df1.dimensions().reset(dimensions);
      return tdf1;
    }
    else if (tdf2.isTmp())
    {
      df2.rename(name);
      df2.dimensions().reset(dimensions);
      return tdf2;
    }
    else
    {
      return tmp<GeometricField<TypeR, PatchField, GeoMesh>>
      {
        new GeometricField<TypeR, PatchField, GeoMesh>
        {
          IOobject
          {
            name,
            df1.instance(),
            df1.db()
          },
          df1.mesh(),
          dimensions
        }
      };
    }
  }

  static void clear
  (
    const tmp<GeometricField<TypeR, PatchField, GeoMesh>>& tdf1,
    const tmp<GeometricField<TypeR, PatchField, GeoMesh>>& tdf2
  )
  {
    if (tdf1.isTmp())
    {
      tdf1.ptr();
      tdf2.clear();
    }
    else if (tdf2.isTmp())
    {
      tdf1.clear();
      tdf2.ptr();
    }
  }
};

}  // namespace mousse

#endif
