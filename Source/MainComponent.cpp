#include "MainComponent.h"

MainComponent::MainComponent() : m_midiKeyboardComponent(m_midikeyboardState, juce::MidiKeyboardComponent::Orientation::horizontalKeyboard),
m_audioDeviceSelection(deviceManager, 0, 0, 2, 2, false, false, true, false)
{
    setSize (800, 600);
    setAudioChannels (0, 2);
	addAndMakeVisible(m_midiKeyboardComponent);
	addAndMakeVisible(m_audioDeviceSelection);
	m_midikeyboardState.addListener(this);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

// This method is called when IO is set
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	
	// Set sample rate
	m_sampleRate = static_cast<float>(sampleRate);
}

// This method is called to fill in the buffer that is going to be played by the device.
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
	// Clear the buffer.
	bufferToFill.clearActiveBufferRegion();
	
	// add a sinewave to the buffer.
	addSineWave(*bufferToFill.buffer, bufferToFill.numSamples);

}

void MainComponent::releaseResources()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
	m_audioDeviceSelection.setBounds(0, 0, getWidth(), getHeight() - 200);
	m_midiKeyboardComponent.setBounds(0, getHeight() - 200, getWidth(), 200);
}

inline void MainComponent::addSineWave(juce::AudioBuffer<float>& buffer, const int& numSamples)
{
	for (int i = 0; i < numSamples; i++)
	{
		m_phase01 += (m_frequency / m_sampleRate);
		if (m_phase01 > 1.0f)
		{
			m_phase01 = m_phase01 - 1.0f;
		}

		float sample = std::sinf(m_phase01 * m_twoPi) * m_gain;

		for (int ch = 0; ch < buffer.getNumChannels(); ch++)
		{
			buffer.getArrayOfWritePointers()[ch][i] = sample;
		}
	}
}

void MainComponent::handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
	m_frequency = static_cast<float>(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
	m_gain = 1.0f;
}

void MainComponent::handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
	if (m_frequency == static_cast<float>(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber)))
	{
		m_gain = 0.0f;
	}
}
