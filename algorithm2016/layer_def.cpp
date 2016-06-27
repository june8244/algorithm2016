//globla variable vector<Layer*> _layerList;
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <string>
#include <ctype.h>
#include <cstring>
#include <cassert>
#include <math.h>
#include <cstdlib>
#include "layer_def.h"
#include "utility/parse.cpp"
#include "netMgr.h"
using namespace std;

Layer::Layer( size_t metalWidth,
  size_t maxMetalWidth, size_t spacing, 
  double resistivity,   bool direction_is_vertical
  ,  string name)
{
    _maxMetalWidth=maxMetalWidth;
    _spacing=spacing;
    _resistivity=resistivity;
    _direction_is_vertical=direction_is_vertical;
    _name=name;
    _type="ROUTING";
}

Network* network;

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

int main()
{
	string fileName("inputs/CAD_contest_case1_ngspice/case1/tech.lef");
	ifstream ifs;
	ifs.open(fileName.c_str(),ifstream::in);
	if(!ifs.good())
		cout << "tech.lef no found!!" << endl;
   	stringstream ss;
   	string s;
   	vector<string> s_vec;
   	while(ifs.good())
   	{
   		getline(ifs,s);
   		ss.str("");
   		ss.clear();
   		s_vec.push_back(s);
   	}
   	vector<string> s_vec_tmp;
   	bool via_flag=false ,layer_flag=false;
   	for(int i=0; i<s_vec.size(); ++i)
   	{
   		if( !myStrNCmp(s_vec[i],"LAYER ",6) )
   		{
   			s_vec_tmp.clear();
   			layer_flag=true;
   		}
   		else if( !myStrNCmp(s_vec[i],"END ",4) && layer_flag)
   		{
   			layer_flag=false;
   			network->set_network_layer(s_vec_tmp);
   			continue;
   		}

   		else if( !myStrNCmp(s_vec[i],"VIA ",4) )
   		{
   			s_vec_tmp.clear();
   			via_flag=true;
   		}
   		else if( !myStrNCmp(s_vec[i],"END ",4) && via_flag)
   		{
   			via_flag=false;
   			network->set_network_via(s_vec_tmp);
   			continue;
   		}
   		s_vec_tmp.push_back(s_vec[i]);
   	}


   	
}
