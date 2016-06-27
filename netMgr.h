#ifndef _NETMGR_H_
#define _NETMGR_H_
#include "utility.cpp"

class Block;
class Block_Type;
class Layer;
class Power_Source;

class Network
{
public:

    Network();
    ~Network();

    bool read(char**);
    bool read_def(char*);
    bool read_v(char*);
    bool read_tech(char*);
    bool read_lef(char*);
    bool read_initial(char*);

    void set_network_layer(vector<string>& );
    void set_network_via(vector<string>& );
    
    map<string, Power_Source*>      get_psList();
    map<string, Block*>             get_blockList();

private:

    map<string, Block*>             _blockList;
    map<string, Power_Source*>      _psList;
    vector<Layer*>                 _layerList;
    map<string, Block_Type*>       _blockTypeList;

    size_t                     _width;
    size_t                     _length;
    size_t                     _unit;
    size_t                     _ld_x;
    size_t                     _ld_y;
    size_t                     _ru_x;
    size_t                     _ru_y;
    string                     _name;
};

#endif // _NETMGR_H_
