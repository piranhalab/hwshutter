#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
  	ofSetLogLevel(OF_LOG_VERBOSE);
  	ofSetLogLevel("ofThread", OF_LOG_ERROR);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
	printer.open("/dev/serial0");
	buffer = ofBufferFromFile("txt/texto.txt"); // reading into the buffer
	frases = ofSplitString(buffer.getText(), "-");
	
	consoleListener.setup(this);
		
	omxCameraSettings.width = 1024;
	omxCameraSettings.height = 1024;
	omxCameraSettings.framerate = 30;
	omxCameraSettings.enableTexture = true;
	
	videoGrabber.setup(omxCameraSettings);
	//filterCollection.setup();

	doShader = true;
	shader.load("shaderExample");
        img.load("imagen.jpg"); 
	fbo.allocate(omxCameraSettings.width, omxCameraSettings.height);
	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();

	//	font.load("fonts/DejaVuSansMono.ttf", 30);
	//snapShot = false;
	//clientTyping = "LOS HOMBRES TIENEN QUE REPENSAR SU MASCULINIDAD O, MEJOR AÚN, SU IDENTIDAD Y SU MANERA DE RELACIONARSE CON LAS MUJERES EN ESTOS ALBORES DEL SIGLO XXI";

	gpio18.setup("17");
	gpio18.export_gpio();
	gpio18.setdir_gpio("in");
	imprime = 0;
	
}	

//--------------------------------------------------------------
void ofApp::update()
{
  if (!doShader || !videoGrabber.isFrameNew())
    {
      return;
    }
  fbo.begin();
  ofClear(0, 0, 0, 0);
  shader.begin();  
  shader.setUniformTexture("tex0", videoGrabber.getTextureReference(), videoGrabber.getTextureID());
  shader.setUniform1f("time", ofGetElapsedTimef());
  shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
  videoGrabber.draw();
  shader.end();
  fbo.end();

  gpio18.getval_gpio(imprime);
  
  if(imprime == 1){
    
    snapShot = true;
//    ofSleepMillis(4000);

  }
  
}


//--------------------------------------------------------------
void ofApp::draw(){

  ofSetColor(255);
  if (doShader)
    {
      fbo.draw(0, 0);		
    }else 
    {
      videoGrabber.draw();
    }

  text = wrapString(clientTyping, 700);
  rect = font.getStringBoundingBox(text, 0, 0);

  //ofDrawRectangle(0, 0, ofGetWidth(), rect.height+20);  

  //ofRectagle rectangulo(0, 0, rect.width(), rect.height());
  //ofSetColor(0);
  //font.drawString(text, (ofGetWidth()*0.5)-(rect.width*0.5), -rect.height/4);

  ofSetColor(255);  
  //ofsetColor(255); 
  
  if (snapShot == true){
    
    int random;
    random = ofRandom(frases.size()); 

    img.grabScreen(0,0, ofGetWidth(),ofGetHeight());
	  
    string fileName = "imagen.jpg";
    //    img.save(fileName, OF_IMAGE_QUALITY_BEST);
    img.setImageType(OF_IMAGE_GRAYSCALE);   // now I am grayscale;
    img.resize(400, 400);
    img.rotate90(2);
    //snapShot = false;
    //    img2.load("imagen.jpg");
    setBrightness(img, 60.0f);
    setContrast(img, 120.0f);
    img.save(fileName, OF_IMAGE_QUALITY_BEST);
    printer.setUpDown(true);
//    printer.setReverse(true);

    string fraseAleatoria; 
    fraseAleatoria = frases[random];
    std::vector < std::string > textAnalisis = ofSplitString(fraseAleatoria, "#");
    std::reverse(textAnalisis.begin(), textAnalisis.end());
    for(int i = 0; i < textAnalisis.size(); i++){
      //for(size_t i = textAnalisis.size(); i--; ){
      //    ofStringReplace(textAnalisis[i], "\"", "\x22");
      //    ofStringReplace(textAnalisis[i], "Anzaldúa", "Anzaldaaa");
    ofStringReplace(textAnalisis[i], "á", "\xA0");
    ofStringReplace(textAnalisis[i], "é", "\x82");
    ofStringReplace(textAnalisis[i], "í", "\xA1");
    ofStringReplace(textAnalisis[i], "ó", "\xA2");
    ofStringReplace(textAnalisis[i], "ú", "\xA3");
    ofStringReplace(textAnalisis[i], "à", "\x85");
    //ofStringReplace(textAnalisis[i], "\61", "a");
//    ofStringReplace(textAnalisis[i], "u", "\x75");
    ofStringReplace(textAnalisis[i], "ñ", "\xA4");
    ofStringReplace(textAnalisis[i], "¿", "\xA8");
    printer.print(textAnalisis[i]);
    //ofSleepMillis(500);
    }
    //printer.println(frases[random]);
//    printer.println("\x23"+"Tercer Pa"+"\xA1"+"s, la frontera"+"\x23"+" Gloria Anzald"+"\xA3"+"a");
//    string hola1;
//    string hola2; 
//    string hola3;
//    hola1 = "\x22No volver\x82 a ser avergonzada ni";
//    hola2 = "me avergonzar\x82 de m\xA1 misma\x22";
//    hola3 = "Gloria Anzald\xA3\x61";
//    hola = "\x22No volver\x82 a ser avergonzada ni me avergonzar\x82 de m\xA1 misma\x22 Gloria Anzald\xA3\x61";
    //hola = "adaznogreva";
//    printer.println(hola3);
//    printer.println(hola2);
//    printer.println(hola1);
//    printer.println(" ");
//    printer.print(img, 127);
    ofSleepMillis(1000);
//    printer.println(" ");
//    printer.println(" ");
//    printer.println(" ");
//    printer.println("--------------------------------");
//    printer.println(" ");
//    printer.println(" ");
//    printer.println(" ");
    printer.println(" ");
    snapShot = false;
  }

  img.draw(0, 0);
  //img.resize(300, 180);
     
}

