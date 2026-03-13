// Copyright (c) 2008-2026 the Urho 4.0 project
// Copyright (c) 2026 Nitro Boost System extension by Shano
// License: MIT

#pragma once

#include "../Scene/LogicComponent.h"
#include "../Physics/PhysicsUtils.h"
#include "../Physics/RigidBody.h"

namespace Urho3D
{
struct RaycastVehicleData;

class URHO3D_API RaycastVehicle : public LogicComponent
{
    URHO3D_OBJECT(RaycastVehicle, LogicComponent);

public:
    /// Construct.
    explicit RaycastVehicle(Urho3D::Context* context);
    /// Destruct.
    ~RaycastVehicle() override;

    /// Register object factory and attributes.
    /// @nobind
    static void RegisterObject(Context* context);

    /// Handle enabled/disabled state change.
    void OnSetEnabled() override;

    /// Perform post-load after deserialization. Acquire the components from the scene nodes.
    void ApplyAttributes() override;

    /// Add a wheel. All parameters are relative to RigidBody / node.
    void AddWheel(Node* wheelNode, Vector3 wheelDirection, Vector3 wheelAxle, float restLength, float wheelRadius, bool frontWheel);
    /// Reset all suspension.
    void ResetSuspension();
    /// Update transform for particular wheel.
    void UpdateWheelTransform(int wheel, bool interpolated);
    /// Set steering value of particular wheel.
    void SetSteeringValue(int wheel, float steeringValue);
    /// Set suspension stiffness for particular wheel.
    void SetWheelSuspensionStiffness(int wheel, float stiffness);
    /// Set wheel max suspension force. Good results are often obtained by a value that is about 3x to 4x the vehicle weight.
    void SetWheelMaxSuspensionForce(int wheel, float force);
    /// Set wheel damping relaxation.
    void SetWheelDampingRelaxation(int wheel, float damping);
    /// Set wheel damping compression.
    void SetWheelDampingCompression(int wheel, float compression);
    /// Set wheel friction slip.
    void SetWheelFrictionSlip(int wheel, float slip);
    /// Set wheel roll influence.
    void SetWheelRollInfluence(int wheel, float rollInfluence);
    /// Set engine force for the wheel.
    void SetEngineForce(int wheel, float force);
    /// Set hand brake (wheel rotation blocking force).
    void SetBrake(int wheel, float force);
    /// Set wheel radius.
    void SetWheelRadius(int wheel, float wheelRadius);
    /// Sets node initial positions.
    void ResetWheels();
    /// Set sliding factor 0 <= x <= 1. The less the value, more sliding.
    void SetWheelSkidInfo(int wheel, float factor);
    /// True if wheel touches ground (raycast hits something).
    bool WheelIsGrounded(int wheel) const;
    /// Set maximum suspension travel value.
    void SetMaxSuspensionTravel(int wheel, float maxSuspensionTravel);
    /// Set wheel direction vector.
    void SetWheelDirection(int wheel, Vector3 direction);
    /// Set wheel axle vector.
    void SetWheelAxle(int wheel, Vector3 axle);
    /// Set side speed which is considered sliding.
    /// @property
    void SetMaxSideSlipSpeed(float speed);
    /// Set cumulative skid info.
    void SetWheelSkidInfoCumulative(int wheel, float skid);
    /// Set revolution per minute value for when wheel doesn't touch ground. If set to 0 (or not set), calculated from engine force (probably not what you want).
    /// @property
    void SetInAirRPM(float rpm);
    /// Set the coordinate system. The default is (0, 1, 2).
    /// @property
    void SetCoordinateSystem(const IntVector3& coordinateSystem = RIGHT_FORWARD_UP);
    /// Init the vehicle component after creation.
    void Init();
    /// Perform fixed step pre-update.
    void FixedUpdate(float timeStep) override;
    /// Perform fixed step post-update.
    void FixedPostUpdate(float timeStep) override;
    /// Perform variable step post-update.
    void PostUpdate(float timeStep) override;

    /// NITRO BOOST SYSTEM METHODS
    /// ============================

    /// Activate or deactivate nitro boost. Boost can only be activated when cooldown has expired.
    /// @param active True to activate nitro, false to deactivate.
    /// @return True if the state change was successful, false if nitro is on cooldown.
    /// @doc Activating nitro applies a multiplier to engine force and triggers visual/audio effects.
    bool SetNitroActive(bool active);

    /// Check if nitro boost is currently active.
    /// @return True if nitro is active and providing power boost.
    bool IsNitroActive() const { return nitroActive_; }

    /// Check if nitro boost can be activated (cooldown has expired and not already active).
    /// @return True if boost is available for activation.
    bool CanActivateNitro() const { return (nitroCooldownTimer_ <= 0.0f) && !nitroActive_; }

