#include <iostream>
#include <fstream> //eof //Manejor de archivos
#include <string>
#include <exception>
#include <stdlib.h> //cls // Limpiar consola
#include <map>
#include <vector>
#include <sstream>
#include <ctime> //obtener fecha actual


using namespace std;

//Estructuras Publicas

//Caracteristicas de cada producto del inventario
struct Producto{
    //campos
    string nombre;
    int cantidad;
    int costo;
};
//Caracteristicas de cada combo perteneciente en el inventario
struct Combo{
    //campos
    int id;
    int cantidad;
};

void Panel1();//menu Principal
void Panel2();//Menu Admin
void Panel3();//Menu Cliente
bool IniciarSesion();//Admin
bool IniciarSesionC();//Cliente
void CrearUsuario();//Admin
void CrearUsuarioC();//Cliente
map<int,Producto> lecturaDeInventario();//lectura de inventario.txt
map<int,Producto> AgregarActualizar(map<int,Producto> Inventario);//actualizar/escritura inventario.txt
void ImprimirInventario(map<int,Producto> Inventario);//por consola
void ImprimirCombos(map<int,vector<Combo>> CombosInv);//ara PRUEBAS, sin detalles por consola
map<int,vector<Combo>> ImprimirCombos2(map<int,vector<Combo>> CombosInv,map<int,Producto> Inventario); //imprime con detalles todos los combos
map<int,vector<Combo>> crearCombos(map<int,vector<Combo>> CombosInv,map<int,Producto> Inventario); //añade un nuevo combo a combos.txt
void GuardarCombos(map<int,vector<Combo>> CombosInv);//Se actualiza el inventario de combos.txt
void CompraDeCombos(map<int,vector<Combo>> CombosInv,map<int,Producto> Inventario);//Transaccion
string obtenerFecha();

