#include "block_def.h"

void
Block::setBlock
       (const Block_Type* type  , const size_t& pos_x ,
        const size_t&     pos_y , const string& orientation , const size_t& unit)
{
    if (orientation == "N"  || orientation == "S" ||
        orientation == "FN" || orientation == "FS" ) {
         _blockWidth = type->_blockWidth;
         _blockLength = type->_blockLength;
    }
    else if (orientation == "W"  || orientation == "E" ||
             orientation == "FW" || orientation == "FE" ) {
         _blockWidth = type->_blockLength;
         _blockLength = type->_blockWidth;
    }

    _ldPos_x = pos_x;
    _ldPos_y = pos_y;
    _ruPos_x = pos_x + _blockWidth * unit;
    _ruPos_y = pos_y + _blockLength * unit;
    
    cout<<"%%%%%"<< _blockWidth<<" "<<_blockLength<<endl;
    cout<<"%%%%%"<<_ldPos_x <<" "<<_ldPos_y  <<" "<<_ruPos_x <<" "<<_ruPos_y<<endl;
    
    for (size_t i = 0 ; i <= type->_occupiedLayer.size() ; ++i) {
        _occupiedLayer.push_back(type->_occupiedLayer[i]);
    }
    
    for (map<string, Power_Pin*>::const_iterator iter = type->_ppList.begin() ; iter != type->_ppList.end() ; ++iter) {
        Power_Pin* ref = iter->second;
        Power_Pin* pin = new Power_Pin(ref->get_name());
        
        for (size_t j = 0 ; j < ref->get_Layer_size(); ++j) {
            (pin->get_Layer()).push_back((ref->get_Layer())[j]);
        }
        
        pin->_current = ref->_current;
        pin->_IRDropConstraint = ref->_IRDropConstraint;

        if (orientation == "N") {
            pin->_ldPos_x = pos_x + ref->_ldPos_x * unit;
            pin->_ldPos_y = pos_y + ref->_ldPos_y * unit;
            cout<<pin->_ldPos_y<<" "<<pos_y<<" "<<ref->_ldPos_y<<endl;
            pin->_ruPos_x = pos_x + ref->_ruPos_x * unit;
            pin->_ruPos_y = pos_y + ref->_ruPos_y * unit;
        }
        else if (orientation == "S") {
            pin->_ldPos_x = pos_x + _blockWidth  - ref->_ruPos_x * unit;
            pin->_ldPos_y = pos_y + _blockLength - ref->_ruPos_y * unit;
            pin->_ruPos_x = pos_x + _blockWidth  - ref->_ldPos_x * unit;
            pin->_ruPos_y = pos_y + _blockLength - ref->_ldPos_y * unit;
        }
        else if (orientation == "E") {
            pin->_ldPos_x = pos_x                + ref->_ldPos_y * unit;
            pin->_ldPos_y = pos_y + _blockLength - ref->_ruPos_x * unit;
            pin->_ruPos_x = pos_x                + ref->_ruPos_y * unit;
            pin->_ruPos_y = pos_y + _blockLength - ref->_ldPos_x * unit;
        }
        else if (orientation == "W") {
            pin->_ldPos_x = pos_x + _blockWidth  - ref->_ruPos_y * unit;
            pin->_ldPos_y = pos_y                + ref->_ldPos_x * unit;
            pin->_ruPos_x = pos_x + _blockWidth  - ref->_ldPos_y * unit;
            pin->_ruPos_y = pos_y                + ref->_ruPos_x * unit;
        }
        else if (orientation == "FN") {
            pin->_ldPos_x = pos_x + _blockWidth  - ref->_ruPos_x * unit;
            pin->_ldPos_y = pos_y                + ref->_ldPos_y * unit;
            pin->_ruPos_x = pos_x + _blockWidth  - ref->_ldPos_x * unit;
            pin->_ruPos_y = pos_y                + ref->_ruPos_y * unit;
        }
        else if (orientation == "FS") {
            pin->_ldPos_x = pos_x                + ref->_ldPos_x * unit;
            pin->_ldPos_y = pos_y + _blockLength - ref->_ruPos_y * unit;
            pin->_ruPos_x = pos_x                + ref->_ruPos_x * unit;
            pin->_ruPos_y = pos_y + _blockLength - ref->_ldPos_y * unit;
        }
        else if (orientation == "FE") {
            pin->_ldPos_x = pos_x + _blockWidth  - ref->_ruPos_y * unit;
            pin->_ldPos_y = pos_y + _blockLength - ref->_ruPos_x * unit;
            pin->_ruPos_x = pos_x + _blockWidth  - ref->_ldPos_y * unit;
            pin->_ruPos_y = pos_y + _blockLength - ref->_ldPos_x * unit;
        }
        else if (orientation == "FW") {
            pin->_ldPos_x = pos_x + ref->_ldPos_y * unit;
            pin->_ldPos_y = pos_y + ref->_ldPos_x * unit;
            pin->_ruPos_x = pos_x + ref->_ruPos_y * unit;
            pin->_ruPos_y = pos_y + ref->_ruPos_x * unit;
        }
        cout<<"%%%%%%%"<<pin->get_name()<<" "<<pin->_ldPos_x<<" "<<pin->_ldPos_y<<" "<<pin->_ruPos_x<<" "<<pin->_ruPos_y<<endl;

        _ppList.insert(pair<string, Power_Pin*>(pin->_name, pin));
    }


}

