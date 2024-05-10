#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSoundStreamSettings settings;
	settings.setApi(ofSoundDevice::Api::MS_DS);
	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::MS_DS);
	for (int i = 0; i < devices.size(); ++i) {
		std::cout << devices[i] << "\n";
	}

	settings.setOutDevice(devices[0]);
	settings.setInDevice(devices[0]);
	settings.setOutListener(this);
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.sampleRate = 44100;
	settings.bufferSize = 1024;
	settings.numBuffers = 4;
	soundStream.setup(settings);

	vert_pnt_1 = { 0, 0, 0 };
	vert_pnt_2 = { 0, 1, 0 };
	std::cout << conv3dCoord.distance(vert_pnt_1, vert_pnt_2) << std::endl;
	std::cout << conv3dCoord.angle(vert_pnt_1, vert_pnt_2) * 180 / PI << std::endl;
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) 
{
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& buffer)
{
	//pan = 0.5f;
	float leftScale = 1 - 0.5;
	float rightScale = 0.5;

	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	while (phaseSineTone > TWO_PI) 
	{
		phaseSineTone -= TWO_PI;
	}

	while (phaseDistance > TWO_PI)
	{
		phaseDistance -= TWO_PI;
	}
	
	for (size_t i = 0; i < buffer.getNumFrames(); i++) {
		phaseSineTone += 220.0 / 44100.0 * TWO_PI;
		sample = sin(phaseSineTone);

		degree += 1.0 / 44100.0 * 360.0;

		phaseDistance += 0.5 / 44100.0 * TWO_PI;
		distance = sin(phaseDistance) * 20;

		buffer[i * buffer.getNumChannels() + 0] = ambiDec2DThirdOrder.play(ambiEnc2DThirdOrder.play(sample, degree, distance), 90);
		buffer[i * buffer.getNumChannels() + 1] = ambiDec2DThirdOrder.play(ambiEnc2DThirdOrder.play(sample, degree, distance), 270);

		/*
		buffer[i * buffer.getNumChannels() + 0] = sample * volume * leftScale;
		buffer[i * buffer.getNumChannels() + 1] = sample * volume * rightScale;
		buffer[i * buffer.getNumChannels() + 2] = sample * volume * leftScale;
		buffer[i * buffer.getNumChannels() + 3] = sample * volume * rightScale;
		buffer[i * buffer.getNumChannels() + 4] = sample * volume * leftScale;
		buffer[i * buffer.getNumChannels() + 5] = sample * volume * rightScale;
		buffer[i * buffer.getNumChannels() + 6] = sample * volume * leftScale;
		buffer[i * buffer.getNumChannels() + 7] = sample * volume * rightScale;
		*/
	}
}