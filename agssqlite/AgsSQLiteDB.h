#pragma once

#ifndef _AGSSQLITEDB_H
#define _AGSSQLITEDB_H

#include "plugin/agsplugin.h"
#include "sqlite3.h"
#include <string>

class AgsSQLiteDB {
public:
	sqlite3 *db;
	AgsSQLiteDB(char* path);
	~AgsSQLiteDB(void);
	int ExecuteQuery(char* query);
	int IsOpen();
	void Close();
	void ClearQueryStatus();
    std::string OpenStatusText;
	int OpenStatus;
    std::string QueryStatusText;
	int QueryStatus;
    std::string Path;
	std::string QueryResult;
	int callback(int argc, char **argv, char **azColName);
};

//------------------------------------------------------------------------------
// AGS interface instances

class AgsSQLiteDBInterface : public IAGSScriptManagedObject
{
public:
    static const char* name;
	AgsSQLiteDBInterface() {};

    int Dispose(void *address, bool force) final;
    const char* GetType() final { return name; }
	int Serialize(void *address, char *buffer, int bufsize) final;
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

class AgsSQLiteDBReader : public IAGSManagedObjectReader
{
public:

	AgsSQLiteDBReader() {}

	virtual void Unserialize(int key, const char* serializedData, int dataSize);

};

//------------------------------------------------------------------------------

extern AgsSQLiteDBInterface AgsSQLiteDB_Interface;
extern AgsSQLiteDBReader AgsSQLiteDB_Reader;

//------------------------------------------------------------------------------

#endif /* _AGSSQLITEDB_H */