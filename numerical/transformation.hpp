#ifndef TRANSFORMATION_HPP_INCLUDED
#define TRANSFORMATION_HPP_INCLUDED
#include "../numerical/matrix.hpp"
#include "../def.hpp"
namespace tipl{

template<unsigned int dim>
struct vdim {};

template<typename input_iter1,typename input_iter2,typename output_iter>
__DEVICE_HOST__ void vector_transformation(input_iter1 vec_in,output_iter vec_out,input_iter2 trans,vdim<2>)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    vec_out[0] = value_type(vec_in[0])*trans[0] +
                 value_type(vec_in[1])*trans[1] +
                 value_type(trans[2]);
    vec_out[1] = value_type(vec_in[0])*trans[3] +
                 value_type(vec_in[1])*trans[4] +
                 value_type(trans[5]);
}

template<typename input_iter1,typename input_iter2,typename output_iter>
__DEVICE_HOST__  void vector_transformation(input_iter1 vec_in,output_iter vec_out,input_iter2 trans,vdim<3>)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    vec_out[0] = value_type(vec_in[0])*trans[0] +
                 value_type(vec_in[1])*trans[1] +
                 value_type(vec_in[2])*trans[2] +
                 value_type(trans[3]);
    vec_out[1] = value_type(vec_in[0])*trans[4] +
                 value_type(vec_in[1])*trans[5] +
                 value_type(vec_in[2])*trans[6] +
                 value_type(trans[7]);
    vec_out[2] = value_type(vec_in[0])*trans[8] +
                 value_type(vec_in[1])*trans[9] +
                 value_type(vec_in[2])*trans[10] +
                 value_type(trans[11]);
}

template<typename input_iter1,typename input_iter2,typename input_iter3,typename output_iter>
__DEVICE_HOST__ void vector_transformation(input_iter1 vec_in,output_iter vec_out,input_iter2 rotation,input_iter3 shift,vdim<2>)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    vec_out[0] = value_type(vec_in[0])*rotation[0] +
                 value_type(vec_in[1])*rotation[1] +
                 value_type(shift[0]);
    vec_out[1] = value_type(vec_in[0])*rotation[2] +
                 value_type(vec_in[1])*rotation[3] +
                 value_type(shift[1]);
}


template<typename input_iter1,typename input_iter2,typename input_iter3,typename output_iter>
__DEVICE_HOST__ void vector_transformation(input_iter1 vec_in,output_iter vec_out,input_iter2 rotation,input_iter3 shift,vdim<3>)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;

    vec_out[0] = value_type(vec_in[0])*rotation[0] +
                 value_type(vec_in[1])*rotation[1] +
                 value_type(vec_in[2])*rotation[2] +
                 value_type(shift[0]);
    vec_out[1] = value_type(vec_in[0])*rotation[3] +
                 value_type(vec_in[1])*rotation[4] +
                 value_type(vec_in[2])*rotation[5] +
                 value_type(shift[1]);
    vec_out[2] = value_type(vec_in[0])*rotation[6] +
                 value_type(vec_in[1])*rotation[7] +
                 value_type(vec_in[2])*rotation[8] +
                 value_type(shift[2]);
}

template<typename input_iter1,typename input_iter2,typename output_iter>
__DEVICE_HOST__ void vector_rotation(input_iter1 vec_in,output_iter vec_out,input_iter2 rotation,vdim<2>)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    vec_out[0] = value_type(vec_in[0])*rotation[0] +
                 value_type(vec_in[1])*rotation[1];
    vec_out[1] = value_type(vec_in[0])*rotation[2] +
                 value_type(vec_in[1])*rotation[3];
}

template<typename input_iter1,typename input_iter2,typename output_iter>
__DEVICE_HOST__ void vector_rotation(input_iter1 vec_in,output_iter vec_out,input_iter2 rotation,vdim<3>)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    vec_out[0] = value_type(vec_in[0])*rotation[0] +
                 value_type(vec_in[1])*rotation[1] +
                 value_type(vec_in[2])*rotation[2];
    vec_out[1] = value_type(vec_in[0])*rotation[3] +
                 value_type(vec_in[1])*rotation[4] +
                 value_type(vec_in[2])*rotation[5];
    vec_out[2] = value_type(vec_in[0])*rotation[6] +
                 value_type(vec_in[1])*rotation[7] +
                 value_type(vec_in[2])*rotation[8];
}


/** Perform C= AB
    A,B,C are 2-by-2 matrices
*/
template<typename input_iterator1,typename input_iterator2,typename output_iterator>
__DEVICE_HOST__ void matrix_product(input_iterator1 A,input_iterator2 B,output_iterator C,vdim<2>)
{
    C[0] = A[0] * B[0] + A[1] * B[2];
    C[1] = A[0] * B[1] + A[1] * B[3];

    C[2] = A[2] * B[0] + A[3] * B[2];
    C[3] = A[2] * B[1] + A[3] * B[3];

}

