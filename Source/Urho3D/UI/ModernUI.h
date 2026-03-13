// Copyright (c) 2008-2026 the Urho 4.0 project
// Copyright (c) 2026 Racing UI System extension by Shano
// License: MIT

#pragma once

#include "../UI/UIElement.h"
#include "../UI/Font.h"
#include "../UI/Text.h"
#include "../UI/Button.h"
#include "../Container/Ptr.h"
#include "../Core/Context.h"

#include <functional>

namespace Urho3D
{

/// Lightweight UI styling structure for ModernUI.
/// Designed for performance on lower-end hardware (i3 4th gen) and to be easy to serialize.
struct UIStyle
{
    /// Base background color.
    Color backgroundColor_ = Color::BLACK;
    /// Default text color.
    Color textColor_ = Color::WHITE;
    /// Font size.
    int fontSize_ = 20;
    /// Optional font resource.
    SharedPtr<Font> font_;
    /// Padding between UI elements.
    int padding_ = 8;
    /// Corner radius for rounded rectangles.
    int cornerRadius_ = 6;

    /// Creates a racing-style UI theme.
    static UIStyle Racing();
};

/// Modern UI container with styling and basic animation support.
///
/// This class is intended for use as the root container for menus, HUDs and other
/// on-screen UI elements. It provides a minimal UI layout system and built-in
/// performance hints for low-end hardware.
class URHO3D_API ModernUI : public UIElement
{
    URHO3D_OBJECT(ModernUI, UIElement);

public:
    /// Construct.
    explicit ModernUI(Context* context);
    /// Destruct.
    ~ModernUI() override;

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    /// Set the overall UI style.
    /// Use SharedPtr for all resources to avoid manual lifetime management.
    void SetStyle(const UIStyle& style);

    /// Get the current style.
    const UIStyle& GetStyle() const { return style_; }

    /// Layout modes for arranging child elements.
    enum LayoutMode
    {
        LAYOUT_HORIZONTAL,
        LAYOUT_VERTICAL
    };

    /// Set layout mode used by AddChild() wrapper.
    void SetLayout(LayoutMode mode);

    /// Get current layout mode.
    LayoutMode GetLayout() const { return layoutMode_; }

    /// Set spacing applied between child elements when using AddChild().
    void SetSpacing(int spacing);

    /// Get current spacing.
    int GetSpacing() const { return spacing_; }

    /// Create a styled button and add it as a child.
    /// @param text Button label.
    /// @param onClick Callback invoked when the button is pressed.
    /// @return SharedPtr to created button.
    SharedPtr<Button> CreateButton(const String& text, const std::function<void()>& onClick);

    /// Add a child element with automatic layout spacing.
    /// @param element Child element to add.
    void AddChildWithLayout(UIElement* element);

    /// Create a simple label with the current style.
    SharedPtr<Text> CreateLabel(const String& text);

    /// Enable or disable debug visualization (borders, spacing guides).
    /// Useful for layout debugging.
    void SetDebugDrawEnabled(bool enabled);

    /// Get debug draw state.
    bool GetDebugDrawEnabled() const { return debugDraw_; }

    /// Update is called each frame. This can dispatch animations and profiling.
    void Update(float timeStep) override;

    /// Perform layout pass for children elements.
    void DoLayout();

private:
    /// Apply style to the given UI element.
    void ApplyStyle(UIElement* element);

    /// Draw debug overlay if enabled.
    void DrawDebugOverlay();

    /// Performance profiling marker scope.
    void ProfileBegin(const String& name);
    void ProfileEnd();

    UIStyle style_;
    LayoutMode layoutMode_ = LAYOUT_VERTICAL;
    int spacing_ = 8;
    bool debugDraw_ = false;

    /// Cached font for performance.
    SharedPtr<Font> font_;

    /// Temporary array used for layout processing (pre-allocated to avoid allocations each frame).
    Vector<WeakPtr<UIElement>> layoutCache_;
};

} // namespace Urho3D
