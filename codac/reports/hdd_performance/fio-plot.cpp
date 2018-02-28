#include <Core/FileUtils.h>
#include <Core/CsvDataFile.h>


int main(int argc, char *argv[])
{
    const char fname[] = "seq-test_bw.1.log";
    CsvDataFile csv(fname, std::ios_base::in, ',');

    return 0;
}
