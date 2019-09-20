#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "LogManager.h"
#include "FileSystemManager.h"
#include "AssimpModelLoader.h"

#define MATERIAL_COLOR(assimp_mat, assimp_matkey, SDEcolor) \
{ \
    aiColor4D color; \
    if (aiGetMaterialColor(assimp_mat, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS) {\
        SDEcolor.r = color.r; SDEcolor.g = color.g; SDEcolor.b = color.b; SDEcolor.a = color.a; \
    } \
}

#define MATERIAL_FLOAT(assimp_mat, assimp_matkey, SDEfloat) \
{ \
    uint32_t max = 1; \
    ai_real val; \
    if (aiGetMaterialFloatArray(assimp_mat, AI_MATKEY_SHININESS, &val, &max) == AI_SUCCESS) {\
        SDEfloat = val; \
    } \
}

#define MATERIAL_TEX_NAME(assimp_mat, SDE_mat_texkey, SDE_tex_fn) \
{ \
    aiString ai_tex_path; \
    aiTextureType assimp_mat_texkey = static_cast<aiTextureType>(MaterialData::AI_TEXTURE_ID[SDE_mat_texkey]); \
    if (assimp_mat->GetTexture(assimp_mat_texkey, 0, &ai_tex_path) == AI_SUCCESS) { \
        FilePathString tex_path = ai_tex_path.C_Str(); \
        FilePathString tex_fp, tex_fn; \
        SDE::Basic::SpliteTargetFileToPathAndName(tex_path, tex_fp, tex_fn); \
        SDE_tex_fn = tex_fn; \
    } \
}

using SDE::Basic::FileSystemManager;

_____________SD_START_GRAPHICS_NAMESPACE_____________

Matrix4X4f ConvertAiMatrixToMatrix4X4f(const aiMatrix4x4 &i_ai_mat)
{
    Matrix4X4f result;
    result.m_matrix[0][0] = i_ai_mat.a1; result.m_matrix[1][0] = i_ai_mat.a2; result.m_matrix[2][0] = i_ai_mat.a3; result.m_matrix[3][0] = i_ai_mat.a4;
    result.m_matrix[0][1] = i_ai_mat.b1; result.m_matrix[1][1] = i_ai_mat.b2; result.m_matrix[2][1] = i_ai_mat.b3; result.m_matrix[3][1] = i_ai_mat.b4;
    result.m_matrix[0][2] = i_ai_mat.c1; result.m_matrix[1][2] = i_ai_mat.c2; result.m_matrix[2][2] = i_ai_mat.c3; result.m_matrix[3][2] = i_ai_mat.c4;
    result.m_matrix[0][3] = i_ai_mat.d1; result.m_matrix[1][3] = i_ai_mat.d2; result.m_matrix[2][3] = i_ai_mat.d3; result.m_matrix[3][3] = i_ai_mat.d4;
    return result;
}

SD_SINGLETON_DECLARATION_IMPL(AssimpModelLoader);

AssimpModelLoader::AssimpModelLoader()
{
    SD_SINGLETON_DECLARATION_REGISTER;
}

AssimpModelLoader::~AssimpModelLoader()
{
}

bool AssimpModelLoader::ImportScene(const FilePathString &i_model_fn, ModelData &io_model)
{
    Assimp::Importer importer;
    FileData fd;
    if (FileSystemManager::GetRef().OpenFile(i_model_fn, fd) == true) {
        const aiScene *scene = importer.ReadFileFromMemory(&fd.m_file_content[0], fd.m_file_content.size(), aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);
        if (scene != nullptr) {
            ParseMaterials(scene, io_model);
            ParseMeshes(scene, io_model);
            ParseNodes(scene, nullptr, scene->mRootNode ,io_model.m_root_node);
        }
    }
    return true;
}

void AssimpModelLoader::ParseMaterials(const aiScene *i_scene, ModelData &io_model)
{
    if (i_scene->mMaterials != nullptr) {
        io_model.m_materials.resize(i_scene->mNumMaterials);
        for (uint32_t matID = 0; matID < i_scene->mNumMaterials; ++matID) {
            MATERIAL_COLOR(i_scene->mMaterials[matID],  AI_MATKEY_COLOR_AMBIENT,   io_model.m_materials[matID].m_ambient);
            MATERIAL_COLOR(i_scene->mMaterials[matID],  AI_MATKEY_COLOR_DIFFUSE,   io_model.m_materials[matID].m_diffuse);
            MATERIAL_COLOR(i_scene->mMaterials[matID], AI_MATKEY_COLOR_SPECULAR,  io_model.m_materials[matID].m_specular);
            MATERIAL_COLOR(i_scene->mMaterials[matID], AI_MATKEY_COLOR_EMISSIVE,  io_model.m_materials[matID].m_emission);
            MATERIAL_FLOAT(i_scene->mMaterials[matID],      AI_MATKEY_SHININESS, io_model.m_materials[matID].m_shineness);
            for (uint32_t texID = MaterialTextureType_Diffuse; texID < MaterialTextureType_MAX_DEFINE_VALUE; ++texID) {
                MATERIAL_TEX_NAME(i_scene->mMaterials[matID], texID, io_model.m_materials[matID].m_textures_fns[texID]);
            }
        }
    }
}

void AssimpModelLoader::ParseMeshes(const aiScene *i_scene, ModelData &io_model)
{
    if (i_scene->mMeshes != nullptr) {
        io_model.m_mesh_datas.resize(i_scene->mNumMeshes);
        //--- tex coordinate
        for (uint32_t meshID = 0; meshID < i_scene->mNumMeshes; ++meshID) {
            aiMesh *cur_ai_mesh = i_scene->mMeshes[meshID];
            MeshData &cur_mesh = io_model.m_mesh_datas[meshID];

            cur_mesh.m_mesh_name = cur_ai_mesh->mName.C_Str();

            if (cur_ai_mesh->mVertices == nullptr) {
                SDLOGW("Mesh[%d(%s)] don't have vertices!!!", meshID, cur_mesh.m_mesh_name.c_str());
                continue;
            }

            if (cur_ai_mesh->mFaces == nullptr) {
                SDLOGW("Mesh[%d] don't have face!!!", meshID, cur_mesh.m_mesh_name.c_str());
                continue;
            }

            for (uint32_t fID = 0; fID < cur_ai_mesh->mNumFaces; ++fID) {
                aiFace* face = &cur_ai_mesh->mFaces[fID];
                if (face->mNumIndices == 1) {
                    cur_mesh.m_face_indices.push_back(face->mIndices[0]);
                    cur_mesh.m_face_indices.push_back(face->mIndices[0]);
                    cur_mesh.m_face_indices.push_back(face->mIndices[0]);
                }
                else if (face->mNumIndices == 2) {
                    //Right
                    cur_mesh.m_face_indices.push_back(face->mIndices[0]);
                    cur_mesh.m_face_indices.push_back(face->mIndices[0]);
                    cur_mesh.m_face_indices.push_back(face->mIndices[1]);
                    //Left
                    cur_mesh.m_face_indices.push_back(face->mIndices[1]);
                    cur_mesh.m_face_indices.push_back(face->mIndices[1]);
                    cur_mesh.m_face_indices.push_back(face->mIndices[0]);
                }
                else if (face->mNumIndices == 3) {
                    for (uint32_t i = 0; i < face->mNumIndices; ++i) {
                        cur_mesh.m_face_indices.push_back(face->mIndices[i]);
                    }
                }
                else {
                    for (uint32_t i = 1; i < (face->mNumIndices - 1); ++i) {
                        cur_mesh.m_face_indices.push_back(face->mIndices[0]);
                        cur_mesh.m_face_indices.push_back(face->mIndices[i]);
                        cur_mesh.m_face_indices.push_back(face->mIndices[i+1]);
                    }
                }
            }

            for (uint32_t vID = 0; vID < cur_ai_mesh->mNumVertices; ++vID) {
                cur_mesh.m_vertex_attribs[VertexBufferUsage_VERTEX_BUFFER].push_back(
                    vec3(cur_ai_mesh->mVertices[vID].x, cur_ai_mesh->mVertices[vID].y, cur_ai_mesh->mVertices[vID].z));
            }

            if (cur_ai_mesh->mBitangents != nullptr) {
                for (uint32_t vID = 0; vID < cur_ai_mesh->mNumVertices; ++vID) {
                    cur_mesh.m_vertex_attribs[VertexBufferUsage_BINORMAL_BUFFER].push_back(
                        vec3(cur_ai_mesh->mBitangents[vID].x, cur_ai_mesh->mBitangents[vID].y, cur_ai_mesh->mBitangents[vID].z));
                }
            }

            if (cur_ai_mesh->mTangents != nullptr) {
                for (uint32_t vID = 0; vID < cur_ai_mesh->mNumVertices; ++vID) {
                    cur_mesh.m_vertex_attribs[VertexBufferUsage_TANGENT_BUFFER].push_back(
                        vec3(cur_ai_mesh->mTangents[vID].x, cur_ai_mesh->mTangents[vID].y, cur_ai_mesh->mTangents[vID].z));
                }
            }

            if (cur_ai_mesh->mTextureCoords[0] != nullptr) {
                for (uint32_t vID = 0; vID < cur_ai_mesh->mNumVertices; ++vID) {
                    cur_mesh.m_vertex_attribs[VertexBufferUsage_TEX_COORD_BUFFER].push_back(
                        vec3(cur_ai_mesh->mTextureCoords[0][vID].x, cur_ai_mesh->mTextureCoords[0][vID].y, cur_ai_mesh->mTextureCoords[0][vID].z));
                }
            }

            if (cur_ai_mesh->mNormals != nullptr) {
                for (uint32_t vID = 0; vID < cur_ai_mesh->mNumVertices; ++vID) {
                    cur_mesh.m_vertex_attribs[VertexBufferUsage_NORMAL_BUFFER].push_back(
                        vec3(cur_ai_mesh->mNormals[vID].x, cur_ai_mesh->mNormals[vID].y, cur_ai_mesh->mNormals[vID].z));
                }
            }

            if (cur_ai_mesh->mColors[0] != nullptr) {
                for (uint32_t vID = 0; vID < cur_ai_mesh->mNumVertices; ++vID) {
                    cur_mesh.m_vertex_colors.push_back(
                        Color4f(cur_ai_mesh->mColors[0][vID].r, cur_ai_mesh->mColors[0][vID].g, cur_ai_mesh->mColors[0][vID].b, cur_ai_mesh->mColors[0][vID].a));
                }
            }
        }
    }
}

void AssimpModelLoader::ParseNodes(const aiScene *i_scene, const Matrix4X4f &i_p_trans, aiNode *i_node, NodeData &io_node)
{
    if (i_node != nullptr) {
        //1. parse node.
        Matrix4X4f world_trans = i_p_trans * ConvertAiMatrixToMatrix4X4f(i_node->mTransformation);
        Matrix4X4f local_trans = ConvertAiMatrixToMatrix4X4f(i_node->mTransformation);
        io_node.m_name = i_node->mName.C_Str();
        io_node.m_local_trans = local_trans;
        io_node.m_world_trans = world_trans;
        io_node.m_mesh_idices.resize(i_node->mNumMeshes);

        for (uint32_t meshID = 0; meshID < i_node->mNumMeshes; ++meshID) {
            io_node.m_mesh_idices[meshID] = i_node->mMeshes[meshID];
        }

        //2. parse child nodes.
        if (i_node->mNumChildren > 0) {
            io_node.m_child_nodes.resize(i_node->mNumChildren);
            for (uint32_t cID = 0; cID < i_node->mNumChildren; ++cID) {
                ParseNodes(i_scene, world_trans, i_node->mChildren[cID], io_node.m_child_nodes[cID]);
            }
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________