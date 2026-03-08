#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
  formatManager.registerBasicFormats();
  transportSource.addChangeListener(this);


  addAndMakeVisible(openButton);
  openButton.onClick = [this] { openButtonClicked(); };

  addAndMakeVisible(playButton);
  playButton.onClick = [this] { transportStateChanged(Starting); };
  playButton.setEnabled(false);

  addAndMakeVisible(stopButton);
  stopButton.onClick = [this] { transportStateChanged(Stopping); };
  stopButton.setEnabled(false);

  addAndMakeVisible(volSlider);
  volSlider.setRange(0.0, 1.0);
  volSlider.onValueChange = [this] { transportSource.setGain(volSlider.getValue());};

  addAndMakeVisible(posSlider);
  posSlider.setRange(0.0, 1.0);
  posSlider.onValueChange = [this] {
    auto duration = transportSource.getLengthInSeconds();
    if (duration > 0.0 && posSlider.isMouseButtonDown()) {
      transportSource.setPosition(posSlider.getValue() * duration);
    }
  };

  addAndMakeVisible(fileNameLabel);
  fileNameLabel.setJustificationType(juce::Justification::centred);

  addAndMakeVisible(volLabel);
  volLabel.attachToComponent(&volSlider, true);

  addAndMakeVisible(posLabel);
  posLabel.attachToComponent(&posSlider, true);

  volSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
  posSlider.onValueChange = [this] {
    auto duration = transportSource.getLengthInSeconds();
    if (duration > 0.0 && posSlider.isMouseButtonDown()) {
      transportSource.setPosition(posSlider.getValue() * duration);
    }
  };

  startTimer(100);


    // Make sure you set the size of the component after
    // you add any child components.
    setSize (600, 300);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()

    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)


    //bufferToFill.clearActiveBufferRegion();
    auto *currentSource = readerSource.get();
   if (currentSource == nullptr) {
      bufferToFill.clearActiveBufferRegion();
      return;
    }

    transportSource.getNextAudioBlock(bufferToFill);

}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    auto area = getLocalBounds().reduced(20);

    fileNameLabel.setBounds(area.removeFromTop(40));

    auto buttonRow = area.removeFromTop(40);
    openButton.setBounds(buttonRow.removeFromLeft(100).reduced(5));
    playButton.setBounds(buttonRow.removeFromLeft(100).reduced(5));
    stopButton.setBounds(buttonRow.removeFromLeft(100).reduced(5));

    area.removeFromTop(20);
    volSlider.setBounds(area.removeFromTop(50).withTrimmedLeft(
        60));

    area.removeFromTop(10);
    posSlider.setBounds(area.removeFromTop(50).withTrimmedLeft(60));
}

void MainComponent::openButtonClicked() {
  chooser = std::make_unique<juce::FileChooser>("Select a file..", juce::File{},
                                                "*.wav;*.mp3;*.aiff");

  auto chooserFlags = juce::FileBrowserComponent::openMode |
                      juce::FileBrowserComponent::canSelectFiles;

  chooser->launchAsync(chooserFlags, [this](const juce::FileChooser &fc) {
    auto file = fc.getResult();
    if (file.exists()) {
      auto *reader = formatManager.createReaderFor(file);

      if (reader != nullptr) {
        auto newSource =
            std::make_unique<juce::AudioFormatReaderSource>(reader, true);

        transportSource.setSource(nullptr);

        readerSource.reset(newSource.release());

        transportSource.setSource(readerSource.get(), 0, nullptr,
                                  reader->sampleRate);

        fileNameLabel.setText(file.getFileName(), juce::dontSendNotification);
        playButton.setEnabled(true);
      }
    }
  });
}
void MainComponent::transportStateChanged(TransportState newState) {
  state = newState;

  switch (state) {
  case Stopped:
    playButton.setEnabled(true);
    stopButton.setEnabled(false);
    break;

  case Starting:
    transportSource.start();
    break;

  case Playing:
    playButton.setEnabled(false);
    stopButton.setEnabled(true);
    break;

  case Stopping:
    transportSource.stop();
    break;
  }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
  if (source == &transportSource) {
    if (transportSource.isPlaying())
      transportStateChanged(Playing);
    else
      transportStateChanged(Stopped);
  }
}

void MainComponent::timerCallback() {
  auto duration = transportSource.getLengthInSeconds();

  if (duration > 0.0)
  {
    auto relativePosition = transportSource.getCurrentPosition() / duration;

    posSlider.setValue(relativePosition, juce::dontSendNotification);
  }
}

