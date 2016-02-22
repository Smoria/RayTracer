#ifndef COBJFILE_H_INCLUDED
#define COBJFILE_H_INCLUDED

#include <vector>
#include <string>
#include <fstream>
#include <Eigen\Dense>

namespace ObjFile
{
    class CObjFile
    {
        public:
            struct SFace
            {
                std::vector<size_t> v_index;// indicies for the verts that make up this triangle
                std::vector<size_t> t_index;// indicies for the tex coords to texture this face
                std::vector<size_t> n_index;// indicies for the normal coords
            };
            using TValue = double;
            using TVertex = Eigen::Matrix<TValue, 3, 1>;
            using TFace = SFace;
            using TTextureUV = Eigen::Matrix<TValue, 2, 1>;
            using TNormal = Eigen::Matrix<TValue, 3, 1>;

            using TVertices = std::vector<TVertex>;
            using TFaces = std::vector<SFace>;
            using TTextureUVs = std::vector<TTextureUV>;
            using TNormals = std::vector<TNormal>;

            //Constructor
            CObjFile(const std::string& fileName);

            inline const CObjFile::TFaces& GetFaces() const { return m_faces; }
            inline const CObjFile::TNormals& GetNormals() const { return m_normals; }
            inline const CObjFile::TVertices& GetVertices() const { return m_vertices; }
            inline CObjFile::TVertices& GetVertices() { return m_vertices; }
            inline const CObjFile::TTextureUVs& GetTextureUVs() const { return m_textureUVs; }

            void NormalizeModel();

        private:
            //Service
            static std::vector<std::string> splitString(const std::string& str, char letter);
            void checkMaxMin(TValue value);

            //Object data
            TValue m_maxVertexCoord;
            TValue m_minVertexCoord;
            TFaces m_faces;
            TNormals m_normals;
            TVertices m_vertices;
            TTextureUVs m_textureUVs;
    };
}

#endif /*COBJFILE_H_INCLUDED*/