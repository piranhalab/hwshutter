#include "ofMain.h"
#include "ofApp.h"

int main()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofGLESWindowSettings settings;
    settings.setSize(1024, 1024);
    settings.setGLESVersion(2);
    ofCreateWindow(settings);
    ofRunApp( new ofApp());
}
