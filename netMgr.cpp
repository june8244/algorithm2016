#include "netMgr.h"
#include "layer_def.h"
#include "block_def.h"

extern Network *network;

Network::Network()
{

}

Network::~Network()
{

}

map<string, Power_Source*> Network::get_psList(){
	return _psList;
}

map<string, Block*> Network::get_blockList(){
	return _blockList;
}


bool Network::read(char **input)
{
    
    if (!read_lef(input[4])) {
        cout << "ERROR: reading block.lef failed." << endl;
        return false;
    }
    if (!read_def(input[2])) {
        cout << "ERROR: reading case1_input.def failed." << endl;
        return false;
    }
    if (!read_tech(input[3])) {
        cout << "ERROR: reading tech.lef failed." << endl;
        return false;
    }
    if (!read_v(input[1])) {
        cout << "ERROR: reading case1.v failed." << endl;
        return false;
    }
    if (!read_initial(input[5])) {
        cout << "ERROR: reading initial_files failed." << endl;
        return false;
    }
}

bool Network::read_def(char *input)
{
    string inputstr(input);
//    inputstr = inputstr + "/" + inputstr + "_input.def";

    ifstream ifs(inputstr.c_str());

    if (!ifs.is_open()) {
        cout << "ERROR: can't open file \"" << inputstr << "\"!!" << endl;
        return false;
    }

//    size_t parse
cout<<"enter def"<<endl;
    while (!ifs.eof()) {
        char buf[2000];
        ifs.getline(buf , 2000);
        string input(buf);
        string header;
        myStrGetTok(input, header);

        if (header == "DESIGN") {
            vector<string> tokens;
            lexOptions(input , tokens);
            _name = tokens[1]; 
            continue;
        }

        if (header == "UNITS") {
            vector<string> tokens;
            lexOptions(input , tokens);
            int num;
            myStr2Int(tokens[3], num);
            _unit = num;
            continue;
        }

        if (header == "DIEAREA") {
            vector<string> tokens;
            lexOptions(input , tokens);
            int num[4];
            myStr2Int(tokens[2] , num[0]);
            myStr2Int(tokens[3] , num[1]);
            myStr2Int(tokens[6] , num[2]);
            myStr2Int(tokens[7] , num[3]);
            _ld_x = num[0];
            _ld_y = num[1];
            _ru_x = num[2];
            _ru_y = num[3];
            _width = num[2] - num[0];
            _length = num[3] - num[1];
            continue;
        }

        if (header == "COMPONENTS") { cout<<"Start reading blocks."<<endl;
            vector<string> tmp;
            lexOptions(input , tmp);
            int componentNum;
            myStr2Int(tmp[1] , componentNum);

            while (componentNum) {
                char blockread[400];
                ifs.getline(blockread , 400 , ';');
                string blockinfo(blockread);
                strSweep(blockinfo , '\n');
                vector<string> tokens;
                lexOptions(blockinfo , tokens);
                int pos_x , pos_y;
                string orientation;
                for (size_t i = 3 ; i < tokens.size() ; ++i) {
                    if (tokens[i] == "PLACED" ||
                        tokens[i] == "FIXED"  ||
                        tokens[i] == "COVER"  ) {

                        myStr2Int(tokens[i + 2] , pos_x);
                        myStr2Int(tokens[i + 3] , pos_y);
                        orientation = tokens[i + 5];
                        break;
                    }
                }
                Block* block = new Block(tokens[1]);
                block->setBlock(_blockTypeList[tokens[2]] ,
                               pos_x , pos_y , orientation, _unit);
                _blockList.insert(pair<string, Block*>(tokens[1], block));
                --componentNum;
            }
        }

        // assume that a single port occupies only one layer
        if (header == "PINS") { cout<<"Start reading pins." <<endl;
            vector<string> tmp;
            lexOptions(input , tmp);
            int pinNum;
            myStr2Int(tmp[1] , pinNum);


            while (pinNum) { 
                char pinread[2000];
                ifs.getline(pinread , 2000 , ';');
                string pininfo(pinread);
                strSweep(pininfo , '\n');
                vector<string> tokens;
                lexOptions(pininfo , tokens);
                vector<int> rect(4 , 0);
                int pos_x , pos_y;
                string orientation;
                Power_Source* pin = new Power_Source(tokens[1]);
                bool single = true;
                for (size_t i = 2 ; i < tokens.size() ; ++i) {
                    if (tokens[i] == "PORT") {
                        single = false;
                        break;
                    }
                } 
                if (single) {
                    Port* port = new Port;
                    for (size_t i = 2 ; i < tokens.size() ; ++i) {
                        if (tokens[i] == "LAYER") {
                            int layerNum;
                            string layerNumStr = tokens[i + 1].substr(5);
                            myStr2Int(layerNumStr , layerNum);
                            port->setLayer((size_t)layerNum);
                            myStr2Int(tokens[i + 3] , rect[0]);
                            myStr2Int(tokens[i + 4] , rect[1]);
                            myStr2Int(tokens[i + 7] , rect[2]);
                            myStr2Int(tokens[i + 8] , rect[3]);
                            i += 9;
                        }
                        else if (tokens[i] == "PLACED" ||
                                 tokens[i] == "FIXED"  ||
                                 tokens[i] == "COVER"  ) {
                            myStr2Int(tokens[i + 2] , pos_x);
                            myStr2Int(tokens[i + 3] , pos_y);
                            orientation = tokens[i + 5];
                            i += 5;
                        }
                    }
                    port->setPort(rect , pos_x , pos_y , orientation);
                    pin->_portList.push_back(port);
                    --pinNum;
                }
                else {
                    for (size_t i = 0 ; i < tokens.size() ; ++i) {
                        if (tokens[i] == "PORT") {
                            Port* port = new Port;
                            for ( ; i < tokens[i].size() ; ++i) {
                                if (tokens[i] == "LAYER") {
                                    int layerNum;
                                    string layerNumStr = tokens[i + 1].substr(5);
                                    myStr2Int(layerNumStr , layerNum);
                                    port->setLayer(layerNum);
                                    myStr2Int(tokens[i + 3] , rect[0]);
                                    myStr2Int(tokens[i + 4] , rect[1]);
                                    myStr2Int(tokens[i + 7] , rect[2]);
                                    myStr2Int(tokens[i + 8] , rect[3]);
                                    i += 9;
                                }
                                else if (tokens[i] == "PLACED" ||
                                         tokens[i] == "FIXED"  ||
                                         tokens[i] == "COVER"  ) {
                                    myStr2Int(tokens[i + 2] , pos_x);
                                    myStr2Int(tokens[i + 3] , pos_y);
                                    orientation = tokens[i + 5];
                                    i += 5;
                                }
                                else if (tokens[i] == "PORT") {
                                    --i;
                                    break;
                                }
                            }
                            port->setPort(rect , pos_x , pos_y , orientation);
                            pin->_portList.push_back(port);
                            --pinNum;
                        }
                    }
                }
                _psList[tokens[1]]= pin;
            }
        }
    }
    
cout<<"leave def"<<endl;
return true;
}


