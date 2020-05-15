#include <iostream>
#include <fstream> //eof //Manejor de archivos
#include <string>
#include <exception>
#include <stdlib.h> //cls // Limpiar consola
#include <map>
#include <vector>
#include <sstream>  //pasar un string a un int
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
bool IniciarSesionC(string *cliente);//Cliente
void CrearUsuario();//Admin
void CrearUsuarioC();//Cliente
map<int,Producto> lecturaDeInventario();//lectura de inventario.txt
map<int,Producto> AgregarActualizar(map<int,Producto> Inventario);//actualizar/escritura inventario.txt
void ImprimirInventario(map<int,Producto> Inventario);//por consola
void ImprimirCombos(map<int,vector<Combo>> CombosInv);//ara PRUEBAS, sin detalles por consola
map<int,vector<Combo>> ImprimirCombos2(map<int,vector<Combo>> CombosInv,map<int,Producto> Inventario); //imprime con detalles todos los combos
map<int,vector<Combo>> crearCombos(map<int,vector<Combo>> CombosInv,map<int,Producto> Inventario); //añade un nuevo combo a combos.txt
void GuardarCombos(map<int,vector<Combo>> CombosInv);//Se actualiza el inventario de combos.txt
void CompraDeCombos(map<int,vector<Combo>> CombosInv,map<int,Producto> Inventario,string cliente);//Transaccion completa
string obtenerFecha();//obtener fecha actual
void funcion_cine1(); // verifica la posicion y la sala del cine
bool pago(int precioCombo);// verificacion de dinero
void imprimirFacturas();//impresion en consola de las ventas

int main()
{
    //variables para movilizacion entre menus
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
                system("cls");
                Panel2();
                cin >> opcion;
                if(opcion==1){
                    //funcion para la creacion de un nuevo usuario admin
                    CrearUsuario();
                }else if(opcion==2){
                    map<int,Producto> Inventario;
                    Inventario = lecturaDeInventario();//Llenamos el Inventario del txt
                    Inventario = AgregarActualizar(Inventario);//agregar nuevo producto o actualizar datos

                }else if(opcion==3){
                    map<int,Producto> Inventario;
                    map<int,vector<Combo>> CombosInv;//vacio
                    Inventario = lecturaDeInventario();//Visualizamos el inventario
                    ImprimirInventario(Inventario);
                    CombosInv = ImprimirCombos2(CombosInv,Inventario); //llenar mapa de combos
                    CombosInv = crearCombos(CombosInv,Inventario); //se crean los combos con los datos introducidos
                    GuardarCombos(CombosInv);//guardar los datos en el txt


                }else if(opcion==4){
                    imprimirFacturas();
                }else if(opcion==5){
                    map<int,Producto> Inventario;//vacio
                    map<int,vector<Combo>> CombosInv;//vacio
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
            string cliente;

            system("cls");
            //Ingresaremos datos para compararlos con el txt
            cout<<"1. Crear Usuario Cliente"<<endl;
            cout<<"2. Iniciar sesion Cliente"<<endl;
            cin >> valorClient;
            if(valorClient==1){
                CrearUsuarioC();
            }else {
                perfilClient = IniciarSesionC(&cliente);
            }

            while(perfilClient==true){
                Panel3();
                cin >> opcion;
                if(opcion==1){
                    map<int,Producto> Inventario;//vacio
                    map<int,vector<Combo>> CombosInv;//vacio
                    Inventario = lecturaDeInventario();//Visualizamos el inventario
                    CombosInv = ImprimirCombos2(CombosInv,Inventario); //visualizar combos
                    CompraDeCombos(CombosInv,Inventario, cliente);//transaccion, todo el proceso de compra
                }else if(opcion==2){
                    perfilClient=false;

                }else{
                    cout << "Ingresa un valor valido";
                }
            }
        }
    }

    return 0;
}
//menu genereal
void Panel1(){
    cout << "       Bienvenidos a Cinema Jupazago:" << endl;
    cout << "Inicia sesion" << endl;
    cout << "-------------------------------------" << endl;
    cout << "1: Administrador" << endl;
    cout << "2: Usuario/Cliente" << endl<<endl;
    cout << "Eleccion: ";
}
//menu de admin
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
//menu de cliente
void Panel3(){
    cout << "----------------------------------" << endl;
    cout << "1: Haz tu compra" << endl;
    cout << "2: Cerrar Sesion"<<endl<<endl;
    cout << "Eleccion: ";
}