/** Perform C= AB
    A,B,C are 3-by-3 matrices
*/
template<typename input_iterator1,typename input_iterator2,typename output_iterator>
__DEVICE_HOST__ void matrix_product(input_iterator1 A,input_iterator2 B,output_iterator C,vdim<3>)
{
    C[0] = A[0] * B[0] + A[1] * B[3] + A[2] * B[6];
    C[1] = A[0] * B[1] + A[1] * B[4] + A[2] * B[7];
    C[2] = A[0] * B[2] + A[1] * B[5] + A[2] * B[8];

    C[3] = A[3] * B[0] + A[4] * B[3] + A[5] * B[6];
    C[4] = A[3] * B[1] + A[4] * B[4] + A[5] * B[7];
    C[5] = A[3] * B[2] + A[4] * B[5] + A[5] * B[8];

    C[6] = A[6] * B[0] + A[7] * B[3] + A[8] * B[6];
    C[7] = A[6] * B[1] + A[7] * B[4] + A[8] * B[7];
    C[8] = A[6] * B[2] + A[7] * B[5] + A[8] * B[8];
}

template<typename angle_type,typename output_iter>
__DEVICE_HOST__ void rotation_matrix(angle_type theta,output_iter m,vdim<2>)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    value_type cos_theta = std::cos(theta);
    value_type sin_theta = std::sin(theta);
    m[0] = cos_theta;
    m[1] = -sin_theta;
    m[2] = sin_theta;
    m[3] = cos_theta;
}

/*
 example

    float angle[3] = {1,2,3};// SPM use [1 -2 3]
    float result[9];
    tipl::rotation_matrix(angle,result,tipl::vdim<3>());
    std::copy(result,result+9,std::ostream_iterator<float>(std::cout," "));
    return 0;

 */
//a clockwise/left-handed rotation with Euler angles
template<typename angle_type,typename output_type>
__DEVICE_HOST__ void rotation_matrix(angle_type theta,output_type m,vdim<3>)
{
    typedef typename std::iterator_traits<angle_type>::value_type value_type;
    value_type sin_x = std::sin(theta[0]);
    value_type cos_x = std::cos(theta[0]);
    value_type sin_y = std::sin(theta[1]);
    value_type cos_y = std::cos(theta[1]);
    value_type sin_z = std::sin(theta[2]);
    value_type cos_z = std::cos(theta[2]);
    value_type cos_x_sin_z = cos_x*sin_z;
    value_type cos_x_cos_z = cos_x*cos_z;
    value_type sin_x_sin_z = sin_x*sin_z;
    value_type sin_x_cos_z = sin_x*cos_z;
    m[0] = cos_y*cos_z;
    m[1] = cos_y*sin_z;
    m[2] = -sin_y;
    m[3] = -cos_x_sin_z+ sin_x_cos_z*sin_y;
    m[4] = cos_x_cos_z+ sin_x_sin_z*sin_y;
    m[5] = sin_x*cos_y;
    m[6] = sin_x_sin_z+cos_x_cos_z*sin_y;
    m[7] = -sin_x_cos_z+cos_x_sin_z*sin_y;
    m[8] = cos_x*cos_y;
    /* Euler angle
    m[0] = cos_x_cos_z - cos_y*sin_x_sin_z;
    m[1] = sin_x_cos_z + cos_y*cos_x_sin_z;
    m[2] = sin_z*sin_y;
    m[3] = -cos_x_sin_z - cos_y*sin_x_cos_z;
    m[4] = -sin_x_sin_z + cos_y*cos_x_cos_z;
    m[5] = cos_z*sin_y;
    m[6] = sin_y*sin_x;
    m[7] = -sin_y*cos_x;
    m[8] = cos_y;
     */
}

template<typename iterator_type1,typename iterator_type2>
__DEVICE_HOST__ void rotation_matrix_to_angles(iterator_type1 m,iterator_type2 theta,vdim<3>)
{
    float sy = std::sqrt(m[0]*m[0]+m[1]*m[1]);
    if (sy > 1.0e-6f)
    {
        theta[0] = atan2(m[5],m[8]);
        theta[2] = atan2(m[1],m[0]);
    }
    else
    {
        theta[0] = atan2(-m[7], m[4]);
        theta[2] = 0;
    }
    theta[1] = atan2(-m[2], sy);
}
/*
 Scaling*Rotate
 */
template<typename angle_type,typename scale_type,typename output_type>
__DEVICE_HOST__ void rotation_scaling_matrix(angle_type theta,scale_type s,output_type m,vdim<2>)
{
    rotation_matrix(theta,m,vdim<2>());
    m[0] *= s[0];
    m[1] *= s[0];
    m[2] *= s[1];
    m[3] *= s[1];
}
/*
 Scaling*Rx*Ry*Rz
 */
template<typename angle_type,typename scale_type,typename output_type>
__DEVICE_HOST__ void rotation_scaling_matrix(angle_type theta,scale_type s,output_type m,vdim<3>)
{
    rotation_matrix(theta,m,vdim<3>());
    m[0] *= s[0];
    m[1] *= s[0];
    m[2] *= s[0];
    m[3] *= s[1];
    m[4] *= s[1];
    m[5] *= s[1];
    m[6] *= s[2];
    m[7] *= s[2];
    m[8] *= s[2];
}

