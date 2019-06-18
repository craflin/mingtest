
#include <getopt.h>

#include <cstring>
#include <cstdio>
#include <cstdlib>

static const char* COPYRIGHT = "Copyright (C) 2019 Colin Graf";

namespace mingtest {

int run(const char* filter, const char* outputFile);
int listTests();

}

static void showVersion()
{
    printf("mingtest %s, drop in replacement for gtest and gtest_main\n", PROJECT_VERSION);
    puts(COPYRIGHT);
    puts("This program comes with ABSOLUTELY NO WARRANTY.");
    puts("This is free software, and you are welcome to redistribute it under certain");
    puts("conditions.");
}

static void showUsage(const char* executable)
{
    for (;;)
    {
        const char* filename = strpbrk(executable, "/\\");
        if(!filename)
            break;
        executable = filename + 1;
    }
    showVersion();
    puts("");
    printf("Usage: %s --gtest_list_tests | [--gtest_filter <filer>]\n", executable);
    puts("        [--gtest_output xml:<output>]");
    puts("");
    puts("Options:");
    puts("");
    puts("    --gtest_filter=<filter>");
    puts("        Run only the test cases that match the filter string <filter>.");
    puts("");
    puts("    --gtest_output=xml:<output>");
    puts("        Create a test report at <output>, which could be a directory or a file.");
    puts("        If <output> ends with / or \\ it is considered to be a directory and a");
    puts("        test report file is created based on the name of the executable.");
    puts("");
    puts("    --gtest_list_tests");
    puts("        Show a list of available test cases.");
    puts("");
    puts("    --gtest_repeat=[<count>]");
    puts("        Repeat running the tests until they fail or up to <count> times.");
    puts("");
    puts("    -h, --help");
    puts("        Display this help message.");
    puts("");
    puts("    -v, --version");
    puts("        Display version information.");
    puts("");
}

int main(int argc, char* const argv[])
{
    option long_options[] = {
        {"help", no_argument , 0, 'h'},
        {"version", no_argument , 0, 'v'},
        {"gtest_filter", required_argument , 0, 0},
        {"gtest_output", required_argument , 0, 0},
        {"gtest_list_tests", no_argument , 0, 0},
        {"gtest_repeat", optional_argument , 0, 0},
        {0, 0, 0, 0}
    };

    const char* gtest_filter = 0;
    const char* gtest_output = 0;
    bool gtest_list_tests = false;
    int gtest_repeat = 1;
    {
        int c, option_index;
        while((c = getopt_long(argc, argv, "hv", long_options, &option_index)) != -1)
            switch(c)
            {
            case 0:
                {
                    const char* name = long_options[option_index].name;
                    if (strcmp(name, "gtest_filter") == 0)
                        gtest_filter = optarg;
                    else if (strcmp(name, "gtest_output") == 0)
                        gtest_output = optarg;
                    else if (strcmp(name, "gtest_list_tests") == 0)
                        gtest_list_tests = true;
                    else if (strcmp(name, "gtest_repeat") == 0)
                        gtest_repeat = optarg ? atoi(optarg) : -1;
                }
                continue;
            case 'h':
                showUsage(argv[0]);
                return EXIT_SUCCESS;
            case 'v':
                showVersion();
                return EXIT_SUCCESS;
            default:
                showUsage(argv[0]);
                return EXIT_FAILURE;
            }
    }

    if (gtest_list_tests)
        return mingtest::listTests();

    for (int i = 0;;)
    {
        int result = mingtest::run(gtest_filter, gtest_output);
        if (result)
            return result;
        if (gtest_repeat < 0 || ++i < gtest_repeat)
            continue;
        return result;
    }
}
