#pragma once
#include <JuceHeader.h>


class MainComponent : public juce::AudioAppComponent, public juce::MidiKeyboardStateListener, public juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;
    
    /// <summary>
    /// Called at initialisation of audio device
    /// </summary>
    /// <param name="samplesPerBlockExpected"> expected samples per buffer</param>
    /// <param name="sampleRate"> the sample rate of the audio device</param>
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
   
    /// <summary>
    /// Called repeatedly to fetch subsequent blocks of audio data
    /// </summary>
    /// <param name="bufferToFill">multichannel buffer to fill with audio data</param>
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
 
    /// <summary>
    /// Method called at shutdown
    /// </summary>
    void releaseResources() override;

    /// <summary>
    /// Method called to draw on this component
    /// </summary>
    /// <param name="g">graphics context</param>
    void paint (juce::Graphics& g) override;

    /// <summary>
    /// Method called when the app is resized, this is also called on ui initialisation .
    /// </summary>
    void resized() override;


    /// <summary>
    /// Adds a sinewave to the buffer passed
    /// </summary>
    /// <param name="buffer">A float buffer that contains seperate float arrays for channels</param>
    /// <param name="numSamples">the number of samples in each channel</param>
    void inline addSineWave(juce::AudioBuffer<float>& buffer, const int& numSamples);
    
private:

    float m_phase01 = 0.f; // Phase variable going from 0 (min) to 1 (max) 
	float m_frequency = 440.f; // frequency in hz
	float m_sampleRate = 48000.f; // samplerate in hz
	float m_gain = 0.0f; // gain going from 0 (min) to 1 (max)
	const float m_twoPi = juce::MathConstants<float>::twoPi; // two pi constant
 
    // On-screen midi keyboard
    juce::MidiKeyboardState m_midikeyboardState;
    juce::MidiKeyboardComponent m_midiKeyboardComponent;
    
    // MidiKeyboardStateListener method implementation
    void handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override ;
    void handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override ;
    
    // device selection component
    juce::AudioDeviceSelectorComponent m_audioDeviceSelection;
    
    
    int m_OSCWriteIndex = 0; //oscilloscope writing index
    std::array<float, 1024 * 4> m_oscilloscopeBuffer; // oscilloscope buffer used for drawing oscilloscope
    
    // Inherited via Timer
    virtual void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
