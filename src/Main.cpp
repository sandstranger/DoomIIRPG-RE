#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include "SDLGL.h"
#include "ZipFile.h"

#include "CAppContainer.h"
#include "App.h"
#include "Image.h"
#include "Resource.h"
#include "Render.h"
#include "GLES.h"

#include "Canvas.h"
#include "Graphics.h"
#include "Player.h"
#include "Game.h"
#include "Graphics.h"
#include "Utils.h"
#include "TinyGL.h"
#include "Input.h"
#ifdef ANDROID
#include <SDL_main.h>
#include <jni.h>
#endif

void drawView(SDLGL* sdlGL);

#ifdef ANDROID
int SDL_main(int argc, char **argv) {
#else
int main(int argc, char* args[]) {
#endif

#ifdef ANDROID
    chdir(getenv("ANDROID_GAME_PATH"));
#endif

    int		UpTime = 0;
    
    ZipFile zipFile;
#ifdef ANDROID
    zipFile.openZipFile(getenv("RESOURCE_FILE_NAME"));
#else
    zipFile.openZipFile("Doom 2 RPG.ipa");
#endif

	SDLGL sdlGL;
	sdlGL.Initialize();

    Input input;
    input.init(); // [GEC] Port: set default Binds

    CAppContainer::getInstance()->Construct(&sdlGL, &zipFile);
    sdlGL.updateVideo(); // [GEC]

    SDL_Event ev;
    

    float x = 0.0f, y = 30.0f;
    int   vp_cx = 480;
    int   vp_cy = 320;


    Uint8 state;
    int mX, mY;           /* mouse location*/
    float mousePressX = 0.f, mousePressY = 0.f;           /* mouse location float*/
    int winVidWidth = sdlGL.winVidWidth;
    int winVidHeight = sdlGL.winVidHeight;
    bool useMouse = false;
   
    

    while (CAppContainer::getInstance()->app->closeApplet != true) {

        int currentTimeMillis = CAppContainer::getInstance()->getTimeMS();

        //if (!useMouse) {
            //float ax = SDL_GameControllerGetAxis(sdlGL.accelerometer, SDL_CONTROLLER_AXIS_LEFTX);
            //float ay = SDL_GameControllerGetAxis(sdlGL.accelerometer, SDL_CONTROLLER_AXIS_LEFTY);
            //CAppContainer::getInstance()->UpdateAccelerometer((ax* (1.0f / 32767))/2, (ay* (1.0f / 32767))/2, 0.f, false);
        //}

        if (currentTimeMillis > UpTime) {
            input.handleEvents();
            UpTime = currentTimeMillis + 15;
            drawView(&sdlGL);
            input.consumeEvents();
        }
    }

    printf("APP_QUIT\n");
    CAppContainer::getInstance()->~CAppContainer();
    zipFile.closeZipFile();
    sdlGL.~SDLGL();
    input.~Input();
	return 0;
}


static uint32_t lastTimems = 0;

void drawView(SDLGL *sdlGL) {

    int cx, cy;
    int w = sdlGL->vidWidth;
    int h = sdlGL->vidHeight;

    if (lastTimems == 0) {
        lastTimems = CAppContainer::getInstance()->getTimeMS();
    }

    SDL_GetWindowSize(sdlGL->window, &cx, &cy);
    if (w != cx || h != cy) {
        w = cx; h = cy;
    }

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, Applet::IOS_WIDTH, Applet::IOS_HEIGHT, 0.0, -1.0, 1.0);
    //glRotatef(90.0, 0.0, 0.0, 1.0);
    //glTranslatef(0.0, -320.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    

    uint32_t startTime = CAppContainer::getInstance()->getTimeMS();
    uint32_t passedTime = startTime - lastTimems;
    lastTimems = startTime;

    if (passedTime >= 125) {
        passedTime = 125;
    }
    //printf("passedTime %d\n", passedTime);

    CAppContainer::getInstance()->DoLoop(passedTime);

    SDL_GL_SwapWindow(sdlGL->window);  // Swap the window/pBmp to display the result.
    
}

#ifdef ANDROID
extern "C" {
JNIEXPORT void JNICALL Java_com_mobilerpgpack_phone_engine_activity_EngineActivity_resumeSound(JNIEnv *env, jobject thisObject) {
    CAppContainer::getInstance()->resumeOpenAL();
}

JNIEXPORT void JNICALL Java_com_mobilerpgpack_phone_engine_activity_EngineActivity_pauseSound(JNIEnv *env, jobject thisObject) {
    CAppContainer::getInstance()->suspendOpenAL();
}
}

extern "C" {
JNIEXPORT jboolean JNICALL Java_com_mobilerpgpack_phone_engine_activity_EngineActivity_needToShowScreenControls(JNIEnv *env, jobject thisObject) {
    CAppContainer *appContainer = CAppContainer::getInstance();
    if (appContainer == nullptr || appContainer->app == nullptr || appContainer->app->canvas == nullptr){
        return true;
    }
    int currentCanvasState = appContainer->app->canvas->state;
    return currentCanvasState ==Canvas::ST_PLAYING || currentCanvasState ==Canvas::ST_COMBAT ||
    currentCanvasState == Canvas::ST_CHARACTER_SELECTION;
}
}
#endif