/*
 Affine*Scaling*R

Affine   = [1   	a[0]    0;
            0   	1 	    0;
            0   	0   	1];
 */
template<typename angle_type,typename scale_type,typename affine_type,typename output_type>
__DEVICE_HOST__ void rotation_scaling_affine_matrix(angle_type theta,scale_type s,affine_type a,output_type m,vdim<2>)
{
    rotation_scaling_matrix(theta,s,m,vdim<2>());
    m[0] += m[2]*a;
    m[1] += m[3]*a;
}

/*
 Affine*Scaling*R1*R2*R3

Affine   = [1   	a[0]    a[1]   0;
            0   	1 	a[2]   0;
            0   	0   	1      0;
            0    	0    	0      1];
 */
template<typename angle_type,typename scale_type,typename affine_type,typename output_type>
__DEVICE_HOST__ void rotation_scaling_affine_matrix(angle_type theta,scale_type s,affine_type a,output_type m,vdim<3>)
{
    rotation_scaling_matrix(theta,s,m,vdim<3>());
    m[0] += m[3]*a[0]+m[6]*a[1];
    m[1] += m[4]*a[0]+m[7]*a[1];
    m[2] += m[5]*a[0]+m[8]*a[1];

    m[3] += m[6]*a[2];
    m[4] += m[7]*a[2];
    m[5] += m[8]*a[2];
}

template<typename intput_type,typename angle_type,typename scale_type,typename affine_type>
__DEVICE_HOST__ void matrix_to_rotation_scaling_affine(intput_type m,angle_type theta,scale_type s,affine_type a,vdim<3>)
{
    typedef typename std::iterator_traits<intput_type>::value_type value_type;
    value_type Q[9];
    tipl::mat::inverse(m,tipl::dim<3,3>());
    tipl::mat::qr_decomposition(m,Q,tipl::dim<3,3>());
    tipl::mat::inverse_upper(m,tipl::dim<3,3>());
    tipl::mat::transpose(Q,tipl::dim<3,3>());
    if(m[0] < 0)
    {
        m[0] = -m[0];
        Q[0] = -Q[0];
        Q[1] = -Q[1];
        Q[2] = -Q[2];
    }
    if(m[4] < 0)
    {
        m[1] = -m[1];
        m[4] = -m[4];
        Q[3] = -Q[3];
        Q[4] = -Q[4];
        Q[5] = -Q[5];
    }
    if(m[8] < 0)
    {
        m[2] = -m[2];
        m[5] = -m[5];
        m[8] = -m[8];
        Q[6] = -Q[6];
        Q[7] = -Q[7];
        Q[8] = -Q[8];
    }
    s[0] = m[0];
    s[1] = m[4];
    s[2] = m[8];
    a[0] = m[1]/m[4];
    a[1] = m[2]/m[8];
    a[2] = m[5]/m[8];
    rotation_matrix_to_angles(Q,theta,tipl::vdim<3>());
}

template<typename output_iter>
__DEVICE_HOST__ void rotation_x_matrix(typename std::iterator_traits<output_iter>::value_type theta,output_iter m/*a 3x3 matrix*/)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    value_type cos_theta = std::cos(theta);
    value_type sin_theta = std::sin(theta);
    m[0] = 1.0;
    m[1] = 0.0;
    m[2] = 0.0;
    m[3] = 0.0;
    m[4] = cos_theta;
    m[5] = sin_theta;
    m[7] = -sin_theta;
    m[6] = 0.0;
    m[8] = cos_theta;
}

template<typename output_iter>
__DEVICE_HOST__ void rotation_y_matrix(typename std::iterator_traits<output_iter>::value_type theta,output_iter m/*a 3x3 matrix*/)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    value_type cos_theta = std::cos(theta);
    value_type sin_theta = std::sin(theta);
    m[0] = cos_theta;
    m[1] = 0.0;
    m[2] = -sin_theta;
    m[3] = 0.0;
    m[4] = 1.0;
    m[5] = 0.0;
    m[6] = sin_theta;
    m[7] = 0.0;
    m[8] = cos_theta;
}

template<typename output_iter>
__DEVICE_HOST__ void rotation_z_matrix(typename std::iterator_traits<output_iter>::value_type theta,output_iter m/*a 3x3 matrix*/)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    value_type cos_theta = std::cos(theta);
    value_type sin_theta = std::sin(theta);
    m[0] = cos_theta;
    m[1] = sin_theta;
    m[2] = 0.0;
    m[3] = -sin_theta;
    m[4] = cos_theta;
    m[5] = 0.0;
    m[6] = 0.0;
    m[7] = 0.0;
    m[8] = 1.0;
}

/**
    rotate from u to v
    R : left roration matrix
*/

