#include "layer_def.h"
#include "netMgr.h"


Layer::Layer( size_t metalWidth,
  size_t maxMetalWidth, size_t spacing,
  double resistivity,   bool direction_is_vertical
  ,  string name)
{
    _metalWidth=metalWidth;
    _maxMetalWidth=maxMetalWidth;
    _spacing=spacing;
    _resistivity=resistivity;
    _direction_is_vertical=direction_is_vertical;
    _name=name;
    _type="ROUTING";
}


void Network::set_network_layer(vector<string>& s)
{
	size_t metalWidth;
 	size_t maxMetalWidth=99999;
 	size_t spacing;
 	double resistivity;
 	string name;
 	bool direction_is_vertical;
 	int pos;
 	string tmp;
 	string tmp2;
 	string type;
	for(int i=0; i<s.size(); ++i)
	{
		pos=0;
		pos=myStrGetTok(s[i],tmp,pos);
		pos=myStrGetTok(s[i],tmp2,pos);
		if(tmp=="LAYER")
			name=tmp2;
		else if(tmp=="TYPE")
			type=tmp2;
		else if(tmp=="MAXWIDTH")
			{
				size_t i;
				stringstream ss;
   				ss << tmp2;
   				ss >> i;
				maxMetalWidth=i;
			}
   		else if(tmp=="WIDTH")
   			{
				size_t i;
				stringstream ss;
   				ss << tmp2;
   				ss >> i;
   				metalWidth=i;
   			}
   		else if(tmp=="SPACING")
	   		{
				size_t i;
				stringstream ss;
   				ss << tmp2;
   				ss >> i;
	   			spacing=i;
	   		}
   		else if(tmp=="DIRECTION")
   			direction_is_vertical=(tmp2=="HORIZONTAL")? 0 : 1;
   		else if(tmp=="RESISTANCE")
   			{
   				pos=myStrGetTok(s[i],tmp2,pos);
   				stringstream ss;
   				ss << tmp2;
   				double a,b;
   				ss >> a;
   				ss.ignore();
   				ss >> b;
   				resistivity=a*pow(10,b);
   			}
	}
		Layer* l;
		if(type=="ROUTING")
		l=new Layer( metalWidth, maxMetalWidth, spacing,
	 	resistivity,direction_is_vertical, name);
		else if(type=="CUT")
		l=new Layer();
		_layerList.push_back( l );
/*
   	string tmp("sss"),test("12345 asd 54321");
   	size_t pos=0;
   	while(pos!=string::npos)
   		{
   			pos=myStrGetTok(test,tmp,pos);
   			cout << tmp << endl;
   		}*/
}
Via::Via(double c,vector<double> d,string s)
{
    _resistance=c;
    _rect=d;
    _name=s;
}
void Network::set_network_via(vector<string>& s)
{
	extern Network *network;
	int pos;
	string tmp;
	pos=myStrGetTok(s[0],tmp,3);
	string via_name=tmp;
	pos=myStrGetTok(s[1],tmp,0);
	pos=myStrGetTok(s[1],tmp,pos);
	double resistance ,_the_pow;
	stringstream ss;
	ss << tmp;
	ss >> resistance;
	ss.ignore();
	ss >> _the_pow;
	resistance=resistance*pow(10,_the_pow);


	for(int i=2; i<s.size() ;i+=2)
	{
		pos=myStrGetTok(s[i],tmp,0);
		pos=myStrGetTok(s[i],tmp,pos);
		string layer_name=tmp;
		vector<double> v;
		stringstream ss;
		ss<<s[i+1];
		ss>>tmp;
		double dou_tmp;

		ss>>dou_tmp;
		v.push_back(dou_tmp);
		ss>>dou_tmp;
		v.push_back(dou_tmp);
		ss>>dou_tmp;
		v.push_back(dou_tmp);
		ss>>dou_tmp;
		v.push_back(dou_tmp);
		for(int i=0; i<network->_layerList.size(); ++i)
			if(network->_layerList[i]->_name==layer_name)
			{
				Via* via_push=new Via(resistance,v,via_name);
				network->_layerList[i]->_viaList.push_back(via_push);
				break;
			}
	}
	//Via* v = Via(float,float,double,double*);
}

