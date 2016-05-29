#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

#include "log.h"
#include "my_list.h"
#include "hero.h"

using namespace std;

void test()
{
    MyList<Hero> myList;
    ifstream file("heros.txt");
    copy(istream_iterator<Hero>(file), istream_iterator<Hero>(), back_inserter(myList));
    myList.sort([](const Hero &a, const Hero &b) {
        return a.power > b.power;
    });
    cout << "Sorted by power:" << endl;
    for (auto i: myList)
        cout << i << endl;
    myList.sort([](const Hero &a, const Hero &b) {
        int scorea = a.strength + a.intelligence + a.power + a.charisma + a.luck;
        int scoreb = b.strength + b.intelligence + b.power + b.charisma + b.luck;
        return scorea > scoreb;
    });
    cout << "Sorted by total score:" << endl;
    for (auto i: myList)
        cout << i << endl;
}
void test2()
{
    cout << "Some common memory faults:" << endl;
    new int;                    // leak
    delete[](new int);          // mismatch
    delete(new int[1]);         // mismatch
    delete((int *)0x563D7C);    // wild pointer (I like this color :-)
}

int main()
{
    // set up clog with message of loglevel "info" logged outputing to file only
    // and message of loglevel "warning" and "error" outputing to both file and original output
    std::ofstream logfile("log.txt");
    teebuf warningbuf = {
        logfile.rdbuf(),
        clog.rdbuf()
    };
    logbuf clogbuf(logfile.rdbuf(), &warningbuf, &warningbuf);
    streambuf *origclogbuf = clog.rdbuf();
    clog.rdbuf(&clogbuf);
    // set up cout with all message outputing to both file and original output
    teebuf coutbuf = {
        logfile.rdbuf(),
        cout.rdbuf()
    };
    streambuf *origcoutbuf = cout.rdbuf();
    cout.rdbuf(&coutbuf);

    cout << "Test case #1:" << endl;
    MEMORY_LEAK_DETECT_TEST(test());
    cout << "\n\nTest case #2:" << endl;
    MEMORY_LEAK_DETECT_TEST(test2());

    cout.rdbuf(origcoutbuf);
    clog.rdbuf(origclogbuf);
    logfile.close();

    cout << "\nDetailed log is available at log.txt." << endl;

    return 0;
}
