//
//  main.cpp
//  test
//
//  Created by HIROAKI YAMANE on 03/04/2020.
//  Copyright © 2020 HIROAKI YAMANE. All rights reserved.
//

#include <iostream>
#include "exposed-tinygltf.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    #ifdef _WIN32
            string path = "C:\\Users\\mnmly\\Downloads\\sample.abc";
            char* p = const_cast<char*>(path.c_str());
    #elif __APPLE__
            std::string path = "/Users/mnmly/Downloads/demo.gltf";
            char* p = const_cast<char*>(path.c_str());
    #endif
    
    float vertices[72] = { -0.5,0,0,-0.5,0,1,0.5,0,0,0.5,0,1,0.5,0,0,0.5,0,1,0.5,1,0,0.5,1,1,0.5,1,0,0.5,1,1,-0.5,1,0,-0.5,1,1,-0.5,1,0,-0.5,1,1,-0.5,0,0,-0.5,0,1,-0.5,0,0,-0.5,1,0,0.5,0,0,0.5,1,0,-0.5,0,1,-0.5,1,1,0.5,0,1,0.5,1,1 };
   
   int numVertices = 72;
   float normals[72] = {-1,-1,-1,-1,-1,1,1,-1,-1,1,-1,1,1,-1,-1,1,-1,1,1,1,-1,1,1,1,1,1,-1,1,1,1,-1,1,-1,-1,1,1,-1,1,-1,-1,1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,1,-1,-1,-1,1,-1,1,1,-1,-1,-1,1,-1,1,1,1,-1,1,1,1,1};
    int numNormals = 72;
    float uvs[48] = {1,0,1,1,0,1,0,0,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1
};
    int numUvs = 24;
   
    int faces[36] = {1,0,2,5,4,6,9,8,10,13,12,14,17,16,18,21,20,22,1,2,3,5,6,7,9,10,11,13,14,15,17,18,19,21,22,23};
    
    
    int numFaces = 36;
    int numFaceCount = 36 / 3;
    
    float points[12] = {0,0,0, 13,7,0, 23,-8,0 ,0,5,5};
    float numPoints = 12;
    
    float points2[12] = {0,0,0, 13,7,0, 23,-8,0 ,0,5,5};
    float numPoints2 = 12;
           
    auto model = tinygltfCreateModel();
    
    tinygltfAddPoints( model, "point", nullptr, points, numPoints, true );
    tinygltfAddLineLoop( model, "point", nullptr, points2, numPoints, true );
    tinygltfAddMesh(model, "/mesh",
    nullptr,
    vertices, numVertices,
    normals, numNormals,
    uvs, numUvs,
    faces, numFaces, true);

    tinygltfSave( model, p, false, true, true, true );
    
//    tinygltfDestory( model );
    return 0;
}
