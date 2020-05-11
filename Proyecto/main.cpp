#include <iostream>
#include <fstream> //eof //Manejor de archivos
#include <string>
#include <exception>
#include <stdlib.h> //cls // Limpiar consola
#include <map>
#include <vector>
#include <sstream>


using namespace std;

//Estructuran Publica
struct Producto{
    string nombre;
    int cantidad;
    int costo;
};
struct Combo{
    int id;
    int cantidad;
};

void Panel1();
void Panel2();
bool IniciarSesion();
void CrearUsuario();
map<int,Producto> lecturaDeInventario();
map<int,Producto> AgregarActualizar(map<int,Producto> Inventario);
void ImprimirInventario(map<int,Producto> Inventario);
void VerCombos();

int main()
{
    int admOuser=1, opcion;
    bool perfilAdm;

    map<int,Producto> Inventario; //Vacío

    while (admOuser > 0) {
        system("cls");
        //Menu principal
        Panel1();
        cin >> admOuser;

        if(admOuser==1) {
            //Menu de Administrador
            system("cls");
            //Ingresaremos datos para compararlos con el txt
            perfilAdm=IniciarSesion();
            //true si se inicio con exito la sesion

            //Encontraremos todo el menu de Administrador
            while(perfilAdm==true){
                Panel2();
                cin >> opcion;
                if(opcion==1){
                    CrearUsuario();
                }else if(opcion==2){
                    Inventario = lecturaDeInventario();//Llenamos el Inventario del txt
                    Inventario = AgregarActualizar(Inventario);

                }else if(opcion==3){
                    VerCombos();

                }
            }

        }else if(admOuser==2) {
            //Menu de cliente
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

    ImprimirInventario(Inventario);
    system("pause");

    return Inventario;

}
map<int,Producto> AgregarActualizar(map<int,Producto> Inventario){
    try {


        Producto producto;
        bool encontrado=false;
        int ncantidad, ncosto;
        int id;
        cout << "Actualizar producto ingresa su ID, o crear producto ingresa 0: ";
        cin >> id;

        if(id!=0){
            for(auto par=begin(Inventario); par!=end(Inventario); par++){
                //cout <<"ID: "<< par->first <<endl;
                if(par->first == id && encontrado==false){

                    cout << "Producto: "<<par->second.nombre<<endl<<endl;
                    cout << "La cantidad actual es: "<< par->second.cantidad<<endl;
                    cout << "La Nueva cantidad sera: ";
                    cin >> ncantidad;
                    par->second.cantidad = ncantidad;

                    cout << "El precio actual por unidad es: "<< par->second.costo<<endl;
                    cout << "El nuevo precio por unidad sera: ";
                    cin >> ncosto;
                    par->second.costo = ncosto;
                    encontrado=true;


                    ofstream escritura;
                    escritura.open("inventario.txt", ios::out);

                    int i=1;
                    for(auto par=begin(Inventario); par!=end(Inventario); par++){
                        //Cantidad de iteraciones por elemento de inventario
                        if(i==1){
                            escritura << i<<" "<< par->second.nombre <<" "<<par->second.cantidad <<" "<<par->second.costo;
                            i++;
                        }else{
                            escritura <<"\n"<< i<<" "<< par->second.nombre <<" "<<par->second.cantidad <<" "<<par->second.costo;
                            i++;
                        }

                    }


                }
            }
        }


        if(encontrado!=true){
            system("cls");
            cout << "     Registrar Producto"<<endl;
            cout << "-----------------------------"<<endl;
            string nombre;
            cout << "Ingresa Nombre producto";
            cin >> nombre;
            cout <<endl<< "Ingresa Cantidad producto";
            cin >> ncantidad;
            cout <<endl<< "Ingresa el Costo";
            cin >> ncosto;

            producto.nombre=nombre;
            producto.cantidad=ncantidad;
            producto.costo=ncosto;

            int i=0;
            for(auto par=begin(Inventario); par!=end(Inventario); par++){
                //Cantidad de iteraciones por elemento de inventario
                i++;
            }


            Inventario[i]=producto;

            ofstream escritura;
            escritura.open("inventario.txt", ios::app);
            escritura << "\n" << i<<" "<<nombre<<" "<< ncantidad<<" "<<ncosto;
            escritura.close();

            cout << "Creacion exitosa"<<endl;
            ImprimirInventario(Inventario);







        }else{
            cout << "Actualizacion exitosa"<<endl;
            ImprimirInventario(Inventario);
        }
        return Inventario;

    } catch (char excepcion) {
        cout << "ERROR"<<endl;
        return Inventario;
    }
}
void ImprimirInventario(map<int,Producto> Inventario){
    cout <<endl<<endl<< "Lista Completa de Inventario"<<endl;
    cout << "-----------------------------"<<endl;
    for(auto par=begin(Inventario); par!=end(Inventario); par++){
        cout <<"ID: "<< par->first <<endl;
        cout <<par->second.nombre<<endl;
        cout << "Cantidad: "<< par->second.cantidad<<endl;
        cout << "Costo: " << par->second.costo<<endl<<endl;
        cout << "-------------------"<<endl;
    }
}
void VerCombos(){
    string dato;
    int ID, n1, n2;

    try {
        map<int,vector<Combo>> contenedor;
        Combo estrCombo;


        ifstream lectura;
        lectura.open("combos.txt",ios::in);

        while (!lectura.eof()) {
            lectura >> dato;
            if(dato == "*"){
                lectura >> dato;
                istringstream(dato) >> ID;
                lectura >> dato;
            }
            if(dato!="*"){
                istringstream(dato) >> n1;
                estrCombo.id = n1;

                lectura >> dato;
                istringstream(dato) >> n2;
                estrCombo.cantidad = n2;

                contenedor[ID].push_back(estrCombo);
            }
        }
        cout <<"---------------------------------" <<endl<<endl;
        for(auto par=begin(contenedor); par!=end(contenedor); par++){
            cout <<"Combo: "<< par->first <<endl;
            cout <<"Id:          Cantidad:" <<endl;
            for(auto emp=begin(par->second);emp!=end(par->second); emp++){
             cout<< emp->id<<"                 "<<emp->cantidad<<endl<<endl;
            }
            cout <<"---------------------------------" <<endl<<endl;
        }
    } catch (char excepcion) {
        cout << "Error"<<endl;
    }
}










