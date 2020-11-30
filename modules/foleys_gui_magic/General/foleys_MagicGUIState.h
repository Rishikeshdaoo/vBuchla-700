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

#pragma once

namespace foleys
{

/**
 The MagicGuiState is the docking station for the MagicGUIBuilder.
 You can register properties or objects there, that your GuiItems will connect to.
 It is also the place, where the data for the visualisers is sent to, which are
 MagicPlotSources and MagicLevelSources.
 */
class MagicGUIState
{
    struct ObjectBase {
        virtual ~ObjectBase() noexcept = default;
    };

    template <typename ToErase> class ErasedObject : public ObjectBase, public ToErase {
    public:
        template <typename... Ts>
        explicit ErasedObject(Ts &&... ts) : ToErase{std::forward<Ts>(ts)...} {}
    };

    template <typename T, typename... Ts>
    std::unique_ptr<ErasedObject<T>> makeErased(Ts &&... t) {
      return std::make_unique<ErasedObject<T>>(std::forward<Ts>(t)...);
    }

public:
    MagicGUIState() = default;

    virtual ~MagicGUIState();

    /**
     Returns the root node for exposed properties for the GUI
     */
    virtual juce::ValueTree getPropertyRoot() const;

    /**
     Set a file to save common settings for all instances
     */
    void setApplicationSettingsFile (juce::File file);

    /**
     Returns the IDs of AudioProcessorParameters for selection
     */
    virtual juce::StringArray getParameterNames() const;

    virtual juce::RangedAudioParameter* getParameter ([[maybe_unused]]const juce::String& paramID)
    { juce::ignoreUnused(paramID); return nullptr; }
    virtual std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>   createAttachment (const juce::String& paramID, juce::Slider&)
    { juce::ignoreUnused(paramID); return nullptr; }
    virtual std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> createAttachment (const juce::String& paramID, juce::ComboBox&)
    { juce::ignoreUnused(paramID); return nullptr; }
    virtual std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>   createAttachment (const juce::String& paramID, juce::Button&)
    { juce::ignoreUnused(paramID); return nullptr; }

    /**
     Override this method to create a default Stylesheet, in case nothing was loaded
     */
    virtual juce::ValueTree createDefaultStylesheet() const;

    /**
     Override this to create a default GUI
     */
    virtual juce::ValueTree createDefaultGUITree() const;

    /**
     Return a hierarchical menu of the AudioParameters

     Implemented in MagicPluginState
     */
    virtual juce::PopupMenu createParameterMenu() const { return {}; }

    /**
     You can store a lambda that can be called from e.g. a TextButton.
     */
    void addTrigger (const juce::Identifier& triggerID, std::function<void()> function);

    /**
     Returns a lambda to be connected to your components
     */
    std::function<void()> getTrigger (const juce::Identifier& triggerID);

    /**
     Returns a property as value inside the ValueTreeState. The nodes are a colon separated list, the last component is the property name
     */
    juce::Value getPropertyAsValue (const juce::String& pathToProperty);

    /**
     Populates a menu with properties found in the persistent ValueTree
     */
    void populatePropertiesMenu (juce::ComboBox& comboBox) const;

    juce::PopupMenu createPropertiesMenu (juce::ComboBox& combo) const;

    juce::PopupMenu createTriggerMenu() const;

    juce::PopupMenu createAssetFilesMenu() const;

    /**
     Create and add an object. The type to create needs to be added as template parameter, the arguments will be forwarded to the constructor.
     */
    template <typename T, typename... Ts>
    T* createAndAddObject (const juce::Identifier& objectID, Ts &&... t)
    {
        const auto& present = advertisedObjects.find (objectID);
        if (present != advertisedObjects.cend())
        {
            // You tried to add two MagicPlotSources with the same sourceID
            jassertfalse;
            return nullptr;
        }

        auto o = std::make_unique<ErasedObject<T>>(std::forward<Ts>(t)...);
        auto* pointerToReturn = o.get();
        advertisedObjects [objectID] = std::move (o);

        return pointerToReturn;
    }

    /**
     Returns all identifiers of objects, that can be casted to the given type.
     */
    template <typename ObjectType>
    juce::StringArray getObjectIDsByType() const
    {
        juce::StringArray identifiers;
        for (const auto& object : advertisedObjects)
            if (dynamic_cast<ObjectType*>(object.second.get()))
                identifiers.add (object.first.toString());

        return identifiers;
    }

    /**
     Return an object by objectID. The returned type needs to be specified as template parameter.
     If there is no object with that objectID, or the object is not of the selected type, this will return a nullptr.

     @param objectID is the ID to identify the object.
     */
    template <typename ObjectType>
    ObjectType* getObjectWithType (juce::Identifier objectID)
    {
        const auto& object = advertisedObjects.find (objectID);
        if (object != advertisedObjects.cend())
            return dynamic_cast<ObjectType*>(object->second.get());

        return nullptr;
    }

    /**
     Call this method in your prepareToPlay implementation, to allow th visualisers to be
     properly setup
     */
    void prepareToPlay (double sampleRate, int samplesPerBlockExpected);

    /**
     Registers background processing
     */
    void addBackgroundProcessing (MagicPlotSource* source);

    juce::MidiKeyboardState& getKeyboardState();

    /**
     Return the referenced AudioProcessor, if this state can provide one
     */
    virtual juce::AudioProcessor* getProcessor() { return nullptr; }

private:

    void addParametersToMenu (const juce::AudioProcessorParameterGroup& group, juce::PopupMenu& menu, int& index) const;
    void addPropertiesToMenu (const juce::ValueTree& tree, juce::ComboBox& combo, juce::PopupMenu& menu, const juce::String& path) const;

    /**
     The ApplicationSettings is used for settings e.g. over many plugin instances.
     */
    SharedApplicationSettings settings;

    juce::ValueTree propertyRoot { "Properties" };

    juce::MidiKeyboardState keyboardState;

    std::map<juce::Identifier, std::function<void()>>       triggers;

    std::map<juce::Identifier, std::unique_ptr<ObjectBase>> advertisedObjects;

    juce::TimeSliceThread visualiserThread { "Visualiser Thread" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MagicGUIState)
};

} // namespace foleys
