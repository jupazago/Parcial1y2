#include <iostream>
#include <fstream> //eof
#include <string>
#include <exception>
#include <stdlib.h> //cls // Limpiar consola

using namespace std;

void Panel1();
void Panel2();
bool IniciarSesion();

int main()
{
    int admOuser=1, opcion;
    bool perfilAdm, perfilClient;

    while (admOuser > 0) {
        system("cls");
        Panel1();//Menu principal
        cin >> admOuser;

        if(admOuser==1) {
            system("cls");
            //Ingresaremos datos para compararlos con el txt
            perfilAdm=IniciarSesion();
            //Encontraremos todo el menu de Administrador
            if(perfilAdm==true){
                Panel2();
                cin >> opcion;
            }

        }else if(admOuser==2) {

        }
    }

    return 0;
}
void Panel1(){
    cout << "OPCIONES: INICIAR SESION:" << endl;
    cout << "/////////////////////////" << endl;
    cout << "1: Administrador" << endl;
    cout << "2: Usuario/Cliente" << endl<<endl;
    cout << "Eleccion: ";
}
void Panel2(){
    cout << "/////////////////////////" << endl;
    cout << "1: Crear Otra cuenta Administracion" << endl;
    cout << "2: Usuario/Cliente" << endl<<endl;
    cout << "Eleccion: ";
}
bool IniciarSesion(){
    /*  Mediante archivo txt consultaremos si los datos suministrados
     *  son correctos para el inicio de sesion
     */

    string userBD, passBD, user, pass, texto;

    ifstream lectAdmin;

    try {
        lectAdmin.open("administrador.txt", ios::in); //abro archivo para su lectura
        if(!lectAdmin.is_open())
            throw '1'; //Capturamos excepcion si hubo errores

        while(!lectAdmin.eof()){ //mientras no sea el final del archivo
            //Obtenemos los datos de la base de datos del unico admin
            getline(lectAdmin,texto);
        }
        lectAdmin.close();

        cout << texto<<endl<<endl;
        cout << "Ingresar Usuario: ";
        cin >> user;
        cout << "Contrasenia: ";
        cin >> pass;

        int tamano_user= user.size();

        if (texto.find(user) != string::npos) {
            //Si se encuentra el usuario continuamos

        } else{
            //Si no existe el usuario capturamos la excepcion
            throw '3';
        }

        size_t pos1 = texto.find(user);
        texto = texto.substr(pos1);
        cout << texto<<endl<<endl; //Buscar el usuario
        string clave = texto.substr(tamano_user+1,4);
        cout <<endl<<clave<<endl;


        if(pass==clave){
            system("cls");
            cout << "Hola administrador"<<endl<<endl;
            return true;
        }else{
            throw '2';
        }

    } catch (char excepcion) {
        if(excepcion == '1'){
            cout << "Error de lectura en SUDO"<<endl;
        }else if(excepcion == '2'){
            cout << "Datos Incorrectos"<<endl;
        }else if(excepcion == '3'){
            cout << "Usuario incorrecto"<<endl;
        }
        system("pause");
        return false;
    }
}
