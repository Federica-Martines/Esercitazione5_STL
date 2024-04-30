#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>


namespace PolygonalLibrary {
bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }

    return true;

}
// ***************
bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;
        char delim;

        converter >> id >> delim >> marker >> delim >> coord(0) >> delim >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {
            //            if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())
            //                mesh.Cell0DMarkers.insert({marker, {id}});
            //            else
            //                mesh.Cell0DMarkers[marker].push_back(id);


            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }
    file.close();
    return true;
}
// ***************
bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;
        char delim;

        converter >> id >> delim >> marker >> delim >> vertices(0) >> delim >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {
            //            if (mesh.Cell1DMarkers.find(marker) == mesh.Cell1DMarkers.end())
            //                mesh.Cell1DMarkers.insert({marker, {id}});
            //            else
            //                mesh.Cell1DMarkers[marker].push_back(id);


            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}
// ***************
bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);
    mesh.Cell2DMarkers.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line); // lo posso anche chiamare iss IStringStream
        unsigned int id, marker, numVertices, numEdges;
        char delim; //delim cioè delimiter

        converter >> id >> delim >> marker >> delim >> numVertices >> delim;

        vector<unsigned int> vertices(numVertices);

        cout <<"Vertices of id " << id << " are:\t";

        for(unsigned int i = 0; i < numVertices; i ++) {
            converter >> vertices[i] >> delim;
            cout << vertices[i] << " ";

        }

        converter >> numEdges >> delim;

        vector<unsigned int> edges(numEdges);

        cout <<"\t Edges of id " << id << " are:\t";

        for(unsigned int i = 0; i < numEdges; i ++)
        {
            converter >> edges[i] >> delim;
            cout << edges[i] << " ";

        }
        cout << endl;

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);
        mesh.Cell2DMarkers.push_back(marker);
    }
    file.close();
    return true;

}
// ***************

//nelle funzioni di importazione controlliamo già che i marker sono diversi da 0, inoltre non ha senso controllare se i marker sono <0 in quanto unsigned di int,
//quindi molto probabilmente la seguente funzione sul controllo dei marker non serve
bool ControlMarkers(const PolygonalMesh &mesh)
{
    for(const auto marker_id : mesh.Cell0DMarkers)
    {
        int marker = marker_id.first;
        if(marker <= 0)
        {
            cerr << "Marker has not been stored correctly " << endl; //stampa in rosso
            return false;
        }
        else
        {
            return true;
        }

        const list<unsigned int>& ids = marker_id.second;

        for(unsigned int id : ids)
        {
            if(id < 0 || id > mesh.NumberCell0D)
            {
                cerr << "This id is not valid for the marker" << marker << ": " << id << endl;
                return false;
            }
        }
    }

    for(const auto marker_id : mesh.Cell1DMarkers)
    {
        int marker = marker_id.first;
        if(marker <= 0)
        {
            cerr << "Marker has not been stored correctly " << endl; //stampa in rosso
            return false;
        }
        else
        {
            return true;
        }

        const list<unsigned int>& ids = marker_id.second;

        for(unsigned int id : ids)
        {
            if(id < 0 || id > mesh.NumberCell1D)
            {
                cerr << "This id is not valid for the marker" << marker << ": " << id << endl;
                return false;
            }
        }
    }

   for(const auto marker : mesh.Cell2DMarkers)
    {
        if (marker != 0)
            {
                cerr << "Marker has not been stored correctly " << endl;
                return false;
            }
    }

   for(auto id : mesh.Cell2DId)
    {
        if(id < 0 || id > mesh.NumberCell2D)
        {
            cerr << "Invalid id " << endl;
            return false;
        }
    }
}
// ***************

bool ControlEdges(const PolygonalMesh &mesh){
    for (unsigned int c = 0; c < mesh.NumberCell2D; c++)
    {
        const vector<unsigned int>& vertices = mesh.Cell2DVertices[c];
        unsigned int numVertices = vertices.size();

        // Controllo che ci siano almeno 3 vertici per formare un poligono
        if (numVertices < 3) {
            cerr << "The polygon " << c << " has less than 3 vertices." << endl;
            return false;
        }

        for (unsigned int i = 0; i < numVertices; ++i)
        {

            unsigned int vertex1 = vertices[i];
            unsigned int vertex2 = vertices[(i + 1) % numVertices]; // L'ultimo vertice è collegato al primo

            // Calcolo la distanza tra i due vertici
            Vector2d coordinate_punto_1 = mesh.Cell0DCoordinates[vertex1];
            Vector2d coordinate_punto_2 = mesh.Cell0DCoordinates[vertex2];
            double distance = sqrt(pow(coordinate_punto_2.x() - coordinate_punto_1.x(), 2) +
                                   pow(coordinate_punto_2.y() - coordinate_punto_1.y(), 2));


            // Verifico se la distanza è nulla o molto vicina a zero (considerando una tolleranza)
            if (distance < 1e-16)
            {
                cerr << "The distance between the vertices of the polygon " << c << " is too small or zero." << endl;
                return false;
            }
        }
    }

}
// ***************

bool ControlAreas(const PolygonalMesh& mesh)
{
    for (unsigned int c = 0; c < mesh.NumberCell2D; c++)
    {
        const vector<unsigned int>& vertices = mesh.Cell2DVertices[c];
        unsigned int numVertices = vertices.size();

        // Controllo che ci siano almeno 3 vertici per formare un poligono
        if (numVertices < 3) {
            cerr << "The polygon " << c << " has less than 3 vertices." << endl;
            return false;
        }

        double sum = 0;
        double area_polygon = 0;
        for (unsigned int i = 0; i < numVertices; ++i)
        {
            unsigned int vertex1 = vertices[i];
            unsigned int vertex2 = vertices[(i + 1) % numVertices];

            Vector2d coordinates1 = mesh.Cell0DCoordinates[vertex1];
            Vector2d coordinates2 = mesh.Cell0DCoordinates[vertex2];

            double sumi = (coordinates1.x()*coordinates2.y())-(coordinates2.x()*coordinates1.y());
            sum = sum + sumi;
        }
        area_polygon = 0.5 * abs(sum);
        if (area_polygon < 1e-16)
        {
            cerr << "Area of the polygon " << c << " is too short or null." << endl;
            return false;
        }
    }
    return true;
}

}

