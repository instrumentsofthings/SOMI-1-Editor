/*
  ==============================================================================

    Company: Instruments of Things GmbH (c)
    Author: Henrik Langer
    Year: 2022

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
: somiMidiIoInitialized(false), dataModel(), editorView(dataModel)
{  
    auto physicalDim = juce::Rectangle<float>(0, 0, 1915, 1320); // Took from design template

#if JUCE_MAC
    // calculate the scale factor needed to fit the design template into a 1280 wide window
    const float scaleFactor = (1280.0 / physicalDim.getWidth());
#else
    auto& display = juce::Desktop::getInstance().getDisplays();
    
    // Scale UI to match logical user area
    const float physicalUserHeight = display.logicalToPhysical(display.getPrimaryDisplay()->userArea).toFloat().getHeight();
    const float physicalUserWidth = display.logicalToPhysical(display.getPrimaryDisplay()->userArea).toFloat().getWidth();
    const float scaleFactor = juce::jmin(physicalUserHeight / physicalDim.getHeight(), physicalUserWidth / physicalDim.getWidth()) * 0.9f; // Add a bit space for OS elements
#endif
    
    juce::Desktop::getInstance().setGlobalScaleFactor(scaleFactor);
    
    setSize(physicalDim.getWidth(), physicalDim.getHeight());

    addAndMakeVisible(&editorView);
    
    setAudioChannels(0, 0); // We only process MIDI data
    
    enableMidiSomi();
    
    startTimer(TIMER_INTERVAL_MS);
    
    editorView.addBtnListener(this);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    
    editorView.removeBtnListener(this);
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
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
    g.fillAll(juce::Colour(0xFF333333));
}

void MainComponent::resized()
{
#if JUCE_IOS || JUCE_ANDROID
    editorView.setBounds(0, 16, getWidth(), getHeight());
#else
    editorView.setBounds(0, 0, getWidth(), getHeight());
#endif
}

void MainComponent::actionListenerCallback(const juce::String& message)
{
    if (message.compare(GLOBAL_EVT_BTN_APPLY_CLICKED) == 0)
    {
        // Transmit all configs
        for (int sensorId=0; sensorId < MAX_NUM_SENSORS; sensorId++){
            for (int i=0; i < MAX_NUM_CC_MSG_PER_SENSOR; i++)
                sendMidiMessage(dataModel.getCcCfg(sensorId, i).createSysExMessage(false));
            
            for (int i=0; i < MAX_NUM_NOTE_MSG_PER_SENSOR; i++)
                sendMidiMessage(dataModel.getNoteCfg(sensorId, i).createSysExMessage(false));
            
            for (int i=0; i < MAX_NUM_PB_MSG_PER_SENSOR; i++)
                sendMidiMessage(dataModel.getPbCfg(sensorId, i).createSysExMessage(false));
        }
    }
    else if (message.compare(GLOBAL_EVT_BTN_SAVE_CLICKED) == 0)
    {
        PersistConfig saveCmd;
        sendMidiMessage(saveCmd.createSysExMessage());
    }
    else if (message.compare(GLOBAL_EVT_BTN_RST_SENSORS) == 0)
    {
        ResetSensorsConfig rstSensorsCmd;
        sendMidiMessage(rstSensorsCmd.createSysExMessage());
    }
    else if (message.compare(GLOBAL_EVT_BTN_RST_MAPPINGS) == 0)
    {
        ResetMappingsConfig rstMappingsCmd;
        sendMidiMessage(rstMappingsCmd.createSysExMessage());
        
        requestSettingsFromSomi();
    }
    else if (message.compare(GLOBAL_EVT_BTN_CC_SOLO) == 0)
    {
        int sensorId = dataModel.getSelectedSensor();
        int ccSlot = dataModel.getSelectedCcSlot();
        CCSoloConfig ccSoloCmd(dataModel.getCcChannel(sensorId, ccSlot),
                               dataModel.getCcController(sensorId, ccSlot));
        ccSoloCmd.setSolo(true);
        sendMidiMessage(ccSoloCmd.createSysExMessage());
    }
    else if (message.compare(GLOBAL_EVT_BTN_CC_UNSOLO) == 0)
    {
        int sensorId = dataModel.getSelectedSensor();
        int ccSlot = dataModel.getSelectedCcSlot();
        CCSoloConfig ccSoloCmd(dataModel.getCcChannel(sensorId, ccSlot),
                               dataModel.getCcController(sensorId, ccSlot));
        ccSoloCmd.setSolo(false);
        sendMidiMessage(ccSoloCmd.createSysExMessage());
    }
}

void MainComponent::enableMidiSomi()
{
    // Check if SOMI-1 MIDI device was connected
    if (midiInputs == juce::MidiInput::getAvailableDevices() && midiOutputs == juce::MidiOutput::getAvailableDevices())
    {
        return;
    }
    
    // MIDI devices changed => update
    midiInputs = juce::MidiInput::getAvailableDevices();
    midiOutputs = juce::MidiOutput::getAvailableDevices();

    // Note: Currently first discovered SOMI-1 MIDI device is used!
    juce::String midiDevName = "N.A.";
    for (const auto& input : midiInputs)
    {
        if (input.name.containsIgnoreCase("SOMI-1")){
            if (!deviceManager.isMidiInputDeviceEnabled(input.identifier))
                deviceManager.setMidiInputDeviceEnabled(input.identifier, true);

            deviceManager.addMidiInputDeviceCallback(input.identifier, this);
            
            midiDevName = input.name;
            
            somiMidiIoInitialized = true;
            
            break;
        }
    }
    
    if (!somiMidiIoInitialized){
        std::wcerr << "Didn't found SOMI-1 MIDI input!" << std::endl;
        return;
    }
    
    somiMidiIoInitialized = false;
    
    for (const auto& output : midiOutputs){
        if (output.name.containsIgnoreCase("SOMI-1")){
            midiOutput = juce::MidiOutput::openDevice(output.identifier);

            somiMidiIoInitialized = true;
            
            break;
        }
    }
    
    if (!somiMidiIoInitialized){
        dataModel.setSomiHubAvailable(false);
        std::wcerr << "Didn't found SOMI-1 MIDI output!" << std::endl;
        return;
    }
    else{
        std::cout << "Enabled device " << midiDevName << " as MIDI port." << std::endl;
        dataModel.setSomiHubAvailable(true);
        
        // ATTENTION! Current configuration is polled from SOMI-1 hub, i.e. local settings are overwritten!
        requestSettingsFromSomi();
    }
}

void MainComponent::requestSettingsFromSomi()
{
    // The constructor arguments are just placeholders showing default values (except sensorId and slot index)
    for (int sensorId = 0; sensorId < MAX_NUM_SENSORS; sensorId++){
        for (int slotIdx=0; slotIdx < MAX_NUM_NOTE_MSG_PER_SENSOR; slotIdx++){
            MidiNoteConfig noteCfg(sensorId,
                                   false,
                                   slotIdx,
                                   sensorId,
                                   TILT_X,
                                   TILT_Z,
                                   36,
                                   60,
                                   127,
                                   0,
                                   0.5f,
                                   false,
                                   false);
            
            sendMidiMessage(noteCfg.createSysExMessage(true));
        }
        for (int slotIdx=0; slotIdx < MAX_NUM_CC_MSG_PER_SENSOR; slotIdx++){
            MidiControlChangeConfig ccCfg(sensorId,
                                          false,
                                          slotIdx,
                                          sensorId,
                                          TILT_X,
                                          16,
                                          1.0f,
                                          0.5f,
                                          0.0f,
                                          0.0f,
                                          false,
                                          false);
            
            sendMidiMessage(ccCfg.createSysExMessage(true));
        }
        for (int slotIdx=0; slotIdx < MAX_NUM_PB_MSG_PER_SENSOR; slotIdx++){
            MidiPitchBendConfig pbCfg(sensorId,
                                      false,
                                      slotIdx,
                                      sensorId,
                                      ACC_X,
                                      1.0f,
                                      false);
            
            sendMidiMessage(pbCfg.createSysExMessage(true));
        }
    }
}

void MainComponent::sendMidiMessage(const juce::MidiMessage& message)
{
    // Only transmit MIDI message if connection to SOMI-1 hub has been initialized
    if (!somiMidiIoInitialized || !midiOutput)
        return;
    
    midiOutput->sendMessageNow(message);
}

void MainComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    // Get activity value of specific sensor to update view.
    if (message.isController()){
        
        int sensorId = dataModel.getSensorIdFromMidiMsg(message);
        if (sensorId < 0 || sensorId >= MAX_NUM_SENSORS){
            std::cerr << "Didn't found sensor ID of incoming Control Change message!" << std::endl;
            return;
        }
        
        auto param = dataModel.getParamFromMidiMsg(message);
        if (param == ACTIVITY){
            
            // Note: For high resolution values, only high part is used.
            float val = UI_ACTIVITY_SCALE * static_cast<float>(message.getControllerValue()) / 127.0f;

            // Update view from MIDI thread via callback message
            (new EditorView::SensorActivityCallback(&editorView, sensorId, val))->post();
        }
    }
    
    if (message.isSysEx()){
        
        // Update data model state
        switch(SysExCmdMessage::getSysExCmd(message)){
            case SYSEX_CMD_CC_SOLO:
                break;
            case SYSEX_CMD_CC_UNSOLO:
                break;
            case SYSEX_CMD_NOTE_CFG:{
                MidiNoteConfig noteCfg(message);
                dataModel.update(noteCfg);
                break;
            }
            case SYSEX_CMD_CC_CFG:{
                MidiControlChangeConfig ccCfg(message);
                dataModel.update(ccCfg);
                break;
            }
            case SYSEX_CMD_PB_CFG:{
                MidiPitchBendConfig pbCfg(message);
                dataModel.update(pbCfg);
                break;
            }
            case SYSEX_CMD_PERSIST:
                break;
            case SYSEX_CMD_MAPPINGS_RESET:
                break;
            case SYSEX_CMD_DISCOVER:
                break;
            case SYSEX_CMD_SENSOR_CFG:
                break;
            case SYSEX_CMD_SENSORS_RESET:
                break;
            case SYSEX_CMD_DISABLE_SLOTS:
                break;
            case SYSEX_CMD_BATTERY_LEVEL:{
                BatteryLevelConfig batteryLevelCmd(message);
                dataModel.update(batteryLevelCmd);
                break;
            }
            case SYSEX_CMD_SENSOR_STATE:
                break;
            case SYSEX_CMD_UPDATE_SAMD:
                break;
            case SYSEX_CMD_UNKNOWN:
                std::cerr << "Unknown SysEx command " << SysExCmdMessage::getSysExCmd(message) << std::endl;
                break;
        }
    }
}

void MainComponent::timerCallback()
{
    // Continuously polls battery level
    // Future: Battery level is sent asynchronously from SOMI-1 hub.
    static unsigned int counter = 0;
        
    BatteryLevelConfig batteryLevelCmd(counter, 0);
    sendMidiMessage(batteryLevelCmd.createSysExMessage());
    
    // Check if new MIDI devices are available
    // TODO: Better to handle events, but doesn't seem to be available in JUCE for MIDI devices currently.
    enableMidiSomi();
    
    counter = (counter + 1) % MAX_NUM_SENSORS;
}
