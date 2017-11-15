#include <iostream>
using namespace std;
#include <cstdlib>
#include <cstring>
#include "io.h"
#include "common.h"
#include "select.h"
#include "insert.h"
#include "createTable.h"
#include "parseTree.h"
#include "stmtDataStructs.h"
#include "Block.h"
#include "Config.h"
#include "Disk.h"
#include "Field.h"
#include "MainMemory.h"
#include "Relation.h"
#include "Schema.h"
#include "SchemaManager.h"
#include "Tuple.h"
#include <unordered_map>

int main() {

    /* Initialize the main memory and disk */
    MainMemory mem;
    Disk disk;
    cout << "The memory contains " << mem.getMemorySize() << " blocks" << endl;
    cout << mem << endl << endl;
    SchemaManager schema_manager(&mem,&disk);

    disk.resetDiskIOs();
    disk.resetDiskTimer();
    unordered_map<string, Relation *> tablePtrs;

    /* read the first word to check the type of statement */
    char *stmtBuf;
    stmtBuf = (char *)malloc(10*sizeof(char));
    read(stmtBuf, false);

    /* call the corresponding subroutine */
    if (strcmp(stmtBuf, "SELECT") == 0) {
      // selectStmt(root);
    }
    else if (strcmp(stmtBuf, "INSERT") == 0){
        read(stmtBuf, false);
        if (strcmp(stmtBuf, "INTO") == 0) {
          insertData inDataObj;
          insertStmt(&inDataObj);
          cout << inDataObj << endl;        }
    }
    else if (strcmp(stmtBuf, "CREATE") == 0){
        read(stmtBuf, false);
        if (strcmp(stmtBuf, "TABLE") == 0){
            createTableData crTableObj;
            createTable(&crTableObj);
            cout << crTableObj << endl;

            Schema schema(crTableObj.field_names,crTableObj.field_types);
            Relation* relation_ptr=schema_manager.createRelation(crTableObj.relationName,schema);
            tablePtrs[crTableObj.relationName] = relation_ptr;
        }
    }
}
