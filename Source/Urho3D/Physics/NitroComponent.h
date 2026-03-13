// Copyright (c) 2008-2026 the Urho 4.0 project
// Copyright (c) 2026 Nitro Component extension by Shano
// License: MIT

#pragma once

#include "../Scene/LogicComponent.h"
#include "../Graphics/Material.h"
#include "../Graphics/ParticleEffect.h"
#include "../Audio/SoundSource3D.h"

namespace Urho3D
{

class RaycastVehicle;
class ParticleEmitter;
class Material;
class Text3D;

/// Nitro Boost visual and audio effect component.
/// Attach this component to a vehicle node that has a RaycastVehicle to automatically
/// handle nitro boost visual effects (materials, particles), audio (boost sounds), and UI feedback.
///
/// ## Features:
/// - Automatic red glow material switching when nitro is active
/// - Particle system spawning for exhaust flames/sparks
/// - Boost activation and loop audio playback
/// - Screen text indicators for boost time and cooldown
/// - Distance-based LOD (particle effects only render when close enough)
/// - Performance-optimized for older CPUs (i3 4th gen target)
///
class URHO3D_API NitroComponent : public LogicComponent
{
    URHO3D_OBJECT(NitroComponent, LogicComponent);

public:
    /// Construct.
    explicit NitroComponent(Urho3D::Context* context);
    /// Destruct.
    ~NitroComponent() override;

    /// Register object factory and attributes.
    /// @nobind
    static void RegisterObject(Context* context);

    /// Handle enabled/disabled state change.
    void OnSetEnabled() override;

    /// Acquire resource references and initialize component state.
    void ApplyAttributes() override;

    /// Per-frame update logic for visual effects and UI.
    void PostUpdate(float timeStep) override;

    // ====== MATERIAL EFFECTS ======

    /// Set the material to apply when nitro is active (red glow effect, etc.).
    /// @param materialPath Path to material resource (e.g., "Materials/NitroGlow.xml").
    void SetNitroMaterial(const String& materialPath);

    /// Get the configured nitro active material path.
    String GetNitroMaterialPath() const { return nitroMaterialPath_; }

    /// Set the material to apply when nitro is inactive (default state).
    /// @param materialPath Path to material resource.
    void SetDefaultMaterial(const String& materialPath);

    /// Get the configured default material path.
    String GetDefaultMaterialPath() const { return defaultMaterialPath_; }

    // ====== PARTICLE EFFECTS ======

    /// Set the particle effect to emit during nitro boost (exhaust, sparks, flames).
    /// @param particleEffectPath Path to particle effect resource (e.g., "Particles/NitroExhaust.xml").
    void SetExhaustParticleEffect(const String& particleEffectPath);

    /// Get the configured exhaust particle effect path.
    String GetExhaustParticleEffectPath() const { return exhaustParticlePath_; }

    /// Set the node where exhaust particles will be emitted (typically rear wheels/exhaust).
    /// @param nodeName Name of child node to use as emission point.
    void SetExhaustEmissionPoint(const String& nodeName);

    /// Get the configured exhaust emission point node name.
    String GetExhaustEmissionPointName() const { return exhaustEmissionNodeName_; }

    /// Set the maximum distance at which particle effects are visible (LOD optimization).
    /// @param distance Distance in world units. Default 50.0 (disable particles beyond this).
    void SetParticleEffectLODDistance(float distance) { particleEffectLODDistance_ = Max(1.0f, distance); }

    /// Get the configured LOD distance for particle effects.
    float GetParticleEffectLODDistance() const { return particleEffectLODDistance_; }

    // ====== AUDIO EFFECTS ======

    /// Set the sound resource for nitro activation (one-shot sound).
    /// @param soundPath Path to sound resource (e.g., "Sounds/BoostActivate.ogg").
    void SetBoostActivationSound(const String& soundPath);

    /// Set the sound resource for continuous nitro loop during active boost.
    /// @param soundPath Path to sound resource (e.g., "Sounds/BoostLoop.ogg").
    void SetBoostLoopSound(const String& soundPath);

    /// Set the sound resource for boost becoming available again (cooldown end).
    /// @param soundPath Path to sound resource (e.g., "Sounds/BoostReady.ogg").
    void SetBoostReadySound(const String& soundPath);

    /// Set the master volume for all nitro-related sounds (0.0 to 1.0).
    void SetSoundVolume(float volume) { soundVolume_ = Clamp(volume, 0.0f, 1.0f); }

    /// Get the master sound volume.
    float GetSoundVolume() const { return soundVolume_; }

    // ====== UI FEEDBACK ======

    /// Set whether to show on-screen boost timer and cooldown indicator.
    /// @param show True to display UI feedback.
    void SetShowUIFeedback(bool show) { showUIFeedback_ = show; }

    /// Check if UI feedback is enabled.
    bool GetShowUIFeedback() const { return showUIFeedback_; }

    /// Set the screen position for boost timer text (0.0-1.0 normalized, default 0.5, 0.1).
    /// @param x Horizontal position (0=left, 1=right).
    /// @param y Vertical position (0=top, 1=bottom).
    void SetUITextPosition(float x, float y) 
    { 
        uiTextX_ = Clamp(x, 0.0f, 1.0f); 
        uiTextY_ = Clamp(y, 0.0f, 1.0f); 
    }

    /// Get the configured UI text X position.
    float GetUITextX() const { return uiTextX_; }

    /// Get the configured UI text Y position.
    float GetUITextY() const { return uiTextY_; }

