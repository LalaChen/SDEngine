/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Kuan-Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include "BasicShapeCreator.h"

#include "MathConstant.h"
#include "StaticIndexBuffer.h"
#include "StaticVertexBuffer.h"

using SDE::Math::PI;
using SDE::Math::TWO_PI;
using SDE::Math::ONE_DEGREE_OF_PI;

_____________SD_START_GRAPHICS_NAMESPACE_____________

MeshStrongReferenceObject BasicShapeCreator::CreateCone(
    const Vector3f &i_center,
    const Vector3f &i_u, const Vector3f &i_v, const Vector3f &i_w,
    float i_height, float i_angle, uint32_t i_slices)
{
    MeshStrongReferenceObject mesh;

    vec3 center(i_center.m_vec.x, i_center.m_vec.y, i_center.m_vec.z);

    //---- basic vertex data
    std::vector<vec3> v_data;
    std::vector<vec3> n_data;
    std::vector<vec2> t_data;
    //---- additional attribute
    std::vector<vec3> tn_data;
    std::vector<vec3> bn_data;
    std::vector<Color4f> c_data;
    //---- face index data
    std::vector<uint32_t> i_data;
    //---- data.
    std::vector<vec3> circle_vertices;
    Vector3f t_vertex;
    vec3 f_vertex[2];
    vec2 f_coord[2];
    vec3 f_normal;
    vec3 f_tangent;
    vec3 f_binormal;
    uint32_t i, face_index_start_id;
    float slice_angle, current_slice_angle, next_slice_angle;
    float radius = i_height * tan(i_angle * ONE_DEGREE_OF_PI);

    //1. calculate slice angle.
    if (i_slices < 3) {
        i_slices = 3;
    }
    slice_angle = TWO_PI / static_cast<float>(i_slices);
    //2. calculate circule vertices.
    for (i = 0, current_slice_angle = 0.0f; i < i_slices; i++, current_slice_angle += slice_angle) {
        //To avoid the angle over 2PI.
        if (current_slice_angle >= TWO_PI) {
            current_slice_angle = TWO_PI;
        }
        //To calculate vertex.
        t_vertex = i_u.normalize().scale(radius * cos(current_slice_angle)) + i_v.normalize().scale(radius * sin(current_slice_angle)) + i_w.normalize().scale(i_height);
        //Save vertex.
        circle_vertices.push_back(vec3(t_vertex.m_vec.x, t_vertex.m_vec.y, t_vertex.m_vec.z));
    }

    //3. calculate vertice model. (cone)
    for (i = 0, face_index_start_id = 0, current_slice_angle = 0.0f;
        i < i_slices;
        i++, face_index_start_id += 3, current_slice_angle += slice_angle)
    {
        //To avoid the angle over 2PI.
        next_slice_angle = current_slice_angle + slice_angle;
        if (current_slice_angle >= TWO_PI) {
            current_slice_angle = TWO_PI;
        }
        
        if (current_slice_angle >= TWO_PI) {
            current_slice_angle = TWO_PI;
        }
        //
        f_vertex[0] = circle_vertices[i];
        f_vertex[1] = circle_vertices[(i + 1) % i_slices];
        f_coord[0].x = 0.5f * cos(current_slice_angle);
        f_coord[0].y = 0.5f * sin(current_slice_angle);
        f_coord[1].x = 0.5f * cos(next_slice_angle);
        f_coord[1].y = 0.5f * sin(next_slice_angle);
        f_normal = (f_vertex[1] - center).cross(f_vertex[0] - center).normalize();
        f_tangent = CalculateTangetVector(vec3::normalize(f_vertex[0] - center), //v1
            vec3::normalize(f_vertex[1] - center), //v2
            vec2::normalize(vec2((f_coord[0] - vec2::Zero).x, (f_coord[0] - vec2::Zero).y)), //st1
            vec2::normalize(vec2((f_coord[1] - vec2::Zero).x, (f_coord[1] - vec2::Zero).y))  //st2
        );

        f_binormal = f_normal.cross(f_tangent).normalize();

        v_data.push_back(center); v_data.push_back(f_vertex[0]); v_data.push_back(f_vertex[1]);
        t_data.push_back(vec2::Zero); t_data.push_back(f_coord[0]); t_data.push_back(f_coord[1]);
        n_data.push_back(f_normal); n_data.push_back(f_normal); n_data.push_back(f_normal);
        tn_data.push_back(f_tangent); tn_data.push_back(f_tangent); tn_data.push_back(f_tangent);
        bn_data.push_back(f_binormal); bn_data.push_back(f_binormal); bn_data.push_back(f_binormal);
        c_data.push_back(Color4f::White()); c_data.push_back(Color4f::White()); c_data.push_back(Color4f::White());

        i_data.push_back(face_index_start_id);
        i_data.push_back(face_index_start_id + 1);
        i_data.push_back(face_index_start_id + 2);
    }
    
    mesh = new Mesh("Cone");

    StaticVertexBufferStrongReferenceObject vbuf = new StaticVertexBuffer("ConeVBuffer", VertexBufferUsage_VERTEX_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
    SD_SREF(vbuf).RefreshBufferData(v_data.data(), v_data.size() * sizeof(vec3));
    StaticVertexBufferStrongReferenceObject nbuf = new StaticVertexBuffer("ConeNBuffer", VertexBufferUsage_NORMAL_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
    SD_SREF(nbuf).RefreshBufferData(n_data.data(), n_data.size() * sizeof(vec3));
    StaticVertexBufferStrongReferenceObject bnbuf = new StaticVertexBuffer("ConeBNBuffer", VertexBufferUsage_BINORMAL_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
    SD_SREF(nbuf).RefreshBufferData(bn_data.data(), bn_data.size() * sizeof(vec3));
    StaticVertexBufferStrongReferenceObject tnbuf = new StaticVertexBuffer("ConeTNBuffer", VertexBufferUsage_TANGENT_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
    SD_SREF(nbuf).RefreshBufferData(tn_data.data(), tn_data.size() * sizeof(vec3));
    StaticVertexBufferStrongReferenceObject tbuf = new StaticVertexBuffer("ConeTBuffer", VertexBufferUsage_TEX_COORD_BUFFER, VertexBufferFormat_X32Y32_SFLOAT);
    SD_SREF(tbuf).RefreshBufferData(t_data.data(), t_data.size() * sizeof(vec2));
    StaticVertexBufferStrongReferenceObject cbuf = new StaticVertexBuffer("ConeCBuffer", VertexBufferUsage_COLOR_BUFFER, VertexBufferFormat_X32Y32Z32W32_SFLOAT);
    SD_SREF(cbuf).RefreshBufferData(c_data.data(), c_data.size() * sizeof(Color4f));
    StaticIndexBufferStrongReferenceObject ibuf = new StaticIndexBuffer("ConeIBuffer", IndexBufferFormat_X32_UINT);
    SD_SREF(ibuf).RefreshBufferData(i_data.data(), i_data.size() * sizeof(uint32_t));

    SD_SREF(mesh).RegisterVertexBuffer(VertexBufferUsage_VERTEX_BUFFER, vbuf.StaticCastTo<VertexBuffer>());
    SD_SREF(mesh).RegisterVertexBuffer(VertexBufferUsage_NORMAL_BUFFER, nbuf.StaticCastTo<VertexBuffer>());
    SD_SREF(mesh).RegisterVertexBuffer(VertexBufferUsage_BINORMAL_BUFFER, bnbuf.StaticCastTo<VertexBuffer>());
    SD_SREF(mesh).RegisterVertexBuffer(VertexBufferUsage_TANGENT_BUFFER, tnbuf.StaticCastTo<VertexBuffer>());
    SD_SREF(mesh).RegisterVertexBuffer(VertexBufferUsage_TEX_COORD_BUFFER, tbuf.StaticCastTo<VertexBuffer>());
    SD_SREF(mesh).RegisterVertexBuffer(VertexBufferUsage_COLOR_BUFFER, cbuf.StaticCastTo<VertexBuffer>());
    SD_SREF(mesh).RegisterIndexBuffer(ibuf.StaticCastTo<IndexBuffer>());

    return mesh;
}

______________SD_END_GRAPHICS_NAMESPACE______________