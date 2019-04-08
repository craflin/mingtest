
namespace mingtest {

int run(const char* filter, const char* outputFile);

}

int main(int argc, const char* argv[])
{
    return mingtest::run(0, 0);
}
