#pragma once
#include "sfbxObject.h"

namespace sfbx {

// NodeAttribute and its subclasses:
//  (NullAttribute, RootAttribute, LimbNodeAttribute, LightAttribute, CameraAttribute)

class NodeAttribute : public Object
{
using super = Object;
public:
    ObjectClass getClass() const override;
};


class NullAttribute : public NodeAttribute
{
using super = NodeAttribute;
public:
    ObjectSubClass getSubClass() const override;
    void exportFBXObjects() override;
};


class RootAttribute : public NodeAttribute
{
using super = NodeAttribute;
public:
    ObjectSubClass getSubClass() const override;
    void exportFBXObjects() override;
};


class LimbNodeAttribute : public NodeAttribute
{
using super = NodeAttribute;
public:
    ObjectSubClass getSubClass() const override;
    void exportFBXObjects() override;
};


class LightAttribute : public NodeAttribute
{
using super = NodeAttribute;
public:
    ObjectSubClass getSubClass() const override;
    void importFBXObjects() override;
    void exportFBXObjects() override;
};


class CameraAttribute : public NodeAttribute
{
using super = NodeAttribute;
public:
    ObjectSubClass getSubClass() const override;
    void importFBXObjects() override;
    void exportFBXObjects() override;
};



// Model and its subclasses:
//  (Null, Root, LimbNode, Light, Camera)

class Model : public Object
{
using super = Object;
public:
    ObjectClass getClass() const override;
    void addChild(Object* v) override;
    void eraseChild(Object* v) override;

    Model* getParentModel() const;

    bool getVisibility() const;
    RotationOrder getRotationOrder() const;
    float3 getPosition() const;
    float3 getPreRotation() const;
    float3 getRotation() const;
    float3 getPostRotation() const;
    float3 getScale() const;
    float4x4 getLocalMatrix() const;
    float4x4 getGlobalMatrix() const;

    void setVisibility(bool v);
    void setRotationOrder(RotationOrder v);
    void setPosition(float3 v);
    void setPreRotation(float3 v);
    void setRotation(float3 v);
    void setPostRotation(float3 v);
    void setScale(float3 v);

protected:
    void importFBXObjects() override;
    void exportFBXObjects() override;
    void addParent(Object* v) override;
    void eraseParent(Object* v) override;
    void propagateDirty();
    void updateMatrices() const;

    Model* m_parent_model{};
    std::vector<Model*> m_child_models;

    bool m_visibility = true;
    RotationOrder m_rotation_order = RotationOrder::XYZ;
    float3 m_position{};
    float3 m_pre_rotation{};
    float3 m_rotation{};
    float3 m_post_rotation{};
    float3 m_scale = float3::one();

    mutable bool m_matrix_dirty = true;
    mutable float4x4 m_matrix_local = float4x4::identity();
    mutable float4x4 m_matrix_global = float4x4::identity();
};


// Null, Root, and LimbNode represent joints.
// usually Null or Root is the root of joints, but a joint structure with LimbNode root also seems to be valid.

class Null : public Model
{
using super = Model;
public:
    ObjectSubClass getSubClass() const override;
    void addChild(Object* v) override;
    void eraseChild(Object* v) override;

protected:
    void exportFBXObjects() override;

    NullAttribute* m_attr{};
};

class Root : public Model
{
using super = Model;
public:
    ObjectSubClass getSubClass() const override;
    void addChild(Object* v) override;
    void eraseChild(Object* v) override;

protected:
    void exportFBXObjects() override;

    RootAttribute* m_attr{};
};

class LimbNode : public Model
{
using super = Model;
public:
    ObjectSubClass getSubClass() const override;
    void addChild(Object* v) override;
    void eraseChild(Object* v) override;

protected:
    void exportFBXObjects() override;

    LimbNodeAttribute* m_attr{};
};


// Mesh represents polygon mesh objects, but it holds just transform data.
// geometry data is stored in GeomMesh. (see sfbxGeometry.h)

class Mesh : public Model
{
using super = Model;
public:
    ObjectSubClass getSubClass() const override;
    void addChild(Object* v) override;
    void eraseChild(Object* v) override;

    GeomMesh* getGeometry();
    span<Material*> getMaterials() const;

protected:
    void importFBXObjects() override;

    GeomMesh* m_geom{};
    std::vector<Material*> m_materials;
};


enum class LightType
{
    Point,
    Directional,
    Spot,
};

class Light : public Model
{
using super = Model;
friend class LightAttribute;
public:
    ObjectSubClass getSubClass() const override;
    void addChild(Object* v) override;
    void eraseChild(Object* v) override;

    LightType getLightType() const;
    float3 getColor() const;
    float getIntensity() const;
    float getInnerAngle() const;
    float getOuterAngle() const;

    void setLightType(LightType v);
    void setColor(float3 v);
    void setIntensity(float v);
    void setInnerAngle(float v);
    void setOuterAngle(float v);

protected:
    void importFBXObjects() override;
    void exportFBXObjects() override;

    LightAttribute* m_attr{};
    LightType m_light_type = LightType::Point;
    float3 m_color = float3::one();
    float m_intensity = 1.0f;
    float m_inner_angle = 45.0f; // in degree, for spot light
    float m_outer_angle = 45.0f; // in degree, for spot light
};


enum class CameraType
{
    Perspective,
    Orthographic,
};

class Camera : public Model
{
using super = Model;
friend class CameraAttribute;
public:
    ObjectSubClass getSubClass() const override;
    void addChild(Object* v) override;
    void eraseChild(Object* v) override;

    CameraType getCameraType() const;
    float3 getUpVector() const;
    float getFocalLength() const;   // in mm
    float2 getApertureSize() const; // in mm
    float2 getLensShift() const;    // in mm
    float2 getAspectSize() const;
    float2 getFildOfView() const;
    float getNearPlane() const;
    float getFarPlane() const;

    // there is no setFildOfView() because fov is computed by aperture and focal length.
    // focal length can be computed by compute_focal_length() in sfbxMath.h

    void setCameraType(CameraType v);
    void setUpVector(float3 v);
    void setFocalLength(float v);   // in mm
    void setApetrueSize(float2 v);  // in mm
    void setLensShift(float2 v);    // in mm
    void setAspectSize(float2 v);
    void setNearPlane(float v);
    void setFarPlane(float v);

protected:
    void importFBXObjects() override;
    void exportFBXObjects() override;

    CameraAttribute* m_attr{};
    CameraType m_camera_type = CameraType::Perspective;
    float3 m_up_vector{ 0.0f, 1.0f, 0.0f };
    float m_focal_length = 50.0f; // in mm
    float2 m_aperture{ 36.0f, 24.0f }; // in mm
    float2 m_lens_shift{}; // in mm
    float2 m_aspect{ 1920.0f, 1080.0f };
    float m_near_plane = 0.1f;
    float m_far_plane = 1000.0f;
};


} // namespace sfbx
