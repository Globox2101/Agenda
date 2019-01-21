#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

class Contacto
{
    string nombre="";
    string numero="";
    
    public:
        void setNombre(string n)
        {
            nombre = n;
        };

        void setNumero(string n)
        {
            numero = n;
        };

        string getNombre()
        {
            return nombre;
        };

        string getNumero()
        {
            return numero;
        };

        ~Contacto(){};

        Contacto(){}
        Contacto(string nombre, string numero): nombre(nombre), numero(numero){}
};

typedef struct Nodo
{
    Contacto contacto;
    struct Nodo* siguiente;
}Nodo;

typedef struct ListaContactos
{
    Nodo* primero;
    int cantidad = 0;
}ListaContactos;

void destruirNodo(Nodo* n)
{
    free(n);
}

Nodo* crearNodo(Contacto* c)
{
    Nodo* n = (Nodo*) malloc(sizeof(Nodo));
    n->contacto.setNombre(c->getNombre());
    n->contacto.setNumero(c->getNumero());
    return n;
}

ListaContactos lista;
ListaContactos* listaContactos = &lista;

void ListaContactos_agregar(Nodo* c)
{
    Nodo* puntero = listaContactos->primero;
    if(listaContactos->primero == NULL)
    {
        listaContactos->primero = c;
        listaContactos->cantidad++;
    }
    else
    {
        while(puntero->siguiente != NULL)
        {
            puntero = puntero->siguiente;
        }
        puntero->siguiente = c;
        listaContactos->cantidad++;
    }
}

void crear_agregarContacto(string nombre, string numero)
{
    Contacto c(nombre, numero); 
    Nodo* n = crearNodo(&c);
    ListaContactos_agregar(n);
}

Contacto* ListaContactos_buscar(int posicion)
{
    posicion--;
    int contador=0;
    Nodo* puntero = listaContactos->primero;
    if(puntero == NULL)
    {
        return NULL;
    }
    else
    {
        while(contador<posicion && puntero->siguiente)
        {
            puntero = puntero->siguiente;
            contador++;
        }
        if(contador!=posicion)
        {
            return NULL;
        }
        else
        {
            return &puntero->contacto;
        }
    }
}

bool ListaContactos_eliminar(int n)
{
    n--;
    if(listaContactos->primero)
    {
        if(n==0)
        {
            Nodo* eliminado = listaContactos->primero;
            listaContactos->primero = listaContactos->primero->siguiente;
            destruirNodo(eliminado);
            listaContactos->cantidad--;
        }
        else if(n < listaContactos -> cantidad)
        {
            Nodo* puntero = listaContactos->primero;
            int posicion = 0;
            while(posicion < (n-1))
            {
                puntero=puntero->siguiente;
                posicion++;
            }
            Nodo* eliminado = puntero->siguiente;
            puntero->siguiente = eliminado->siguiente;
            destruirNodo(eliminado);
            listaContactos->cantidad--;
        }
        else
        {
            printf("El valor indicado no es valido\n\n");
            return false;
        }
    }
    return true;
}

class Archivo
{
    string nombre;
    int contactos;
    ListaContactos* lista;
    public:

        void sincronizarArchivo_con_ListaContactos()
        {
            Nodo* puntero = lista->primero;
            remove(getNombre().c_str());
            ofstream ficheroSalida(getNombre());
            if(puntero == NULL)
            {
                printf("En la lista no hay nada\n");
            }
            else
            {
                while(puntero->siguiente)
                {
                    ficheroSalida << puntero->contacto.getNombre() << "<>" << puntero->contacto.getNumero() << endl;                   
                    puntero = puntero->siguiente;
                }
                ficheroSalida << puntero->contacto.getNombre() << "<>" << puntero->contacto.getNumero() << endl;
            }
            ficheroSalida.close();
        }

        void sincronizarListaContactos_con_archivo()
        {

            for(int i=0;i<lista->cantidad;i++)
            {
                ListaContactos_eliminar(i);
            }

            int cantidad = 0;
            ifstream ficheroEntrada(getNombre());
            if(ficheroEntrada)
            {
                string linea;
                while(!ficheroEntrada.eof())
                {
                    getline(ficheroEntrada, linea);
                    if(linea != "")
                    {
                        string nombre = linea.substr(0, linea.find("<>"));
                        string numero = linea.substr(linea.find_last_of("<>") +1, linea.length());
                        crear_agregarContacto(nombre, numero);
                        cantidad++;
                    }        
                }
                ficheroEntrada.close(); 
                setContactos(cantidad);
            }
            else
            {
                ofstream creandoFichero(getNombre());
                creandoFichero.close();
            }
            
        }

