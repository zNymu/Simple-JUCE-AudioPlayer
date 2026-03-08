#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent,
                      public juce::ChangeListener,
                      public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void timerCallback() override;

private:
    //==============================================================================
    // Your private member variables go here...
     //UI
  juce::TextButton openButton { " Open file "};
  juce::TextButton playButton{" Play "};
  juce::TextButton stopButton{" Stop "};
  juce::Slider posSlider;
  juce::Slider volSlider;
  juce::Label fileNameLabel{"fileLabel", "No file loaded"};
  juce::Label volLabel{"volLabel", "Volume"};
  juce::Label posLabel{"posLabel", "Position"};


  //logic
  juce::AudioFormatManager formatManager;
  std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
  juce::AudioTransportSource transportSource;
  std::unique_ptr<juce::FileChooser> chooser;

  enum TransportState {Stopped, Starting, Playing, Stopping};
  TransportState state{Stopped};
  void transportStateChanged(TransportState newState);
  void openButtonClicked();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