    /// Set the duration of nitro boost effect in seconds.
    /// @param duration Duration in seconds. Default is 3.0 seconds.
    void SetNitroDuration(float duration) { nitroDuration_ = Max(0.1f, duration); }

    /// Get the configured duration of nitro boost effect.
    /// @return Duration in seconds.
    float GetNitroDuration() const { return nitroDuration_; }

    /// Set the cooldown period before nitro can be used again in seconds.
    /// @param cooldown Cooldown in seconds. Default is 5.0 seconds.
    void SetNitroCooldown(float cooldown) { nitroCooldown_ = Max(0.1f, cooldown); }

    /// Get the configured cooldown period for nitro.
    /// @return Cooldown in seconds.
    float GetNitroCooldown() const { return nitroCooldown_; }

    /// Set the engine force multiplier applied during nitro boost.
    /// @param multiplier Multiplier value (e.g., 2.5 for 2.5x power). Must be >= 1.0.
    /// @doc This multiplier is gradually applied based on the startup curve, not applied instantly.
    void SetNitroMultiplier(float multiplier) { nitroMultiplier_ = Max(1.0f, multiplier); }

    /// Get the configured engine force multiplier.
    /// @return Multiplier value.
    float GetNitroMultiplier() const { return nitroMultiplier_; }

    /// Set the time for nitro to reach full power from activation.
    /// @param time Time in seconds. Default is 0.3 seconds. Lower values mean snappier response.
    /// @doc Prevents instant power spike; smooths acceleration curve for realism.
    void SetNitroStartupTime(float time) { nitroStartupTime_ = Max(0.01f, time); }

    /// Get the configured startup time for nitro power curve.
    /// @return Startup time in seconds.
    float GetNitroStartupTime() const { return nitroStartupTime_; }

    /// Get the remaining time on current nitro boost in seconds.
    /// @return Remaining boost time. 0.0 if boost is inactive.
    float GetNitroTimer() const { return nitroActive_ ? nitroTimer_ : 0.0f; }

    /// Get the remaining cooldown time before nitro can be activated again in seconds.
    /// @return Remaining cooldown time. 0.0 if nitro is available.
    float GetNitroCooldownTimer() const { return Max(0.0f, nitroCooldownTimer_); }

    /// Get the current power curve factor (0.0 to 1.0) during nitro boost.
    /// @return Power curve value. Starts at 0.0 and smoothly transitions to 1.0 based on startup time.
    /// @doc Used for smooth power application and visual feedback. 0.0 = cold start, 1.0 = full power.
    float GetNitroPowerCurve() const { return nitroPowerCurve_; }

    /// Get the next available time when nitro can be re-activated (UTC seconds).
    /// @return World time when nitro will be available again.
    float GetNitroNextAvailableTime() const;

    /// Reset nitro system to initial state (cooldown expired, boost inactive).
    /// @doc Useful for respawning or resetting vehicle state.
    void ResetNitroSystem();

    /// Get wheel position relative to RigidBody.
    Vector3 GetWheelPosition(int wheel);
    /// Get wheel rotation relative to RigidBody.
    Quaternion GetWheelRotation(int wheel);
    /// Get wheel connection point relative to RigidBody.
    Vector3 GetWheelConnectionPoint(int wheel) const;
    /// Get number of attached wheels.
    /// @property
    int GetNumWheels() const;
    /// Get node of the wheel.
    Node* GetWheelNode(int wheel) const;
    /// Get steering value of particular wheel.
    float GetSteeringValue(int wheel) const;
    /// Get suspension stiffness for particular wheel.
    float GetWheelSuspensionStiffness(int wheel) const;
    /// Get wheel max suspension force.
    float GetWheelMaxSuspensionForce(int wheel) const;
    /// Get wheel damping relaxation.
    float GetWheelDampingRelaxation(int wheel) const;
    /// Get wheel damping compression.
    float GetWheelDampingCompression(int wheel) const;
    /// Get wheel friction slip.
    float GetWheelFrictionSlip(int wheel) const;
    /// Get wheel roll influence.
    float GetWheelRollInfluence(int wheel) const;
    /// Get engine force for the wheel.
    float GetEngineForce(int wheel) const;
    /// Get hand brake value.
    float GetBrake(int wheel) const;
    /// Get wheel radius.
    float GetWheelRadius(int wheel) const;
    /// Get wheel rest length.
    void SetWheelRestLength(int wheel, float length);
    /// Get wheel rest length.
    float GetWheelRestLength(int wheel) const;
    /// Get maximum suspension travel value.
    float GetMaxSuspensionTravel(int wheel);
    /// Get wheel axle vector.
    Vector3 GetWheelAxle(int wheel) const;
    /// Get wheel slide speed.
    float GetWheelSideSlipSpeed(int wheel) const;
    /// Get side speed which is considered sliding.
    /// @property
    float GetMaxSideSlipSpeed() const;
    /// Sliding factor 0 <= x <= 1.
    float GetWheelSkidInfo(int wheel) const;
    /// Get wheel direction vector.
    Vector3 GetWheelDirection(int wheel) const;
    /// Get cumulative skid info.
    float GetWheelSkidInfoCumulative(int wheel) const;
    /// True if front wheel, otherwise false.
    bool IsFrontWheel(int wheel) const;
    /// Get wheel contact position.
    Vector3 GetContactPosition(int wheel) const;
    /// Get contact normal.
    Vector3 GetContactNormal(int wheel) const;
    /// Get revolution per minute value for when wheel doesn't touch ground.
    /// @property
    float GetInAirRPM() const;
    /// Get the coordinate system.
    /// @property
    IntVector3 GetCoordinateSystem() const { return coordinateSystem_; }

