//---------------------------------------------------------------------------
#ifndef basic_imageH
#define basic_imageH
#include <vector>
#include <thread>
#include <future>
#include "../def.hpp"
#include "shape.hpp"
#include "pixel_value.hpp"
#include "pixel_index.hpp"

//---------------------------------------------------------------------------
namespace tipl
{

template<typename vtype>
class pointer_container
{
public:
    using value_type        = vtype;
    using iterator          = vtype*;
    using const_iterator    = vtype*;
    using reference         = vtype&;
protected:
    iterator from,to;
    size_t size_;
public:
    __INLINE__ pointer_container(void):from(0),to(0),size_(0){}
    __INLINE__ pointer_container(size_t size_):from(0),to(0),size_(size_){}
    template<typename any_iterator_type>
    __INLINE__ pointer_container(any_iterator_type from_,any_iterator_type to_):
        from(&*from_),to(0),size_(to_-from_){to = from + size_;}
    template<typename any_container_type>
    __INLINE__ pointer_container(const any_container_type& rhs){operator=(rhs);}
    __INLINE__ pointer_container(std::vector<value_type>& rhs)
    {
        if((size_ = rhs.size()))
        {
            from = &rhs[0];
            to = from + size_;
        }
        else
            from = to = 0;
    }
public:
    template<typename any_container_type>
    __INLINE__ pointer_container& operator=(const any_container_type& rhs)
    {
        if (this == &rhs)
            return *this;
        from = rhs.from;
        to = rhs.to;
        size_ = rhs.size_;
    }
public:
    template<typename index_type>
    __INLINE__ reference operator[](index_type index) const
    {
        return from[index];
    }
    template<typename index_type>
    __INLINE__ reference operator[](index_type index)
    {
        return from[index];
    }
    __INLINE__ iterator begin(void) const
    {
        return from;
    }
    __INLINE__ iterator end(void) const
    {
        return to;
    }
    __INLINE__ iterator begin(void)
    {
        return from;
    }
    __INLINE__ iterator end(void)
    {
        return to;
    }

    __INLINE__ size_t size(void)            const
    {
        return size_;
    }

    __INLINE__ bool empty(void) const
    {
        return size_ == 0;
    }
    __INLINE__ void clear(void)
    {
        size_ = 0;
        to = from;
    }

public:
    __INLINE__ void swap(pointer_container& rhs)
    {
        std::swap(from,rhs.from);
        std::swap(to,rhs.to);
        std::swap(size_,rhs.size_);
    }
    __INLINE__ void resize(size_t new_size)
    {
        size_ = new_size;
        to = from + size_;
    }
};

template<typename vtype>
class const_pointer_container
{
public:
    using value_type        = vtype;
    using iterator          = const vtype*;
    using const_iterator    = const vtype*;
    using reference         = const vtype&;
protected:
    const_iterator from,to;
    size_t size_;
public:
    __INLINE__ const_pointer_container(void):from(0),to(0),size_(0){}
    __INLINE__ const_pointer_container(size_t size_):from(0),to(0),size_(size_){}
    template<typename any_iterator_type>
    __INLINE__ const_pointer_container(any_iterator_type from_,any_iterator_type to_):
        from(&*from_),to(0),size_(to_-from_){to = from + size_;}
    template<typename any_container_type>
    __INLINE__ const_pointer_container(const any_container_type& rhs){operator=(rhs);}
    __INLINE__ const_pointer_container(const std::vector<value_type>& rhs)
    {
        if((size_ = rhs.size()))
        {
            from = &rhs[0];
            to = from + size_;
        }
        else
            from = to = 0;
    }
public:
    template<typename any_container_type>
    __INLINE__ const_pointer_container& operator=(const any_container_type& rhs)
    {
        if (this == &rhs)
            return *this;
        from = rhs.begin();
        to = rhs.end();
        size_ = rhs.size();
        return *this;
    }
public:
    template<typename index_type>
    __INLINE__ reference operator[](index_type index) const
    {
        return from[index];
    }
    template<typename index_type>
    __INLINE__ reference operator[](index_type index)
    {
        return from[index];
    }
    __INLINE__ const_iterator begin(void) const
    {
        return from;
    }
    __INLINE__ const_iterator end(void) const
    {
        return to;
    }
    __INLINE__ const_iterator begin(void)
    {
        return from;
    }
    __INLINE__ const_iterator end(void)
    {
        return to;
    }