//--------------------------------------------------------------

void ofApp::keyPressed  (int key)
{
  if(key ==  'c'){
    snapShot = true;
    //img.resize(350, 300);
  } else if (key == 'p'){
    //img.resize(400, 400);
    printer.println("HABRá PLENA IGUALDAD DE GÉNERO CUANDO SE REIVINDIQUEN A CABALIDAD LOS DERECHOS DE LAS MUJERES, PERO HOY TIENE QUE HABER UNA NUEVA MASCULINIDAD");
    printer.println(" ");
//    printer.print(img, 127);
    ofSleepMillis(14000);
    printer.println(" ");
    printer.println(" ");
    printer.println(" ");
    printer.println("14 segundos después");

  } else if (key == 's'){
    printer.println(" ");
    printer.println(" ");
  }
}


void ofApp::onCharacterReceived(KeyListenerEventData& e)
{
	keyPressed((int)e.character);
}

void ofApp::setBrightness(ofImage &image, const int brightness) {

  int numChannels;

  switch (image.getImageType()) {

  case OF_IMAGE_GRAYSCALE:
    numChannels = 1;
    break;
  case OF_IMAGE_COLOR:
    numChannels = 3;
    break;
  case OF_IMAGE_COLOR_ALPHA:
    numChannels = 4;
    break;
  default:
    break;
  }
  ofPixels &pix = image.getPixels();
  const size_t pixSize = static_cast<size_t>(image.getWidth() * image.getHeight() * numChannels);

  if (numChannels == 1) {

    for (size_t i=0; i<pixSize; ++i) {

      const int g = pix[i] + brightness;
      pix[i] = static_cast<unsigned char>(g < 0 ? 0 : g > 255 ? 255 : g);
    }
  }
  else {

    for (size_t i=0; i<pixSize; i+=numChannels) {

      const int r = pix[i] + brightness;
      const int g = pix[i+1] + brightness;
      const int b = pix[i+2] + brightness;
      pix[i] = static_cast<unsigned char>(r < 0 ? 0 : r > 255 ? 255 : r);
      pix[i+1] = static_cast<unsigned char>(g < 0 ? 0 : g > 255 ? 255 : g);
      pix[i+2] = static_cast<unsigned char>(b < 0 ? 0 : b > 255 ? 255 : b);
    }
  }
  image.update();
}

void ofApp::setContrast(ofImage &image, const int contrast) {

  int numChannels;

  switch (image.getImageType()) {

  case OF_IMAGE_GRAYSCALE:
    numChannels = 1;
    break;
  case OF_IMAGE_COLOR:
    numChannels = 3;
    break;
  case OF_IMAGE_COLOR_ALPHA:
    numChannels = 4;
    break;
  default:
    break;
  }
  ofPixels &pix = image.getPixels();
  const size_t pixSize = static_cast<size_t>(image.getWidth() * image.getHeight() * numChannels);
  const float factor = (259.0f * (contrast + 255)) / (255.0f * (259 - contrast));

  if (numChannels == 1) {

    for (size_t i=0; i<pixSize; ++i) {

      const int g = static_cast<int>(factor * (pix[i] - 128) + 128);
      pix[i] = static_cast<unsigned char>(g < 0 ? 0 : g > 255 ? 255 : g);
    }
  }
  else {

    for (size_t i=0; i<pixSize; i+=numChannels) {

      const int r = static_cast<int>(factor * (pix[i] - 128) + 128);
      const int g = static_cast<int>(factor * (pix[i+1] - 128) + 128);
      const int b = static_cast<int>(factor * (pix[i+2] - 128) + 128);
      pix[i] = static_cast<unsigned char>(r < 0 ? 0 : r > 255 ? 255 : r);
      pix[i+1] = static_cast<unsigned char>(g < 0 ? 0 : g > 255 ? 255 : g);
      pix[i+2] = static_cast<unsigned char>(b < 0 ? 0 : b > 255 ? 255 : b);
    }
  }
  image.update();
}
