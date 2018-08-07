#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <set>
#include "vec3.h"
#include "uvec3.h"
#include <QString>
#include <GL/gl.h>
#include <object.h>

class Radiosity
{
    public:
        Radiosity() {}

        void assembly(Object *obj);
        void calculateFormFactorsHemicube (int hemicubeResolution); //расчет форм факторов
        void calculateRadiosities ();                       //расчет излучательности
        void render();                                      //рендер
        void renderColorCoded();
        void autoCalculateNormals();                        //расчет нормалей
        void buildVertexToFaceMap ();
        void interpolateColors ();
        uvec3 encodeColor (unsigned int face);
        unsigned int decodeColor (uvec3 color);

    protected:
        int faceIndex = 0, vertxCount = 0;
        std::vector<vec3> vertices;		
        std::vector<uvec3> faces;		
        std::vector<vec3> faceNormals;	
        std::vector< std::vector<int> > VtoF;
        std::vector<vec3> faceEmissions;
        std::vector<vec3> faceReflectivities;
        std::vector<uvec3> faceColorCodes;
        std::vector<std::vector<float>> formFactors;
        std::vector<vec3> radiosities;
        std::vector<vec3> vertexColors;
};

#endif 