bool Network::read_lef(char* path){

	ifstream fin(path);
    if(!fin.is_open()){
        cout<<"Cannot open input file blocks.lef!!"<<endl;
        return false;
    }
 cout<<"enter lef"<<endl;       
	string data, temp;//ifstream, tran_type
	vector<size_t> numbers;//scan-in
	int count=1;//nth Block_Type
	Block_Type* bb=new Block_Type;
	
	while(myStrNCmp(data, "END LIBRAR",sizeof("END LIBRAR")-1)!=0){
		tran_type(count, temp);
		temp="block"+temp;
		if(myStrNCmp(data, "MACRO block",sizeof("MACRO block")-1)==0){//MACRO block*
			bb=new Block_Type;		
			_blockTypeList[temp]=bb;
			
			//Origin * * ;
			while(myStrNCmp(data, "  ORIGIN", sizeof("  ORIGIN")-1)!=0){
				getline(fin, data);
			}
			numbers.clear();
			numbers=scan(data);
			_blockTypeList[temp]->set_origin_x(numbers[0]);
			_blockTypeList[temp]->set_origin_y(numbers[1]);
			
			//SIZE * BY * ;
			while(myStrNCmp(data, "  SIZE", sizeof("  SIZE")-1)!=0){
				getline(fin, data);
			}
			numbers.clear();
			numbers=scan(data);
			_blockTypeList[temp]->set_blockWidth(numbers[0]);
			_blockTypeList[temp]->set_blockLength(numbers[1]);
      cout<<numbers[0]<<" "<<numbers[1]<<endl;
			
			char count_pin='A';
			string temp_2;
			
			//PIN VDD_* stop at OBS;
			while(myStrNCmp(data, "  OBS", sizeof("  OBS")-1)!=0){				
				if(myStrNCmp(data, "  PIN VDD_", sizeof("  PIN VDD_")-1)==0){
					tran_type(count_pin, temp_2);
					temp_2="VDD_"+temp_2;
					_blockTypeList[temp]->Pin_add(temp_2);
					
					//PORT stop at END VDD_;
					while(myStrNCmp(data, "  END VDD_", sizeof("  END VDD_")-1)!=0){
						if(myStrNCmp(data, "    PORT", sizeof("    PORT")-1)==0){
							_blockTypeList[temp]->PPLayer_add(temp_2, 0);//start from [1]
							//Layer stop at END;
							while(myStrNCmp(data, "    END", sizeof("    END")-1)!=0){
								if(myStrNCmp(data, "    LAYER", sizeof("    LAYER")-1)==0){
									numbers.clear();
									numbers=scan(data);
									while(_blockTypeList[temp]->get_PPLayer_size(temp_2)<numbers[0]){
										_blockTypeList[temp]->PPLayer_add(temp_2, 0);
										}
									if(_blockTypeList[temp]->get_PPLayer_size(temp_2)==numbers[0]){
										_blockTypeList[temp]->PPLayer_add(temp_2, 1);
										}
									else{
										cout<<"	Caution: repeated Layer"<<numbers[0]<<" input!"<<endl;
									}
									getline(fin, data);//RECT * * * *;
									numbers.clear();
									numbers=scan(data);
									_blockTypeList[temp]->set_PPPos(temp_2, numbers);
								}
								else{
									getline(fin, data);
								}
							}
						}
						else{
							getline(fin, data);
						}
					}
					count_pin++;
				}
				else{
					getline(fin, data);
				}
			}
			//LAYER METAL stop at END;
			_blockTypeList[temp]->Layer_add(0);//start from [1]
			while(myStrNCmp(data, "  END", sizeof("  END")-1)!=0){
				if(myStrNCmp(data, "    LAYER METAL", sizeof("    LAYER METAL")-1)==0){
					numbers.clear();
					numbers=scan(data);
					while(_blockTypeList[temp]->get_Layer_size()<numbers[0]){
						_blockTypeList[temp]->Layer_add(0);
					}
					if(_blockTypeList[temp]->get_Layer_size()==numbers[0]){
						_blockTypeList[temp]->Layer_add(1);
					}
					else{
						cout<<"	Caution: repeated Layer"<<numbers[0]<<" input!"<<endl;
					}
					
					getline(fin, data);//       RECT * * * * ;
					numbers.clear();
					numbers=scan(data);
					_blockTypeList[temp]->set_Pos(numbers);
					}
				else{
					getline(fin, data);
				}
			}
			count++;
		}
		else{
			getline(fin, data);
		}
	}
cout<<"leave lef"<<endl;
	return true;
}