int main()
{
    int admOuser=1, opcion, valorClient;
    bool perfilAdm=false, perfilClient=false;

    //Mapa de estructuras, de  Inventario
    map<int,Producto> Inventario; //Vacio

    //Mapa de Vector de estructuras, de  Combos
    map<int,vector<Combo>> CombosInv;//vacio

    //Menu principal
    while (admOuser > 0) {
        system("cls");
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
                    Inventario = lecturaDeInventario();//Visualizamos el inventario
                    ImprimirInventario(Inventario);
                    CombosInv = ImprimirCombos2(CombosInv,Inventario); //llenar mapa de combos
                    CombosInv = crearCombos(CombosInv,Inventario);
                    GuardarCombos(CombosInv);

                }else if(opcion==4){

                }else if(opcion==5){
                    Inventario = lecturaDeInventario();//Visualizamos el inventario
                    CombosInv = ImprimirCombos2(CombosInv,Inventario);//Visualizamos los Combos
                }else if(opcion==6){
                    perfilAdm=false;
                }else{
                    cout << "Ingresa un valor valido";
                }
            }

        }else if(admOuser==2) {
            //Menu de cliente

            system("cls");
            //Ingresaremos datos para compararlos con el txt
            cout<<"1. Crear Usuario Cliente"<<endl;
            cout<<"2. Iniciar sesion Cliente"<<endl;
            cin >> valorClient;
            if(valorClient==1){
                CrearUsuarioC();
            }else {
                perfilClient = IniciarSesionC();
            }

            while(perfilClient==true){
                Panel3();
                cin >> opcion;
                if(opcion==1){
                    Inventario = lecturaDeInventario();//Visualizamos el inventario
                    CombosInv = ImprimirCombos2(CombosInv,Inventario); //visualizar combos
                    CompraDeCombos(CombosInv,Inventario);
                }else if(opcion==2){
                    perfilAdm=false;

                }else{
                    cout << "Ingresa un valor valido";
                }
            }
        }
    }

    return 0;
}
void Panel1(){
    cout << "       Bienvenidos a Cinema Jupazago:" << endl;
    cout << "Inicia sesion" << endl;
    cout << "-------------------------------------" << endl;
    cout << "1: Administrador" << endl;
    cout << "2: Usuario/Cliente" << endl<<endl;
    cout << "Eleccion: ";
}
void Panel2(){
    cout << "-----------------------------------" << endl;
    cout << "1: Crear Otra cuenta Administracion" << endl;
    cout << "2: Agregar/Actualizar producto Inventario" << endl;
    cout << "3: Crear combos" << endl;
    cout << "4: Generar Reporte de Ventas" << endl;
    cout << "5: Ver Inventario y Combos"<<endl<<endl;
    cout << "6: Cerrar Sesion"<<endl<<endl;
    cout << "Eleccion: ";
}
void Panel3(){
    cout << "----------------------------------" << endl;
    cout << "1: Haz tu compra" << endl;
    cout << "2: Cerrar Sesion"<<endl<<endl;
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
            cout << "   Hola administrador"<<endl;
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
bool IniciarSesionC(){
    /*  Mediante archivo txt consultaremos si los datos suministrados
     *  son correctos para el inicio de sesion
     */

    string userBD, passBD, user, pass, texto;

    ifstream lectAdmin;

    try {
        lectAdmin.open("cliente.txt", ios::in); //abro archivo para su lectura
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
            cout << "   Hola Cliente"<<endl;
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
void CrearUsuarioC(){
    ifstream lectAdmin;
    try {
        string usuario, contrasenia, texto;
        bool quedarse=false;
        int tcontrasenia;
        lectAdmin.open("cliente.txt", ios::in); //abro archivo para su lectura
        if(!lectAdmin.is_open())
            throw '1'; //Capturamos excepcion si hubo errores

        while(!lectAdmin.eof()){ //mientras no sea el final del archivo
            //Obtenemos los datos de la base de datos del unico admin
            getline(lectAdmin,texto);
        }
        lectAdmin.close();

        system("cls");
        cout << endl<<"Tenga en Cuenta"<<endl;
        cout << "*La clave debe tene unicamente 4 digitos"<<endl;

        cout<<endl<<"Ingrese el usuario a registrar: ";
        cin >> usuario;
        if (texto.find(usuario) != string::npos) {
            //Si se encuentra el usuario capturar la excepcion
            throw '2';

        } else{
            //Si no existe el usuario continuamos con el proceso

        }

        while(quedarse==false){
            cout<<endl<<"Ingrese la clave numerica: ";
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
        escritura.open("cliente.txt",ios::app);
        escritura << datos;
        escritura.close();
    } catch (char excepcion) {
        if(excepcion=='1'){
            cout << "Error de lectura de archivo cliente.txt"<<endl;
        }if(excepcion=='2'){
            cout << "Usuario ya existe con ese nombre"<<endl;
        }
    }
}
void CrearUsuario(){
    ifstream lect;
    try {
        string usuario, contrasenia, texto;
        bool quedarse=false;
        int tcontrasenia;
        lect.open("cliente.txt", ios::in); //abro archivo para su lectura
        if(!lect.is_open())
            throw '1'; //Capturamos excepcion si hubo errores

        while(!lect.eof()){ //mientras no sea el final del archivo
            //Obtenemos los datos de la base de datos del unico admin
            getline(lect,texto);
        }
        lect.close();

        system("cls");
        cout << endl<<"Tenga en Cuenta"<<endl;
        cout << "*La clave debe tene unicamente 4 digitos"<<endl;
        cout<<endl<<"Ingrese el usuario a registrar: ";
        cin >> usuario;
        if (texto.find(usuario) != string::npos) {
            //Si se encuentra el usuario capturar la excepcion
            throw '2';
        } else{
            //Si no existe el usuario continuamos con el proceso
        }
        while(quedarse==false){
            cout<<endl<<"Ingrese la clave numerica: ";
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
        escritura.open("cliente.txt",ios::app);
        escritura << datos;
        escritura.close();
    } catch (char excepcion) {
        if(excepcion=='1'){
            cout << "Error de lectura de archivo cliente.txt"<<endl;
        }
        if(excepcion=='2'){
            cout << "Usuario ya existe con ese nombre"<<endl;
        }
    }
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

    //ImprimirInventario(Inventario);
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
void ImprimirCombos(map<int,vector<Combo>> CombosInv){
    cout <<endl<<endl<< "Lista Completa de Combos"<<endl;
    cout << "-----------------------------"<<endl;
    for(auto par=begin(CombosInv); par!=end(CombosInv); par++){
        cout <<"Combo: "<< par->first <<endl;
        cout <<"Id:    Cantidad:" <<endl;
        for(auto emp=begin(par->second);emp!=end(par->second); emp++){
         cout<< emp->id<<"     "<<emp->cantidad<<endl<<endl;
        }
        cout <<"---------------------------------" <<endl<<endl;
    }
}
map<int,vector<Combo>> ImprimirCombos2(map<int,vector<Combo>> contenedor,map<int,Producto> Inventario){
    string dato;
    int ID, n1, n2, precioCombo;

    try {
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
        precioCombo = 0;
        cout <<"--------------------------------------" <<endl;
        for(auto par=begin(contenedor); par!=end(contenedor); par++){
            cout <<"Combo: #"<< par->first <<endl;
            cout <<"Id:  Cantidad:    Nombre:" <<endl;
            for(auto emp=begin(par->second);emp!=end(par->second); emp++){
                for(auto par=begin(Inventario); par!=end(Inventario); par++){
                    if(emp->id==par->first){
                        precioCombo=precioCombo+(par->second.costo * emp->cantidad);
                        cout<< emp->id<<"    X"<<emp->cantidad<<"            "<<par->second.nombre<<endl;
                    }
                }
            }
            precioCombo*=0.9;
            cout <<endl<< "Precio del combo:  "<<precioCombo;
            cout <<endl<<"--------------------------------------" <<endl<<endl;
            precioCombo = 0;
        }
        system("pause");
        return contenedor;
    } catch (char excepcion) {
        cout << "Error"<<endl;
        return contenedor;
    }
}

map<int,vector<Combo>> crearCombos(map<int,vector<Combo>> CombosInv,map<int,Producto> Inventario){
    system("cls");
    int valor=1, cantid, id, contador=1, claveInv;
    bool encontrado=false;
    Producto produto;
    Combo combo;
    //  Verificamos cuantos combos tenemos en nuestro inventario de combos
    //  para asigar asignar el ID al nuevo combo
    for(auto par=begin(CombosInv); par!=end(CombosInv); par++){
        contador++;
    }

    try {
        cout<<"Ingresa 0 para no agregar productos"<<endl<<endl;
        cout<<"Cualquier otro valor, continuar agregando"<<endl;
        cin >> valor;
        while(valor>0){
            encontrado=false;

            cout<<"Que producto deseas agregar al nuevo combo, ingresa su ID"<<endl;
            cin >> id;
            cout<<"cantidad: ";
            cin >> cantid;

            for(auto par=begin(Inventario); par!=end(Inventario); par++){
                claveInv = par->first;
                if(id==claveInv){
                    combo.id=id;
                    combo.cantidad=cantid;
                    CombosInv[contador].push_back(combo);
                    encontrado=true;
                }
            }
            if(encontrado==false){
                cout << "No se encontró en producto en el inventario"<<endl<<endl;
            }
            cout<<"0. Finalizar"<<endl;
            cout<<"Cualquier otro valor, continuar agregando"<<endl;
            cin >> valor;
        }
        cout <<"---------------------------------" <<endl;
        //ImprimirCombos(CombosInv);
        return CombosInv;
    } catch (char exepcion) {
        if(exepcion=='1'){
            cout << "producto no encontrado en el inventario"<<endl;
        }
        return CombosInv;
    }
}
void GuardarCombos(map<int,vector<Combo>> CombosInv){
    ofstream escritura;
    escritura.open("combos.txt",ios::out);
    for(auto par=begin(CombosInv); par!=end(CombosInv); par++){
        escritura << "* ";
        escritura << par->first;
        escritura << "\n";
        for(auto emp=begin(par->second);emp!=end(par->second); emp++){
            escritura << emp->id;
            escritura <<" ";
            escritura <<emp->cantidad;
            escritura <<"\n";
        }
    }
    escritura.close();
}
void CompraDeCombos(map<int,vector<Combo>> CombosInv,map<int,Producto> Inventario){
    int NdeCombo, precioCombo=0;
    bool compra=false, verificacion=false;
    string Sino;

    cout << "Ingresa el # de combo que deseas llevar"<<endl;
    cin >> NdeCombo;

    while (compra==false) {
        for(auto par=begin(CombosInv); par!=end(CombosInv); par++){
            if(NdeCombo==par->first){
                cout <<"Combo: #"<< par->first <<endl;
                for(auto emp=begin(par->second);emp!=end(par->second); emp++){
                    for(auto par=begin(Inventario); par!=end(Inventario); par++){
                        if(emp->id==par->first){
                            precioCombo=precioCombo+(par->second.costo * emp->cantidad);
                        }
                    }
                }
                precioCombo*=0.9;
                cout <<endl<< "Precio del combo:  "<<precioCombo;
                cout <<endl<<"--------------------------------------" <<endl<<endl;
            }
        }
        while (verificacion==false) {
            cout << "Es correcta tu eleccion?"<<endl;
            cout << "si / no"<<endl;
            cin >> Sino;
            if(Sino=="si" || Sino=="no"){
                verificacion=true;
            }
        }
        if(Sino=="si"){
            compra=true;
        }
    }
    cout << obtenerFecha()<<endl;


}


string obtenerFecha(){
    time_t t = time(NULL);
        tm* tiempo = localtime(&t);

    stringstream ss_anio;
    ss_anio << tiempo->tm_year+1900;
    string anio = ss_anio.str();

    stringstream ss_mes;
    ss_mes << tiempo->tm_mon+1;
    string mes = ss_mes.str();
    if(atoi(mes.c_str()) < 10)
        mes = "0"+mes;

    stringstream ss_dia;
    ss_dia << tiempo->tm_mday;
    string dia = ss_dia.str();
    if(atoi(dia.c_str()) < 10)
        dia = "0"+dia;

    string Fecha = anio+" "+mes+" "+dia;

    return Fecha;
}



