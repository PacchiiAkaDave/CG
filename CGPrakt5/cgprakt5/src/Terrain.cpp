#include "Terrain.h"
#include "rgbimage.h"
#include "Terrainshader.h"

Terrain::Terrain(const char* HeightMap, const char* DetailMap1, const char* DetailMap2) : Size(10,1,10)
{
    if(HeightMap && DetailMap1 && DetailMap2)
    {
        bool loaded = load( HeightMap, DetailMap1, DetailMap2);
        if(!loaded)
            throw std::exception();
    }
}

Terrain::~Terrain()
{
    
}

bool Terrain::load( const char* HeightMap, const char* DetailMap1, const char* DetailMap2)
{
    if( !HeightTex.load(HeightMap) )
        return false;
    if( !DetailTex[0].load(DetailMap1) )
        return false;
    if( !DetailTex[1].load(DetailMap2) )
        return false;
    
    // TODO: add code for creating terrain model
    /*float height = HeightTex.getRGBImage()->height();
    float width = HeightTex.getRGBImage()->width();

    std::vector<Vector> normal;
    std::vector<Vector> vertices;

    float xcoord, zcoord;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            normal.push_back(Vector(0, 0, 0));

            xcoord = (x - (width * 0.5)) - 0.5f;
            zcoord = (y - (height * 0.5)) - 0.5f;

            float ycoord = HeightTex.getRGBImage()->getPixelColor(x, y).R;

            vertices.push_back(Vector(xcoord * this->width(), ycoord * this->height(), zcoord * this->depth()));

        }
    }

    IB.begin();
    int a, b, c, d;
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            a = (x + y * width) + 1;
            b = (x + y * width);
            c = (x + y * width) + width;
            d = (x + y * width) + width + 1;

            IB.addIndex(a);
            IB.addIndex(b);
            IB.addIndex(c);

            IB.addIndex(a);
            IB.addIndex(c);
            IB.addIndex(d);

            Vector v1(vertices[a]), v2(vertices[b]), v3(vertices[c]);

            Vector norm = ((v1 - v2).cross(v1 - v3)).normalize();

            normal[x + y * width] = norm;
        }
    }
    IB.end();

    VB.begin();
    float u = 0.0f, v = 0.0f;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            u = x / width;
            v = y / height;

            VB.addTexcoord0(u, v);
            VB.addNormal(normal[x + y * width]);
            VB.addVertex(vertices[x + y * width]);
        }
    }
    VB.end();

    const RGBImage* image = HeightTex.getRGBImage();
    float imageWidth = image->width(); // TODO: int funktioniert nicht, warum? Irgendwo cast?
    float imageHeight = image->height();

    // Wir müssen das Bild zwei Mal durchgehen, da wir für die Berechnung der Normalen 4 Pixel (pro Fläche) brauchen
    // und es günstiger ist, sich die vertices vorher zu speichern und dann abzufragen anstatt einmal durchzugehen 
    // und dann die nachbarn zu berechnen (Beispiel: 1K Bild = 2000 Pixel -> O(4000) vs O(2000*3 pixel = 6000)

    // Vector array 
    std::vector<Vector> normals;
    std::vector<Vector> vertices;

    // Erster Durchlauf: Vertices ermitteln und normalen initialisieren
    for (int y = 0; y < imageWidth; y++)
    {
        for (int x = 0; x < imageHeight; x++)
        {
            normals.push_back(Vector(0, 0, 0));

            // Auf XZ Achse um eine halbe Einheit weniger starten, damit das Terrain zentriert ist
            float xCoord = ((x / imageHeight) - 0.5f);
            float zCoord = ((y / imageWidth) - 0.5f);

            // Heightmap enthält nur Farben im Spektrum von Schwarz zu Weiß, daher sind alle Farbanteile gleich und wir können einen beliebigen Wert nehmen
            //float greyvalue = 0.299 * pixel.R + 0.587 * pixel.G + 0.114 * pixel.B;
            float yCoord = image->getPixelColor(x, y).R;

            vertices.push_back(Vector(xCoord * width(), yCoord * height(), zCoord * depth()));
        }
    }

    // Zweiter Durchlauf: IndexBuffer befüllen und Normalen anhand der bereits gespeicherten Vertices berechnen
    // Wichtig: hier gehen wir bis zur vorletzten Zeile, da nur der IndexBuffer gefüllt werden muss und wir dafür die Nachbarn bestimmen
    // TODO: evtl mehr normalen?
    IB.begin();
    for (int y = 0; y < imageWidth - 1; y++)
    {
        for (int x = 0; x < imageHeight - 1; x++)
        {
            unsigned int a = (x + y * imageHeight) + 1;
            unsigned int b = (x + y * imageHeight);
            unsigned int c = (x + y * imageHeight) + imageHeight;
            unsigned int d = (x + y * imageHeight) + imageHeight + 1;

            // 1. Dreieck
            IB.addIndex(a);
            IB.addIndex(b);
            IB.addIndex(c);

            // 2. Dreieck
            IB.addIndex(c);
            IB.addIndex(d);
            IB.addIndex(a);

            Vector v01(vertices[a]), v02(vertices[b]), v03(vertices[c]);
            Vector normaleDreieck01 = (v02 - v01).cross(v03 - v01).normalize();
            normals[x + y * imageWidth] += normaleDreieck01;

        }
    }
    IB.end();

    // Dritter Durchlauf: VertexBuffer befüllen anhand der gespeicherten Normalen und Vertices
    VB.begin();
    float u = 0.0f;
    float v = 0.0f;

    for (int y = 0; y < imageWidth; y++)
    {
        for (int x = 0; x < imageHeight; x++)
        {
            // uv Koordinaten (zwischen 0 und 1)
            u = (float)x / (float)imageHeight;
            v = (float)y / (float)imageWidth;

            VB.addTexcoord0(u, v);
            VB.addNormal(normals[x + y * imageHeight]);
            VB.addVertex(vertices[x + y * imageHeight]);
        }
    }
    VB.end();*/

    //rgb Bild aus der Textur laden
    const RGBImage* tmp = HeightTex.getRGBImage();

    //Faktor berechnen um die reale Bildgröße in die gewünschte Terraingröße umrechnen zu können
    int imgWidth = tmp->width();
    int imgHeight = tmp->height();

    float widthScale = this->Size.X / imgWidth;
float heightScale = this->Size.Z / imgHeight;

//Texturkoordinaten
float texCordX = 1 / imgWidth;
float texCordY = 1 / imgHeight;

//Vertices vorläufig erstellen
Vector** vertices = new Vector * [imgWidth + 2];
for (int i = 0; i < imgWidth + 2; ++i) {
    vertices[i] = new Vector[imgHeight + 2];
}

{

    for (int col = 0; col < imgHeight; col++) {
        for (int row = 0; row < imgWidth; row++) {
            //Grauwert für Höhe bestimmen
            Color pixelColor = tmp->getPixelColor(row, col);
            float pixelY = (pixelColor.R + pixelColor.G + pixelColor.B) / 3;

            //x und z Wert berechnen
            float pixelX = (col * widthScale - (this->Size.X / 2));
            float pixelZ = (row * heightScale - (this->Size.Z / 2));
            //Vertex zum Buffer hinzufügen
            vertices[col + 1][row + 1] = Vector(pixelX, pixelY, pixelZ);
        }
    }
}

//Vertexbuffer befüllen
this->VB.begin();

for (int col = 0; col < imgHeight; col++) {
    for (int row = 0; row < imgWidth; row++) {
        //Normale und Texturkoordinate hinzufügen
        int x = col + 1;
        int y = row + 1;

        //Normalen berechnen, addieren und teilen
        Vector normal;
        normal += this->normal(vertices[x][y], vertices[x - 1][y], vertices[x - 1][y - 1]);

        normal += this->normal(vertices[x][y], vertices[x - 1][y - 1], vertices[x][y - 1]);

        normal += this->normal(vertices[x][y], vertices[x][y - 1], vertices[x + 1][y]);

        normal += this->normal(vertices[x][y], vertices[x + 1][y], vertices[x + 1][y + 1]);

        normal += this->normal(vertices[x][y], vertices[x + 1][y + 1], vertices[x][y + 1]);

        normal += this->normal(vertices[x][y], vertices[x][y + 1], vertices[x - 1][y]);

        normal.normalize();
        int tmpHeigth = imgHeight / 40;
        int tmpWidth = imgWidth / 40;


        this->VB.addNormal(normal * -1);
        this->VB.addTexcoord0(row / (float)imgWidth - 1, col / (float)imgHeight - 1);
        this->VB.addTexcoord1(row / ((float)imgWidth - 1) * 100, col / ((float)imgHeight - 1) * 100);

        this->VB.addVertex(vertices[x][y]);

    }
}
    this->VB.end();

    for (int i = 0; i < imgWidth + 2; i++) {
    delete[](vertices[i]);
}
delete[](vertices);

this->IB.begin();
for (int col = 0; col < imgHeight - 1; col++) {
    for (int row = 0; row < imgWidth - 1; row++) {
        int index = row + col * imgWidth;
        this->IB.addIndex(index);
        this->IB.addIndex(index + 1);
        this->IB.addIndex(index + 1 + imgWidth);

        this->IB.addIndex(index + 1 + imgWidth);
        this->IB.addIndex(index + imgWidth);
        this->IB.addIndex(index);
    }
}
this->IB.end();
    
    return true;
}

Vector Terrain::normal(const Vector &a,const Vector &b,const Vector &c) {
    Vector norm = (a - b).cross(a - c);
    return norm.normalize();
}

void Terrain::shader( BaseShader* shader, bool deleteOnDestruction )
{
    BaseModel::shader(shader, deleteOnDestruction);
}

void Terrain::draw(const BaseCamera& Cam)
{
    applyShaderParameter();
    BaseModel::draw(Cam);
    // TODO: add code for drawing index and vertex buffers
    
}

void Terrain::applyShaderParameter()
{
    TerrainShader* Shader = dynamic_cast<TerrainShader*>(BaseModel::shader());
    if(!Shader)
        return;
    
    Shader->mixTex(&MixTex);
    for(int i=0; i<2; i++)
        Shader->detailTex(i,&DetailTex[i]);
    Shader->scaling(Size);
    
    // TODO: add additional parameters if needed..
}
