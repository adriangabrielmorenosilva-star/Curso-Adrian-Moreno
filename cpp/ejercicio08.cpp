#include <iostream>
using namespace std;

int main() {
    int opcion;

    do {

        cout<<"/n===Cajero Automatico==="<< endl;
        cout<<"1.Depositar Dinero"<< endl;
        cout<<"2.Retirar Dinero"<< endl;
        cout<<"3.Enviar Dinero"<< endl;
        cout<<"4.Salir"<< endl;
        cin>> opcion;

        switch (opcion){
            case 1:
            cout <<" ha depositado 500$"<<endl;
            break;

            case 2:
            cout <<"ha retirado 1500$"<<endl;
            break;

            case 3:
            cout <<"ha enviado 200$"<<endl;
            break;

            case 4:
            cout <<"ha salido del sitema"<<endl;
            break;

            default:
            cout <<"Opcion Invalidad, elija una opcion del 1 al 3"<<endl;
            break;     
        }
          
    } while (opcion!=4);
    
}