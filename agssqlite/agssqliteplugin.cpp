#pragma region Defines_and_Includes

#include "core/platform.h"

#define MIN_EDITOR_VERSION 1
#define MIN_ENGINE_VERSION 3

#if AGS_PLATFORM_OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <cstring>

#if !defined(BUILTIN_PLUGINS)
#define THIS_IS_THE_PLUGIN
#endif

#include "plugin/agsplugin.h"

#if defined(BUILTIN_PLUGINS)
namespace agssqlite {
#endif

#if !AGS_PLATFORM_OS_WINDOWS
#define min(x,y) (((x) < (y)) ? (x) : (y))
#define max(x,y) (((x) > (y)) ? (x) : (y))
#endif

#if AGS_PLATFORM_OS_WINDOWS
	// The standard Windows DLL entry point

	BOOL APIENTRY DllMain(HANDLE hModule,
		DWORD ul_reason_for_call,
		LPVOID lpReserved) {

		switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		}
		return TRUE;
	}
#endif


//define engine
IAGSEngine* engine;

#include "AgsSQLiteDB.h"


AgsSQLiteDB* AgsSQLite_Open(const char* database_path) {
	AgsSQLiteDB* agsSQLiteDB = new AgsSQLiteDB((char*) database_path);

	return agsSQLiteDB;
}

bool AgsSQLite_IsOpen(AgsSQLiteDB* self) {
	return self->IsOpen();
}

const char* AgsSQLite_GetDBPath(AgsSQLiteDB* self) {
	return engine->CreateScriptString(self->Path);
}

int AgsSQLite_ExecuteQuery(AgsSQLiteDB* self, const char* query) {
	return self->ExecuteQuery((char*) query);
}

int AgsSQLite_GetQueryStatus(AgsSQLiteDB* self) {
	return self->QueryStatus;
}

const char* AgsSQLite_GetQueryStatusText(AgsSQLiteDB* self) {
	return engine->CreateScriptString(self->QueryStatusText);
}

const char* AgsSQLite_GetQueryResult(AgsSQLiteDB* self) {
	return engine->CreateScriptString(self->QueryResult);
}

void AgsSQLite_ClearResultStatus(AgsSQLiteDB* self) {
	self->ClearQueryStatus();
}

void AgsSQLite_Close(AgsSQLiteDB* self) {
	self->Close();
}



#if AGS_PLATFORM_OS_WINDOWS

//==============================================================================

// ***** Design time *****

IAGSEditor* editor; // Editor interface

const char* ourScriptHeader =
"enum SQLiteQueryStatus { \r\n"
"  eSQLiteQueryNotRun=0, \r\n"
"  eSQLiteQueryRunning=1, \r\n"
"  eSQLiteQueryFinished=2, \r\n"
"}; \r\n"
" \r\n"
"struct AgsSQLite { \r\n"
"  /// Opens a SQLite DB and returns error message \r\n"
"  static AgsSQLite* Open(String db); // $AUTOCOMPLETESTATICONLY$\r\n"
"   \r\n"
"  /// Returns the path from the open DB \r\n"
"  String GetDBPath(); \r\n"
"  /// Returns true if connection is open \r\n"
"  bool IsOpen(); \r\n"
"   \r\n"
"  /// Run a query on previously opened DB \r\n"
"  String ExecuteQuery(String query); \r\n"
"   \r\n"
"  /// Get the status of currently executed query \r\n"
"  SQLiteQueryStatus GetQueryStatus(); \r\n"
"   \r\n"
"  /// Get the results from the previously executed query \r\n"
"  String GetQueryResult(); \r\n"
"   \r\n"
"  /// Clear query results. They are also cleared when a new query is executed or connection is closed. \r\n"
"  String ClearResultStatus(); \r\n"
"   \r\n"
"  /// Close DB connection.  \r\n"
"  void Close(); \r\n"
"}; \r\n";

//------------------------------------------------------------------------------

LPCSTR AGS_GetPluginName()
{
	return ("agssqlite");
}

//------------------------------------------------------------------------------

int AGS_EditorStartup(IAGSEditor* lpEditor)
{
	// User has checked the plugin to use it in their game

	// If it's an earlier version than what we need, abort.
	if (lpEditor->version < MIN_EDITOR_VERSION)
		return (-1);

	editor = lpEditor;
	editor->RegisterScriptHeader(ourScriptHeader);

	// Return 0 to indicate success
	return (0);
}

//------------------------------------------------------------------------------

void AGS_EditorShutdown()
{
	// User has un-checked the plugin from their game
	editor->UnregisterScriptHeader(ourScriptHeader);
}

//------------------------------------------------------------------------------

void AGS_EditorProperties(HWND parent)                        //*** optional ***
{
	// User has chosen to view the Properties of the plugin
	// We could load up an options dialog or something here instead
/*	MessageBox(parent,
			 L"agsfastwfc v1.0 By Calin Leafshade",
			 L"About",
		 MB_OK | MB_ICONINFORMATION);
 */
}

