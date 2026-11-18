#include <iostream>
using namespace std;

int main(){
    int opcion;
    cout<<"ELIJA SU OPCION DEL 1 AL 3"<<endl;
    cout <<"1: Hamburguesa con papas y refresco."<<endl;
    cout<<"2: Pizza personal con bebida."<<endl;
    cout<<"3: Ensalada especial con agua."<<endl;
    cin >> opcion;

    switch (opcion){
        case 1:
        cout <<"Ha elejido la hmburguesa con papas y refresco"<<endl;
        break;
        case 2:
        cout <<"Ha elejido la pizza personal con bebida"<<endl;
        break;
        case 3:
        cout <<"Ha elejido la ensalada"<<endl;
        break;
        default:
        cout <<"Opcion no valida, elija una opcion del 1 al 3"<<endl;
        break;
  
    }

    return 0;
    

}