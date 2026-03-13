// Copyright (c) 2008-2026 the Urho 4.0 project
// Copyright (c) 2026 Racing HUD extension by Shano
// License: MIT

#include "RacingHUD.h"
#include "../UI/Text.h"
#include "../UI/BorderImage.h"
#include "../UI/Sprite.h"
#include "../Core/Profiler.h"

namespace Urho3D
{

RacingHUD::RacingHUD(Context* context)
    : ModernUI(context)
{
    // Default styling for racing HUD.
    SetStyle(UIStyle::Racing());

    // Ensure we draw on top of other UI elements.
    SetBringToFront(true);

    // Create placeholders for HUD panels
    speedometerRoot_ = new BorderImage(context);
    tachometerRoot_ = new BorderImage(context);
    minimapRoot_ = new BorderImage(context);

    AddChild(speedometerRoot_);
    AddChild(tachometerRoot_);
    AddChild(minimapRoot_);

    // Create text object for messages
    messageText_ = new Text(context);
    messageText_->SetFont(style_.font_, style_.fontSize_);
    messageText_->SetColor(Color::YELLOW);
    messageText_->SetHorizontalAlignment(HA_CENTER);
    messageText_->SetVerticalAlignment(VA_TOP);
    messageText_->SetPosition(0, 20);
    AddChild(messageText_);
}

RacingHUD::~RacingHUD() = default;

void RacingHUD::RegisterObject(Context* context)
{
    context->RegisterFactory<RacingHUD>();
}

void RacingHUD::CreateSpeedometer(const IntRect& rect)
{
    if (!speedometerRoot_)
        return;

    speedometerRoot_->SetPosition(rect.left_, rect.top_);
    speedometerRoot_->SetSize(rect.Width(), rect.Height());
    speedometerRoot_->SetLayoutMode(LM_VERTICAL);
    speedometerRoot_->SetLayoutSpacing(4);

    // TODO: Implement actual needle/dial assets
}

void RacingHUD::CreateTachometer(const IntRect& rect)
{
    if (!tachometerRoot_)
        return;

    tachometerRoot_->SetPosition(rect.left_, rect.top_);
    tachometerRoot_->SetSize(rect.Width(), rect.Height());
    tachometerRoot_->SetLayoutMode(LM_VERTICAL);
    tachometerRoot_->SetLayoutSpacing(4);

    // TODO: Implement actual needle/dial assets
}

void RacingHUD::CreateMinimap(const IntRect& rect)
{
    if (!minimapRoot_)
        return;

    minimapRoot_->SetPosition(rect.left_, rect.top_);
    minimapRoot_->SetSize(rect.Width(), rect.Height());

    // TODO: Implement minimap rendering (can use RenderSurface or Sprite)
}

void RacingHUD::UpdateRaceData(const RaceData& data)
{
    currentData_ = data;
}

void RacingHUD::ShowMessage(const String& message, float duration, const Color& color)
{
    if (messageText_)
    {
        messageText_->SetText(message);
        messageText_->SetColor(color);
        messageTimeRemaining_ = Max(0.0f, duration);
    }
}

void RacingHUD::Update(float timeStep)
{
    // Update message timer
    if (messageTimeRemaining_ > 0.0f)
    {
        messageTimeRemaining_ -= timeStep;
        if (messageTimeRemaining_ <= 0.0f && messageText_)
        {
            messageText_->SetText(String::EMPTY);
        }
    }

    UpdateGauges(timeStep);
}

void RacingHUD::UpdateGauges(float /*timeStep*/)
{
    // TODO: Update needle rotation and fill states based on currentData_.
    // Intended to be lightweight on CPU for i3 4th gen.
}

} // namespace Urho3D
