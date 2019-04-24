#ifndef BRUKER2DSEQ_HPP
#define BRUKER2DSEQ_HPP
#include <string>
#include <map>
#include <sstream>
#include <iterator>
#include <memory>
#include "tipl/numerical/basic_op.hpp"

namespace tipl
{



namespace io
{

class bruker_info
{
    typedef std::map<std::string,std::string> info_type;
    info_type info;
    std::string dummy;
    /*
    ##$RECO_size=( 2 )
    128 128                 <--info
    */
private:
    void load_info(std::ifstream& in)
    {
        std::string line;
        info.clear();
        while(std::getline(in,line))
        {
            if(line.size() < 4 ||
                    line[0] != '#' ||
                    line[1] != '#')
                continue;

            std::string::iterator sep = std::find(line.begin(),line.end(),'=');
            if(sep == line.end() || sep+1 == line.end())
                continue;
            std::string name(line.begin()+((line[2] == '$') ? 3: 2),sep);
            info[name] =
                std::string(sep+1,line.end());
            if(*(sep+1) == '(')
            {
                std::string accumulated_info;
                while(in && in.peek() != '#')
                {
		    std::getline(in,line);
		    if(line[0] == '$')
    			continue;
                    accumulated_info += line;
                    accumulated_info += " ";
                }
                using namespace std;
                accumulated_info.erase(remove(accumulated_info.begin(),accumulated_info.end(),'<'),accumulated_info.end());
                accumulated_info.erase(remove(accumulated_info.begin(),accumulated_info.end(),'>'),accumulated_info.end());
                info[name] = accumulated_info;
            }
        }
    }
public:
    template<class char_type>
    bool load_from_file(const char_type* file_name)

    {
        std::ifstream info(file_name);
        if(!info)
            return false;
        load_info(info);
        return true;
    }
    bool has_field(const std::string& tag) const
    {
        return info.find(tag) != info.end();
    }
    const std::string& operator[](const std::string& tag) const
    {
        info_type::const_iterator iter = info.find(tag);
        if(iter == info.end())
            return dummy;
        return iter->second;
    }
    template<typename value_type>
    bool read(const std::string& tag,std::vector<value_type>& data) const
    {
        data.clear();
        std::istringstream in((*this)[tag]);
        while(in)
        {
            std::string item;
            in >> item;
            if(item.empty())
                continue;
            // handle repeat value in parameter file e.g. @40*(1233717)
            if(item[0] == '@' && item.find('*') != std::string::npos)
            {
                std::string count_text = item.substr(1,item.find('*')-1);
                int count = std::stoi(count_text);
                int start = item.find('(');
                int end = item.find(')');
                if(start != std::string::npos && end != std::string::npos)
                {
                    std::string value_text = item.substr(start+1,end-start-1);
                    double value = std::stod(value_text);
                    for(int i = 0;i < count;++i)
                        data.push_back(value);
                }
                continue;
            }
            value_type value;
            std::istringstream(item) >> value;
            data.push_back(value);
        }
        return !data.empty();
    }
};


class bruker_2dseq
{
    // the 2dseq data
    tipl::image<float,3> data;


    // spatial resolution
    float resolution[3];
    float orientation[9];
    bool slice_2d = true;
private:
    std::string tmp;
    std::wstring wtmp;

    bool check_name(const char* filename)
    {
        std::string str = filename;
        if(str.length() < 5)
            return false;
        std::string name(str.end()-5,str.end());
        if(name[0] != '2' || name[1] != 'd' || name[2] != 's' || name[3] != 'e' || name[4] != 'q')
            return false;
        return true;
    }
    bool check_name(const wchar_t* filename)
    {
        std::wstring str = filename;
        if(str.length() < 5)
            return false;
        std::wstring name(str.end()-5,str.end());
        if(name[0] != L'2' || name[1] != L'd' || name[2] != L's' || name[3] != L'e' || name[4] != L'q')
            return false;
        return true;
    }
    const char* load_reco(const char* filename)
    {
        std::string str = filename;
        tmp = std::string(str.begin(),str.end()-5);
        tmp += "reco";
        return tmp.c_str();
    }
    const wchar_t* load_reco(const wchar_t* filename)
    {
        std::wstring str = filename;
        wtmp = std::wstring(str.begin(),str.end()-5);
        wtmp += L"reco";
        return wtmp.c_str();
    }
    const char* load_visu(const char* filename)
    {
        std::string str = filename;
        tmp = std::string(str.begin(),str.end()-5);
        tmp += "visu_pars";
        return tmp.c_str();
    }
    const wchar_t* load_visu(const wchar_t* filename)
    {
        std::wstring str = filename;
        wtmp = std::wstring(str.begin(),str.end()-5);
        wtmp += L"visu_pars";
        return wtmp.c_str();
    }
    
public:
    std::vector<float> slopes;

