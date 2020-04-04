//
//  exposed-tinygltf.cpp
//  tinygltf
//
//  Created by HIROAKI YAMANE on 03/04/2020.
//  Copyright © 2020 HIROAKI YAMANE. All rights reserved.
//

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION


#include "exposed-tinygltf.h"
#include <iostream>

using namespace tinygltf;

ghAPI tinygltf::Model* tinygltfCreateModel()
{
    auto model = new tinygltf::Model();
    auto scene = Scene();
    scene.name = "DefaultScene";
    model->scenes.push_back( scene );
    model->defaultScene = 0;
    auto asset = Asset();
    asset.version = "2.0";
    asset.generator = "mnml tinyGLTF based on tinygltf";
    model->asset = asset;
    return model;
}


void flipAxis( float* vertices, int numVertices )
{
    for (int i = 0; i < numVertices; i ++) {
        float tempY = vertices[i * 3 + 1];
        vertices[i * 3 + 1] = vertices[i * 3 + 2];
        vertices[i * 3 + 2] = -tempY;
    }
}


ghAPI void tinygltfAddPrimitive(tinygltf::Model* model, const char *name, const char *materialName, float* vertices, int numVertices, bool _flipAxis, int drawingMode )
{
    if ( _flipAxis ) flipAxis( vertices, numVertices / 3 );
    
    auto scene = model->scenes[ model->defaultScene ];
    auto node = Node();
    auto mesh = Mesh();
    auto material = Material();
    if ( materialName != nullptr ) {
        material.name = std::string( materialName );
    }
    auto accessor = Accessor();
    auto bufferView = BufferView();
    auto buffer = Buffer();

    node.name = std::string( name );
    
    auto length = numVertices * sizeof(float);
    
    std::vector<unsigned char> dst;
    dst.resize( length );
    std::memcpy( &dst[0], vertices, length );

    buffer.data = dst;
    bufferView.buffer = (int)model->buffers.size();
    bufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;
    bufferView.byteLength = length;

    accessor.count = numVertices / 3;
    accessor.bufferView = (int)model->bufferViews.size();
    accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
    accessor.type = TINYGLTF_TYPE_VEC3;
    
    auto primitive = Primitive();
    primitive.attributes["POSITION"] = (int)model->accessors.size();
    primitive.mode = drawingMode;
    if ( materialName != nullptr ) {
        primitive.material = (int)model->materials.size();
        model->materials.push_back( material );
    }
    mesh.primitives.push_back( primitive );
    
    scene.nodes.push_back( (int)scene.nodes.size() );
    node.mesh = (int)model->meshes.size();
    
    model->scenes[ 0 ] = scene;
    model->buffers.push_back( buffer );
    model->bufferViews.push_back( bufferView );
    model->accessors.push_back( accessor );
    model->materials.push_back( material );
    model->meshes.push_back( mesh );
    model->nodes.push_back( node );
    
}

ghAPI void tinygltfAddLine(tinygltf::Model* model, const char *name, const char *materialName, float* vertices, int numVertices, bool _flipAxis)
{
    tinygltfAddPrimitive( model, name, materialName, vertices, numVertices, _flipAxis, TINYGLTF_MODE_LINE_STRIP);
}

ghAPI void tinygltfAddLineLoop(tinygltf::Model* model, const char *name, const char *materialName, float* vertices, int numVertices, bool _flipAxis)
{
    tinygltfAddPrimitive( model, name, materialName, vertices, numVertices, _flipAxis, TINYGLTF_MODE_LINE_LOOP);
}

ghAPI void tinygltfAddPoints(tinygltf::Model* model, const char *name, const char *materialName, float* vertices, int numVertices, bool _flipAxis)
{
    tinygltfAddPrimitive( model, name, materialName, vertices, numVertices, _flipAxis, TINYGLTF_MODE_POINTS);
}