//funcion para el proceso de inicio de sesion de admin

bool IniciarSesion(){
    /*  Mediante archivo txt consultaremos si los datos suministrados
     *  son correctos para el inicio de sesion
     */

    string userBD, passBD, user, pass, texto;

    ifstream lectAdmin;

    try {
        //leemos el archivo txt y lo almacenamos en la variable texto
        lectAdmin.open("administrador.txt", ios::in); //abro archivo para su lectura
        if(!lectAdmin.is_open())
            throw '1'; //Capturamos excepcion si hubo errores

        while(!lectAdmin.eof()){ //mientras no sea el final del archivo
            //Obtenemos los datos de la base de datos del unico admin
            getline(lectAdmin,texto);
        }
        lectAdmin.close();

        //cout << texto<<endl<<endl;
        cout << "Ingresar Usuario: ";
        cin >> user;
        cout << "Contrasenia: ";
        cin >> pass;

        int tamano_user= user.size();
        //verificacion de existencia del usuario
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
            //si es un exito
            system("cls");
            cout << "   Hola administrador"<<endl;
            return true;
        }else{
            //si falla captura la excepcion
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
bool IniciarSesionC(string *cliente){
    /*  Mediante archivo txt consultaremos si los datos suministrados
     *  son correctos para el inicio de sesion
     */

    string userBD, passBD, user, pass, texto;

    ifstream lectAdmin;

    try {
        //lectura de la base de datos de cliente del txt y se almacena en la variable texto
        lectAdmin.open("cliente.txt", ios::in); //abro archivo para su lectura
        if(!lectAdmin.is_open())
            throw '1'; //Capturamos excepcion si hubo errores

        while(!lectAdmin.eof()){ //mientras no sea el final del archivo
            //Obtenemos los datos de la base de datos del unico admin
            getline(lectAdmin,texto);
        }
        lectAdmin.close();

        //cout << texto<<endl<<endl;
        cout << "Ingresar Usuario: ";
        cin >> user;
        cout << "Contrasenia: ";
        cin >> pass;

        int tamano_user= user.size();
        //se verifica la existencia de usuario
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
            //exito
            system("cls");
            *cliente=user;
            cout << "   Hola Cliente"<<endl;
            return true;
        }else{
            //falla
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

        //lectura de la base de datos txt de clientes
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
        //ingreso de una contrasena valida
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
        //ingreso de una contrasena valida
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
        escritura.open("administrador.txt",ios::app);
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

    Producto producto;

    lectura.open("inventario.txt", ios::in);
    int i=1;
    while (!lectura.eof()) {
        //vamos almacenando los valores en el objeto
        //anadiendolo a la estructura y mapa
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
        cout << "Actualizar producto ingresa su ID, o crear producto ingresa el numero 0: ";
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

        //registro de un  nuevo producto
        if(encontrado!=true){
            system("cls");
            cout << "     Registrar Producto"<<endl;
            cout << "-----------------------------"<<endl;
            string nombre;
            cout << "Ingresa Nombre producto: ";
            cin >> nombre;
            cout <<endl<< "Ingresa Cantidad producto: ";
            cin >> ncantidad;
            cout <<endl<< "Ingresa el Costo: ";
            cin >> ncosto;

            //almacenado en el objeto
            producto.nombre=nombre;
            producto.cantidad=ncantidad;
            producto.costo=ncosto;

            int i=0;
            for(auto par=begin(Inventario); par!=end(Inventario); par++){
                //Cantidad de iteraciones por elemento de inventario
                i++;
            }


            Inventario[i]=producto;
            //actualizar la base de datos con los nuevos valores
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
        //lectura de los combos para su almacenamiento

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
        //se imprimen los valores del contenedor
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
            //los almacenamos en el objeto
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

    //almacenar combos en el txt
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
void CompraDeCombos(map<int,vector<Combo>> CombosInv,map<int,Producto> Inventario,string cliente){
    int NdeCombo, precioCombo=0;
    bool compra=false, verificacion=false;
    string Sino;

    cout << "Ingresa el # de combo que deseas llevar"<<endl;
    cin >> NdeCombo;
    //vericiacion de la existencia del combo y de su precio respectivamente
    while (compra==false) {
        verificacion=false;
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
        if(Sino=="no"){
            precioCombo=0;
            cout << "Ingresa el # de combo que deseas llevar"<<endl;
            cin >> NdeCombo;
        }
        if(Sino=="si"){
            compra=true;
        }
    }
    /*  Consultar si hay productos disponibles en el inventario
    *   si hay disponibilidad, actualiza el inventario
    */
    bool disponibilidad[2]= {false,false};
    //par ID combos
    for(auto par=begin(CombosInv); par!=end(CombosInv); par++){
        //emp Vector
        if(NdeCombo==par->first){
            for(auto emp=begin(par->second);emp!=end(par->second); emp++){
                //siguiente for de inventario

                for(auto par2=begin(Inventario); par2!=end(Inventario); par2++){
                    //par2->second.cantidad cantidad del inventario
                    if(emp->id == par2->first){
                        //emp->id------emp->cantidad------par->second.nombre<<endl;
                        if((par2->second.cantidad)-2 >=0){
                            //actualizacion de valores en los obejtos
                            par2->second.cantidad -= emp->cantidad;
                            disponibilidad[0]=true;
                        }else{
                            disponibilidad[0]=false;
                            cout << "Cantidad insuficiente en el inventario: "<< par2->second.nombre<<endl;
                        }
                    }
                }
            }
        }
    }
    if(disponibilidad[0]==true){
        disponibilidad[1]=pago(precioCombo);
    }

    if(disponibilidad[0]==true && disponibilidad[1]==true){
        //actualizar inventario en su base de datos
        ofstream escritura;
        escritura.open("inventario.txt", ios::out);
        for(auto par=begin(Inventario); par!=end(Inventario); par++){
            escritura << par->first;
            escritura << " ";
            escritura << par->second.nombre;
            escritura << " ";
            escritura << par->second.cantidad;
            escritura << " ";
            escritura << par->second.costo;
            escritura << "\n";

        }
        escritura.close();

        //obtenerFecha();
        //cliente
        //precioCombo



        //se guarda el reporte de transaccion en la base de datos de ventas
        string fecha = obtenerFecha();
        ofstream Factura;
        Factura.open("facturas.txt",ios::app);
        Factura <<"\n"<<fecha<<" "<<precioCombo<<" "<<cliente;
        Factura.close();

        //Variables para el cine
        string cine[15][20];
        int decision=1;
        string auxiliar_cine= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O'};
        char fila_cine;
        int columna_cine;

        //Genero salas de cine
        system("cls");
        for(int i=0; i<15;i++){
            for(int j=0;j<20;j++){
                cine[i][j]="-";
            }
        }
        //preguntar
        system("pause");
        int sala_cine;
        while(decision==1){

            cout << "1. Danos tu ubicacion en la sala de cine" << endl << endl;
            cout<<"------------------------------"<<endl<<endl;


            //Imprimir Sala de cine actual

            funcion_cine1();
            cout << endl;
            for(int i=0; i<15;i++){
                cout << auxiliar_cine[i] << "    ";
                for(int j=0;j<20;j++){
                    cout << cine[i][j] << "   ";
                }
                cout << endl;
            }

            //Reservar
            cout << "Ingresa Numero de sala" << endl;
            cin >> sala_cine;
            cout << "Ingresa Fila a seleccionar: letra A-O" << endl;
            cin >> fila_cine;
            cout << "Ingresa Columna a seleccionar: numero 1-20" << endl;
            cin >> columna_cine;
            //fila_cine-65 FILAS
            //columna_cine-1COLUMNAS
            cine[fila_cine-65][columna_cine-1]="+";
            decision=0;

        }

        cout << "Exito de compra, se generara su factura"<<endl;
        system("pause");
        system("cls");

        cout<<"|---------------------|"<<endl;
        cout<<"|      Su factura     |"<<endl;
        cout<<"|---------------------|"<<endl;
        cout<<"| Valor:       "<<precioCombo<<endl;
        cout<<"|"<<endl;
        cout<<"| A nombre de: "<<cliente<<endl;
        cout<<"|"<<endl;
        cout<<"| Sala:        "<<sala_cine<<endl;
        cout<<"|"<<endl;
        cout<<"| Puesto:    "<<fila_cine<<columna_cine<<endl;
        cout<<"|---------------------|"<<endl;
    }
}


string obtenerFecha(){
    time_t t = time(NULL);
    tm* tiempo = localtime(&t);

    //Obtenemos los valores de anio
    stringstream ss_anio;
    ss_anio << tiempo->tm_year+1900;
    string anio = ss_anio.str();

    //Obtenemos los valores del mes
    stringstream ss_mes;
    ss_mes << tiempo->tm_mon+1;
    string mes = ss_mes.str();
    if(atoi(mes.c_str()) < 10)
        mes = "0"+mes;

    //Obtenemos los valores del dia
    stringstream ss_dia;
    ss_dia << tiempo->tm_mday;
    string dia = ss_dia.str();
    if(atoi(dia.c_str()) < 10)
        dia = "0"+dia;

    string Fecha = anio+" "+mes+" "+dia;

    return Fecha;
}
void funcion_cine1(){
    //Imprime puestos
    cout << "Total de puestos"<<endl;
    cout << "     ";
    cout << 1;
    for(int i=1; i<10;i++){
        cout << "   ";
        cout << (i+1);
    }
    for(int i=1; i<11;i++){
        cout << "  ";
        cout << (i+10);
    }
}

bool pago(int precioCombo){
    bool pasa=false;
    int dinero, billetes_50000,billetes_20000,billetes_10000, billetes_5000, billetes_2000, billetes_1000;
    int residuo_50000, residuo_20000, residuo_10000, residuo_5000, residuo_2000, residuo_1000;
    int moneda_500, moneda_200, moneda_100, moneda_50;
    int residuo_500, residuo_200, residuo_100, residuo_50;

    while(pasa==false){
        cout <<"Ingrese la cantidad de dinero con la que pagaras"<<endl;
        cout <<"0. Cancelar trasaccion"<<endl;
        cin >>dinero;
        if(dinero>precioCombo){

            //proceso para devolucion de dinero en cantidad de dinero
            //y en cantidad de billetes y monedas a devolver de la manera
            //mas eficiente
            dinero-=precioCombo;
            precioCombo =dinero;

            billetes_50000 = dinero/50000; //Billetes de 50
            residuo_50000 = dinero%50000; //sobrante

            billetes_20000 = residuo_50000/20000;
            residuo_20000 = residuo_50000%20000;

            billetes_10000 = residuo_20000/10000;
            residuo_10000 = residuo_20000%10000;

            billetes_5000 = residuo_10000/5000;
            residuo_5000 = residuo_10000%5000;

            billetes_2000 = residuo_5000/2000;
            residuo_2000 = residuo_5000%2000;

            billetes_1000 = residuo_2000/1000;
            residuo_1000 = residuo_2000%1000;

            moneda_500 = residuo_1000/500;
            residuo_500 = residuo_1000%500;

            moneda_200 = residuo_500/200;
            residuo_200 = residuo_500%200;

            moneda_100 = residuo_200/100;
            residuo_100 = residuo_200%100;

            moneda_50 = residuo_100/50;
            residuo_50 = residuo_100%50;
            cout << "Su Devuelta es : " <<precioCombo<< "\n";
            cout << "Billetes de 50 mil : " << billetes_50000 << "\n";
            cout << "Billetes de 20 mil : " << billetes_20000 << "\n";
            cout << "Billetes de 10 mil : " << billetes_10000 << "\n";
            cout << "Billetes de 5 mil : " << billetes_5000 << "\n";
            cout << "Billetes de 2 mil : " << billetes_2000 << "\n";
            cout << "Billetes de 1 mil : " << billetes_1000 << "\n";
            cout << "Moneda de 500 : " << moneda_500 << "\n";
            cout << "Moneda de 200 : " << moneda_200 << "\n";
            cout << "Moneda de 100 : " << moneda_100 << "\n";
            cout << "Moneda de 50 : " << moneda_50 << "\n";
            cout << "Residuo : " << residuo_50 << "\n";
            system("pause");
            pasa=true;
            return true;
        }else if(dinero==0){
            return false;
        }else{
            pasa=false;
            cout << "Dinero insuficiente, intentalo de nuevo"<<endl<<endl<<endl;
        }
    }
    return false;
}



void imprimirFacturas(){

    //impresion en consola de la base de datos de ventas
    ifstream Factura;
    string cadena;
    Factura.open("facturas.txt",ios::in);
    cout<<"Fecha:        Valor:     Comprador:"<<endl<<endl;
    while (!Factura.eof()) {
        Factura >> cadena;
        cout << cadena<<" ";
        Factura >> cadena;
        cout << cadena<<" ";
        Factura >> cadena;
        cout << cadena<<"      ";
        Factura >> cadena;
        cout << cadena<<"    ";
        Factura >> cadena;
        cout << cadena<<endl;
    }
    cout << endl<<endl;
    Factura.close();
    system("pause");

}





