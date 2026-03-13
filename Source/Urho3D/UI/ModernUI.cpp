// Copyright (c) 2008-2026 the Urho 4.0 project
// Copyright (c) 2026 Racing UI System extension by Shano
// License: MIT

#include "ModernUI.h"
#include "../Graphics/Graphics.h"
#include "../UI/UI.h"
#include "../UI/BorderImage.h"
#include "../Core/Profiler.h"
#include "../Core/Context.h"

namespace Urho3D
{

static const char* PROFILE_MODERN_UI = "ModernUI";

UIStyle UIStyle::Racing()
{
    UIStyle style;
    style.backgroundColor_ = Color(0.06f, 0.06f, 0.08f, 0.85f);
    style.textColor_ = Color(0.92f, 0.92f, 1.0f);
    style.fontSize_ = 18;
    style.padding_ = 10;
    style.cornerRadius_ = 8;
    return style;
}

ModernUI::ModernUI(Context* context)
    : UIElement(context)
{
    // Basic layout
    SetClipChildren(true);
    SetBringToFront(true);
    SetEnabled(true);

    // Default style
    style_ = UIStyle::Racing();

    // Create a font cache for repeated use
    if (!style_.font_)
    {
        style_.font_ = GetSubsystem<ResourceCache>()->GetResource<Font>("Fonts/Anonymous Pro.ttf");
        if (!style_.font_)
            URHO3D_LOGWARNING("ModernUI: Could not load default font 'Fonts/Anonymous Pro.ttf'");
    }

    font_ = style_.font_;

    // Hook update event for animations and performance updates.
    SubscribeToEvent(this, E_UPDATE, [&](StringHash, VariantMap& args)
    {
        using namespace Update;
        float timeStep = args[P_TIMESTEP].GetFloat();
        Update(timeStep);
    });
}

ModernUI::~ModernUI() = default;

void ModernUI::RegisterObject(Context* context)
{
    context->RegisterFactory<ModernUI>();
}

void ModernUI::SetStyle(const UIStyle& style)
{
    ProfileBegin("SetStyle");
    style_ = style;
    if (style_.font_)
        font_ = style_.font_;
    for (auto* child : GetChildren())
        ApplyStyle(child);
    ProfileEnd();
}

void ModernUI::SetLayout(LayoutMode mode)
{
    layoutMode_ = mode;
    DoLayout();
}

void ModernUI::SetSpacing(int spacing)
{
    spacing_ = Max(0, spacing);
    DoLayout();
}

SharedPtr<Button> ModernUI::CreateButton(const String& text, const std::function<void()>& onClick)
{
    ProfileBegin("CreateButton");

    auto* button = new Button(context_);
    button->SetMinHeight(32);
    button->SetName(text);

    auto* textElement = new Text(context_);
    textElement->SetText(text);
    textElement->SetFont(font_, style_.fontSize_);
    textElement->SetColor(style_.textColor_);
    textElement->SetAlignment(HA_CENTER, VA_CENTER);

    button->AddChild(textElement);

    ApplyStyle(button);

    // Use Urho3D event system instead of custom callback hooks (ensures safety and lifetime handling).
    if (onClick)
    {
        SubscribeToEvent(button, E_RELEASED, [onClick](StringHash, VariantMap&)
        {
            onClick();
        });
    }

    AddChildWithLayout(button);

    ProfileEnd();
    return SharedPtr<Button>(button);
}

SharedPtr<Text> ModernUI::CreateLabel(const String& text)
{
    auto* label = new Text(context_);
    label->SetText(text);
    label->SetFont(font_, style_.fontSize_);
    label->SetColor(style_.textColor_);
    AddChildWithLayout(label);
    return SharedPtr<Text>(label);
}

void ModernUI::AddChildWithLayout(UIElement* element)
{
    if (!element)
        return;

    AddChild(element);

    // Layout immediately to avoid visual shift.
    DoLayout();
}

void ModernUI::SetDebugDrawEnabled(bool enabled)
{
    debugDraw_ = enabled;
    SetDebugUI(enabled);
}

void ModernUI::Update(float timeStep)
{
    ProfileBegin("Update");

    // Layout and debug draw are cheap but keep them conditional.
    if (debugDraw_)
        DrawDebugOverlay();

    ProfileEnd();
}

void ModernUI::DoLayout()
{
    // Simple linear layout.
    float offset = 0.0f;
    for (UIElement* child : GetChildren())
    {
        if (!child->IsVisible())
            continue;

        IntRect rect = child->GetIntRect();
        if (layoutMode_ == LAYOUT_VERTICAL)
        {
            rect.top_ = (int)offset;
            rect.bottom_ = rect.top_ + rect.Height();
            child->SetPosition(0, (int)offset);
            offset += rect.Height() + spacing_;
        }
        else
        {
            rect.left_ = (int)offset;
            rect.right_ = rect.left_ + rect.Width();
            child->SetPosition((int)offset, 0);
            offset += rect.Width() + spacing_;
        }
    }
}

void ModernUI::ApplyStyle(UIElement* element)
{
    if (!element)
        return;

    // Apply background if element is a BorderImage.
    if (auto* border = element->Cast<BorderImage>())
    {
        border->SetColor(style_.backgroundColor_);
        border->SetBorder(IntRect(style_.cornerRadius_, style_.cornerRadius_, style_.cornerRadius_, style_.cornerRadius_));
    }

    // Apply font to Text elements
    if (auto* text = element->Cast<Text>())
    {
        text->SetFont(font_, style_.fontSize_);
        text->SetColor(style_.textColor_);
    }
}

void ModernUI::DrawDebugOverlay()
{
    // Use built-in debug drawing provided by Urho3D UI system.
    // This avoids low-level graphics calls and stays within the UI subsystem.
    SetDebugDraw(true);
}

void ModernUI::ProfileBegin(const String& name)
{
    PROFILE(name.CString());
}

void ModernUI::ProfileEnd()
{
    // No-op: PROFILE macro ends automatically at scope exit.
}

} // namespace Urho3D