    // ====== SCREEN SHAKE EFFECT ======

    /// Set whether to apply screen shake during high-speed boost.
    /// @param enable True to enable screen shake effect at high speeds.
    void SetScreenShakeEnabled(bool enable) { screenShakeEnabled_ = enable; }

    /// Check if screen shake effect is enabled.
    bool GetScreenShakeEnabled() const { return screenShakeEnabled_; }

    /// Set the speed threshold for triggering screen shake during boost.
    /// @param speedThreshold Speed in world units/second. Default 40.0 (only shake above this speed).
    void SetScreenShakeSpeedThreshold(float speedThreshold) { screenShakeSpeedThreshold_ = Max(5.0f, speedThreshold); }

    /// Get the configured screen shake speed threshold.
    float GetScreenShakeSpeedThreshold() const { return screenShakeSpeedThreshold_; }

    /// Set the shake intensity (0.0-1.0). Higher values = stronger shake.
    void SetScreenShakeIntensity(float intensity) { screenShakeIntensity_ = Clamp(intensity, 0.0f, 1.0f); }

    /// Get the configured screen shake intensity.
    float GetScreenShakeIntensity() const { return screenShakeIntensity_; }

private:
    /// Reference to the parent RaycastVehicle component.
    WeakPtr<RaycastVehicle> vehicle_;

    // MATERIAL EFFECTS MEMBERS
    /// Path to material applied when nitro is active.
    String nitroMaterialPath_;
    /// Path to material applied when nitro is inactive.
    String defaultMaterialPath_;
    /// Cached nitro boost material resource.
    SharedPtr<Material> nitroMaterial_;
    /// Cached default material resource (saved on first update).
    SharedPtr<Material> defaultMaterial_;
    /// Previous material states for each node component (for restoration).
    HashMap<Node*, SharedPtr<Material>> originalMaterials_;

    // PARTICLE EFFECT MEMBERS
    /// Path to exhaust particle effect.
    String exhaustParticlePath_;
    /// Name of child node used as particle emission point.
    String exhaustEmissionNodeName_;
    /// Maximum distance for particle effects to be visible (LOD).
    float particleEffectLODDistance_;
    /// Cached particle effect resource.
    SharedPtr<ParticleEffect> exhaustParticleEffect_;
    /// Active particle emitter during boost (nullptr when not boosting or off-screen).
    WeakPtr<ParticleEmitter> activeExhaustEmitter_;
    /// Last frame's camera distance for LOD calculations.
    float lastCameraDistance_;

    // AUDIO EFFECT MEMBERS
    /// Path to boost activation sound (one-shot).
    String boostActivationSoundPath_;
    /// Path to boost loop sound (continuous during boost).
    String boostLoopSoundPath_;
    /// Path to boost ready sound (cooldown complete).
    String boostReadySoundPath_;
    /// Master volume for all nitro sounds (0.0-1.0).
    float soundVolume_;
    /// Cached sound source for playing boost sounds.
    WeakPtr<SoundSource3D> soundSource_;
    /// Track if loop sound is currently playing (to avoid repeated starts).
    bool isLoopSoundPlaying_;
    /// Track if we've already played the boost activation sound this session.
    bool playedActivationSound_;
    /// Track if we've already played the ready sound for this cycle.
    bool playedReadySound_;

    // UI FEEDBACK MEMBERS
    /// Whether to display on-screen boost timer/cooldown indicator.
    bool showUIFeedback_;
    /// Normalized X position for UI text display (0-1).
    float uiTextX_;
    /// Normalized Y position for UI text display (0-1).
    float uiTextY_;
    /// Cached 3D text for boost timer display (created on demand).
    WeakPtr<Text3D> boostTimerText_;
    /// Last frame's remaining boost time (to detect state changes).
    float lastBoostTimer_;
    /// Last frame's cooldown timer (to detect state changes).
    float lastCooldownTimer_;

    // SCREEN SHAKE EFFECT MEMBERS
    /// Whether screen shake effect is enabled during high-speed boost.
    bool screenShakeEnabled_;
    /// Speed threshold to trigger screen shake (world units/sec).
    float screenShakeSpeedThreshold_;
    /// Intensity multiplier for screen shake effect (0-1).
    float screenShakeIntensity_;
    /// Current shake offset for camera (accumulator).
    Vector3 currentShakeOffset_;
    /// Time remaining in current shake effect.
    float shakeTimeRemaining_;

    // STATE TRACKING
    /// Previous boost active state (to detect transitions).
    bool wasNitroActivePrevFrame_;
    /// Track whether we transitioned from active to cooldown this frame.
    bool transitionedToCooldownThisFrame_;

    /// Update material effects based on nitro state.
    void UpdateMaterialEffect();

    /// Update particle emission based on nitro state and LOD.
    void UpdateParticleEffects();

    /// Update audio effects based on nitro state transitions.
    void UpdateAudioEffects();

    /// Update on-screen UI feedback text.
    void UpdateUIFeedback();

    /// Apply screen shake effect based on boost state and current speed.
    void UpdateScreenShake(float timeStep);

    /// Calculate distance to current camera for LOD calculations.
    float GetCameraDistance() const;

    /// Get or create a SoundSource3D component for playing nitro sounds.
    SoundSource3D* GetOrCreateSoundSource();

    /// Apply screen shake to the current camera view.
    void ApplyCameraShake(const Vector3& shakeOffset);

    /// Check if vehicle has sufficient speed for screen shake during boost.
    bool HasSufficientSpeedForShake() const;
};

}  // namespace Urho3D