bool Network::read_tech(char* path)
{
	extern Network* network;
	string fileName(path);
	ifstream ifs;
	ifs.open(fileName.c_str(),ifstream::in);
	if(!ifs.good())
		cout << "tech.lef no found!!" << endl;
   	stringstream ss;
   	string s;
   	vector<string> s_vec;
cout<<"enter tech"<<endl;
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
   			set_network_layer(s_vec_tmp);
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
   			set_network_via(s_vec_tmp);
   			continue;
   		}
   		s_vec_tmp.push_back(s_vec[i]);
   	}


/*
   	string tmp("sss"),test("12345 asd 54321");
   	size_t pos=0;
   	while(pos!=string::npos)
   		{
   			pos=myStrGetTok(test,tmp,pos);
   			cout << tmp << endl;
   		}*/
cout<<"leave tech"<<endl;
	return true;
}

bool Network::read_initial(char* path)
{
    extern Network *network;
    fstream file;
    string str;
    file.open(path, ios::in);
cout<<"open initial"<<endl;
    if(!file)
    {
	cout<<"Can't open the file!"<<endl;
	return false;
    }
    while(getline(file, str, ' '))
	if(str == "current")
	    break;
    getline(file, str, '\n');
    while(getline(file, str, '\n'))
    {
        if(str[0] != 'B')
	    break;
	else
	{
	    size_t find_t = str.find(' ');
	    string block_id(str.substr(0, find_t));
	    size_t find_h = str.find('V');
	    find_t = str.find(' ', find_h);
	    string pin_id(str.substr(find_h, find_t-find_h));
	    string cur(str.substr(find_t+1, str.size()-find_t-1));
            double current;
	    tran_type(cur, current);

            cout<<"block:"<<block_id<<endl<<"power pin:"<<pin_id<<endl<<"current:"<<current<<endl;
	    ((((network->_blockList).find(block_id)->second)->_ppList).find(pin_id)->second)->set_current(current);
	}
    }
    while(getline(file, str, ' '))
	if(str == "voltage")
	    break;
    getline(file, str, '\n');
    while(getline(file, str, '\n'))
    {
	if(str[0] != 'V')
	    break;
	else
	{
	    size_t find = str.find(' ');
	    string ps_id(str.substr(0, find));
	    string volt(str.substr(find+1, str.size()-find-1));
	    double voltage;
	    tran_type(volt, voltage);
            cout<<"power source:"<<ps_id<<endl<<"voltage:"<<voltage<<endl;
            ((network->_psList).find(ps_id)->second)->set_voltage(voltage);
	}
    }
    while(getline(file, str, ' '))
	if(str == "IR-drop")
	    break;
    getline(file, str, '\n');
    while(getline(file, str, '\n'))
    {
	if(str[0] != 'B')
	    break;
	else
	{
	    size_t find_h = str.find(' ');
	    string block_id(str.substr(0, find_h));
	    find_h = str.find('V');
	    size_t find_t = str.find(' ', find_h);
	    string pin_id(str.substr(find_h, find_t-find_h));
	    string IR(str.substr(find_t+1, str.size()-find_t-1));
	    double ir_drop;
	    tran_type(IR, ir_drop);
            cout<<"block:"<<block_id<<endl<<"power pin:"<<pin_id<<endl<<"IR-drop:"<<ir_drop<<endl;
            ((((network->_blockList).find(block_id)->second)->_ppList).find(pin_id)->second)->set_IRDropConstraint(ir_drop);
	}
 
    }
    while(getline(file, str, ' '))
	if(str == "metal")
	    break;

    getline(file, str, '\n');
    while(getline(file, str, '\n'))
    {
	if(str[0] != 'M')
	    break;
	else
	{
	    size_t find = str.find(' ');
	    string metal_name(str.substr(1, find));
	    string weig(str.substr(find+1, str.size()-find-1));
	    double weight;
	    tran_type(weig, weight);
            cout<<"metal:"<<metal_name<<endl<<"weight:"<<weight<<endl;
    for(int i = 0; i < (network->_layerList).size(); i++)
        if(((network->_layerList).at(i))->_name == "Metal"+metal_name)
            ((network->_layerList).at(i))->_metalWeight = weight;
	}
    }
cout<<"leave initial"<<endl;
    file.close();
    return true;
}

