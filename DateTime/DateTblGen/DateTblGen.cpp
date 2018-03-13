#include "Platform.h"
#include <iostream>

#include "MRDateTbl.h"
#include "DebugNew.h"

using std::cerr;
using std::cout;
using std::endl;

///////////////////////////////////////////////////////////////////////////////

/*****  Functions  *****/

int
main(int argc,
     char *argv[])
{
    cout << "#include \"MRDateTbl.h\"" << endl << endl
         << "////////////////////////////////////////////////////////////"
         << "///////////////////" << endl << endl
         << "/*****  Macros and Constants  *****/" << endl << endl
         << "const long mr::DateTbl::secy_date_tbl[] = {";

    unsigned long i;
    int month = 0;

    cout << endl << "    -1,-1,";

    for (i = 0 ; i < mr::DateTbl::PRIM_DATE_TBL_SIZE ; i++) {
        int tmp = (mr::DateTbl::prim_date_tbl[i] / 100) % 100;

        if (month == tmp) {
            continue;
        }

        month = tmp;

        if (month == 1) {
            cout << endl << "    ";
        }

        cout << i << ',';
    }

    cout << "-1,-1,-1,-1,-1,-1,-1,-1,-1" << endl << "};" << endl << endl
         << "const unsigned long mr::DateTbl::SECY_DATE_TBL_SIZE = " << endl
         << "             sizeof(mr::DateTbl::secy_date_tbl) / sizeof(long);"
         << endl << endl
         << "////////////////////////////////////////////////////////////"
         << "///////////////////" << endl;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