    __INLINE__ size_t size(void) const
    {
        return size_;
    }

    __INLINE__ bool empty(void) const
    {
        return size_ == 0;
    }
public:
    __INLINE__ void swap(const_pointer_container<value_type>& rhs)
    {
        std::swap(from,rhs.from);
        std::swap(to,rhs.to);
        std::swap(size_,rhs.size_);
    }
    __INLINE__ void resize(size_t new_size)
    {
        size_ = new_size;
        to = from + size_;
    }
};


template <int dim,typename vtype = float,template <typename...> typename stype = std::vector>
class image
{
public:
    using value_type        = vtype;
    using storage_type      = stype<vtype>;
    using iterator          = typename storage_type::iterator;
    using const_iterator    = typename storage_type::const_iterator ;
    using reference         = typename storage_type::reference ;
    using slice_type        = tipl::image<dim-1,value_type,pointer_container> ;
    using const_slice_type  = tipl::image<dim-1,value_type,const_pointer_container>;
    using shape_type        = tipl::shape<dim>;
    static constexpr int dimension = dim;
protected:
    storage_type data;
    shape_type geo;
public:
    __INLINE__ const shape_type& shape(void)    const   {return geo;}
    __INLINE__ int width(void)                  const   {return geo.width();}
    __INLINE__ int height(void)                 const   {return geo.height();}
    __INLINE__ int depth(void)                  const   {return geo.depth();}
    __INLINE__ size_t plane_size(void)          const   {return geo.plane_size();}
public:
    __INLINE__ value_type at(unsigned int x,unsigned int y) const   {return data[size_t(y)*geo[0]+x];}
    __INLINE__ reference at(unsigned int x,unsigned int y)          {return data[size_t(y)*geo[0]+x];}
    __INLINE__ value_type at(unsigned int x,unsigned int y,unsigned int z) const    {return data[size_t(z*geo[1]+y)*geo[0]+x];}
    __INLINE__ reference at(unsigned int x,unsigned int y,unsigned int z)           {return data[size_t(z*geo[1]+y)*geo[0]+x];}
public:
    __INLINE__ image(void) {}
    template<typename T,typename std::enable_if<T::dimension==dimension && !std::is_same<storage_type,typename T::storage_type>::value,bool>::type = true>
    __INLINE__ image(const T& rhs)      {operator=(rhs);}
    __INLINE__ image(const image& rhs)  {operator=(rhs);}
    __INLINE__ image(image&& rhs)       {operator=(rhs);}
public:
    template<typename T>
    __INLINE__ image(const std::initializer_list<T>& rhs):geo(rhs)      {data.resize(geo.size());}
    __INLINE__ image(const shape_type& geo_):data(geo_.size()),geo(geo_){}
public:
    template<typename T,typename std::enable_if<std::is_fundamental<T>::value,bool>::type = true>
    __INLINE__ image(T* pointer,const shape_type& geo_):data(pointer,pointer+geo_.size()),geo(geo_)         {}
    template<typename T,typename std::enable_if<std::is_fundamental<T>::value,bool>::type = true>
    __INLINE__ image(const T* pointer,const shape_type& geo_):data(pointer,pointer+geo_.size()),geo(geo_)   {}
public:
    template<typename T,typename std::enable_if<T::dimension==dimension && !std::is_same<storage_type,typename T::storage_type>::value,bool>::type = true>
    __INLINE__ image& operator=(const T& rhs)
    {
        storage_type new_data(rhs.begin(),rhs.end());
        data.swap(new_data);
        geo = rhs.shape();
        return *this;
    }
    __INLINE__ image& operator=(const image& rhs)
    {
        storage_type new_data(rhs.begin(),rhs.end());
        data.swap(new_data);
        geo = rhs.shape();
        return *this;
    }
    __INLINE__ image& operator=(image&& rhs)
    {
        data.swap(rhs.data);
        geo.swap(rhs.geo);
        return *this;
    }
public:
    __INLINE__ storage_type& get(void){return data;}
    __INLINE__ const storage_type& get(void)const{return data;}
    __INLINE__ void swap(image& rhs)
    {
        data.swap(rhs.data);
        geo.swap(rhs.geo);
    }
    __INLINE__ void resize(const shape_type& geo_)
    {
        geo = geo_;
        data.resize(geo.size());
    }
    __INLINE__ void clear(void)
    {
        data.clear();
        geo.clear();
    }
    __INLINE__ size_t size(void)    const    {return data.size();}
    __INLINE__ bool empty(void)     const   {return data.empty();}

