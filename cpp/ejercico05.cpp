#include <iostream>
using namespace std;

int main(){
    int edad;
    bool tieneMembresia;
    
    cout << "Ingrese su edad:" << endl;
    cin >> edad;
    
    cout << "Tiene membresia VIP? (1=Si, 0=No):" << endl;
    cin >> tieneMembresia;

    
    bool puedeIngresar = (edad > 25 || tieneMembresia);
    
    cout << "Cumple con las normas para entrar al VIP (1=Si, 0=No): " << puedeIngresar << endl;

    return 0;
}