#include "AgsSQLiteDB.h"

AgsSQLiteDB::AgsSQLiteDB(char* path) {

}

char* AgsSQLiteDB::ExecuteQuery(char* query) {

}

int AgsSQLiteDB::IsOpen() {

}

void AgsSQLiteDB::Close() {

}

void AgsSQLiteDB::ClearResultStatus() {

}

int AgsSQLiteDB::callback(void *NotUsed, int argc, char **argv, char **azColName) {

}

AgsSQLiteDB::~AgsSQLiteDB(void)
{

}

// -------------

extern IAGSEngine* engine;

AgsSQLiteDBInterface AgsSQLiteDB_Interface;
AgsSQLiteDBReader AgsSQLiteDB_Reader;

const char* AgsSQLiteDBInterface::name = "AgsSQLiteDB";

//------------------------------------------------------------------------------

int AgsSQLiteDBInterface::Dispose(const char* address, bool force)
{
	delete ((AgsSQLiteDB*)address);
	return (1);
}

//------------------------------------------------------------------------------

int AgsSQLiteDBInterface::Serialize(const char* address, char* buffer, int bufsize)
{
	AgsSQLiteDB* arr = (AgsSQLiteDB*)address;
	char* ptr = buffer;

	
	return (ptr - buffer);
}

//------------------------------------------------------------------------------

void AgsSQLiteDBReader::Unserialize(int key, const char* serializedData, int dataSize)
{
	AgsSQLiteDB* arr = new AgsSQLiteDB((char*)"");

	const char* ptr = serializedData;

	engine->RegisterUnserializedObject(key, arr, &AgsSQLiteDB_Interface);
}

//..............................................................................
