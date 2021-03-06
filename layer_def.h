#ifndef _LAYER_DEF_H_
#define _LATER_DEF_H_
#include "utility.cpp"


class Via
{
public:
    Via(){};
    Via(double,vector<double>,string);
    ~Via();
private:
    double          _resistance;
    vector<double>  _rect;
    string          _name;//ex:via1_A
};


class Layer
{
    friend class Network;
public:
    Layer(){_type="CUT";};
    Layer(size_t ,size_t ,size_t ,double ,bool,string);
    ~Layer();

private:
    vector<Via*>    _viaList;
    size_t          _metalWidth;
    size_t          _maxMetalWidth;
    size_t          _spacing;
    double          _resistivity;
    bool            _direction_is_vertical; //true is vertical
    string          _name; //ex:METAL5

    string          _type;
    double          _metalWeight;
};




// extern int myStrNCmp(const string& s1, const string& s2, unsigned n);
// extern size_t myStrGetTok(const string& str, string& tok, size_t pos = 0, const char del = ' ');
// extern bool myStr2Int(const string& str, int& num);

#endif // _LAYER_DEF_H_
