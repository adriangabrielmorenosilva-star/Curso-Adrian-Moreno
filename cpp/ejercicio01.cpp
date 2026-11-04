#include <iostream>
using namespace std;

int main(){

    string nombre;
    int edad;
    string sexo;
    double altura;

    cout << "Cual es tu nombre:"<< endl;
    cin >> nombre;
    cout << "Cuantos anos tienes:"<< endl;
    cin >> edad;
    cout << "Eres mujer o hombre:"<< endl;
    cin >> sexo;
    cout << "Cuanto mides:"<< endl;
    cin >> altura;
    cout << "Genial, tu nombre es: " << nombre << " | Tienes: " << edad << " anos | Eres: " << sexo << " | Y mides: " << altura << endl;
    return 0;


}