    template<class char_type>
    bool load_from_file(const char_type* file_name)
    {
        if(!check_name(file_name))
            return false;

        // read image dimension
        bool no_visu = false;
        bruker_info visu,info;
        if(!info.load_from_file(load_reco(file_name)))
            return false;
        if(!visu.load_from_file(load_visu(file_name)))
            no_visu = true;

        tipl::geometry<3> dim;
        // get image dimension
        if(!no_visu)
        {
            std::istringstream(visu["VisuCoreSize"]) >> dim[0] >> dim[1] >> dim[2];
            std::istringstream(visu["VisuCoreOrientation"])
                >> orientation[0] >> orientation[1] >> orientation[2]
                >> orientation[3] >> orientation[4] >> orientation[5]
                >> orientation[6] >> orientation[7] >> orientation[8];
        }
        // get image slope
        {
            if(!info.read("RECO_map_slope",slopes))
                return false;
            float max_slope = *std::max_element(slopes.begin(),slopes.end());
            for(unsigned int i = 0;i < slopes.size();++i)
                slopes[i] /= max_slope;
        }
        // get resolution
        {
            std::vector<float> fov_data,size; // in cm
            info.read("RECO_fov",fov_data);
            info.read("RECO_size",size);
            std::fill(resolution,resolution+3,0.0);
            for(unsigned int index = 0;index < 3 && index < fov_data.size() && index < size.size();++index)
                resolution[index] = fov_data[index]*10.0/size[index]; // in mm

            if(no_visu)
            {
                dim[0] = size[0];
                dim[1] = size[1];
                dim[2] = slopes.size();
            }

            if(resolution[2] == 0)
                resolution[2] = resolution[0];
        }


        // read image data
        std::vector<char> buffer;
        std::ifstream in(file_name,std::ios::binary);
        in.seekg(0, std::ifstream::end);
        buffer.resize(in.tellg());
        in.seekg(0, std::ifstream::beg);
        in.read((char*)&*buffer.begin(),buffer.size());

        int word_size = 1;
        if (info["RECO_wordtype"].find("16BIT") != std::string::npos)
            word_size = 2;
        if (info["RECO_wordtype"].find("32BIT") != std::string::npos)
            word_size = 4;
        if(info["RECO_byte_order"] == std::string("bigEndian"))
        {
            if (word_size == 2)
                change_endian((short*)&buffer[0],buffer.size()/word_size);
            if (word_size == 4)
                change_endian((int*)&buffer[0],buffer.size()/word_size);
        }
        // read 2dseq and convert to float
        dim[2] = buffer.size()/word_size/dim[0]/dim[1];
        data.resize(dim);
        if (info["RECO_wordtype"] == std::string("_8BIT_SGN_INT"))
            std::copy((char*)&buffer[0],(char*)&buffer[0]+data.size(),data.begin());
        if (info["RECO_wordtype"] == std::string("_8BIT_USGN_INT"))
            std::copy((unsigned char*)&buffer[0],(unsigned char*)&buffer[0]+data.size(),data.begin());
        if (info["RECO_wordtype"] == std::string("_16BIT_SGN_INT"))
            std::copy((int16_t*)&buffer[0],(int16_t*)&buffer[0]+data.size(),data.begin());
        if (info["RECO_wordtype"] == std::string("_16BIT_USGN_INT"))
            std::copy((uint16_t*)&buffer[0],(uint16_t*)&buffer[0]+data.size(),data.begin());
        if (info["RECO_wordtype"] == std::string("_32BIT_USGN_INT"))
            std::copy((uint32_t*)&buffer[0],(uint32_t*)&buffer[0]+data.size(),data.begin());
        if (info["RECO_wordtype"] == std::string("_32BIT_SGN_INT"))
            std::copy((int32_t*)&buffer[0],(int32_t*)&buffer[0]+data.size(),data.begin());
        if (info["RECO_wordtype"] == std::string("_32BIT_FLOAT"))
            std::copy((float*)&buffer[0],(float*)&buffer[0]+data.size(),data.begin());

        if(!slopes.empty())
        {
            unsigned int plane_size = dim[0]*dim[1];
            std::vector<float>::iterator iter = data.begin();
            for(unsigned int z = 0;z < dim[2];++z)
            {
                int slope_index = std::floor(float(z)*slopes.size()/dim[2]);
                if(slope_index >= slopes.size())
                   slope_index = slopes.size()-1;
                float s = slopes[slope_index];
                for(unsigned int index = 0;index < plane_size;++index,++iter)
                    *iter /= s;
            }
        }

        slice_2d = (dim[2] == 0);
        return true;
    }
    const tipl::image<float,3>& get_image(void) const{return data;}
    tipl::image<float,3>& get_image(void){return data;}

    bool is_2d(void) const
    {
        return slice_2d;
    }

    void get_voxel_size(tipl::vector<3>& vs) const
    {
        std::copy(resolution,resolution+3,vs.begin());
    }

    template<class image_type>
    void save_to_image(image_type& out) const
    {
        out.resize(data.geometry());
        std::copy(data.begin(),data.end(),out.begin());
    }

    template<class image_type>
    const bruker_2dseq& operator>>(image_type& source) const
    {
        save_to_image(source);
        return *this;
    }
};




}






}





#endif//2DSEQ_HPP
