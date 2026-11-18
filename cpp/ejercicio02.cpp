#include <iostream>
using namespace std;

int main(){

    double precioProducto = 100.0;
    bool precioIva = 20;
    int cantidadProducto = 5;
    double subTotal = (precioProducto + precioIva) * cantidadProducto;
    cout<<"El monto que va a cancelar es:"<< subTotal<< endl;
    return 0;

}