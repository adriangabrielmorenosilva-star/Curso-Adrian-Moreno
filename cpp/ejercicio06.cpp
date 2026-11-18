#include <iostream>
using namespace std;

int main(){
    char vocal;
    cout<<"Introduce una Vocal:"<< endl;
    cin>> vocal;

    if (vocal == 'a' || vocal =='e'|| vocal == 'i'|| vocal =='o' || vocal == 'u' ){
     cout<<"Tu vocal es una minuscula:"<<endl;

    }
    else if (vocal == 'A' || vocal =='E'|| vocal == 'I'|| vocal =='O' || vocal == 'U') {
        cout<<"Tu vocal es una Mayuscula:"<<endl;
    } 
    else {
        cout<<"Opcion no Valida, intriduce una vocal:"<<endl;

    }    
       
  


}