        void setContactos(int n)
        {
            contactos = n;
        }

        int getContactos()
        {
            return contactos;
        }

        void setNombre(string n)
        {
            nombre = n;
        }

        string getNombre()
        {
            return nombre;
        }

        Archivo(){};
        ~Archivo(){};

        Archivo(string nombre, ListaContactos* lista): nombre(nombre), lista(lista){}
};

void menu();

bool verContactos();
void agregarContacto();
void editarContacto();
void eliminarContacto();

bool verContactos()
{
    system("clear");
    Nodo* puntero = listaContactos->primero;
    int contador = 1;
    if(puntero==NULL)
    {
        printf("No hay ningun contacto en la agenda\n");
        return false;
    }
    else if(puntero->siguiente == NULL)
    {
        printf("1. Nombre: %s Numero: %s \n\n", puntero->contacto.getNombre().c_str(), puntero->contacto.getNumero().c_str());
    }
    else
    {
        while(puntero->siguiente)
        {
            printf("%i. Nombre: %s Numero: %s \n", contador ,puntero->contacto.getNombre().c_str(), puntero->contacto.getNumero().c_str());
            contador++;
            puntero = puntero->siguiente;
        }
        printf("%i. Nombre: %s Numero: %s \n", contador ,puntero->contacto.getNombre().c_str(), puntero->contacto.getNumero().c_str());
    }
    return true;
}

void agregarContacto(Archivo* archivo)
{
    system("clear");
    string nombre, numero;
    printf("Inserte el nombre del contacto: ");
    getline(cin, nombre);
    printf("Inserte el numero telefonico del contacto: ");
    getline(cin, numero);
    crear_agregarContacto(nombre, numero);
    archivo->sincronizarArchivo_con_ListaContactos();
}

void editarContacto(Archivo* archivo)
{
    if(verContactos())
    {
        string numero = "", nuevoNombre, nuevoNumero;
        cout << "Que elemento desea Editar? ";
        getline(cin, numero);
        int n = atoi(numero.c_str());
        Contacto* puntero = ListaContactos_buscar(n);
        if(puntero!=NULL)
        {
            printf("Inserte el nuevo nombre: ");
            getline(cin, nuevoNombre);
            printf("Inserte el nuevo numero: ");
            getline(cin, nuevoNumero);
            puntero->setNombre(nuevoNombre);
            puntero->setNumero(nuevoNumero);
            archivo->sincronizarArchivo_con_ListaContactos();
            system("clear");
            cout << "Contacto editado satisfactoriamente :)\n\n";
        }
        else
        {
            printf("El valor indicado no es valido\n\n");
        }
        
    }
}

void eliminarContacto(Archivo* archivo)
{
    if(verContactos())
    {
        string numero = "";
        cout << "Que elemento desea eliminar? ";
        getline(cin, numero);
        int n = atoi(numero.c_str());
        if(ListaContactos_eliminar(n))
        {
            archivo->sincronizarArchivo_con_ListaContactos();
        }
    }
}

void menu(Archivo* archivo)
{
    printf("Bienvenido a tu agenda personal \n\n 1. Ver Contactos\n 2. Agregar Contactos\n 3. Editar Contactos\n 4. Eliminar Contactos\n 5. Salir\n Que desea hacer? ");
    string opcion = "";
    getline(cin, opcion);
    if(opcion == "1")
    {
        system("clear");
        verContactos();
    }

    else if(opcion == "2")
    {
        system("clear");
        agregarContacto(archivo);
    }

    else if(opcion=="3")
    {
        system("clear");
        editarContacto(archivo);
    }

    else if(opcion=="4")
    {
        system("clear");
        eliminarContacto(archivo);
    }

    else if(opcion=="5")
    {
        system("clear");
        exit(EXIT_SUCCESS);
        free(archivo);
    }

    else
    {
        system("clear");
        printf("Ingrese una opcion valida\n\n");
    }
}

int main()
{
    Archivo* punteroArchivo = new Archivo("contactos.txt", listaContactos);
    punteroArchivo->sincronizarListaContactos_con_archivo();
    while(true)
    {
        menu(punteroArchivo);
    }
}
