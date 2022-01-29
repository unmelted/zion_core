#include <cstdio>
#include "DaemonMgr.h"
#include <string.h>
#include "Util/logger.h"
using namespace toolkit;

//deamon mode
/*
int main()
{
    Logger::Instance().add(std::make_shared<ConsoleChannel>());
    Logger::Instance().add(std::make_shared<FileChannel>());
    Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());

    InfoL << "Version : " << CURRENTVERSION;
    InfoL << "v : Version check";

    DaemonMgr daemonMgr;
    char pStr[100];
    while (1)
    {
        // memset(pStr, 0, 100);

        // scanf("%s", &pStr);
        // if(strcmp(pStr, "exit") == 0)
        // {
        //     //socketmgr.Close();
        //     break;
        // }
        // else if (strcmp(pStr, "1") == 0)
        // {
        //     //socketMgr.SendData("{\"hello\" : \"world\",\"t\" : true,\"f\" : false,\"n\" : null,\"i\" : 123,\"pi\" : 3.1416,\"a\" : [1, 2, 3, 4],\"To\" : \"ProducingApp\"} ");
        //     printf("Connect CCD\n");
        // }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
*/

//local file mode
int main(int argc, char* argv[]) {
    if (argc < 2 ) {
        printf("Please input VIDEO JSON FILE");
        return 0;
    }

    string jsonfile(argv[1]);
    cout << jsonfile;
    ExpUtil in;
    VIDEO_INFO info;
    int result  = in.ImportVideoInfo(jsonfile, &info, true);
    if(result == dove::ERR_NONE) {
        Dove stblz(&info);
        result = stblz.Process();
    }

    return 1;
}