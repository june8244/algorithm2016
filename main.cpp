#include "block_def.h"
#include "layer_def.h"
#include "netMgr.h"
#include "utility.cpp"

Network* network;

int main(int argc, char **argv)
{
    network = new Network;
    network->read(argv);

}
