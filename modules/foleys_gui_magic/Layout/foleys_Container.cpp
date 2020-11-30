/*
 ==============================================================================
    Copyright (c) 2019-2020 Foleys Finest Audio Ltd. - Daniel Walz
    All rights reserved.

    License for non-commercial projects:

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    License for commercial products:

    To sell commercial products containing this module, you are required to buy a
    License from https://foleysfinest.com/developer/pluginguimagic/

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
    OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
    OF THE POSSIBILITY OF SUCH DAMAGE.
 ==============================================================================
 */

namespace foleys
{

Container::Container (MagicGUIBuilder& builder, juce::ValueTree node)
  : GuiItem (builder, node)
{
}

void Container::update()
{
    configureFlexBox (configNode);

    for (auto& child : *this)
        child->updateInternal();

    const auto display = magicBuilder.getStyleProperty (IDs::display, configNode).toString();
    if (display == IDs::contents)
        setLayoutMode (Container::Layout::Contents);
    else if (display == IDs::tabbed)
        setLayoutMode (Container::Layout::Tabbed);
    else
        setLayoutMode (Container::Layout::FlexBox);

    auto repaintHz = magicBuilder.getStyleProperty (IDs::repaintHz, configNode).toString();
    if (repaintHz.isNotEmpty())
    {
        refreshRateHz = repaintHz.getIntValue();
        updateContinuousRedraw();
    }
}

void Container::addChildItem (std::unique_ptr<GuiItem> child)
{
    addAndMakeVisible (child.get());
    children.push_back (std::move (child));
}

void Container::createSubComponents()
{
    children.clear();

    for (auto childNode : configNode)
    {
        auto childItem = magicBuilder.createGuiItem (childNode);
        if (childItem)
        {
            addAndMakeVisible (childItem.get());
            childItem->createSubComponents();

            children.push_back (std::move (childItem));
        }
    }

    updateLayout();
    updateContinuousRedraw();
}

GuiItem* Container::findGuiItemWithId (const juce::String& name)
{
    if (configNode.getProperty (IDs::id, juce::String()).toString() == name)
        return this;

    for (auto& item : children)
        if (auto* matching = item->findGuiItemWithId (name))
            return matching;

    return nullptr;
}

void Container::setLayoutMode (Layout layoutToUse)
{
    layout = layoutToUse;
    if (layout == Layout::Tabbed)
    {
        updateTabbedButtons();
    }
    else
    {
        tabbedButtons.reset();
        for (auto& child : children)
            child->setVisible (true);
    }

    updateLayout();
}

void Container::resized()
{
    updateLayout();
}

void Container::updateLayout()
{
    if (children.empty())
        return;

    auto clientBounds = getClientBounds();

    if (layout == Layout::FlexBox)
    {
        flexBox.items.clear();
        for (auto& child : children)
            flexBox.items.add (child->getFlexItem());

        flexBox.performLayout (clientBounds);
    }
    else
    {
        if (layout == Layout::Tabbed)
        {
            updateTabbedButtons();
            tabbedButtons->setBounds (clientBounds.removeFromTop (30));
        }
        else
            tabbedButtons.reset();

        for (auto& child : children)
            child->setBounds (clientBounds);
    }

    for (auto& child : children)
        child->updateLayout();
}

void Container::updateColours()
{
    decorator.updateColours (magicBuilder, configNode);

    for (auto& child : children)
        child->updateColours();
}

void Container::updateContinuousRedraw()
{
    stopTimer();
    plotComponents.clear();

    for (auto& child : children)
        if (auto* p = dynamic_cast<MagicPlotComponent*>(child->getWrappedComponent()))
            plotComponents.push_back (p);

    if (! plotComponents.empty())
        startTimerHz (refreshRateHz);
}

void Container::updateTabbedButtons()
{
    tabbedButtons = std::make_unique<juce::TabbedButtonBar>(juce::TabbedButtonBar::TabsAtTop);
    addAndMakeVisible (*tabbedButtons);

    for (auto& child : children)
    {
        tabbedButtons->addTab (child->getTabCaption ("Tab " + juce::String (tabbedButtons->getNumTabs())),
                               child->getTabColour(), -1);
    }

    tabbedButtons->addChangeListener (this);
    tabbedButtons->setCurrentTabIndex (currentTab, false);
    updateSelectedTab();
}

void Container::configureFlexBox (const juce::ValueTree& node)
{
    flexBox = juce::FlexBox();

    const auto direction = magicBuilder.getStyleProperty (IDs::flexDirection, node).toString();
    if (direction == IDs::flexDirRow)
        flexBox.flexDirection = juce::FlexBox::Direction::row;
    else if (direction == IDs::flexDirRowReverse)
        flexBox.flexDirection = juce::FlexBox::Direction::rowReverse;
    else if (direction == IDs::flexDirColumn)
        flexBox.flexDirection = juce::FlexBox::Direction::column;
    else if (direction == IDs::flexDirColumnReverse)
        flexBox.flexDirection = juce::FlexBox::Direction::columnReverse;

    const auto wrap = magicBuilder.getStyleProperty (IDs::flexWrap, node).toString();
    if (wrap == IDs::flexWrapNormal)
        flexBox.flexWrap = juce::FlexBox::Wrap::wrap;
    else if (wrap == IDs::flexWrapReverse)
        flexBox.flexWrap = juce::FlexBox::Wrap::wrapReverse;
    else
        flexBox.flexWrap = juce::FlexBox::Wrap::noWrap;

    const auto alignContent = magicBuilder.getStyleProperty (IDs::flexAlignContent, node).toString();
    if (alignContent == IDs::flexStart)
        flexBox.alignContent = juce::FlexBox::AlignContent::flexStart;
    else if (alignContent == IDs::flexEnd)
        flexBox.alignContent = juce::FlexBox::AlignContent::flexEnd;
    else if (alignContent == IDs::flexCenter)
        flexBox.alignContent = juce::FlexBox::AlignContent::center;
    else if (alignContent == IDs::flexSpaceAround)
        flexBox.alignContent = juce::FlexBox::AlignContent::spaceAround;
    else if (alignContent == IDs::flexSpaceBetween)
        flexBox.alignContent = juce::FlexBox::AlignContent::spaceBetween;
    else
        flexBox.alignContent = juce::FlexBox::AlignContent::stretch;

    const auto alignItems = magicBuilder.getStyleProperty (IDs::flexAlignItems, node).toString();
    if (alignItems == IDs::flexStart)
        flexBox.alignItems = juce::FlexBox::AlignItems::flexStart;
    else if (alignItems == IDs::flexEnd)
        flexBox.alignItems = juce::FlexBox::AlignItems::flexEnd;
    else if (alignItems == IDs::flexCenter)
        flexBox.alignItems = juce::FlexBox::AlignItems::center;
    else
        flexBox.alignItems = juce::FlexBox::AlignItems::stretch;

    const auto justify = magicBuilder.getStyleProperty (IDs::flexJustifyContent, node).toString();
    if (justify == IDs::flexEnd)
        flexBox.justifyContent = juce::FlexBox::JustifyContent::flexEnd;
    else if (justify == IDs::flexCenter)
        flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
    else if (justify == IDs::flexSpaceAround)
        flexBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    else if (justify == IDs::flexSpaceBetween)
        flexBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    else
        flexBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
}

void Container::timerCallback()
{
    auto needsRepaint = false;
    for (auto p : plotComponents)
        if (p) needsRepaint |= p->needsUpdate();

    if (needsRepaint)
        repaint();
}

void Container::changeListenerCallback (juce::ChangeBroadcaster*)
{
    currentTab = tabbedButtons ? tabbedButtons->getCurrentTabIndex() : 0;
    updateSelectedTab();
}

void Container::updateSelectedTab()
{
    int index = 0;
    for (auto& child : children)
        child->setVisible (currentTab == index++);
}

std::vector<std::unique_ptr<GuiItem>>::iterator Container::begin()
{
    return children.begin();
}

std::vector<std::unique_ptr<GuiItem>>::iterator Container::end()
{
    return children.end();
}

#if FOLEYS_SHOW_GUI_EDITOR_PALLETTE
void Container::setEditMode (bool shouldEdit)
{
    for (auto& child : children)
        child->setEditMode (shouldEdit);

    GuiItem::setEditMode (shouldEdit);
}
#endif

} // namespace foleys