template<typename input_iter1,typename input_iter2,typename output_iter>
__DEVICE_HOST__ void rotation_vector_matrix(output_iter r,input_iter1 u,input_iter2 v)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;

    value_type value = u[0]*v[0]+u[1]*v[1]+u[2]*v[2]+ 1.0;
    if (value == 0.0)
    {
        r[0] = r[4] = r[8] = -1.0;
        r[1] = r[2] = r[3] = r[5] = r[6] = r[7] = 0.0;
        return;
    }
    value_type uv[3];
    uv[0] = u[0] + v[0];
    uv[1] = u[1] + v[1];
    uv[2] = u[2] + v[2];
    //R(u->v) = (u+v)*(u+v)T/(uT*v+1) - I
    r[0] = uv[0]*uv[0]/value-1;
    r[1] = uv[1]*uv[0]/value;
    r[2] = uv[2]*uv[0]/value;
    r[3] = uv[0]*uv[1]/value;
    r[4] = uv[1]*uv[1]/value-1;
    r[5] = uv[2]*uv[1]/value;
    r[6] = uv[0]*uv[2]/value;
    r[7] = uv[1]*uv[2]/value;
    r[8] = uv[2]*uv[2]/value-1;
}


template<typename input_iter,typename output_iter>
__DEVICE_HOST__ void rotation_matrix(input_iter uv/*a 3d unit vector as the axis*/,
                     typename std::iterator_traits<input_iter>::value_type theta,output_iter m/*a 3x3 matrix*/,vdim<3>)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    value_type cos_theta = std::cos(theta);
    value_type sin_theta = std::sin(theta);
    value_type cos_theta_1 = (1-cos_theta);
    value_type zs = uv[2]*sin_theta;
    value_type ys = uv[1]*sin_theta;
    value_type xs = uv[0]*sin_theta;
    m[0] = uv[0]*uv[0]*cos_theta_1+cos_theta;
    m[1] = uv[1]*uv[0]*cos_theta_1-zs;
    m[2] = uv[2]*uv[0]*cos_theta_1+ys;
    m[3] = uv[0]*uv[1]*cos_theta_1+zs;
    m[4] = uv[1]*uv[1]*cos_theta_1+cos_theta;
    m[5] = uv[2]*uv[1]*cos_theta_1-xs;
    m[6] = uv[0]*uv[2]*cos_theta_1-ys;
    m[7] = uv[1]*uv[2]*cos_theta_1+xs;
    m[8] = uv[2]*uv[2]*cos_theta_1+cos_theta;
}

template<typename input_iter,typename output_iter>
__DEVICE_HOST__ void scaling_matrix(input_iter scaling,output_iter m,vdim<2>)
{
    m[0] = scaling[0];
    m[1] = 0.0;
    m[2] = 0.0;
    m[3] = scaling[1];
}

template<typename input_iter,typename output_iter>
__DEVICE_HOST__ void scaling_matrix(input_iter scaling,output_iter m,vdim<3>)
{
    m[0] = scaling[0];
    m[1] = 0.0;
    m[2] = 0.0;
    m[3] = 0.0;
    m[4] = scaling[1];
    m[5] = 0.0;
    m[6] = 0.0;
    m[7] = 0.0;
    m[8] = scaling[2];
}



template<typename input_scaling_iter,typename angle_type,typename output_iter>
__DEVICE_HOST__ void rotation_angle_to_rotation_matrix(input_scaling_iter scaling,angle_type rotation,output_iter m,vdim<2>)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    value_type S[4],R[4];
    scaling_matrix(scaling,S,vdim<2>());
    rotation_matrix(rotation[0],R,vdim<2>());
    matrix_product(R,S,m,vdim<2>());
}

// the rotation is the Euler angles, which has Z-X-Z configuration
template<typename input_scaling_iter,typename input_rotation_iter,typename output_iter>
__DEVICE_HOST__ void rotation_angle_to_rotation_matrix(input_scaling_iter scaling,input_rotation_iter rotation,output_iter m,vdim<3>)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    value_type S[9],R[9],M[9];
    scaling_matrix(scaling,S,vdim<3>());
    rotation_z_matrix(rotation[0],R);
    matrix_product(R,S,M,vdim<3>());
    rotation_x_matrix(rotation[1],R);
    matrix_product(R,M,S,vdim<3>());
    rotation_z_matrix(rotation[2],R);
    matrix_product(R,S,m,vdim<3>());
}

// the rotation is the Euler angles, which has Z-X-Z configuration
/*
template<typename input_rotation_iter,typename output_iter>
void rotation_angle_to_rotation_matrix(input_rotation_iter rotation,output_iter m)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    value_type S[9],R[9],M[9];
    rotation_z_matrix(rotation[0],M);
    rotation_x_matrix(rotation[1],R);
    matrix_product(R,M,S,vdim<3>());
    rotation_z_matrix(rotation[2],R);
    matrix_product(R,S,m,vdim<3>());
}
*/

// Output Euler angle from rotation matrix
template<typename input_rotation_iter,typename output_iter>
__DEVICE_HOST__ void rotation_matrix_to_rotation_angle(input_rotation_iter rotation_matrix,output_iter rotation_angle,vdim<3>)
{
    rotation_angle[0] = std::atan2(rotation_matrix[6],rotation_matrix[7]); //Z
    rotation_angle[1] = std::acos(rotation_matrix[8]);//X
    rotation_angle[2] = -std::atan2(rotation_matrix[2],rotation_matrix[5]);//Z
}


