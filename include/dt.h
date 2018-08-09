#ifndef _DT
#define _DT

#include<iostream>
#include<vector>
#include<map>
#include<fstream>
#include<string.h>
#include<iomanip>
#include<cmath>
#include<set>

#define MAXIMUM 32767

extern std::ofstream log_file;
double getEnt(std::vector<std::map<std::string, std::string> > data, std::string classify_res, std::string split_attr = "", std::string split_attr_value = "");

class TreeNode
{
public:
     
    TreeNode(){leaf_flag = false;}

    bool is_leaf() { return leaf_flag; }
    void set_leaf_flag(bool flag){leaf_flag = flag;}

    std::string get_split_addr(){return attr;}
    TreeNode get_result(std::map<std::string, std::string> & one_item);
    TreeNode * add_child(std::string value);
    
    void set_attr(std::string & attr) {this->attr = attr;}
    std::string get_attr(){return attr;}
    
    
private:
    std::string attr; //划分属性
    std::map<std::string, TreeNode *> childs; //map的key代表属性所取值，value为子节点。
    bool leaf_flag;
};

class DtData
{
public:
    DtData(std::string data_file);
    void loadData(); 

    void dumpData();
    std::map<std::string, std::string> back_one();
    std::vector<std::string> get_attrs();

    std::vector<std::map<std::string, std::string> > & get_data() {return data;}
    
private:
    std::string data_file;
    std::vector<std::map<std::string, std::string> > data;
    std::vector<std::string> attrs;
    
    TreeNode root;
};

class ID3: public DtData
{
public:
    ID3(std::string data_file): DtData(data_file) {};
    
    void build_tree(std::vector<std::map<std::string, std::string> > & data, std::string classify_attr, TreeNode &root);
private:
};

#endif