    /// Get wheel data attribute for serialization.
    VariantVector GetWheelDataAttr() const;
    /// Set wheel data attribute during loading.
    void SetWheelDataAttr(const VariantVector& value);

    /// (0, 1, 2) coordinate system (default).
    static const IntVector3 RIGHT_UP_FORWARD;
    /// (0, 2, 1) coordinate system.
    static const IntVector3 RIGHT_FORWARD_UP;
    /// (1, 2, 0) coordinate system.
    static const IntVector3 UP_FORWARD_RIGHT;
    /// (1, 0, 2) coordinate system.
    static const IntVector3 UP_RIGHT_FORWARD;
    /// (2, 0, 1) coordinate system.
    static const IntVector3 FORWARD_RIGHT_UP;
    /// (2, 1, 0) coordinate system.
    static const IntVector3 FORWARD_UP_RIGHT;

private:
    /// If the RigidBody should be activated.
    bool activate_;
    /// Hull RigidBody.
    WeakPtr<RigidBody> hullBody_;
    /// Opaque Bullet data hidden from public.
    RaycastVehicleData* vehicleData_;
    /// Coordinate system.
    IntVector3 coordinateSystem_;
    /// Nodes of all wheels.
    Vector<Node*> wheelNodes_;
    /// All wheels original rotations. These are applied in addition to wheel rotations by btRaycastVehicle.
    Vector<Quaternion> origRotation_;
    /// Revolutions per minute value for in-air motor wheels. FIXME: set this one per wheel.
    float inAirRPM_;
    /// Per-wheel extra settings.
    Vector<float> skidInfoCumulative_;
    /// Wheel side movement speed.
    Vector<float> wheelSideSlipSpeed_;
    /// Side slip speed threshold.
    float maxSideSlipSpeed_;
    /// Loaded data temporarily wait here for ApplyAttributes to come pick them up.
    VariantVector loadedWheelData_;

    // NITRO BOOST SYSTEM MEMBERS
    // ==========================

    /// Flag indicating if nitro boost is currently active.
    bool nitroActive_;
    /// Remaining time for current nitro boost in seconds.
    float nitroTimer_;
    /// Remaining cooldown time before nitro can be re-activated in seconds.
    /// When this reaches 0, nitro becomes available again.
    float nitroCooldownTimer_;
    /// Total duration of nitro boost effect in seconds (default: 3.0).
    float nitroDuration_;
    /// Cooldown period after boost expires before can activate again in seconds (default: 5.0).
    float nitroCooldown_;
    /// Engine force multiplier applied during boost (default: 2.5x).
    /// Used to scale SetEngineForce() calls for all wheels proportionally.
    float nitroMultiplier_;
    /// Time for power to smoothly ramp from 0 to full multiplier in seconds (default: 0.3).
    /// Creates gradual acceleration curve to avoid instant power spike.
    float nitroStartupTime_;
    /// Current power curve factor (0.0 to 1.0). Smoothly transitions during startup.
    /// 0.0 = boost just activated, 1.0 = full boost power available.
    /// Used in calculations: effective_multiplier = 1.0 + (nitroMultiplier_ - 1.0) * nitroPowerCurve_
    float nitroPowerCurve_;
    /// World start time when nitro was activated (for reference timestamp).
    /// Updated when SetNitroActive(true) is called.
    float nitroStartTime_;

    /// Update nitro system state each frame. Called from FixedUpdate().
    /// @param timeStep Time step in seconds.
    /// @doc Manages timer countdowns, power curve interpolation, and state transitions.
    void UpdateNitroState(float timeStep);

    /// Get the effective engine force multiplier based on current nitro state.
    /// @return Multiplier value (1.0 = no boost, up to nitroMultiplier_ at full power).
    /// @doc Returns 1.0 if nitro inactive. Interpolates between 1.0 and nitroMultiplier_
    ///      based on nitroPowerCurve_ if nitro is active.
    float GetNitroEngineForceFactor() const;
};

}
