#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include<stdlib.h>      // Se importa para poder usar la funcion exit

using namespace std;

// Clase para representar la estructura M
class StructureM {
public:
    StructureM(int size) : size(size) {
        // Implementación de la creación y manipulación de la matriz
    }

    void rotate(int dir) {
        // Implementación de la rotación de la matriz
    }

    int get_value(int row, int col) {
        // Implementación para obtener el valor de una celda
    }

    int get_size() const { return size; }

private:
    int size;
    unique_ptr<int[]> matrix;
};

// Clase para representar la cerradura X
class LockX {
public:
    LockX(const vector<int>& dimensions) {
        for (int size : dimensions) {
            structures.emplace_back(size);
        }
    }


int** matriz (int);
int** rot_mat (int**, int);
int*** cerradura (int, int[]);
int* cambio_crd(int[], int);
int* valores(int[], int***, int, int[]);
bool validar_regla(int[], int***, int, int[]);
bool validar_relacion(int[], int, int***, int, int[]);
int* nueva_cerradura(int[], int);
void ingreso_regla(int, int*);
void entrega_cerradura(int[], int);

int main()
{
    cout << "GENERADOR DE CERRADURAS" << endl << endl;

    int* K;
    int t;

    cout << " Ingrese el tamano de su regla: ";
    cin >> t;
    K = new int[t];

    cout << endl << "Ingrese los valores de su regla. " << endl;

    for(int i=0; i<t; i++){
        cout << " Valor " <<i+1<<": ";
        cin >> K[i];
    }

    cout << endl << "Regla ingresada: K(";

    for(int i=0; i<t; i++)
        if(i == t-1)
            cout << K[i] << ")" << endl;
        else
            cout << K[i] << ",";

    int n = t-1;

    int* X = nueva_cerradura(K,t);

    cout << "Cerradura configurada: X(";
    for(int i=0; i<n;i++)
        if(i == n-1)
            cout << X[i] << ")" << endl;
        else
            cout << X[i] << ",";

    return 0;
}


// Funcion para creacion de matrices

int** matriz (int t){

    // Creamos la matriz con dimensiones txt
    int **M = new int*[t];

    for(int i=0;i<t;i++)
        M[i] = new int[t];

    // Rellenamos la matriz

    int k = 1;

    for(int i=0;i<t;i++)
        for(int j=0;j<t;j++){

            if(i==t/2 & j==t/2) // Si estamos en el centro se coloca un 0
                M[i][j]= 0;
            else{
                M[i][j] = k;
                k++;
            }
        }
    return M;
}

// Funcion para rotar matrices

int** rot_mat (int** M, int dir){

    // Identificamos el tamano de la matriz a rotar
    // Lo podemos hacer conociendo la ubicacion de su centro

    int t=0;
    for(int i=0; M[i][i]!=0;i++)
        t++;
    t = 2*t+1;

    // Creamos la matriz que correspondera a la matriz rotada
    int **MR = new int*[t];

    for(int i=0;i<t;i++)
        MR[i] = new int[t];

    // Cambiamos filas por columnas de acuerdo a la entrada dir

    int pm = t-1;       // maxima posicion de la matriz

    if(dir == 1){
        for(int i=0; i<t; i++)
            for(int j=0; j<t; j++)
                MR[j][i] = M[pm-i][j];
    }
    else if(dir == -1){
        for(int i=0; i<t; i++)
                for(int j=0; j<t; j++)
                    MR[j][i] = M[i][pm-j];
    }
    else
        return M;

    return MR;
}

// Creación de estructuras

int*** cerradura (int n, int t[]){

    int ***c = new int**[n];

    for(int i=0; i<n; i++)
        c[i] = matriz(t[i]);

   return c;
}

// Extraccion de valores

int* valores(int cc[], int*** c, int n, int t[]){

    int* v = new int[n];

    for(int i=0; i<n; i++){

        // Cambio de coordendadas cartesianas a posicion de arreglo
        int x = cc[0];
        int y = cc[1];
        int col = int(t[i]/2) + x;
        int fil = int(t[i]/2) - y;

        int p[] = {fil,col};

        v[i] = c[i][p[0]][p[1]];        // Extraccion de valores
    }

    return v;
}

// Validacion de reglas

bool validate_rule_part(const vector<int>& rule, int index) {
        // Implementación para validar una parte de la regla K
        int row = rule[index];
        int col = rule[index + 1];
        int structure = index / 3;
        int value1 = structures[structure].get_value(row, col);
        int value2 = structures[structure + 1].get_value(rule[index + 2], rule[index + 3]);
        int cmp = rule[index + 4];
        if (cmp == -1 && value1 <= value2) return false;
        if (cmp == 1 && value1 >= value2) return false;
        return true;
    }
// Validacion de relaciones

bool validar_relacion(int cc[], int r, int*** cer, int n, int t[]){

    bool res = true;

    // Valores alineados en dicha casilla
    int* v = new int[n];
    v = valores(cc, cer, n, t);

    // Comparamos cada par de valores adyacentes
    for (int i=0;i<n-1;i++){

        if(r==1 & v[i]>v[i+1])
            res = res*true;
        else if(r==0 & v[i]==v[i+1])
            res = res*true;
        else if(r==-1 & v[i]<v[i+1])
            res = res*true;
        else
            res = res*false;
    }

    return res;
}

bool validate_rule(const vector<int>& rule) {
        // Implementación de la validación de la regla K completa
        for (int i = 0; i < rule.size() - 1; i += 5) {
            if (!validate_rule_part(rule, i)) {
                return false;
            }
        }
        return true;
    }

// Generación de cerradura

int* nueva_cerradura(int K[], int t) {
    int fil = K[0];
    int col = K[1];

    // Instanciación de cerradura
    int n = t - 1;                        // Tamaño de la cerradura
    int* X = new int[n];

    // Se define el tamaño de la primera matriz
    if (fil < 3 && col < 3)
        X[0] = 3;
    else if (fil > col) {
        if (fil % 2 == 0)
            X[0] = fil + 1;
        else
            X[0] = fil;
    }
    else {
        if (col % 2 == 0)
            X[0] = col + 1;
        else
            X[0] = col;
    }

    // Conversión a coordenadas cartesianas
    int x = col - int(X[0] / 2) - 1;
    int y = int(X[0] / 2) - fil + 1;

    int cc[] = { x, y };  // Casillas que se compararán

    // Construcción de cerradura
    int*** cer = new int**[n];

    cer[0] = matriz(X[0]);

    // Comparamos pares de matrices para validar las relaciones de la regla
    int j, u;
    bool cumple;

    for (int i = 0; i < n - 1; i++) {
        int m = 2;                  // Número de matrices que se comparan
        int*** cb = new int**[m];
        cb[0] = cer[i];
        cb[1] = matriz(X[0]);

        int tb[] = { X[i], X[0] };

        int* v = valores(cc, cb, m, tb);

        j = 0;
        u = 0;
        cumple = false;

        while (!cumple) {
            if (j == 1)
                cb[1] = rot_mat(cb[1], 1);
            if (j == 2)
                cb[1] = rot_mat(cb[1], 1);
            if (j == 3)
                cb[1] = rot_mat(cb[1], 1);

            int r = K[i + 2];                             // Relación a validar
            cumple = validar_relacion(cc, r, cb, m, tb);

            if (!cumple)
                j++;

            if (j > 3) {
                tb[1] = tb[1] + 2;
                cb[1] = matriz(tb[1]);

                j = 0;
                u++;
            }

            if (u == 2 && r == 1) {
                cout << "No es posible hacer la configuración." << endl;
                exit(-1);
            }
        }

        cer[i + 1] = cb[1];
        X[i + 1] = tb[1];
    }

    return X;
}


// Codigos prueba

void pruebas(){

    // Validacion de reglas
/*
    int n = 3;
    int t[] = {3,5,3};

    int ***cer = cerradura(n,t);

    int K[] = {1,2,-1,0};

    bool res = validar_regla(K,cer,n,t);

    cout << res;
*/

    // Se imprime la cerradura
/*
    for (int p=0; p<n; p++){

        int **r = cer[p];

        for(int i=0;i<X[p];i++){
            for(int j=0;j<X[p];j++)
                if(r[i][j] < 10)
                    cout << " " << r[i][j] << " ";
                else
                    cout << r[i][j] <<" ";
            cout << endl;
        }
        cout << endl;
    }
*/

    // Rotacion de matriz
/*
    int **r = rot_mat(b,1);
    cout << endl;

    for(int i=0;i<a;i++){
        for(int j=0;j<a;j++)
            if(r[i][j] < 10)
                cout << " " << r[i][j] << " ";
            else
                cout << r[i][j] <<" ";
        cout << endl;
    }

*/
    return;
}
