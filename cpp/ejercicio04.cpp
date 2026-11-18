#include <iostream>
using namespace std;

int main(){
    int edad;
    cout<<"Ingresa tu edad:"<< endl;
    cin >> edad;
    bool rangoEdad = (edad < 18 || edad > 60);
    cout << "se encuentra en el siguiente rango de edad:"<< rangoEdad<< endl;
    return 0;
}

