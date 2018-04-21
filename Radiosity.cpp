#include "Radiosity.h"
#include "vec3.h"
#include "uvec3.h"
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <object.h>

using namespace std;

long faceIndex = 0;
void Radiosity::assembly(Object *obj)
{
    for(int i=0;i<obj->countVertex;i++)
    {
        vertices.push_back(obj->getVertices()[i]);
        vertexColors.push_back(obj->getVertexColors()[i]);
    }
    for(int i=0;i<obj->countFaces;i++)
    {
        faces.push_back(obj->getFaces()[i]);
        faceEmissions.push_back(obj->getFaceEmissions()[i]);
        faceReflectivities.push_back(obj->getFaceReflectivities()[i]);
        faceIndex++;
    }
}

void Radiosity::buildVertexToFaceMap() {
    for(int i=0;i<faceIndex;i++)
    {
        uvec3 code = encodeColor(i);
        faceColorCodes.push_back(code);
    }
    vec3 r(0.8, 0.8, 0.8);
    radiosities = vector<vec3> (faces.size(), r);
    VtoF = vector< vector<int> >(vertices.size());

    for (unsigned int i = 0; i < faces.size(); i++) {
        uvec3& face = faces[i];
        VtoF[face.x].push_back(i);
        VtoF[face.y].push_back(i);
        VtoF[face.z].push_back(i);
    }
}

void Radiosity::render() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (unsigned int fi = 0; fi < faces.size(); fi++) {
        uvec3& face =  faces[fi];
        vec3& v1 = vertices[face.x];
        vec3& v2 = vertices[face.y];
        vec3& v3 = vertices[face.z];

        vec3& c1 = vertexColors[face.x];
        vec3& c2 = vertexColors[face.y];
        vec3& c3 = vertexColors[face.z];

        glBegin(GL_TRIANGLES);
            glColor3f(c1.x, c1.y, c1.z);
            glVertex3f(v1.x, v1.y, v1.z);
            glColor3f(c2.x, c2.y, c2.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glColor3f(c3.x, c3.y, c3.z);
            glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }
}


void Radiosity::autoCalculateNormals () {
    faceNormals.clear();
    for (unsigned int i = 0; i < faces.size(); i++) {
        uvec3& face =  faces[i];
        vec3& v1 = vertices[face.x];
        vec3& v2 = vertices[face.y];
        vec3& v3 = vertices[face.z];
        vec3 normal = cross(v2 - v1, v3 - v1);
        normal.normalize();
        faceNormals.push_back(normal);
    }
}

void Radiosity::calculateFormFactors(int hemicubeResolution) {
    formFactors.clear();
    formFactors = vector<vector<float>> (faces.size(), vector<float>(faces.size(), 0.0f));
    for (unsigned int f = 0; f < faces.size(); f++) {

        int *pixelCount =new int[faces.size()]{0};
        uvec3& face = faces[f];
        vec3 centroid = (vertices[face.x] + vertices[face.y] + vertices[face.z]) * 0.333333f;

        vec3& n = faceNormals[f];
        vec3 d1, d2;

        vec3 a = vertices[face.x] - centroid;
        vec3 b =  vertices[face.y] - centroid;
        vec3 c = vertices[face.z] - centroid;
        d1 = longestVector(a,b,c);
        d1.normalize();
        d2 =  cross(n, d1);

        float r = 0.5;

        float left, right, bottom, top;
        vec3& eye = centroid;
        vec3 lookat, up;

        for (int i = 1; i <= 5; i++) {
            if (i == 1) { 
                left = -r; right = r; bottom = -r; top = r;
                lookat = centroid + n; up = d2;
            }
            else if (i == 2) { 
                left = -r; right = r; bottom = 0; top = r;
                lookat = centroid + d1; up = n;
            }
            else if (i == 3) { 
                left = -r; right = r; bottom = 0; top = r;
                lookat = centroid - d1; up = n;
            }
            else if (i == 4) { 
                left = -r; right = r; bottom = 0; top = r;
                lookat = centroid - d2; up = n;
            }
            else if (i == 5) { 
                left = -r; right = r; bottom = 0; top = r;
                lookat = centroid + d2; up = n;
            }

            unsigned int viewWidth = (right - left) / (2 * r) * hemicubeResolution;
            unsigned int viewHeight = (top - bottom) / (2 * r) * hemicubeResolution;
            glViewport(0, 0, viewWidth, viewHeight);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glFrustum(left, right, bottom, top, r, 1000);
            gluLookAt(eye.x, eye.y, eye.z, lookat.x, lookat.y, lookat.z, up.x, up.y, up.z);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderColorCoded();

            GLubyte* buffer = new GLubyte[viewWidth * viewHeight * 3];
            glReadPixels(0, 0, viewWidth, viewHeight, GL_RGB, GL_UNSIGNED_BYTE, buffer);

            for (int y = 0; y < viewHeight; y++) {
                for (int x = 0; x < viewWidth; x++) {
                    uvec3 color(buffer[(y*viewWidth+x)*3], buffer[(y*viewWidth+x)*3+1], buffer[(y*viewWidth+x)*3+2]);
                    unsigned int face = decodeColor(color);
                    if (face == -1) {
                        continue;
                    }
                    pixelCount[face]++;
                }
            }

            delete buffer;
        }

        int totalPixels = 3 * hemicubeResolution * hemicubeResolution;

        for (unsigned int k = 0; k < faces.size(); k++) {
            if (k == f) {
                formFactors[f][k] = 0;
                continue;
            }
            float factor = pixelCount[k] / (float) totalPixels;
            formFactors[f][k] = factor;
        }
    }
}

void Radiosity::renderColorCoded() {
    for (unsigned int fi = 0; fi < faces.size(); fi++) {
        uvec3& face =  faces[fi];
        vec3& v1 = vertices[face.x];
        vec3& v2 = vertices[face.y];
        vec3& v3 = vertices[face.z];

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        uvec3& color = faceColorCodes[fi];
        glColor3ub(color.x, color.y, color.z);
        glBegin(GL_TRIANGLES);
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }
}

uvec3 Radiosity::encodeColor (unsigned int f) {
    uvec3 code;
    f = f + 1; 
    code.x = f % 256;
    code.y = (f >> 8) % 256;
    code.z = (f >> 16) % 256; 
    return code;
}

unsigned int Radiosity::decodeColor (uvec3 color) {
    return color.x + ((unsigned int)color.y << 8) + ((unsigned int)color.z << 16) - 1;
}

void Radiosity::calculateRadiosities() {
    for (int i = 0; i < faces.size(); i++) {
        radiosities[i] = faceEmissions[i];
        for (int j = 0; j < faces.size(); j++) {
            radiosities[i].add(faceReflectivities[i] * formFactors[j][i] * radiosities[j]);
        }
    }
}

void Radiosity::interpolateColors() {
    for (int vi = 0; vi < vertices.size(); vi++) {
        vector<int>& nearFaces = VtoF[vi];
        vec3 color(0,0,0);
        for (int fi = 0; fi < nearFaces.size(); fi++) {
            color.add(radiosities[nearFaces[fi]]);
        }
        color = color / (float) nearFaces.size();
        vertexColors[vi] = color;
    }
}
