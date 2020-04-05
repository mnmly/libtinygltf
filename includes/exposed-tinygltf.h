//
//  exposed-tinygltf.h
//  tinygltf
//
//  Created by HIROAKI YAMANE on 03/04/2020.
//  Copyright © 2020 HIROAKI YAMANE. All rights reserved.
//
#pragma once

#include "tiny_gltf.h"

#ifdef _WIN32
#define ghAPI extern "C" __declspec(dllexport)
#elif __APPLE__
#define ghAPI extern "C"
#endif

#pragma mark - exposed APIs

ghAPI tinygltf::Model* tinygltfCreateModel();
ghAPI void tinygltfAddMesh( tinygltf::Model* model,
                            const char *name,
                            const char *materialName,
                            float* vertices, int numVertices,
                            float* normals, int numNormals,
                            float* uvs, int numUVs,
                            int* faces, int numFaces,
                            bool _flipAxis );
ghAPI void tinygltfAddPoints( tinygltf::Model* model,
                             const char *name,
                             const char *materialName,
                             float* vertices,
                             int numVertices,
                             bool _flipAxis );
ghAPI void tinygltfAddLine( tinygltf::Model* model,
                            const char *name,
                            const char *materialName,
                            float* vertices,
                            int numVertices,
                            bool _flipAxis );
ghAPI void tinygltfAddLineLoop( tinygltf::Model* model,
                                const char *name,
                                const char *materialName,
                                float* vertices,
                                int numVertices,
                                bool _flipAxis );
ghAPI void tinygltfSave( tinygltf::Model* model,
                         char* filepath, bool useStream,
                        bool embedImages, bool embedBuffers,
                        bool prettyPrint, bool writeBinary );
ghAPI void tinygltfDestory( tinygltf::Model* model );
