#include "AgsSQLiteDB.h"
#include <cstdio>

static int redir_callback(void *instanceAgsSQLiteDB, int argc, char **argv, char **azColName)
{
	AgsSQLiteDB* myAgsSQLiteDB = reinterpret_cast<AgsSQLiteDB*>(instanceAgsSQLiteDB);
	return myAgsSQLiteDB->callback(argc, argv, azColName);
}

AgsSQLiteDB::AgsSQLiteDB(char* path) {
	OpenStatus = sqlite3_open(path, &db);
	
	if (OpenStatus) {
		snprintf(OpenStatusText, 300, "Error: Can't open database: %s\n", sqlite3_errmsg(db));
	}
	else {
		snprintf(OpenStatusText, 300, "Opened database successfully\n");
	}
}

char* AgsSQLiteDB::ExecuteQuery(char* query) {
	char* zErrMsg;

	QueryStatus = sqlite3_exec(db, query, redir_callback, this, &zErrMsg);
	if (QueryStatus != SQLITE_OK) {
		snprintf(QueryStatusText, 300, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		snprintf(QueryStatusText, 300, "Query executed successfully\n");
	}
}

int AgsSQLiteDB::IsOpen() {
	if (OpenStatus || db != NULL) {
		return false;
	}
	else {
		return true;
	}
}

void AgsSQLiteDB::Close() {

	if (OpenStatus || db != NULL) {
		sqlite3_close(db);
		if (db != NULL) {
			delete  db;
			db = NULL;
		}
	}
	OpenStatus = true;

}

void AgsSQLiteDB::ClearQueryStatus() {
	QueryStatus = -1;
	delete QueryStatusText;
	QueryStatusText = (char*) "";
}

int AgsSQLiteDB::callback(int argc, char **argv, char **azColName) {

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
