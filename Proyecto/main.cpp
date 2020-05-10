#include <iostream>
#include <fstream> //eof
#include <string>
#include <exception>
#include <stdlib.h> //cls // Limpiar consola
#include <map>
#include <vector>

using namespace std;

//Estructuran Publica
struct Producto{
    string nombre;
    int cantidad;
    int costo;
};

void Panel1();
void Panel2();
bool IniciarSesion();
void CrearUsuario();
map<int,Producto> lecturaDeInventario();

int main()
{
    int admOuser=1, opcion;
    bool perfilAdm;

    map<int,Producto> Inventario; //Vacío

    while (admOuser > 0) {
        system("cls");
        Panel1();//Menu principal
        cin >> admOuser;

        if(admOuser==1) {
            system("cls");
            //Ingresaremos datos para compararlos con el txt
            perfilAdm=IniciarSesion();
            //true si se inicio con exito la sesion

            //Encontraremos todo el menu de Administrador
            if(perfilAdm==true){
                Panel2();
                cin >> opcion;
                if(opcion==1){
                    CrearUsuario();
                }else if(opcion==2){

                    Inventario=lecturaDeInventario();//Llenamos el Inventario del txt
                }
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
    cout << "2: Agregar/Actualizar producto Inventario" << endl;
    cout << "3: Crear combos" << endl;
    cout << "4: Generar Reporte de Ventas" << endl<<endl;
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

        //Comparamos la clave ingresada por el usuario vs la registrada en el txt
        size_t pos1 = texto.find(user);
        texto = texto.substr(pos1);
        //cout << texto<<endl<<endl; //Buscar el usuario
        string clave = texto.substr(tamano_user+1,4);
        //cout <<endl<<clave<<endl;


        if(pass==clave){
            system("cls");
            cout << "Hola administrador"<<endl;
            return true;
        }else{
            throw '2';
        }

    } catch (char excepcion) {
        if(excepcion == '1'){
            cout << "Error de lectura"<<endl;
        }else if(excepcion == '2'){
            cout << "Datos Incorrectos"<<endl;
        }else if(excepcion == '3'){
            cout << "Usuario incorrecto"<<endl;
        }
        system("pause");
        return false;
    }
}
void CrearUsuario(){
    string usuario, contrasenia;
    bool quedarse=false;
    int tcontrasenia;

    system("cls");
    cout << endl<<"Tenga en Cuenta"<<endl;
    cout << "*La clave debe tene unicamente 4 digitos"<<endl;

    cout<<endl<<"Ingrese el usuario a registrar: ";
    cin >> usuario;

    while(quedarse==false){
        cout<<endl<<"Ingrese la clave: ";
        cin >> contrasenia;
        tcontrasenia = contrasenia.size();
        if(tcontrasenia==4){
            quedarse=true;
        }else{
            quedarse=false;
        }
    }

    string datos= " "+usuario+" "+contrasenia;

    ofstream escritura;
    //guardar en el texto de base de datos
    escritura.open("administrador.txt",ios::app);
    escritura << datos;
    escritura.close();
}
map<int,Producto> lecturaDeInventario(){
    string NombreProdu;
    int IDprodu, CantiProdu, CostoProdu;
    ifstream lectura;

    //mapa de clave ID y valor de tipo estructura Producto
    map<int,Producto> Inventario; //mapa se llama Inventario, está vacio.
    map<int,Producto>::iterator it;

    Producto producto;

    lectura.open("inventario.txt", ios::in);
    int i=1;
    while (!lectura.eof()) {
        lectura >> IDprodu;
        lectura >> NombreProdu;
        lectura >> CantiProdu;
        lectura >> CostoProdu;

        producto.nombre=NombreProdu;
        producto.cantidad=CantiProdu;
        producto.costo=CostoProdu;
        Inventario[i]=producto;
        i++;
    }

    lectura.close();

    for(auto par=begin(Inventario); par!=end(Inventario); par++){
        cout << par->first <<" ";
        cout<<par->second.nombre <<" "<<par->second.cantidad <<" "<<par->second.costo<<endl;
    }
    system("pause");

    return Inventario;

}
