#include "obj\CObjFile.h"
#include <climits>
#include <fstream>
#include <algorithm>

namespace ObjFile
{
    CObjFile::CObjFile(const std::string& fileName)
        :
        m_maxVertexCoord(std::numeric_limits<TValue>::min()),
        m_minVertexCoord(std::numeric_limits<TValue>::max()),
        m_faces(),
        m_normals(),
        m_vertices(),
        m_textureUVs()
    {
        std::ifstream file(fileName);

        if (!file.is_open())
            throw std::exception("Can't open file.");

        char c;
        file.get(c);

        while (!file.eof())
        {
            // If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
            if (c == 'v')
            {
                file.get(c);
                switch (c)
                {
                case ' ':
                {
                    TValue vertexData[3];
                    // Read object data
                    // Read x
                    file >> vertexData[0];

                    // Read y
                    file >> vertexData[1];

                    // Read y
                    file >> vertexData[2];

                    // Create object
                    TVertex vertex(vertexData);

                    checkMaxMin(vertex[0]);
                    checkMaxMin(vertex[1]);
                    checkMaxMin(vertex[2]);
                    // Invert the Z vertex to change to left hand system.
                    vertex[2] = vertex[2]*-1.0;
                    // Add to collection
                    m_vertices.push_back(vertex);
                }
                break;
                case 't':
                {
                    // Read object data
                    TValue textureUVData[2];

                    // Read u
                    file >> textureUVData[0];

                    // Read v
                    file >> textureUVData[1];

                    // Create object
                    TTextureUV textureUV(textureUVData);
                    // Invert the V texture coordinates to left hand system.
                    textureUV[1] = 1.0f - textureUV[1];
                    // Add to collection
                    m_textureUVs.push_back(textureUV);
                }
                break;
                case 'n':
                {
                    TValue normalData[3];

                    // Read object data
                    // Read normal x
                    file >> normalData[0];

                    // Read normal y
                    file >> normalData[1];

                    // Read normal z
                    file >> normalData[2];

                    // Create object
                    TNormal normal(normalData);
                    // Invert the Z normal to change to left hand system.
                    normal[2] = -1.0 * normal[2];
                    // Add to collection
                    m_normals.push_back(normal);
                }
                break;
                }
            }

            // If the line starts with 'f' then increment the face count.
            if (c == 'f')
            {
                file.get(c);
                if (c == ' ')
                {
                    // Create object
                    TFace face;
                    // Read object data
                    std::string line;
                    while ((c = file.get()) != '\n')
                        line.push_back(c);

                    const std::vector<std::string> facePoints = splitString(line, ' ');

                    for (auto& facePoint : facePoints)
                    {
                        const std::vector<std::string> values = splitString(facePoint, '/');
                        const size_t elemsCount = values.size();

                        if (elemsCount > 0)
                        {
                            const std::string& val1 = values[0];
                            face.v_index.push_back(val1.empty() ? 0 : std::stoi(val1));

                            if (elemsCount > 1)
                            {
                                const std::string& val2 = values[1];
                                face.t_index.push_back(val2.empty() ? 0 : std::stoi(val2));

                                if (elemsCount > 2)
                                {
                                    const std::string& val3 = values[2];
                                    face.n_index.push_back(val3.empty() ? 0 : std::stoi(val3));
                                }
                            }
                        }
                    }

                    // Add to collection
                    m_faces.push_back(face);
                }
            }

            // Otherwise read in the remainder of the line.
            //while (c != '\n')
            //    file.get(c);

            // Start reading the beginning of the next line.
            file.get(c);
        }

        file.close();
    }

    void CObjFile::NormalizeModel()
    {
        const TValue value(std::max(std::abs(m_maxVertexCoord), std::abs(m_minVertexCoord)));

        for (auto& vertex : m_vertices)
        {
            vertex[0] = vertex[0] / (value * 0.5);
            vertex[1] = vertex[1] / (value * 0.5);
            vertex[2] = vertex[2] / (value * 0.5);
        }
    }

    std::vector<std::string> CObjFile::splitString(const std::string& str, char letter)
    {
        std::vector<std::string> result;

        size_t lastSpliIndex = 0;
        for (size_t i = 0; i < str.size(); ++i)
        {
            char c = str[i];
            if (c == letter)
            {
                result.push_back(str.substr(lastSpliIndex, i - lastSpliIndex));
                lastSpliIndex = i + 1;
            }
        }

        if (str.back() != letter)
        {
            result.push_back(str.substr(lastSpliIndex));
        }

        return result;
    }

    void CObjFile::checkMaxMin(TValue value)
    {
        if (value > m_maxVertexCoord) m_maxVertexCoord = value;
        if (value < m_minVertexCoord) m_minVertexCoord = value;
    }
}