//---------------------------------------------------------------------------
#ifndef HPP
#define HPP
#include <map>
#include <list>
#include <set>
#include "../numerical/basic_op.hpp"
#include "../utility/basic_image.hpp"
#include "../utility/pixel_index.hpp"
#include "../numerical/index_algorithm.hpp"
#include "../numerical/window.hpp"


namespace tipl
{

namespace morphology
{

template<typename ImageType>
void erosion(ImageType& I,const std::vector<int>& index_shift)
{
    std::vector<typename ImageType::value_type> act(I.size());
    for (unsigned int index = 0;index < index_shift.size();++index)
    {
        int shift = index_shift[index];
        if (shift > 0)
        {
            auto iter1 = &*act.begin() + shift;
            auto iter2 = &*I.begin();
            auto end = &*act.begin() + act.size();
            for (;iter1 < end;++iter1,++iter2)
                if (*iter2 == 0)
                    *iter1 = 1;
        }
        if (shift < 0)
        {
            auto iter1 = &*act.begin();
            auto iter2 = &*I.begin() - shift;
            auto end = &*I.begin() + I.size();
            for (;iter2 < end;++iter1,++iter2)
                if (*iter2 == 0)
                    *iter1 = 1;
        }
    }

    for (unsigned int index = 0;index < I.size();++index)
        if (act[index])
            I[index] = 0;
}

template<typename ImageType>
void erosion(ImageType& I)
{
    neighbor_index_shift_narrow<ImageType::dimension> neighborhood(I.shape());
    erosion(I,neighborhood.index_shift);
}

template<typename ImageType>
void erosion2(ImageType& I,int radius)
{
    neighbor_index_shift<ImageType::dimension> neighborhood(I.shape(),radius);
    erosion(I,neighborhood.index_shift);
}

template<typename ImageType>
void dilation(ImageType& I,const std::vector<int>& index_shift)
{
    std::vector<typename ImageType::value_type> act(I.size());
    for (unsigned int index = 0;index < index_shift.size();++index)
    {
        int shift = index_shift[index];
        if (shift > 0)
        {
            auto iter1 = &*act.begin() + shift;
            auto iter2 = &*I.begin();
            auto end = &*act.begin() + act.size();
            for (;iter1 < end;++iter1,++iter2)
                *iter1 |= *iter2;
        }
        if (shift < 0)
        {
            auto iter1 = &*act.begin();
            auto iter2 = &*I.begin() - shift;
            auto end = &*I.begin() + I.size();
            for (;iter2 < end;++iter1,++iter2)
                *iter1 |= *iter2;
        }
    }

    for (unsigned int index = 0;index < I.size();++index)
        I[index] |= act[index];
}

template<typename ImageType>
void dilation_mt(ImageType& I,const std::vector<int>& index_shift)
{
    std::vector<typename ImageType::value_type> act(I.size());
    tipl::par_for (index_shift.size(),[&](unsigned int index)
    {
        int shift = index_shift[index];
        if (shift > 0)
        {
            auto iter1 = &*act.begin() + shift;
            auto iter2 = &*I.begin();
            auto end = &*act.begin() + act.size();
            for (;iter1 < end;++iter1,++iter2)
                *iter1 |= *iter2;
        }
        if (shift < 0)
        {
            auto iter1 = &*act.begin();
            auto iter2 = &*I.begin() - shift;
            auto end = &*I.begin() + I.size();
            for (;iter2 < end;++iter1,++iter2)
                *iter1 |= *iter2;
        }
    });
    for (unsigned int index = 0;index < I.size();++index)
        I[index] |= act[index];
}

template<typename ImageType>
void dilation(ImageType& I)
{
    neighbor_index_shift_narrow<ImageType::dimension> neighborhood(I.shape());
    dilation(I,neighborhood.index_shift);
}

template<typename ImageType>
void dilation2(ImageType& I,int radius)
{
    neighbor_index_shift<ImageType::dimension> neighborhood(I.shape(),radius);
    dilation_mt(I,neighborhood.index_shift);
}

/*
template<typename ImageType>
void opening(ImageType& I)
{
    erosion(I);
    dilation(I);
}

template<typename ImageType>
void closing(ImageType& I)
{
    dilation(I);
    erosion(I);
}
*/

template<typename ImageType,typename LabelType,typename ShiftType>
void edge(const ImageType& I,LabelType& act,const ShiftType& shift_list)
{
    act.resize(I.shape());
    for (unsigned int index = 0;index < shift_list.size();++index)
    {
        int shift = shift_list[index];
        if (shift > 0)
        {
            auto iter1 = &*act.begin() + shift;
            auto iter2 = &*I.begin();
            auto iter3 = &*I.begin()+shift;
            auto end = &*act.begin() + act.size();
            for (;iter1 < end;++iter1,++iter2,++iter3)
                if (*iter2 != *iter3)
                    *iter1 = 1;
        }
        if (shift < 0)
        {
            auto iter1 = &*act.begin();
            auto iter2 = &*I.begin() - shift;
            auto iter3 = &*I.begin();
            auto end = &*I.begin() + I.size();
            for (;iter2 < end;++iter1,++iter2,++iter3)
                if (*iter2 != *iter3)
                    *iter1 = 1;
        }
    }
}
template<typename ImageType,typename LabelType>
void edge(const ImageType& I,LabelType& act)
{
    neighbor_index_shift<ImageType::dimension> neighborhood(I.shape());
    edge(I,act,neighborhood.index_shift);
}
template<typename ImageType>
void edge(ImageType& I)
{
    ImageType out;
    edge(I,out);
    I = out;
}

template<typename ImageType>
void edge_thin(ImageType& I)
{
    ImageType out;
    neighbor_index_shift_narrow<ImageType::dimension> neighborhood(I.shape());
    neighborhood.index_shift.resize(neighborhood.index_shift.size()/2);
    edge(I,out,neighborhood.index_shift);
    I = out;
}
template<typename ImageType>
void edge_xy(ImageType& I)
{
    ImageType out;
    std::vector<int> index_shift;
    index_shift.push_back(-1);
    index_shift.push_back(-int(I.width()));
    edge(I,out,index_shift);
    I = out;
}

template<typename ImageType>
void edge_yz(ImageType& I)
{
    ImageType out;
    std::vector<int> index_shift;
    index_shift.push_back(-int(I.width()));
    index_shift.push_back(-int(I.plane_size()));
    edge(I,out,index_shift);
    I = out;
}

template<typename ImageType>
void edge_xz(ImageType& I)
{
    ImageType out;
    std::vector<int> index_shift;
    index_shift.push_back(-1);
    index_shift.push_back(-int(I.plane_size()));
    edge(I,out,index_shift);
    I = out;
}

template<typename ImageType,typename LabelType>
void inner_edge(const ImageType& I,LabelType& act)
{
    act.resize(I.shape());
    neighbor_index_shift<ImageType::dimension> neighborhood(I.shape());
    for (unsigned int index = 0;index < neighborhood.index_shift.size();++index)
    {
        int shift = neighborhood.index_shift[index];
        if (shift > 0)
        {
            typename LabelType::value_type* iter1 = &*act.begin() + shift;
            const auto iter2 = &*I.begin();
            const auto iter3 = &*I.begin()+shift;
            typename LabelType::value_type* end = &*act.begin() + act.size();
            for (;iter1 < end;++iter1,++iter2,++iter3)
                if (*iter2 < *iter3)
                    *iter1 = 1;
        }
        if (shift < 0)
        {
            typename LabelType::value_type* iter1 = &*act.begin();
            const auto iter2 = &*I.begin() - shift;
            const auto iter3 = &*I.begin();
            const auto end = &*I.begin() + I.size();
            for (;iter2 < end;++iter1,++iter2,++iter3)
                if (*iter2 < *iter3)
                    *iter1 = 1;
        }
    }
}

template<typename ImageType>
void inner_edge(ImageType& I)
{
        ImageType out;
        inner_edge(I,out);
        I = out;
}

template<typename ImageType>
bool is_edge(ImageType& I,tipl::pixel_index<2> index)
{
    typename ImageType::value_type center = I[index.index()];
    unsigned int width = I.width();
    unsigned int height = I.height();
    bool have_left = index.x() >= 1;
    bool have_right = index.x()+1 < width;
    if (index.y() >= 1)
    {
        unsigned int base_index = index.index()-width;
        if ((have_left && I[base_index-1] != center) ||
                I[base_index] != center                  ||
                (have_right && I[base_index+1] != center))
            return true;
    }

    if ((have_left && I[index.index()-1] != center) ||
            (have_right && I[index.index()+1] != center))
        return true;

    if (index.y()+1 < height)
    {
        unsigned int base_index = index.index()+width;
        if ((have_left && I[base_index-1] != center) ||
                I[base_index] != center                  ||
                (have_right && I[base_index+1] != center))
            return true;
    }
    return false;
}

template<typename ImageType>
bool is_edge(ImageType& I,tipl::pixel_index<3> index)
{
    typename ImageType::value_type center = I[index.index()];
    unsigned int z_offset = I.shape().plane_size();
    unsigned int y_offset = I.width();
    bool have_left = index.x() >= 1;
    bool have_right = index.x()+1 < I.width();
    bool has_top = index.y() >= 1;
    bool has_bottom = index.y()+1 < I.height();
    if (index.z() >= 1)
    {
        if (has_top)
        {
            if ((have_left  && I[index.index()-1-y_offset-z_offset] != center) ||
                    I[index.index()  -y_offset-z_offset] != center  ||
                    (have_right && I[index.index()+1-y_offset-z_offset] != center))
                return true;
        }

        if ((have_left  && I[index.index()-1-z_offset] != center) ||
                I[index.index()  -z_offset] != center  ||
                (have_right && I[index.index()+1-z_offset] != center))
            return true;

        if (has_bottom)
        {
            if ((have_left  && I[index.index()-1+y_offset-z_offset] != center) ||
                    I[index.index()  +y_offset-z_offset] != center  ||
                    (have_right && I[index.index()+1+y_offset-z_offset] != center))
                return true;
        }
    }

    {
        if (has_top)
        {
            if ((have_left  && I[index.index()-1-y_offset] != center) ||
                    I[index.index()  -y_offset] != center  ||
                    (have_right && I[index.index()+1-y_offset] != center))
                return true;
        }
        {
            if ((have_left  && I[index.index()-1] != center) ||
                    (have_right && I[index.index()+1] != center))
                return true;
        }
        if (has_bottom)
        {
            if ((have_left  && I[index.index()-1+y_offset] != center) ||
                    I[index.index()  +y_offset] != center  ||
                    (have_right && I[index.index()+1+y_offset] != center))
                return true;
        }

    }
    if (index.z()+1 < I.depth())
    {
        if (has_top)
        {
            if ((have_left  && I[index.index()-1-y_offset+z_offset] != center) ||
                    I[index.index()  -y_offset+z_offset] != center  ||
                    (have_right && I[index.index()+1-y_offset+z_offset] != center))
                return true;
        }

        if ((have_left  && I[index.index()-1+z_offset] != center) ||
                I[index.index()  +z_offset] != center  ||
                (have_right && I[index.index()+1+z_offset] != center))
            return true;

        if (has_bottom)
        {
            if ((have_left  && I[index.index()-1+y_offset+z_offset] != center) ||
                    I[index.index()  +y_offset+z_offset] != center  ||
                    (have_right && I[index.index()+1+y_offset+z_offset] != center))
                return true;
        }
    }
    return false;
}

template<typename ImageType>
unsigned char get_neighbor_count(ImageType& I,std::vector<unsigned char>& act)
{
    act.resize(I.size());
    neighbor_index_shift<ImageType::dimension> neighborhood(I.shape());
    tipl::par_for(neighborhood.index_shift.size(),[&](int index)
    {
        int shift = neighborhood.index_shift[index];
        if (shift > 0)
        {
            unsigned char* iter1 = &*act.begin() + shift;
            auto iter2 = &*I.begin();
            unsigned char* end = &*act.begin() + act.size();
            for (;iter1 < end;++iter1,++iter2)
                if (*iter2)
                    (++*iter1);
        }
        if (shift < 0)
        {
            unsigned char* iter1 = &*act.begin();
            auto iter2 = &*I.begin() - shift;
            auto end = &*I.begin() + I.size();
            for (;iter2 < end;++iter1,++iter2)
                if (*iter2)
                    (++*iter1);
        }
    });
    return neighborhood.index_shift.size();
}

template<typename ImageType>
size_t closing(ImageType& I,int threshold_shift = 0)
{
    std::vector<unsigned char> act;
    unsigned int threshold = get_neighbor_count(I,act) >> 1;
    threshold += threshold_shift;
    size_t count = 0;
    for (unsigned int index = 0;index < I.size();++index)
        if (!I[index] && act[index] > threshold)
        {
            I[index] = 1;
            ++count;
        }
    return count;
}

template<typename ImageType>
size_t opening(ImageType& I,int threshold_shift = 0)
{
    std::vector<unsigned char> act;
    unsigned int threshold = get_neighbor_count(I,act) >> 1;
    threshold += threshold_shift;
    size_t count = 0;
    for (unsigned int index = 0;index < I.size();++index)
        if (I[index] && act[index] < threshold)
        {
            I[index] = 0;
            ++count;
        }
    return count;
}

template<typename ImageType>
void negate(ImageType& I)
{
    for (unsigned int index = 0;index < I.size();++index)
        I[index] = I[index] ? 0:1;
}

template<typename ImageType>
void smoothing(ImageType& I)
{
    std::vector<unsigned char> act;
    unsigned int threshold = get_neighbor_count(I,act) >> 1;
    tipl::par_for (I.size(),[&](size_t index)
    {
        if (act[index] > threshold)
        {
            if (!I[index])
                I[index] = 1;
        }
        if (act[index] < threshold)
        {
            if (I[index])
                I[index] = 0;
        }

    });
}

template<typename ImageType>
bool smoothing_fill(ImageType& I)
{
    bool filled = false;
    std::vector<unsigned char> act;
    unsigned int threshold = get_neighbor_count(I,act) >> 1;
    for (unsigned int index = 0;index < I.size();++index)
    {
        if (act[index] > threshold)
        {
            if (!I[index])
            {
                I[index] = 1;
                filled = true;
            }
        }
    }
    return filled;
}

template<typename ImageType>
void recursive_smoothing_mt(ImageType& I,unsigned int max_iteration = 100)
{
    for(unsigned int iter = 0;iter < max_iteration;++iter)
    {
        bool has_change = false;
        std::vector<unsigned char> act;
        unsigned int threshold = get_neighbor_count(I,act) >> 1;
        tipl::par_for(I.size(),[&](size_t index)
        {
            if (act[index] > threshold)
            {
                if (!I[index])
                {
                    I[index] = 1;
                    has_change = true;
                }
            }
            if (act[index] < threshold)
            {
                if (I[index])
                {
                    I[index] = 0;
                    has_change = true;
                }
            }
        });
        if(!has_change)
            break;
    }
}

template<typename ImageType>
void recursive_smoothing(ImageType& I,unsigned int max_iteration = 100)
{
    for(unsigned int iter = 0;iter < max_iteration;++iter)
    {
        bool has_change = false;
        std::vector<unsigned char> act;
        unsigned int threshold = get_neighbor_count(I,act) >> 1;
        for(size_t index = 0;index < I.size();++index)
        {
            if (act[index] > threshold)
            {
                if (!I[index])
                {
                    I[index] = 1;
                    has_change = true;
                }
            }
            if (act[index] < threshold)
            {
                if (I[index])
                {
                    I[index] = 0;
                    has_change = true;
                }
            }
        }
        if(!has_change)
            break;
    }
}

/**
//  grow can be std::equal_to, std::less, std::greater  std::greater_equal std::less_equal
//
*/
//-------------------------------------------------------------------------------
template<typename ImageType,typename IndexType,typename GrowFunc>
void region_growing(const ImageType& I,const IndexType& seed_point,
                    std::vector<IndexType>& grown_region,GrowFunc grow)
{
    std::vector<unsigned char> label_map(I.size());
    std::vector<IndexType> seeds;
    std::vector<IndexType> neighbor;
    seeds.push_back(seed_point);
    label_map[seed_point.index()] = 1;
    for (unsigned int index = 0;index < seeds.size();++index)
    {
        IndexType active_point = seeds[index];
        get_neighbors(active_point,I.shape(),neighbor);
        for (unsigned int index = 0;index < neighbor.size();++index)
        {
            unsigned int cur_neighbor_index = neighbor[index].index();
            if (label_map[cur_neighbor_index])
                continue;
            if (grow(I[active_point.index()],I[cur_neighbor_index]))
            {
                seeds.push_back(neighbor[index]);
                label_map[cur_neighbor_index] = 1;
            }
        }
    }
    seeds.swap(grown_region);
}

template<typename ImageType>
void convex_xy(ImageType& I)
{
    tipl::shape<ImageType::dimension> range_min,range_max;
    bounding_box(I,range_min,range_max);
    // get the bounding box first
    int dirs[8][2] = {{1,0},{2,1},{1,1},{1,2},{0,1},{-1,2},{-1,1},{-2,1}};
    std::vector<unsigned int> fill_buf;
    for(unsigned int i = 0;i < 8;++i)
    {
        int shift = dirs[i][0] + I.width()*dirs[i][1];
        if(shift <= 0)
            continue;
        std::vector<unsigned char> label(I.size());
        for(pixel_index<ImageType::dimension> index;
            index.is_valid(I.shape());
            index.next(I.shape()))
        {
            if(index[0] < range_min[0] || index[0] >= range_max[0] ||
               index[1] < range_min[1] || index[1] >= range_max[1] ||
                    label[index.index()])
                continue;
            bool has_first = false;
            fill_buf.clear();
            for(pixel_index<ImageType::dimension> index2(index);
                index.is_valid(I.shape());)
            {
                if(I[index2.index()])
                {
                    if(!has_first)
                        has_first = true;
                    else
                    {
                        for(unsigned int i = 0;i < fill_buf.size();++i)
                            I[fill_buf[i]] = 1;
                        fill_buf.clear();
                    }
                }
                else
                {
                    if(has_first)
                        fill_buf.push_back(index2.index());
                }
                index2[0] += dirs[i][0];
                index2[1] += dirs[i][1];
                index2.index() += shift;
                if(index2[0] < range_min[0] || index2[0] >= range_max[0] ||
                   index2[1] < range_min[1] || index2[1] >= range_max[1])
                    break;
            }
        }
    }
}

/*
 convex in x direction
*/
template<typename ImageType>
void convex_x(ImageType& I,typename ImageType::value_type assign_value = 1)
{
    typename ImageType::iterator iter = I.begin();
    typename ImageType::iterator end = iter+I.size();
    while(iter != end)
    {
        typename ImageType::iterator next_iter = iter + I.width();
        typename ImageType::iterator first = next_iter;
        typename ImageType::iterator last = next_iter;
        for(;iter != next_iter;++iter)
            if(*iter > 0)
            {
                if(first == next_iter)
                    first = iter;
                else
                    last = iter;
            }
        if(first != next_iter && last != next_iter)
            std::fill(first,last,assign_value);
    }
}

template<typename ImageType>
void convex_y(ImageType& I)
{
    unsigned int plane_size = I.plane_size();
    for(unsigned int iter_plane = 0;iter_plane < I.size();iter_plane += plane_size)
    {
        for(int iter_x = iter_plane,iter_x_end = iter_x + I.width()
                ;iter_x < iter_x_end;++iter_x)
        {
            int iter_y = iter_x;
            int iter_y_end = iter_y+(plane_size-I.width());
            int first,last;
            int find_count = 0;
            for(;iter_y <= iter_y_end;iter_y += I.width())
                if(I[iter_y] > 0)
                {
                    ++find_count;
                    if(find_count == 1)
                        first = iter_y;
                    else
                        last = iter_y;
                }
            if(find_count >= 2)
            {
                for(first += I.width();first != last;first += I.width())
                    I[first] = 1;
            }
        }
    }
}


/*
perform region growing in one dimension
shift = 1 : grow in x dimension
shift = I.width() : grow in y dimension
shift = I.width()*I.height() : grow in z dimension
*/

template<typename ImageType,typename LabelImageType>
void connected_component_labeling_pass(const ImageType& I,
                                       LabelImageType& labels,
                                       std::vector<std::vector<unsigned int> >& regions,
                                       unsigned int shift)
{
    typedef typename std::vector<unsigned int>::const_iterator region_iterator;
    if (shift == 1) // growing in one dimension
    {
        regions.clear();
        labels.resize(I.shape());
        std::mutex add_lock;

        unsigned int width = I.width();
        tipl::par_for(I.size()/width,[&](unsigned int y)
        {
            unsigned int index = y*width;
            unsigned int end_index = index+width;
            while (index < end_index)
            {
                if (I[index] == 0)
                {
                    labels[index] = 0;
                    ++index;
                    continue;
                }
                unsigned int start_index = index;
                do{
                    ++index;
                }while(index < end_index && I[index] != 0);
                std::vector<unsigned int> voxel_pos(index-start_index);
                std::iota(voxel_pos.begin(),voxel_pos.end(),start_index);
                unsigned int group_id;
                {
                    std::lock_guard<std::mutex> lock(add_lock);
                    regions.push_back(std::move(voxel_pos));
                    group_id = regions.size();
                }
                std::fill(labels.begin()+start_index,labels.begin()+index,group_id);
            }
        });
    }
    else
    // growing in higher dimension
    {
        for (unsigned int x = 0;x < shift;++x)
        {
            for (unsigned int index = x,group_id = 0;index < I.size();index += shift)
            {
                if (group_id && labels[index] != 0 && group_id != labels[index])
                {
                    unsigned int from_id = group_id-1;
                    unsigned int to_id = labels[index]-1;
                    if (regions[from_id].size() > regions[to_id].size())
                        std::swap(from_id,to_id);

                    // change the labeling of the merged region
                    {
                        region_iterator end = regions[from_id].end();
                        unsigned int new_id = to_id +1;
                        for (region_iterator iter = regions[from_id].begin();iter != end;++iter)
                            labels[*iter] = new_id;
                    }

                    // merge the region information
                    {
                        regions[to_id].insert(regions[to_id].end(),regions[from_id].begin(),regions[from_id].end());
                        regions[from_id] = std::vector<unsigned int>();
                    }
                }
                group_id = labels[index];
            }
        }
    }
}

template<typename PixelType,typename StorageType,typename LabelImageType>
void connected_component_labeling(const tipl::image<1,PixelType,StorageType>& I,
                                  LabelImageType& labels,
                                  std::vector<std::vector<unsigned int> >& regions)
{
    connected_component_labeling_pass(I,labels,regions,1);
}

template<typename PixelType,typename StorageType,typename LabelImageType>
void connected_component_labeling(const tipl::image<2,PixelType,StorageType>& I,
                                  LabelImageType& labels,
                                  std::vector<std::vector<unsigned int> >& regions)
{
    connected_component_labeling_pass(I,labels,regions,1);
    connected_component_labeling_pass(I,labels,regions,I.width());
}


template<typename PixelType,typename StorageType,typename LabelImageType>
void connected_component_labeling(const tipl::image<3,PixelType,StorageType>& I,
                                  LabelImageType& labels,
                                  std::vector<std::vector<unsigned int> >& regions)
{
    connected_component_labeling_pass(I,labels,regions,1);
    connected_component_labeling_pass(I,labels,regions,I.width());
    connected_component_labeling_pass(I,labels,regions,I.shape().plane_size());
}

template<typename LabelImageType>
void get_region_bounding_box(const LabelImageType& labels,
                             const std::vector<std::vector<unsigned int> >& regions,
                             std::vector<tipl::vector<2,int> >& min_pos,
                             std::vector<tipl::vector<2,int> >& max_pos)
{
    min_pos.clear();
    min_pos.resize(regions.size());
    max_pos.clear();
    max_pos.resize(regions.size());
    std::fill(min_pos.begin(),min_pos.end(),tipl::vector<2,float>(labels.shape()[0],labels.shape()[1]));
    for(tipl::pixel_index<2> index(labels.shape());index < labels.size();++index)
    if (labels[index.index()])
    {
        size_t region_id = labels[index.index()]-1;
        if (regions[region_id].empty())
            continue;
        max_pos[region_id][0] = std::max<int>(index[0],max_pos[region_id][0]);
        max_pos[region_id][1] = std::max<int>(index[1],max_pos[region_id][1]);
        min_pos[region_id][0] = std::min<int>(index[0],min_pos[region_id][0]);
        min_pos[region_id][1] = std::min<int>(index[1],min_pos[region_id][1]);
    }
}

template<typename LabelImageType>
void get_region_bounding_size(const LabelImageType& labels,
                              const std::vector<std::vector<unsigned int> >& regions,
                              std::vector<int>& size_x,
                              std::vector<int>& size_y)
{
    std::vector<tipl::vector<2,int> > max_pos,min_pos;
    tipl::morphology::get_region_bounding_box(labels,regions,min_pos,max_pos);
    size_x.clear();
    size_x.resize(regions.size());
    size_y.clear();
    size_y.resize(regions.size());

    for(size_t index = 0;index < regions.size();++index)
        if(!regions[index].empty())
        {
            size_x[index] = max_pos[index][0]-min_pos[index][0];
            size_y[index] = max_pos[index][1]-min_pos[index][1];
        }
}

template<typename LabelImageType>
void get_region_center(const LabelImageType& labels,
                       const std::vector<std::vector<unsigned int> >& regions,
                       std::vector<tipl::vector<2,float> >& center_of_mass)
{
    center_of_mass.clear();
    center_of_mass.resize(regions.size());
    for(tipl::pixel_index<2> index(labels.shape());index < labels.size();++index)
        if (labels[index.index()])
        {
            size_t region_id = labels[index.index()]-1;
            if (regions[region_id].empty())
                continue;
            center_of_mass[region_id] += tipl::vector<2,float>(index);
        }

    for(size_t index = 0;index < regions.size();++index)
        if(!regions[index].empty())
            center_of_mass[index] /= regions[index].size();
}

template<typename ImageType>
void defragment(ImageType& I)
{
    tipl::image<ImageType::dimension,unsigned int> labels(I.shape());
    std::vector<std::vector<unsigned int> > regions;

    connected_component_labeling(I,labels,regions);

    unsigned int max_size_group_id = 1;
    if (!regions.empty())
    {
        unsigned int max_size = regions[0].size();
        for (unsigned int index = 1;index < regions.size();++index)
            if (regions[index].size() > max_size)
            {
                max_size = regions[index].size();
                max_size_group_id = index+1;
            }
    }

    for (unsigned int index = 0;index < I.size();++index)
        if (I[index] && labels[index] != max_size_group_id)
            I[index] = 0;
}

template<typename ImageType>
void defragment_by_size(ImageType& I,unsigned int area_threshold)
{
    tipl::image<ImageType::dimension,unsigned int> labels(I.shape());
    std::vector<std::vector<unsigned int> > regions;

    connected_component_labeling(I,labels,regions);

    std::vector<unsigned char> region_filter(regions.size()+1);

    for (unsigned int index = 0;index < regions.size();++index)
        region_filter[index+1] = regions[index].size() > area_threshold;

    for (unsigned int index = 0;index < I.size();++index)
        if (I[index] && !region_filter[labels[index]])
            I[index] = 0;
}

template<typename ImageType,typename PixelIndexType,typename ValueType>
void fill(ImageType& I,PixelIndexType seed_point,ValueType new_value)
{
    std::deque<PixelIndexType> seeds;
    seeds.push_back(seed_point);
    ValueType old_value = I[seed_point.index()];
    I[seed_point.index()] = new_value;
    while (seeds.size())
    {
        PixelIndexType active_point = seeds.front();
        seeds.pop_front();
        std::vector<PixelIndexType> neighbor;
        get_neighbors(active_point,I.shape(),neighbor);
        for (unsigned int index = 0;index < neighbor.size();++index)
        {
            if (I[neighbor[index].index()] != old_value)
                continue;
            seeds.push_back(neighbor[index]);
            I[neighbor[index].index()] = new_value;
        }
    }
}

}
}
#endif
