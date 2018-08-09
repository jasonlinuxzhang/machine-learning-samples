#include "dt.h"

DtData::DtData(std::string data_file): data_file(data_file)
{
    ;
}

void DtData::loadData()
{
    char one_line[256] = {0};
   
    std::ifstream file(data_file.c_str(), std::ios_base::in);
    
    file.getline(one_line, sizeof(one_line)/sizeof(char)); 

    char *pre_pos = one_line, *pos = 0;
    
    for(pos = strchr(pre_pos, ','); NULL != pos; pre_pos = pos + 1, pos = strchr(pre_pos, ','))
    {
        attrs.push_back(std::string(pre_pos, pos - pre_pos));
    }
    attrs.push_back(std::string(pre_pos));

    file.getline(one_line, sizeof(one_line)/sizeof(char));
    while(file.good())
    {
        int i = 0;
        std::map<std::string, std::string> one_item;
        pre_pos = one_line;
        pos = 0;
        for(i = 0; i < attrs.size() - 1; i++)
        {
            pos = strchr(pre_pos, ',');
            one_item.insert(std::pair<std::string, std::string>(attrs[i], std::string(pre_pos, pos - pre_pos))); 
            pre_pos = pos + 1;
        }
        one_item.insert(std::pair<std::string, std::string>(attrs[i], std::string(pre_pos)));
        data.push_back(one_item);

        file.getline(one_line, sizeof(one_line)/sizeof(char));
    } 
    
    log_file<<data_file<<" load complete.\n";

    file.close();
    
}

std::map<std::string, std::string> DtData::back_one()
{
    if(0 == data.size())
        throw 0;
    return data.back();
}

std::vector<std::string> DtData::get_attrs()
{
    return attrs;
}

void DtData::dumpData()
{
    for(std::vector<std::string>::iterator it= attrs.begin(); it != attrs.end(); it++)
    {
        std::cout<<std::setw(15)<<*it;
    }
    std::cout<<"\n";

    for(std::vector<std::map<std::string, std::string> >::iterator it = data.begin(); it != data.end(); it++)
    {
        
        for(std::vector<std::string>::iterator key_it= attrs.begin(); key_it != attrs.end(); key_it++)
        {
            std::cout<<std::setw(15)<<(*it)[*key_it];
        }
        std::cout<<"\n";
    }
}

void ID3::build_tree(std::vector<std::map<std::string, std::string> > & data, std::string classify_attr, TreeNode &cur_node)
{
    double ent = getEnt(data, classify_attr); 
    int total = data.size();
    
    //all cla
    std::set<std::string> temp;
    for(std::vector<std::map<std::string, std::string> >::iterator it = data.begin(); it != data.end(); it++)
    {
        temp.insert((*it)[classify_attr]);
    }
    
    if(1 == temp.size())
    {
        std::string temp_class = (*temp.begin());
        cur_node.set_leaf_flag(true);
        cur_node.set_attr(temp_class);
        return;
    }


    std::vector<std::string> attrs_collection;
    for(std::map<std::string, std::string>::iterator it = (data.begin())->begin(); it != (data.begin())->end(); it++)
    {
        attrs_collection.push_back(it->first);
    }


    double min_ent = MAXIMUM;
    std::string split_attr;
    for(std::vector<std::string>::iterator attrs_it = attrs_collection.begin(); attrs_it != attrs_collection.end(); attrs_it ++)
    {
        std::set<std::string> values;
        std::map<std::string, double> ent_collections;
        std::map<std::string, int> dv;
        if((*attrs_it) == classify_attr)
            continue;

        for(std::vector<std::map<std::string, std::string> >::iterator it = data.begin(); it != data.end(); it++)
        {
            if(values.find((*it)[(*attrs_it)]) == values.end())
            {
                dv[(*it)[(*attrs_it)]] = 1;
                values.insert((*it)[(*attrs_it)]);
                ent_collections.insert(std::pair<std::string, double>((*it)[(*attrs_it)], getEnt(data, classify_attr, *attrs_it, (*it)[(*attrs_it)]) ) );            
                
            }
            else
            {
                dv[(*it)[(*attrs_it)]]++;
            }
        }

        double sum = 0;
        for(std::map<std::string, double>::iterator ent_it = ent_collections.begin(); ent_it != ent_collections.end(); ent_it++)
        {
            sum = sum + dv[(*ent_it).first]/total * (*ent_it).second; 
        }
    
        if(sum < min_ent)
        {
            sum = min_ent;
            split_attr = *attrs_it;
        }
        
    }

    //划分属性
    cur_node.set_attr(split_attr);
    
    std::map<std::string, std::vector<std::map<std::string, std::string> > > sub_data;
    
    for(std::vector<std::map<std::string, std::string> >::iterator it = data.begin(); it != data.end(); it++)
    {
        if(sub_data.find((*it)[split_attr]) == sub_data.end()) 
        {
            (*it).erase(split_attr);
            sub_data[(*it)[split_attr]].push_back((*it)); 
            data.erase(it);   
        }   
    }

    for(std::map<std::string, std::vector<std::map<std::string, std::string> > >::iterator it = sub_data.begin(); it != sub_data.end(); it++)
    {
        TreeNode *child = new TreeNode();
        build_tree(it->second, classify_attr, (*cur_node.add_child(it->first)));
    }

    
       

    
}

/*
    data:数据集
    classify_res:分类所属属性
    split_attr:划分属性，默认为空，若为空则在data上求entropy
    split_attr_value:划分属性的属性值，配合split_attr使用。若split_attr不为empty，则求其值为split_attr_value的子数据集的entropy。 
*/
double getEnt(std::vector<std::map<std::string, std::string> > data, std::string classify_res, std::string split_attr, std::string split_attr_value)
{
    int total_number = data.size();
    double ent = 0;
    std::map<std::string, int> temprecorder;
    if(split_attr.empty())
    {
        for(std::vector<std::map<std::string, std::string> >::iterator it = data.begin(); it != data.end(); it++)
        {
            std::map<std::string, int>::iterator temp_it = temprecorder.find((*it)[classify_res]);        
            if(temprecorder.end() == temp_it)
            {
                temprecorder[(*it)[classify_res]] = 1;
            }
            else
            {
                temprecorder[(*it)[classify_res]]++;
            }
        }
    }
    else
    {
        for(std::vector<std::map<std::string, std::string> >::iterator it = data.begin(); it != data.end(); it++)
        {
            if((*it)[split_attr] != split_attr_value)
                continue;

            std::map<std::string, int>::iterator temp_it = temprecorder.find((*it)[classify_res]); 
            if(temprecorder.end() == temp_it)
            {
                temprecorder[(*it)[classify_res]] = 1;
            }
            else
            {
                temprecorder[(*it)[classify_res]]++;
            }
        }
    } 

    for(std::map<std::string, int>::iterator it = temprecorder.begin(); it != temprecorder.end(); it++)
    {
       ent += (it->second)*log2(it->second);
    }
    return (0 - ent);
}

TreeNode TreeNode::get_result(std::map<std::string, std::string> & one_item)
{
    if(is_leaf())
        return (*this);
    else
    {
        for(std::map<std::string, TreeNode *>::iterator it = childs.begin(); it != childs.end(); it++)
        {
            if((it->first) == one_item[this->attr])
            {
                return (it->second)->get_result(one_item);
            }
        }
    }
}

TreeNode * TreeNode::add_child(std::string value)
{
    if(childs.find(value) == childs.end()) 
        return NULL;
    
    childs[value] = new TreeNode(); 

    return childs[value];
    
}
    