template<typename input_rotation_iter,typename input_shift_iter,typename output_iter>
__DEVICE_HOST__ void create_affine_transformation_matrix(input_rotation_iter rotation_scaling,input_shift_iter shift,output_iter m,vdim<3>)
{
    std::copy(rotation_scaling,rotation_scaling+3,m);
    std::copy(rotation_scaling+3,rotation_scaling+6,m+4);
    std::copy(rotation_scaling+6,rotation_scaling+9,m+8);
    m[3] = shift[0];
    m[7] = shift[1];
    m[11] = shift[2];
    m[12] = m[13] = m[14] = 0;
    m[15] = 1;

}

template<typename input_scaling_iter,typename input_rotation_iter,typename input_shift_iter,typename output_iter>
__DEVICE_HOST__ void create_affine_transformation_matrix(input_scaling_iter scaling,input_rotation_iter rotation,input_shift_iter shift,output_iter m,vdim<3>)
{
    typedef typename std::iterator_traits<output_iter>::value_type value_type;
    value_type M[9];
    sr_matrix(scaling,rotation,M);
    create_transformation_matrixation_matrix(M,shift,m,vdim<3>());
}



template<typename value_type_ = float>
class affine_transform_2d
{
public:
    typedef value_type_ value_type;
    static const unsigned int dimension = 2;
    static const unsigned int affine_dim = dimension;
    static const unsigned int total_size = 6;
    union
    {
        struct
        {
            value_type translocation[2];
            value_type rotation;
            value_type scaling[2];
            value_type affine;
        };
        value_type data[6];
    };
public:
    __INLINE__ affine_transform_2d(void)
    {
        clear();
    }
    __INLINE__ affine_transform_2d(const value_type* data_)
    {
        std::copy(data_,data_+total_size,data);
    }
    template<typename rhs_type>
    __INLINE__ affine_transform_2d(const rhs_type& rhs){operator=(rhs);}
    template<typename rhs_type>
    __INLINE__ affine_transform_2d& operator=(const rhs_type& rhs)
    {
        std::copy(rhs.begin(),rhs.end(),data);
        return *this;
    }
public:
    __INLINE__ void clear(void)
    {
        std::fill(data,data+total_size,0);
        std::fill(scaling,scaling+dimension,1);
    }
    __INLINE__ value_type operator[](unsigned int i) const{return data[i];}
    __INLINE__ value_type& operator[](unsigned int i) {return data[i];}
    __INLINE__ const value_type* begin(void) const{return data;}
    __INLINE__ const value_type* end(void) const{return data+total_size;}
    __INLINE__ value_type* begin(void) {return data;}
    __INLINE__ value_type* end(void) {return data+total_size;}
    __INLINE__ unsigned int size(void) const{return total_size;}
    __INLINE__ void downsampling(void)
    {
        translocation[0] *= 0.5;
        translocation[1] *= 0.5;
    }
    __INLINE__ void upsampling(void)
    {
        translocation[0] *= 2;
        translocation[1] *= 2;
    }
    friend std::ostream & operator<<(std::ostream& out, const affine_transform_2d<value_type> &T)
    {
        out << "translocation=" << T.translocation[0] << " " << T.translocation[1] << std::endl;
        out << "rotation=" << T.rotation << std::endl;
        out << "scaling=" << T.scaling[0] << " " << T.scaling[1] << std::endl;
        out << "affine=" << T.affine<< std::endl;
        return out;
    }
};

template<typename value_type_ = float>
class affine_transform
{
public:
    typedef value_type_ value_type;
    static const unsigned int dimension = 3;
    static const unsigned int affine_dim = dimension;
    static const unsigned int total_size = 12;
    union
    {
        struct
        {
            value_type translocation[3];
            value_type rotation[3];
            value_type scaling[3];
            value_type affine[3];
        };
        value_type data[12];
    };
public:
    __INLINE__ affine_transform(void)
    {
        clear();
    }
    __INLINE__ affine_transform(const value_type* data_)
    {
        std::copy(data_,data_+total_size,data);
    }
    template<typename rhs_type>
    __INLINE__ affine_transform(const rhs_type& rhs){operator=(rhs);}
    template<typename rhs_type>
    __INLINE__ affine_transform& operator=(const rhs_type& rhs)
    {
        std::copy(rhs.begin(),rhs.end(),data);
        return *this;
    }
public:
    __INLINE__ void clear(void)
    {
        std::fill(data,data+total_size,0);
        std::fill(scaling,scaling+dimension,1);
    }
    __INLINE__ value_type operator[](unsigned int i) const{return data[i];}
    __INLINE__ value_type& operator[](unsigned int i) {return data[i];}
    __INLINE__ const value_type* begin(void) const{return data;}
    __INLINE__ const value_type* end(void) const{return data+total_size;}
    __INLINE__ value_type* begin(void) {return data;}
    __INLINE__ value_type* end(void) {return data+total_size;}
    __INLINE__ unsigned int size(void) const{return total_size;}
    __INLINE__ void downsampling(void)
    {
        translocation[0] *= 0.5;
        translocation[1] *= 0.5;
        translocation[2] *= 0.5;
    }
    __INLINE__ void upsampling(void)
    {
        translocation[0] *= 2;
        translocation[1] *= 2;
        translocation[2] *= 2;
    }
    friend std::ostream & operator<<(std::ostream& out, const affine_transform<value_type> &T)
    {
        out << "translocation:" << T.translocation[0] << " " << T.translocation[1] << " " << T.translocation[2] << std::endl;
        out << "rotation:" << T.rotation[0] << " " << T.rotation[1] << " " << T.rotation[2] << std::endl;
        out << "scaling:" << T.scaling[0] << " " << T.scaling[1] << " " << T.scaling[2] << std::endl;
        out << "shear:" << T.affine[0] << " " << T.affine[1] << " " << T.affine[2] << std::endl;
        return out;
    }

};

