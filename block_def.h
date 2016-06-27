#ifndef _BLOCK_DEF_H_
#define _BLOCK_DEF_H_
#include "utility.cpp"

class Block_Type;
class Power_Source;

class Power_Pin
{
    friend class Network;
    friend bool read_v(char*);
    friend class Block;
    friend void setBlock
         (const Block_Type* , const size_t& , const size_t& , const string& , const size_t&);
public:

    Power_Pin(string = "VDD");
    ~Power_Pin();
    void set_powerSource(Power_Source*);
    void set_current(double);
    void set_IRDropConstraint(double);
    void set_ldPos_x(size_t);
	  void set_ldPos_y(size_t);
  	void set_ruPos_x(size_t);
  	void set_ruPos_y(size_t);
  	void Layer_add(bool);
    size_t get_Layer_size();
    vector<bool>& get_Layer();
    string get_name();


private:


    vector<bool>    	      Layer;
    double                  _current;
    double                  _IRDropConstraint;
    Power_Source*           _powerSource;
    string                  _name;
    size_t                  _ldPos_x;
    size_t           	      _ldPos_y;
    size_t            		  _ruPos_x;
    size_t            		  _ruPos_y;

};




class Block_Type
{
    friend class Block;
    friend void setBlock
         (const Block_Type* , const size_t& , const size_t& , const string& , const size_t& );
public:

    Block_Type();
    ~Block_Type();
  	void set_blockWidth(size_t);
  	void set_blockLength(size_t);
  	void set_origin_x(size_t);
  	void set_origin_y(size_t);
  	void set_Pos(vector<size_t>);
  	void set_PPPos(string, vector<size_t>);
  	void Layer_add(bool);
	  void PPLayer_add(string, bool);
	  void Pin_add(string);
    size_t get_Layer_size();
    size_t get_PPLayer_size(string);



private:

    map<string, Power_Pin*>    _ppList;
    vector<bool>               _occupiedLayer;
    size_t                     _blockWidth;
    size_t                     _blockLength;
    size_t	  	          		 _origin_x;
    size_t		  		           _origin_y;
    size_t                     _ldPos_x;
    size_t                     _ldPos_y;
    size_t                     _ruPos_x;
    size_t                     _ruPos_y;

};

class Block
{
    friend class Network;
    friend bool read_initial(char*);
public:

    Block();
    Block(const string& name) : _name(name) {}
    ~Block();
    void setBlock
         (const Block_Type* , const size_t& , const size_t& , const string& , const size_t& );


private:

    map<string, Power_Pin*>    _ppList;
    vector<bool>               _occupiedLayer;
    size_t                     _blockWidth;
    size_t                     _blockLength;
    size_t                     _ldPos_x;
    size_t                     _ldPos_y;
    size_t                     _ruPos_x;
    size_t                     _ruPos_y;
    string                     _name;
};

class Port
{
public:

    Port();
    ~Port();

    void setPort(vector<int>& , int  , int  , string&);
    void setLayer(size_t l) { _layer = l; }

private:

    size_t               _ldPos_x;
    size_t               _ldPos_y;
    size_t               _ruPos_x;
    size_t               _ruPos_y;
    size_t               _portWidth;
    size_t               _portLength;
    size_t               _layer;
};

class Power_Source
{
    friend class Network;
    friend bool read_def(char *);
public:

    Power_Source();
    Power_Source(const string& name) : _name(name) {}
    ~Power_Source();
    double get_voltage();
    void set_voltage(double);

private:

    vector<Port*>        _portList;
    double               _voltage;
    string               _name;

};
#endif // _BLOCK_DEF_H_



