#ifndef SCULPTOR_H
#define SCULPTOR_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iomanip>

struct Voxel {
  float r, g, b, a; // (RGB = cores) e (A  = transparência)
  bool show = false; // Ativado ou desativado
};

class Sculptor {
private:
  Voxel ***v;      // Matriz 3D de voxels
  int nx, ny, nz;  // Dimensões da matriz (Ponto inicial)
  float r, g, b, a; // Cor atual da figura a ser colocada

public:
  Sculptor(int _nx, int _ny, int _nz);
  ~Sculptor();
  void setColor(float r, float g, float b, float a);
  void putVoxel(int x, int y, int z);
  void cutVoxel(int x, int y, int z);
  void putBox(int x0, int x1, int y0, int y1, int z0, int z1);
  void cutBox(int x0, int x1, int y0, int y1, int z0, int z1);
  void putSphere(int xcenter, int ycenter, int zcenter, int radius);
  void cutSphere(int xcenter, int ycenter, int zcenter, int radius);
  void putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz);
  void cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz);
  void writeOFF(const char* filename);
};

#endif 

int main() {
    Sculptor nemoMarcelo(30, 30, 20); // Dimensões 30x30x20 para o espaço do peixe (Apesar de ter usado menos que isso)

    // Cor do peixe (laranja)
    nemoMarcelo.setColor(1.0, 0.5, 0.0, 1.0); 

    // Criação do peixe (várias elipses)
    nemoMarcelo.putEllipsoid(15, 15, 10, 4, 3, 1); // Corpo
    nemoMarcelo.putEllipsoid(16, 15, 10, 1, 1, 1); 

    //Cor dos olhos (pretos)
    nemoMarcelo.setColor(0,0,0,1.0);
    nemoMarcelo.putBox(13,13,16,16,9,9); //Olho esquerdo
    nemoMarcelo.putBox(13,13,16,16,11,11); //Olho direito

    //Cor das listras (brancas)
    nemoMarcelo.setColor(1.0,1.0,1.0,1.0); 
    nemoMarcelo.putBox(14,14,13,17,10,10);//listra perto dos olhos
    nemoMarcelo.putBox(16,16,13,17,10,10);//listra longe dos olhos


    // Salve a escultura do peixe em um arquivo OFF
    nemoMarcelo.writeOFF("nemoMarcelo.off");

    return 0;
}

// Método contrutor, configura as dimensões do objeto tridimensional.
Sculptor ::Sculptor(int _nx, int _ny, int _nz) {
  nx = _nx;
  ny = _ny;
  nz = _nz;

  // Alocação dinâmica.
  v = new Voxel **[nx];
  for (int i = 0; i < nx; i++) {
    v[i] = new Voxel *[ny];
    for (int j = 0; j < ny; j++) {
      v[i][j] = new Voxel[nz];
    }
  }
}

// Método destrutor, responsável por liberar a memória alocada dinamicamente para a matriz de voxels.
Sculptor ::~Sculptor() {

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      delete[] v[i][j];
    }
    delete[] v[i];
  }
  delete[] v;
}

// Método que define a cor atual do desenho, toda vez que usa-la ira definir uma nova cor.
void Sculptor ::setColor(float _r, float _g, float _b, float _a) {
  r = _r;
  g = _g;
  b = _b;
  a = _a;
}

// Ativa um voxel na posição e atribui ao mesmo a cor atual de desenho.
void Sculptor ::putVoxel(int x, int y, int z) {
  v[x][y][z].show = true;
  v[x][y][z].r = r;
  v[x][y][z].g = g;
  v[x][y][z].b = b;
  v[x][y][z].a = a;

}

// Desativa um voxel na posição.
void Sculptor ::cutVoxel(int x, int y, int z) {
  v[x][y][z].show = false;
}

void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1) {
// Coloca Voxels dentro de um formato de cubo com medidas dadas pelo usuário. O cubo vai seguir a cor atual definida.
  for (int i = x0; i <= x1; i++) {
    for (int j = y0; j <= y1; j++) {
      for (int k = z0; k <= z1; k++) {
        putVoxel(i, j, k);
      }
    }
  }
}

void Sculptor::cutBox(int x0, int x1, int y0, int y1, int z0, int z1) {
  // Retira Voxels dentro de um formato de cubo, com medidas dadas pelo usuário.(Basicamente ao invés de adicionar um cubo, remove ele).
  for (int i = x0; i <= x1; i++) {
    for (int j = y0; j <= y1; j++) {
      for (int k = z0; k <= z1; k++) {
        cutVoxel(i, j, k);
      }
    }
  }
}