Port::Port()
{

}
void
Port::setPort
(vector<int>& rect , int pos_x , int pos_y , string& orientation)
{
    assert(rect.size() == 4); 

    if (orientation == "N"  || orientation == "S" ||
        orientation == "FN" || orientation == "FS" ) {
         _portWidth  = rect[2] - rect[0];
         _portLength = rect[3] - rect[1];
    }
    else if (orientation == "W"  || orientation == "E" ||
             orientation == "FW" || orientation == "FE" ) {
         _portWidth  = rect[3] - rect[1];
         _portLength = rect[2] - rect[0];
    }

    if (orientation == "N") {
        _ldPos_x = pos_x + rect[0];
        _ldPos_y = pos_y + rect[1];
        _ruPos_x = pos_x + rect[2];
        _ruPos_y = pos_y + rect[3];
    }
    else if (orientation == "S") {
        _ldPos_x = pos_x - rect[2];
        _ldPos_y = pos_y - rect[3];
        _ruPos_x = pos_x - rect[0];
        _ruPos_y = pos_y - rect[1];
    }
    else if (orientation == "E") {
        _ldPos_x = pos_x + rect[1];
        _ldPos_y = pos_y - rect[2];
        _ruPos_x = pos_x + rect[3];
        _ruPos_y = pos_y - rect[0];
    }
    else if (orientation == "W") {
        _ldPos_x = pos_x - rect[3];
        _ldPos_y = pos_y + rect[0];
        _ruPos_x = pos_x - rect[1];
        _ruPos_y = pos_y + rect[2];
    }
    else if (orientation == "FN") {
        _ldPos_x = pos_x - rect[2];
        _ldPos_y = pos_y + rect[1];
        _ruPos_x = pos_x - rect[0];
        _ruPos_y = pos_y + rect[3];
    }
    else if (orientation == "FS") {
        _ldPos_x = pos_x + rect[0];
        _ldPos_y = pos_y - rect[3];
        _ruPos_x = pos_x + rect[2];
        _ruPos_y = pos_y - rect[1];
    }
    else if (orientation == "FE") {
        _ldPos_x = pos_x - rect[3];
        _ldPos_y = pos_y - rect[2];
        _ruPos_x = pos_x - rect[1];
        _ruPos_y = pos_y - rect[0];
    }
    else if (orientation == "FW") {
        _ldPos_x = pos_x + rect[1];
        _ldPos_y = pos_y + rect[0];
        _ruPos_x = pos_x + rect[3];
        _ruPos_y = pos_y + rect[2];
    }
}

Power_Pin::Power_Pin(string name){
	Layer.clear();
	_current=0.;
	_IRDropConstraint=0.;
	_ldPos_x=0;
	_ldPos_y=0;
	_ruPos_x=0;
	_ruPos_y=0;
	_name=name;
}

Power_Pin::~Power_Pin(){

}

void Power_Pin::set_powerSource(Power_Source* ps){
	_powerSource = ps;
}

void Power_Pin::set_current(double a){
	_current=a;
}
void Power_Pin::set_IRDropConstraint(double a){
	_IRDropConstraint=a;
}
void Power_Pin::set_ldPos_x(size_t a){
	_ldPos_x=a;
}
void Power_Pin::set_ldPos_y(size_t a){
	_ldPos_y=a;
}
void Power_Pin::set_ruPos_x(size_t a){
	_ruPos_x=a;
}
void Power_Pin::set_ruPos_y(size_t a){
	_ruPos_y=a;
}
void Power_Pin::Layer_add(bool b){
	Layer.push_back(b);
}
size_t Power_Pin::get_Layer_size(){
	return Layer.size();
}
vector<bool>& Power_Pin::get_Layer(){
	return Layer;
}
string Power_Pin::get_name(){
	return _name;
}

double Power_Source::get_voltage(){
	return _voltage;
}

void Power_Source::set_voltage(double v){
	_voltage = v;
}

Block_Type::Block_Type(){
	  _ppList.clear();
  	_occupiedLayer.clear();
  	_blockWidth=0;
    _blockLength=0;
    _origin_x=0;
    _origin_y=0;
    _ldPos_x=0;
    _ldPos_y=0;
    _ruPos_x=0;
    _ruPos_y=0;
}

Block_Type::~Block_Type(){
}

void Block_Type::set_blockWidth(size_t a){
	_blockWidth=a;
}
void Block_Type::set_blockLength(size_t a){
	_blockLength=a;
}
void Block_Type::set_origin_x(size_t a){
	_origin_x=a;
}
void Block_Type::set_origin_y(size_t a){
	_origin_y=a;
}
void Block_Type::set_Pos(vector<size_t> a){
	if(a.size()==4){
    _ldPos_x=a[0];
    _ldPos_y=a[1];
    _ruPos_x=a[2];
    _ruPos_y=a[3];
  }
  else{
      cout<<"Not enough information for block type"<<endl;
  }
}

void Block_Type::set_PPPos(string a, vector<size_t> b){
    if(b.size()==4){
      _ppList[a]->set_ldPos_x(b[0]);
      _ppList[a]->set_ldPos_y(b[1]);
      _ppList[a]->set_ruPos_x(b[2]);
      _ppList[a]->set_ruPos_y(b[3]);
    }
    else{
      cout<<"Not enough information for power pin position"<<endl;
    }
}


void Block_Type::Layer_add(bool b){
	_occupiedLayer.push_back(b);
}
void Block_Type::PPLayer_add(string a, bool b){
	_ppList[a]->Layer_add(b);
}
void Block_Type::Pin_add(string a){
	Power_Pin* pp=new Power_Pin(a);
	_ppList[a]=pp;
}
size_t Block_Type::get_Layer_size(){
	return _occupiedLayer.size();
}
size_t Block_Type::get_PPLayer_size(string a){
	return _ppList[a]->get_Layer_size();
}

