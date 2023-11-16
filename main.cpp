#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Sculptor.h"
#include "FiguraGeometrica.h"
#include "PutVoxel.h"
#include "CutVoxel.h"
#include "PutBox.h"
#include "CutBox.h"
#include "PutSphere.h"
#include "CutSphere.h"
#include "PutEllipsoid.h"
#include "CutEllipsoid.h"

int main() {
    std::ifstream inputFile("descricaodafigura.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Erro ao entender o arquivo da descricao da figura." << std::endl;
        return 1;
    }

    Sculptor sculptor(20, 20, 20);
    std::vector<FiguraGeometrica*> figuras;

    std::string linha;
    while (std::getline(inputFile, linha)) {
        std::stringstream ss(linha);
        std::string comando;
        ss >> comando;

        if (comando == "PutVoxel") {
            int x, y, z;
            float r, g, b, a;
            ss >> x >> y >> z >> r >> g >> b >> a;
            figuras.push_back(new PutVoxel(x, y, z, r, g, b, a));
        } else if (comando == "CutVoxel") {
            int x, y, z;
            ss >> x >> y >> z;
            figuras.push_back(new CutVoxel(x, y, z));
        } else if (comando == "PutBox") {
            int x0, x1, y0, y1, z0, z1;
            float r, g, b, a;
            ss >> x0 >> x1 >> y0 >> y1 >> z0 >> z1 >> r >> g >> b >> a;
            figuras.push_back(new PutBox(x0, x1, y0, y1, z0, z1, r, g, b, a));
        } else if (comando == "CutBox") {
            int x0, x1, y0, y1, z0, z1;
            ss >> x0 >> x1 >> y0 >> y1 >> z0 >> z1;
            figuras.push_back(new CutBox(x0, x1, y0, y1, z0, z1));
        } else if (comando == "PutSphere") {
            int xcenter, ycenter, zcenter, radius;
            float r, g, b, a;
            ss >> xcenter >> ycenter >> zcenter >> radius >> r >> g >> b >> a;
            figuras.push_back(new PutSphere(xcenter, ycenter, zcenter, radius, r, g, b, a));
        } else if (comando == "CutSphere") {
            int xcenter, ycenter, zcenter, radius;
            ss >> xcenter >> ycenter >> zcenter >> radius;
            figuras.push_back(new CutSphere(xcenter, ycenter, zcenter, radius));
        } else if (comando == "PutEllipsoid") {
            int xcenter, ycenter, zcenter, rx, ry, rz;
            float r, g, b, a;
            ss >> xcenter >> ycenter >> zcenter >> rx >> ry >> rz >> r >> g >> b >> a;
            figuras.push_back(new PutEllipsoid(xcenter, ycenter, zcenter, rx, ry, rz, r, g, b, a));
        } else if (comando == "CutEllipsoid") {
            int xcenter, ycenter, zcenter, rx, ry, rz;
            ss >> xcenter >> ycenter >> zcenter >> rx >> ry >> rz;
            figuras.push_back(new CutEllipsoid(xcenter, ycenter, zcenter, rx, ry, rz));
        }
    }

    inputFile.close();

    for (FiguraGeometrica* figura : figuras) {
        figura->draw(sculptor);
        delete figura;
    }

    sculptor.writeOFF("nemoMarcelo.off");

    return 0;
}