// Coloca Voxels dentro de um formato de esfera com medidas dadas pelo usuário. A esfera vai seguir a cor atual definida.
void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius) {
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      for (int k = 0; k < nz; k++) {
        float dx = i - xcenter;
        float dy = j - ycenter;
        float dz = k - zcenter;
        float distance = sqrt(dx*dx + dy*dy + dz*dz);
        if (distance <= radius) {
          putVoxel(i, j, k);
        }
      }
    }
  }
}

void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius) {
  // Retira Voxels dentro de um formato de esfera, com medidas dadas pelo usuário.(Basicamente ao invés de adicionar uma esfera, remove ela).
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      for (int k = 0; k < nz; k++) {
        if ((i - xcenter) * (i - xcenter) + (j - ycenter) * (j - ycenter) +
                (k - zcenter) * (k - zcenter) <=
            radius * radius) {
          cutVoxel(i, j, k);
        }
      }
    }
  }
}

void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz) {
// Coloca Voxels dentro de um formato de uma elipse, com medidas dadas pelo usuário. A elipse vai seguir a cor atual definida.
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                float normalizedX = static_cast<float>(i - xcenter) / rx;
                float normalizedY = static_cast<float>(j - ycenter) / ry;
                float normalizedZ = static_cast<float>(k - zcenter) / rz;
                float distance = normalizedX * normalizedX + normalizedY * normalizedY + normalizedZ * normalizedZ;

                if (distance <= 1.0) {
                    putVoxel(i, j, k);
                }
            }
        }
    }
}

void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz) {
  // Retira Voxels dentro de um formato de elipse, com medidas dadas pelo usuário.(Basicamente ao invés de adicionar uma elipse, remove ela).
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                float normalizedX = static_cast<float>(i - xcenter) / rx;
                float normalizedY = static_cast<float>(j - ycenter) / ry;
                float normalizedZ = static_cast<float>(k - zcenter) / rz;
                float distance = normalizedX * normalizedX + normalizedY * normalizedY + normalizedZ * normalizedZ;

                if (distance <= 1.0) {
                    cutVoxel(i, j, k);
                }
            }
        }
    }
}

void Sculptor::writeOFF(char const *filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita." << std::endl;
        return;
    }

    // Escreve os dados da escultura no formato OFF
    int numVertices = 0;
    int numFaces = 0;

    // Calcula o número de Voxels ativos.
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                if (v[i][j][k].show) {
                    numVertices += 8;
                    numFaces += 6;
                }
            }
        }
    }

    outfile << "OFF" << std::endl;
    outfile << numVertices << " " << numFaces << " 0" << std::endl;

    // Escreve as coordenadas dos vértices
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                if (v[i][j][k].show) {
                    outfile << i - 0.5 << " " << j - 0.5 << " " << k - 0.5 << std::endl;
                    outfile << i - 0.5 << " " << j + 0.5 << " " << k - 0.5 << std::endl;
                    outfile << i + 0.5 << " " << j + 0.5 << " " << k - 0.5 << std::endl;
                    outfile << i + 0.5 << " " << j - 0.5 << " " << k - 0.5 << std::endl;
                    outfile << i - 0.5 << " " << j - 0.5 << " " << k + 0.5 << std::endl;
                    outfile << i - 0.5 << " " << j + 0.5 << " " << k + 0.5 << std::endl;
                    outfile << i + 0.5 << " " << j + 0.5 << " " << k + 0.5 << std::endl;
                    outfile << i + 0.5 << " " << j - 0.5 << " " << k + 0.5 << std::endl;
                }
            }
        }
    }

    // Escreve os índices das faces,
    int offset = 0;
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                if (v[i][j][k].show) {
                    outfile << std::fixed << std::setprecision(2); // Use std::fixed e std::setprecision
                    outfile << "4 " << 0 + offset << " " << 3 + offset << " " << 2 + offset << " " << 1 + offset << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    outfile << "4 " << 4 + offset << " " << 5 + offset << " " << 6 + offset << " " << 7 + offset << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    outfile << "4 " << 0 + offset << " " << 1 + offset << " " << 5 + offset << " " << 4 + offset << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    outfile << "4 " << 0 + offset << " " << 4 + offset << " " << 7 + offset << " " << 3 + offset << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    outfile << "4 " << 1 + offset << " " << 2 + offset << " " << 6 + offset << " " << 5 + offset << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    outfile << "4 " << 2 + offset << " " << 3 + offset << " " << 7 + offset << " " << 6 + offset << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    offset += 8;
                }
            }
        }
    }

    // Fecha a seção OFF
    outfile.close();

    std :: cout << "Figura " << filename << " criada e pronta para vizualizacao" << std::endl;
}