template<typename value_type_ = float>
struct transformation_matrix_2d
{
    typedef value_type_ value_type;
    static const unsigned int dimension = 2;
    static const unsigned int sr_size = 4;
    static const unsigned int total_size = 6;
public:
    union
    {
        struct
        {
            value_type sr[4];
            value_type shift[2];
        };
        value_type data[6];
    };

public:
    __INLINE__ transformation_matrix_2d(void)
    {
        std::fill((value_type*)data,(value_type*)data+total_size,0);
    }

    // (Affine*Scaling*R1*R2*R3*vs*Translocation*shift_center)*from = (vs*shift_center)*to;
    __DEVICE_HOST__ transformation_matrix_2d(const affine_transform_2d<value_type>& rb,
                          const tipl::shape<2>& from,
                          const tipl::vector<2>& from_vs,
                          const tipl::shape<2>& to,
                          const tipl::vector<2>& to_vs)
    {
        //now sr = Scaling*R1*R2*R3
        tipl::rotation_scaling_affine_matrix(rb.rotation,rb.scaling,rb.affine,sr,tipl::vdim<dimension>());
        // calculate (vs*Translocation*shift_center)
        tipl::vector<2> t(from[0],from[1]);
        t *= -0.5;
        t += rb.translocation;
        t[0] *= from_vs[0];
        t[1] *= from_vs[1];
        // (Affine*Scaling*R1*R2*R3)*(vs*Translocation*shift_center)
        shift[0] = sr[0]*t[0]+sr[1]*t[1];
        shift[1] = sr[2]*t[0]+sr[3]*t[1];
        sr[0] *= from_vs[0];
        sr[1] *= from_vs[1];
        sr[2] *= from_vs[0];
        sr[3] *= from_vs[1];
        // inv(vs) ... = inv(vs)(vs*shift_center)...
        if(to_vs[0] != 1.0f)
        {
            sr[0] /= to_vs[0];
            sr[1] /= to_vs[0];
            sr[2] /= to_vs[0];
            shift[0] /= to_vs[0];
        }
        if(to_vs[1] != 1.0f)
        {
            sr[3] /= to_vs[1];
            sr[4] /= to_vs[1];
            sr[5] /= to_vs[1];
            shift[1] /= to_vs[1];
        }
        // inv(shift_center) ... = inv(shift_center)(shift_center)...
        shift[0] += to[0]*0.5;
        shift[1] += to[1]*0.5;
        shift[2] += to[2]*0.5;
    }

    template<typename other_value_type>
    __INLINE__ const transformation_matrix_2d<value_type>& operator=(const transformation_matrix_2d<other_value_type>& rhs)
    {
        std::copy(rhs.data,rhs.data+total_size,data);
        return *this;
    }
    __DEVICE_HOST__ void operator*=(const transformation_matrix_2d& rhs)
    {
        tipl::matrix<2,2,value_type> sr_tmp(sr);
        tipl::mat::product(rhs.sr,sr_tmp.begin(),sr,tipl::dim<3,3>(),tipl::dim<2,2>());
        tipl::vector<2> shift_t(shift);
        vector_transformation(shift_t.begin(),shift,rhs.sr,rhs.shift,tipl::dim<2,2>());
    }

    __INLINE__ value_type* get(void){return data;}
    __INLINE__ const value_type* get(void) const{return data;}
    __INLINE__ value_type operator[](unsigned int i) const{return data[i];}
    __INLINE__ value_type& operator[](unsigned int i) {return data[i];}

    __DEVICE_HOST__ bool inverse(void)
    {
        tipl::matrix<2,2,value_type> iT(sr);
        if(!iT.inv())
            return false;
        value_type new_shift[3];
        vector_rotation(shift,new_shift,iT.begin(),tipl::vdim<2>());
        for(unsigned int d = 0;d < 2;++d)
            shift[d] = -new_shift[d];
        std::copy(iT.begin(),iT.end(),sr);
        return true;
    }