    template<typename index_type>
    __INLINE__ const value_type& operator[](index_type index)   const   {return data[index];}
    __INLINE__ value_type front(void)                           const   {return data.front();}
    __INLINE__ value_type back(void)                            const   {return data.back();}
    __INLINE__ const_iterator begin(void)                       const   {return data.begin();}
    __INLINE__ const_iterator end(void)                         const   {return data.end();}

    template<typename index_type>
    __INLINE__ reference operator[](index_type index)                   {return data[index];}
    __INLINE__ reference front(void)                                    {return data.front();}
    __INLINE__ reference back(void)                                     {return data.back();}
    __INLINE__ iterator begin(void)                                     {return data.begin();}
    __INLINE__ iterator end(void)                                       {return data.end();}
public:
    __INLINE__ slice_type slice_at(unsigned int pos)
    {
        tipl::shape<dim-1> slice_geo(geo.begin());
        return slice_type(&*data.begin()+pos*slice_geo.size(),slice_geo);
    }
    __INLINE__ const_slice_type slice_at(unsigned int pos) const
    {
        tipl::shape<dim-1> slice_geo(geo.begin());
        return const_slice_type(&*data.begin()+pos*slice_geo.size(),slice_geo);
    }
public:
    template<typename T,typename std::enable_if<std::is_fundamental<T>::value,bool>::type = true>
    __INLINE__ const image operator+=(T value)
    {
        iterator end_iter = data.end();
        for(iterator iter = data.begin();iter != end_iter;++iter)
            *iter += value;
        return *this;
    }
    template<typename T,typename std::enable_if<std::is_fundamental<T>::value,bool>::type = true>
    __INLINE__ const image operator-=(T value)
    {
        iterator end_iter = data.end();
        for(iterator iter = data.begin();iter != end_iter;++iter)
            *iter -= value;
        return *this;
    }
    template<typename T,typename std::enable_if<std::is_fundamental<T>::value,bool>::type = true>
    __INLINE__ const image operator*=(T value)
    {
        iterator end_iter = data.end();
        for(iterator iter = data.begin();iter != end_iter;++iter)
            *iter *= value;
        return *this;
    }
    template<typename T,typename std::enable_if<std::is_fundamental<T>::value,bool>::type = true>
    __INLINE__ const image operator/=(T value)
    {
        iterator end_iter = data.end();
        for(iterator iter = data.begin();iter != end_iter;++iter)
            *iter /= value;
        return *this;
    }
    template<typename T,typename std::enable_if<std::is_class<T>::value,bool>::type = true>
    __INLINE__ const image& operator+=(const T& rhs)
    {
        iterator end_iter = data.end();
        auto iter2 = rhs.begin();
        for(iterator iter = data.begin();iter != end_iter;++iter,++iter2)
            *iter += *iter2;
        return *this;
    }
    template<typename T,typename std::enable_if<std::is_class<T>::value,bool>::type = true>
    __INLINE__ const image& operator-=(const T& rhs)
    {
        iterator end_iter = data.end();
        auto iter2 = rhs.begin();
        for(iterator iter = data.begin();iter != end_iter;++iter,++iter2)
            *iter -= *iter2;
        return *this;
    }
    template<typename T,typename std::enable_if<std::is_class<T>::value,bool>::type = true>
    __INLINE__ const image& operator*=(const T& rhs)
    {
        iterator end_iter = data.end();
        auto iter2 = rhs.begin();
        for(iterator iter = data.begin();iter != end_iter;++iter,++iter2)
            *iter *= *iter2;
        return *this;
    }
    template<typename T,typename std::enable_if<std::is_class<T>::value,bool>::type = true>
    __INLINE__ const image& operator/=(const T& rhs)
    {
        iterator end_iter = data.end();
        auto iter2 = rhs.begin();
        for(iterator iter = data.begin();iter != end_iter;++iter,++iter2)
            *iter /= *iter2;
        return *this;
    }
public:
    template<typename format_type>
    bool save_to_file(const char* file_name) const
    {
        format_type out;
        out.load_from_image(*this);
        return out.save_to_file(file_name);
    }
    template<typename format_type,typename voxel_type>
    bool save_to_file(const char* file_name,voxel_type vs) const
    {
        format_type out;
        out.load_from_image(*this);
        out.set_voxel_size(vs);
        return out.save_to_file(file_name);
    }
    template<typename format_type>
    bool load_from_file(const char* file_name)
    {
        format_type out;
        if(!out.load_from_file(file_name))
            return false;
        out.save_to_image(*this);
        return true;
    }
};


typedef image<2,rgb> color_image;
typedef image<2,unsigned char> grayscale_image;

template<int dim,typename vtype = float>
class pointer_image : public image<dim,vtype,pointer_container>
{
public:
    using base_type         = image<dim,vtype,pointer_container>;
    using iterator          = typename base_type::iterator;
    using const_iterator    = typename base_type::iterator;
    using storage_type      = typename image<dim,vtype,pointer_container>::storage_type;
    static const int dimension = dim;
public:
    __INLINE__ pointer_image(void) {}
    __INLINE__ pointer_image(const pointer_image& rhs):base_type(){operator=(rhs);}
    template<typename T,typename std::enable_if<T::dimension==dimension && !std::is_same<storage_type,typename T::storage_type>::value,bool>::type = true>
    __INLINE__ pointer_image(const T& rhs):base_type(&*rhs.begin(),rhs.shape()) {}
    __INLINE__ pointer_image(vtype* pointer,const tipl::shape<dim>& geo_):base_type(pointer,geo_) {}
public:
    __INLINE__ pointer_image& operator=(const pointer_image& rhs)
    {
        base_type::data = rhs.data;
        base_type::geo = rhs.shape();
        return *this;
    }
};

template<int dim,typename vtype = float>
class const_pointer_image : public image<dim,vtype,const_pointer_container>
{
public:
    using value_type        =   vtype;
    using base_type         =   image<dim,value_type,const_pointer_container>;
    using iterator          =   typename base_type::iterator;
    using const_iterator    =   typename base_type::const_iterator;
    using storage_type      =   typename image<dim,vtype,const_pointer_container>::storage_type;
    static const int dimension = dim;
public:
    __INLINE__ const_pointer_image(void) {}
    __INLINE__ const_pointer_image(const const_pointer_image& rhs):base_type(){operator=(rhs);}
    template<typename T,typename std::enable_if<T::dimension==dimension && !std::is_same<storage_type,typename T::storage_type>::value,bool>::type = true>
    __INLINE__ const_pointer_image(const T& rhs):base_type(&*rhs.begin(),rhs.shape()) {}
    __INLINE__ const_pointer_image(const vtype* pointer,const tipl::shape<dim>& geo_):base_type(pointer,geo_){}
public:
    __INLINE__ const_pointer_image& operator=(const const_pointer_image& rhs)
    {
        base_type::data = rhs.data;
        base_type::geo = rhs.shape();
        return *this;
    }

};


template<typename value_type,typename shape_type>
__INLINE__ pointer_image<shape_type::dimension,value_type>
    make_image(value_type* pointer,const shape_type& geo)
{
    return pointer_image<shape_type::dimension,value_type>(pointer,geo);
}

template<typename value_type,typename shape_type>
__INLINE__ const_pointer_image<shape_type::dimension,value_type>
    make_image(const value_type* pointer,const shape_type& geo)
{
    return const_pointer_image<shape_type::dimension,value_type>(pointer,geo);
}

}
#endif
