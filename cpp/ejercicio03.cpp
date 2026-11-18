#include <iostream>
using namespace std;

int main(){

    int nota1;
    int nota2;
    int nota3;  
    cout << "Cuanto saco en su primera nota:"<< endl;
    cin >> nota1;
    cout << "Cuanto saco en su segunda nota:"<< endl;
    cin >> nota2;
    cout << "Cuanto saco en su tercera nota:"<< endl;
    cin >> nota3;

    double promedio = (nota1 + nota2 + nota3)/ 3.0;
    cout << "Su promedio es:"<< promedio<< endl;

}