    template<typename vtype1,typename vtype2>
    __INLINE__ void operator()(const vtype1& from,vtype2& to) const
    {
        vector_transformation(from.begin(),to.begin(),sr,shift,tipl::vdim<2>());
    }
    template<typename vtype>
    __INLINE__ void operator()(vtype& pos) const
    {
        vtype result;
        vector_transformation(pos.begin(),result.begin(),sr,shift,tipl::vdim<2>());
        pos = result;
    }

    friend std::ostream & operator<<(std::ostream& out, const transformation_matrix_2d& T)
    {
        out << T[0] << " " << T[1] << " " << T[2] << std::endl;
        out << T[3] << " " << T[4] << " " << T[5] << std::endl;
        return out;
    }
};


template<typename value_type_ = float>
struct transformation_matrix
{
    typedef value_type_ value_type;
    static const unsigned int dimension = 3;
    static const unsigned int sr_size = 9;
    static const unsigned int total_size = 12;
public:
    union
    {
        struct
        {
            value_type sr[9];
            value_type shift[3];
        };
        value_type data[12];
    };
public:
    __INLINE__ const value_type& operator[](unsigned int index) const
    {
        return data[index];
    }
    __INLINE__ value_type& operator[](unsigned int index)
    {
        return data[index];
    }
    __INLINE__ value_type* begin(void)
    {
        return data;
    }
    __INLINE__ value_type* end(void)
    {
        return data+12;
    }
    __INLINE__ const value_type* begin(void)const
    {
        return data;
    }
    __INLINE__ const value_type* end(void)	const
    {
        return data+12;
    }
    __INLINE__ size_t size(void) const{return 12;}
public:
    __INLINE__ transformation_matrix(void)
    {
        std::fill(data,data+total_size,0);
    }
    template<typename rhs_value_type>
    __INLINE__ transformation_matrix(const rhs_value_type& M){operator=(M);}
    // (Affine*Scaling*R1*R2*R3*vs*Translocation*shift_center)*from = (vs*shift_center)*to;
    template<typename geo_type,typename vs_type>
    __DEVICE_HOST__ transformation_matrix(const affine_transform<value_type>& rb,
                          const geo_type& from,
                          const vs_type& from_vs,
                          const geo_type& to,
                          const vs_type& to_vs)
    {
        //now sr = Affine*Scaling*R1*R2*R3
        rotation_scaling_affine_matrix(rb.rotation,rb.scaling,rb.affine,sr,vdim<dimension>());
        // calculate (vs*Translocation*shift_center)
        vs_type t(from[0],from[1],from[2]);
        t *= -0.5;
        t += rb.translocation;
        t[0] *= from_vs[0];
        t[1] *= from_vs[1];
        t[2] *= from_vs[2];
        // (Affine*Scaling*R1*R2*R3)*(vs*Translocation*shift_center)
        shift[0] = sr[0]*t[0]+sr[1]*t[1]+sr[2]*t[2];
        shift[1] = sr[3]*t[0]+sr[4]*t[1]+sr[5]*t[2];
        shift[2] = sr[6]*t[0]+sr[7]*t[1]+sr[8]*t[2];
        sr[0] *= from_vs[0];
        sr[1] *= from_vs[1];
        sr[2] *= from_vs[2];
        sr[3] *= from_vs[0];
        sr[4] *= from_vs[1];
        sr[5] *= from_vs[2];
        sr[6] *= from_vs[0];
        sr[7] *= from_vs[1];
        sr[8] *= from_vs[2];
        // inv(vs) ... = inv(vs)(vs*shift_center)...
        if(to_vs[0] != value_type(1))
        {
            sr[0] /= to_vs[0];
            sr[1] /= to_vs[0];
            sr[2] /= to_vs[0];
            shift[0] /= to_vs[0];
        }
        if(to_vs[1] != value_type(1))
        {
            sr[3] /= to_vs[1];
            sr[4] /= to_vs[1];
            sr[5] /= to_vs[1];
            shift[1] /= to_vs[1];
        }
        if(to_vs[2] != value_type(1))
        {
            sr[6] /= to_vs[2];
            sr[7] /= to_vs[2];
            sr[8] /= to_vs[2];
            shift[2] /= to_vs[2];
        }
        // inv(shift_center) ... = inv(shift_center)(shift_center)...
        shift[0] += to[0]*value_type(0.5);
        shift[1] += to[1]*value_type(0.5);
        shift[2] += to[2]*value_type(0.5);
    }
    template<typename geo_type,typename vs_type>
    __DEVICE_HOST__ void to_affine_transform(affine_transform<value_type>& rb,
                          const geo_type& from,
                          const vs_type& from_vs,
                          const geo_type& to,
                          const vs_type& to_vs) const
    {
        tipl::matrix<3,3,float> R,iR;
        std::copy(sr,sr+9,R.begin());

        value_type t[3];
        t[0] = shift[0]-to[0]*value_type(0.5);
        t[1] = shift[1]-to[1]*value_type(0.5);
        t[2] = shift[2]-to[2]*value_type(0.5);

        if(to_vs[2] != value_type(1))
        {
            R[6] *= to_vs[2];
            R[7] *= to_vs[2];
            R[8] *= to_vs[2];
            t[2] *= to_vs[2];
        }

        if(to_vs[1] != value_type(1))
        {
            R[3] *= to_vs[1];
            R[4] *= to_vs[1];
            R[5] *= to_vs[1];
            t[1] *= to_vs[1];
        }

        if(to_vs[0] != value_type(1))
        {
            R[0] *= to_vs[0];
            R[1] *= to_vs[0];
            R[2] *= to_vs[0];
            t[0] *= to_vs[0];
        }
        R[0] /= from_vs[0];
        R[1] /= from_vs[1];
        R[2] /= from_vs[2];
        R[3] /= from_vs[0];
        R[4] /= from_vs[1];
        R[5] /= from_vs[2];
        R[6] /= from_vs[0];
        R[7] /= from_vs[1];
        R[8] /= from_vs[2];
        iR = tipl::inverse(R);
        rb.translocation[0] = (iR[0]*t[0]+iR[1]*t[1]+iR[2]*t[2])/from_vs[0]+from[0]*value_type(0.5);
        rb.translocation[1] = (iR[3]*t[0]+iR[4]*t[1]+iR[5]*t[2])/from_vs[1]+from[1]*value_type(0.5);
        rb.translocation[2] = (iR[6]*t[0]+iR[7]*t[1]+iR[8]*t[2])/from_vs[2]+from[2]*value_type(0.5);
        matrix_to_rotation_scaling_affine(R.begin(),rb.rotation,rb.scaling,rb.affine,vdim<dimension>());
    }
public:
    template<typename rhs_value_type>
    __INLINE__ transformation_matrix& operator=(const transformation_matrix<rhs_value_type>& M)
    {
        std::copy(M.begin(),M.begin()+total_size,data);
        return *this;
    }
    template<typename rhs_value_type>
    __INLINE__ transformation_matrix& operator=(const tipl::matrix<4,4,rhs_value_type>& M)
    {
        data[0] = M[0];
        data[1] = M[1];
        data[2] = M[2];

        data[3] = M[4];
        data[4] = M[5];
        data[5] = M[6];

        data[6] = M[8];
        data[7] = M[9];
        data[8] = M[10];

        data[9] = M[3];
        data[10] = M[7];
        data[11] = M[11];
        return *this;
    }

public:

    __DEVICE_HOST__  const transformation_matrix<value_type>& operator*=(const transformation_matrix& rhs)
    {
        tipl::matrix<3,3,value_type> sr_tmp(sr);
        tipl::mat::product(rhs.sr,sr_tmp.begin(),sr,tipl::dim<3,3>(),tipl::dim<3,3>());
        value_type shift_t[3] = {shift[0],shift[1],shift[2]};
        vector_transformation(shift_t,shift,rhs.sr,rhs.shift,vdim<3>());
        return *this;
    }

    template<typename InputIterType>
    __DEVICE_HOST__  void save_to_transform(InputIterType M)
    {
        std::copy(data,data+3,M);
        std::copy(data+3,data+6,M+4);
        std::copy(data+6,data+9,M+8);
        M[3] = data[9];
        M[7] = data[10];
        M[11] = data[11];
    }

    __DEVICE_HOST__ bool inverse(void)
    {
        tipl::matrix<3,3,value_type> iT(sr);
        if(!iT.inv())
            return false;
        value_type new_shift[3];
        vector_rotation(shift,new_shift,iT.begin(),vdim<3>());
        for(unsigned int d = 0;d < 3;++d)
            shift[d] = -new_shift[d];
        std::copy(iT.begin(),iT.end(),sr);
        return true;
    }

    template<typename vtype1,typename vtype2>
    __INLINE__ void operator()(const vtype1& from,vtype2& to) const
    {
        vector_transformation(from.begin(),to.begin(),sr,shift,vdim<3>());
    }
    template<typename vtype>
    __INLINE__ void operator()(vtype& pos) const
    {
        vtype result;
        vector_transformation(pos.begin(),result.begin(),sr,shift,vdim<3>());
        pos = result;
    }

    friend std::ostream & operator<<(std::ostream& out, const transformation_matrix<value_type> &T)
    {
        out << T.data[0] << " " << T.data[1] << " " << T.data[2] << " " << T.data[9] << std::endl;
        out << T.data[3] << " " << T.data[4] << " " << T.data[5] << " " << T.data[10] << std::endl;
        out << T.data[6] << " " << T.data[7] << " " << T.data[8] << " " << T.data[11] << std::endl;
        return out;
    }
};


template<typename value_type = float>
class from_space : public tipl::matrix<4,4,value_type>{
private:
    const tipl::matrix<4,4,value_type>& origin;
public:
    __INLINE__ from_space(const tipl::matrix<4,4,value_type>& space_):tipl::matrix<4,4,value_type>(),origin(space_){}
    __INLINE__ from_space& to(const tipl::matrix<4,4,value_type>& target)
    {
        std::copy(target.begin(),target.end(),tipl::matrix<4,4,value_type>::begin());
        tipl::matrix<4,4,value_type>::inv();
        (*this) *= origin;
        return *this;
    }
};




}


#endif // TRANSFORMATION_HPP_INCLUDED
