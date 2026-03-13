// Copyright (c) 2008-2026 the Urho 4.0 project
// Copyright (c) 2026 Racing HUD extension by Shano
// License: MIT

#pragma once

#include "ModernUI.h"
#include "../Math/Color.h"

namespace Urho3D
{

/// Struct carrying common race telemetry data.
struct RaceData
{
    float speed = 0.0f;      ///< Speed in km/h.
    float rpm = 0.0f;        ///< Normalized RPM (0-1).
    int gear = 1;            ///< Current gear.
    int lap = 1;             ///< Current lap.
    int totalLaps = 1;       ///< Total laps in race.
};

/// Racing HUD overlay containing speedometer, tachometer, minimap and messages.
/// Designed for high performance; uses SharedPtr and minimal allocations.
class URHO3D_API RacingHUD : public ModernUI
{
    URHO3D_OBJECT(RacingHUD, ModernUI);

public:
    explicit RacingHUD(Context* context);
    ~RacingHUD() override;

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    /// Create a speedometer widget.
    /// @param rect Screen rectangle where the speedometer will be rendered.
    void CreateSpeedometer(const IntRect& rect);

    /// Create a tachometer widget.
    /// @param rect Screen rectangle where the tachometer will be rendered.
    void CreateTachometer(const IntRect& rect);

    /// Create a simple minimap widget.
    /// @param rect Screen rectangle for minimap.
    void CreateMinimap(const IntRect& rect);

    /// Update the HUD with current race telemetry.
    void UpdateRaceData(const RaceData& data);

    /// Show an on-screen message for the specified duration.
    void ShowMessage(const String& message, float duration, const Color& color);

    /// Per-frame update.
    void Update(float timeStep) override;

private:
    /// Update internal gauge visuals. Called from Update.
    void UpdateGauges(float timeStep);

    /// Background elements for each gauge.
    SharedPtr<UIElement> speedometerRoot_;
    SharedPtr<UIElement> tachometerRoot_;
    SharedPtr<UIElement> minimapRoot_;
    SharedPtr<Text> messageText_;

    /// Internal state.
    RaceData currentData_;
    float messageTimeRemaining_ = 0.0f;
};

} // namespace Urho3D