bool Network::read_v(char* path) {
    extern Network *network;
    fstream file;
    file.open(path, ios::in);
    if(!file)
    {
	cout<<"Can't open the file!"<<endl;
	return false;
    }
    string str;
cout<<"open v"<<endl;
    while(getline(file, str, '\n'))
    {
	if(str.find('B') != string::npos)
	{
	    size_t find_h = str.find('B');
	    size_t find_t = str.find('(')-1;
	    string block_id(str.substr(find_h, find_t-find_h));
	    tran_type(block_id, block_id);
	    while(str.find('.', find_t) != string::npos)
	    {
	        find_h = str.find('V', find_t);
	        find_t = str.find('(', find_h);
	        string pp_id(str.substr(find_h, find_t-find_h));
	        find_h = str.find('V', find_t);
	        find_t = str.find(')', find_h);
	        string ps_id(str.substr(find_h, find_t-find_h));
	        cout<<"block:"<<block_id<<endl<<"power source:"<<ps_id<<endl<<"power pin:"<<pp_id<<endl;
		((((network->_blockList).find(block_id)->second)->_ppList).find(pp_id)->second)->_powerSource = (network->_psList).find(ps_id)->second;
	    }
	}
    }
cout<<"leave v"<<endl;
    file.close();
    return true;
}
