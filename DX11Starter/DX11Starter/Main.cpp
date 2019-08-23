//#define DEFAULT
#define TYRIAN2000
//#define TETRIS
#if !defined DEFAULT && !defined TYRIAN2000 && !defined TETRIS
#define DEFAULT
#endif // !DEFAULT && !defined TYRIAN2000 && !defined TETRIS


#ifdef DEFAULT
#include "DefaultScene.h"
#elif defined TYRIAN2000
#include "Tyrian2000.h"
#elif defined TETRIS
#include "TetrisScene.h"
#endif

#include <Windows.h>
#include "CosmicEngine.h"
#include "EngineManager.h"

// --------------------------------------------------------
// Entry point for a graphical (non-console) Windows application
// --------------------------------------------------------
int WINAPI WinMain(
	HINSTANCE hInstance,		// The handle to this app's instance
	HINSTANCE hPrevInstance,	// A handle to the previous instance of the app (always NULL)
	LPSTR lpCmdLine,			// Command line params
	int nCmdShow)				// How the window should be shown (we ignore this)
{
#if defined(DEBUG) | defined(_DEBUG)
	// Enable memory leak detection as a quick and dirty
	// way of determining if we forgot to clean something up
	//  - You may want to use something more advanced, like Visual Leak Detector
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Ensure "Current Directory" (relative path) is always the .exe's folder
	// - Without this, the relative path is different when running through VS
	//    and when running the .exe directly, which makes it a pain to load files
	//    - Running through VS: Current Dir is the *project folder*
	//    - Running from .exe:  Current Dir is the .exe's folder
	// - This has nothing to do with DEBUG and RELEASE modes - it's purely a 
	//    Visual Studio "thing", and isn't obvious unless you know to look 
	//    for it.  In fact, it could be fixed by changing a setting in VS, but
	//    the option is stored in a user file (.suo), which is ignored by most
	//    version control packages by default.  Meaning: the option must be
	//    changed every on every PC.  Ugh.  So instead, I fixed it here.
	// - This is a new change this year to simplify a long-standing headache.  
	//    If it breaks something on your end, feel free to comment this section out
	{
		// Get the real, full path to this executable, end the string before
		// the filename itself and then set that as the current directory
		char currentDir[1024] = {};
		GetModuleFileName(0, currentDir, 1024);
		char* lastSlash = strrchr(currentDir, '\\');
		if (lastSlash)
		{
			*lastSlash = 0; // End the string at the last slash character
			SetCurrentDirectory(currentDir);
		}
	}

	// Create the Game object using
	// the app handle we got from WinMain
	CosmicEngine cosmic(hInstance);

	// Result variable for function calls below
	HRESULT hr = S_OK;

	// Attempt to create the window for our program, and
	// exit early if something failed
	hr = cosmic.InitWindow();
	if(FAILED(hr)) return hr;

	// Attempt to initialize DirectX, and exit
	// early if something failed
	hr = cosmic.InitDirectX();
	if(FAILED(hr)) return hr;



#ifdef DEFAULT
	DefaultScene* defaultLevel = new DefaultScene();
	cosmic.LoadDefaultScene(defaultLevel);
#elif defined TYRIAN2000
	Tyrian2000* tyrian = new Tyrian2000();
	cosmic.LoadDefaultScene(tyrian);
#elif defined TETRIS
	TetrisScene* tetris = new TetrisScene();
	cosmic.LoadDefaultScene(tetris);
#endif
	EngineManager::GetInstance();
	EngineManager::SetUp(&cosmic);
	// Begin the message and game loop, and then return
	// whatever we get back once the game loop is over
	HRESULT end = cosmic.Run();
	EngineManager::DestroyInstance();
	return end;
}