ghAPI void tinygltfAddMesh(tinygltf::Model* model, const char *name, const char *materialName, float* vertices, int numVertices, float* normals, int numNormals, float* uvs, int numUVs, int* faces, int numFaces, bool _flipAxis)
{
    
    if (_flipAxis) {
        flipAxis( vertices, numVertices / 3 );
    }
    
    auto scene = model->scenes[ model->defaultScene ];
    auto node = Node();
    auto mesh = Mesh();
    auto material = Material();
    if ( materialName != nullptr ) {
        material.name = std::string( materialName );
    }
    
    auto startAccessorIndex = (int)model->accessors.size();
    
    node.name = std::string( name );
    
    std::vector<int> facesInt(faces, faces + numFaces);
    std::vector<unsigned short> facesShort(facesInt.begin(), facesInt.end());
    
    void* buffers[4] = { &facesShort[0], vertices, uvs, normals };
    int bufferCount[4] = { numFaces, numVertices, numUVs, numNormals };
    int bufferComponents[4] = { 1, 3, 2, 3 };
    size_t bufferComponentSizes[4] = { sizeof(unsigned short), sizeof(float), sizeof(float), sizeof(float) };
    
    int totalLength = 0;
    
    for (auto i = 0; i < 4; i++ )
     {
         auto count = bufferCount[ i ];
         auto length = count * bufferComponentSizes[ i ] * bufferComponents[ i ];
         totalLength += length;
    }
    
    std::vector<unsigned char> dst;
    dst.reserve( totalLength );
    auto buffer = Buffer();
    totalLength = 0;
    for (auto i = 0; i < 4; i++ )
    {
        auto bufferView = BufferView();
        auto count = bufferCount[ i ];
        auto length = count * bufferComponentSizes[ i ] * bufferComponents[ i ];
        auto accessor = Accessor();
        accessor.count = count / bufferComponents[ i ];
        accessor.bufferView = (int)model->bufferViews.size();
        accessor.componentType = i != 0 ? TINYGLTF_COMPONENT_TYPE_FLOAT : TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
        if ( i == 1 || i == 3 )
        {
            accessor.type = TINYGLTF_TYPE_VEC3;
        } else if ( i == 0 )
        {
            accessor.type = TINYGLTF_TYPE_SCALAR;
        } else if ( i == 2 )
        {
           accessor.type = TINYGLTF_TYPE_VEC2;
        }

        bufferView.buffer = (int)model->buffers.size();
        
        std::vector<unsigned char> tmp;
        tmp.resize( length );
        std::memcpy( &tmp[0], buffers[ i ], length );
        
        dst.insert( dst.end(), tmp.begin(), tmp.end() );
        
        bufferView.byteLength = length;
        bufferView.byteOffset = totalLength;
        bufferView.target = i == 0 ? TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER : TINYGLTF_TARGET_ARRAY_BUFFER;
        model->bufferViews.push_back( bufferView );
        model->accessors.push_back( accessor );
        totalLength += length;
    }
    buffer.data = dst;

    
    auto primitive = Primitive();
    primitive.indices = startAccessorIndex;
    primitive.attributes["POSITION"] = startAccessorIndex + 1;
    primitive.attributes["TEXCOORD_0"] = startAccessorIndex + 2;
    primitive.attributes["NORMAL"] = startAccessorIndex + 3;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;

    mesh.primitives.push_back( primitive );

    node.mesh = (int)model->meshes.size();
    scene.nodes.push_back( (int)model->nodes.size() );
    
    model->scenes[ 0 ] = scene;
    model->buffers.push_back( buffer );
    
    if ( materialName != nullptr ) {
        primitive.material = (int)model->materials.size();
        model->materials.push_back( material );
    }
    model->meshes.push_back( mesh );
    model->nodes.push_back( node );

}

ghAPI void tinygltfSave( tinygltf::Model* model, char* filepath, bool embedImages = false, bool embedBuffers = false, bool prettyPrint = true, bool writeBinary = false )
{
    std::string output( filepath );
    tinygltf::TinyGLTF context;
    context.WriteGltfSceneToFile(model, output, embedImages, embedBuffers, prettyPrint, writeBinary);
//    context.WriteGl
    delete model;
}
