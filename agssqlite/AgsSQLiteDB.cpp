#include "AgsSQLiteDB.h"
#include <cstdio>
#include <string.h>
#include <vector>

static int redir_callback(void *instanceAgsSQLiteDB, int argc, char **argv, char **azColName)
{
	AgsSQLiteDB* myAgsSQLiteDB = reinterpret_cast<AgsSQLiteDB*>(instanceAgsSQLiteDB);
	return myAgsSQLiteDB->callback(argc, argv, azColName);
}

AgsSQLiteDB::AgsSQLiteDB(char* path) {
	OpenStatus = sqlite3_open(path, &db);

    if (OpenStatus)
    {
        int str_size = snprintf(nullptr, 0, "Error: Can't open database: %s\n", sqlite3_errmsg(db));
        OpenStatusText.resize(str_size + 1);
        snprintf(&OpenStatusText[0], str_size + 1, "Error: Can't open database: %s\n", sqlite3_errmsg(db));
        Path = "";
    }
    else
    {
        OpenStatusText = "Opened database successfully\n";
        Path = path;
    }

}

int AgsSQLiteDB::ExecuteQuery(char* query) {
	char* zErrMsg;

    QueryStatusText = "";

	QueryResult.clear();

	QueryStatus = sqlite3_exec(db, query, redir_callback, this, &zErrMsg);
	if (QueryStatus != SQLITE_OK) {
        int str_size = snprintf(nullptr, 0, "SQL error: %s\n", zErrMsg);
        QueryStatusText.resize(str_size + 1);
        snprintf(&QueryStatusText[0], str_size + 1, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
    {
        QueryStatusText = "Query executed successfully\n";
	}

	return QueryStatus;
}

int AgsSQLiteDB::IsOpen() {
	if (OpenStatus || db != nullptr) {
		return false;
	}
	else {
		return true;
	}
}

void AgsSQLiteDB::Close() {
	if (OpenStatus || db != nullptr) {
		sqlite3_close(db);
	}
	OpenStatus = true;
}

void AgsSQLiteDB::ClearQueryStatus() {
	QueryStatus = -1;
	QueryStatusText = "";
}

int AgsSQLiteDB::callback(int argc, char **argv, char **azColName) {	
	int i;
	int str_size = 0;
	for (i = 0; i < argc; i++) {
		str_size += snprintf(nullptr, 0, "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	str_size += snprintf(nullptr, 0, "\n");

	char* TempStr = new char[str_size + 1];

	char* cur = TempStr;
	const char* end = cur + str_size;

	for (i = 0; i < argc; i++) {
		cur += snprintf(cur, end-cur, "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	cur += snprintf(cur, end-cur, "\n");

	QueryResult.append(TempStr);

	return 0;
}

AgsSQLiteDB::~AgsSQLiteDB(void)
{

}

// -------------

extern IAGSEngine* engine;

AgsSQLiteDBInterface AgsSQLiteDB_Interface;
AgsSQLiteDBReader AgsSQLiteDB_Reader;

const char* AgsSQLiteDBInterface::name = "AgsSQLite";

//------------------------------------------------------------------------------

int AgsSQLiteDBInterface::Dispose(void *address, bool force) {
    ((AgsSQLiteDB*)address)->Close();
    delete ((AgsSQLiteDB*)address);
    return (1);
}

//------------------------------------------------------------------------------

int AgsSQLiteDBInterface::Serialize(void *address, char *buffer, int bufsize) {
    AgsSQLiteDB const* agsSQLiteDB = (AgsSQLiteDB*)address;
    int path_length = (*agsSQLiteDB).Path.length();

    memcpy(buffer, (*agsSQLiteDB).Path.c_str(), path_length);
    return sizeof(path_length);
}

//------------------------------------------------------------------------------

void AgsSQLiteDBReader::Unserialize(int key, const char* serializedData, int dataSize)
{
    std::vector<char> path(serializedData, serializedData + dataSize);

	AgsSQLiteDB* agsSQLiteDB = new AgsSQLiteDB(path.data());

	engine->RegisterUnserializedObject(key, agsSQLiteDB, &AgsSQLiteDB_Interface);
}

//..............................................................................