//------------------------------------------------------------------------------

int AGS_EditorSaveGame(char* buffer, int bufsize)             //*** optional ***
{
	// Called by the editor when the current game is saved to disk.
	// Plugin configuration can be stored in [buffer] (max [bufsize] bytes)
	// Return the amount of bytes written in the buffer
	return (0);
}

//------------------------------------------------------------------------------

void AGS_EditorLoadGame(char* buffer, int bufsize)            //*** optional ***
{
	// Called by the editor when a game is loaded from disk
	// Previous written data can be read from [buffer] (size [bufsize]).
	// Make a copy of the data, the buffer is freed after this function call.
}

//==============================================================================

#endif

// ***** Run time *****

// Engine interface

//------------------------------------------------------------------------------



#define REGISTER(x) engine->RegisterScriptFunction(#x, (void *) (x));
#define REG_CLASS(c,x,a) engine->RegisterScriptFunction(#c "::" #x "^" #a, (void *) (c ## _ ## x));
#define REG_ATTR(c,x) engine->RegisterScriptFunction(#c "::get_" #x, (void *) (c ## _Get ## x)); \
                      engine->RegisterScriptFunction(#c "::set_" #x, (void *) (c ## _Set ## x));
#define REG_ARR(c,x) engine->RegisterScriptFunction(#c "::geti_" #x, (void *) (c ## _Get ## x)); \
                     engine->RegisterScriptFunction(#c "::seti_" #x, (void *) (c ## _Set ## x));

#define STRINGIFY(s) STRINGIFY_X(s)
#define STRINGIFY_X(s) #s

void AGS_EngineStartup(IAGSEngine* lpEngine)
{
	engine = lpEngine;

	// Make sure it's got the version with the features we need
	if (engine->version < MIN_ENGINE_VERSION)
		engine->AbortGame("Plugin needs engine version " STRINGIFY(MIN_ENGINE_VERSION) " or newer.");
	
	engine->RegisterScriptFunction("AgsSQLite::Open^1", (void*)AgsSQLite_Open);
	engine->RegisterScriptFunction("AgsSQLite::IsOpen^0", (void*)AgsSQLite_IsOpen);
	engine->RegisterScriptFunction("AgsSQLite::GetDBPath^0", (void*)AgsSQLite_GetDBPath);
	engine->RegisterScriptFunction("AgsSQLite::ExecuteQuery^1", (void*)AgsSQLite_ExecuteQuery);
	engine->RegisterScriptFunction("AgsSQLite::GetQueryStatus^0", (void*)AgsSQLite_GetQueryStatus);
	engine->RegisterScriptFunction("AgsSQLite::GetQueryStatusText^0", (void*)AgsSQLite_GetQueryStatusText);
	engine->RegisterScriptFunction("AgsSQLite::GetQueryResult^0", (void*)AgsSQLite_GetQueryResult);
	engine->RegisterScriptFunction("AgsSQLite::ClearResultStatus^0", (void*)AgsSQLite_ClearResultStatus);
	engine->RegisterScriptFunction("AgsSQLite::Close^0", (void*)AgsSQLite_Close);
}

//------------------------------------------------------------------------------

void AGS_EngineShutdown()
{
	// Called by the game engine just before it exits.
	// This gives you a chance to free any memory and do any cleanup
	// that you need to do before the engine shuts down.
}

//------------------------------------------------------------------------------

int AGS_EngineOnEvent(int event, int data)                    //*** optional ***
{
	switch (event)
	{
		/*
			case AGSE_KEYPRESS:
			case AGSE_MOUSECLICK:
			case AGSE_POSTSCREENDRAW:
			case AGSE_PRESCREENDRAW:
			case AGSE_SAVEGAME:
			case AGSE_RESTOREGAME:
			case AGSE_PREGUIDRAW:
			case AGSE_LEAVEROOM:
			case AGSE_ENTERROOM:
			case AGSE_TRANSITIONIN:
			case AGSE_TRANSITIONOUT:
			case AGSE_FINALSCREENDRAW:
			case AGSE_TRANSLATETEXT:
			case AGSE_SCRIPTDEBUG:
			case AGSE_SPRITELOAD:
			case AGSE_PRERENDER:
			case AGSE_PRESAVEGAME:
			case AGSE_POSTRESTOREGAME:
		 */
	default:
		break;
	}

	// Return 1 to stop event from processing further (when needed)
	return (0);
}

//------------------------------------------------------------------------------

int AGS_EngineDebugHook(const char* scriptName,
	int lineNum, int reserved)            //*** optional ***
{
	// Can be used to debug scripts, see documentation
	return 0;
}

//------------------------------------------------------------------------------

void AGS_EngineInitGfx(const char* driverID, void* data)      //*** optional ***
{
	// This allows you to make changes to how the graphics driver starts up.
	// See documentation
}

//..............................................................................


#if defined(BUILTIN_PLUGINS)
}
#endif