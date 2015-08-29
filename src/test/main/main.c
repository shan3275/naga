#include "cmdline.h"
#include "vsr_dp.h"

int main(int argc, char **argv)
{

    vsr_dp_init();
    cmdline (argc, argv);
    return 0;
}
