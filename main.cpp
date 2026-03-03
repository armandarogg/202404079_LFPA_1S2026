#include <iostream> 
#include <fstream> 
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip> 
#include <set>

using namespace std;

void cargarEstudiantes();
void cargarCursos();
void cargarNotas();
void reporteEstudiante();
void generarReporte1HTML();
void generarReporte2HTML();
void generarReporte3HTML();
void generarReporte4HTML();
void generarReporte5HTML();

double calcularPromedio(vector<double> notas);
double calcularMediana(vector<double> notas);
double calcularDesviacion(vector<double> notas);
double calcularPercentil(vector<double> notas, double p);

//TIPOS DE DATOS :))

struct Estudiante {
    int carnet;
    string nombre;
    string apellido;
    string carrera;
    int semestre;
};
vector<Estudiante> listaEstudiantes;

struct Curso {
    string codigo;
    string nombre;
    int creditos;
    int semestre;
    string carrera;
};
vector<Curso> listaCursos;

struct Nota {
    int carnet;
    string codigoCurso;
    double nota;
};
vector<Nota> listaNotas;

struct PromedioEstudiante {
    Estudiante est;
    double promedio;
};

//MAIN

int main() {
    int opcion = 0;

    while (opcion != 9) {
        cout << "\nSISTEMA DE ANALISIS ACADEMICO\n";
        cout << "1. Cargar archivo de estudiantes\n";
        cout << "2. Cargar archivo de cursos\n";
        cout << "3. Cargar archivo de notas\n";
        cout << "4. Generar Reporte: Estadisticas por Curso\n";
        cout << "5. Generar Reporte: Rendimiento por Estudiante\n";
        cout << "6. Generar Reporte: Top 10 Mejores Estudiantes\n";
        cout << "7. Generar Reporte: Cursos con Mayor Reprobacion\n";
        cout << "8. Generar Reporte: Analisis por Carrera\n";
        cout << "9. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) cargarEstudiantes();
        else if (opcion == 2) cargarCursos();
        else if (opcion == 3) cargarNotas();
        else if (opcion == 4) generarReporte1HTML();            // Estadísticas por curso
        else if (opcion == 5) generarReporte2HTML();            // Rendimiento por estudiante
        else if (opcion == 6) generarReporte3HTML();            // Top 10
        else if (opcion == 7) generarReporte4HTML();            // Reprobación por curso
        else if (opcion == 8) generarReporte5HTML();            // Análisis por carrera (completo)
        else if (opcion == 9) cout << "salio\n";
        else cout << "Opcion invalida\n";
    }

    return 0;
}

//CARGA

void cargarEstudiantes() {

    ifstream archivo("estudiantes.lfp");

    if (!archivo.is_open()) {
        cout << "no se pudo abrir el archivo\n";
        return;
    }

    string linea;

    while (getline(archivo, linea)) {

        stringstream ss(linea);
        string campo;
        Estudiante e;
        getline(ss, campo, ',');
        e.carnet = stoi(campo);
        getline(ss, e.nombre, ',');
        getline(ss, e.apellido, ',');
        getline(ss, e.carrera, ',');
        getline(ss, campo, ',');
        e.semestre = stoi(campo);
        listaEstudiantes.push_back(e);
    }
    cout << "Estudiantes cargados: " << listaEstudiantes.size() << "\n";
    archivo.close();
}

void cargarCursos() {

    ifstream archivo("cursos.lfp");
    if (!archivo.is_open()) {
        cout << "no se pudo abrir archivo cursos\n";
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.find_first_not_of(" \t\r\n") == string::npos) continue;
        stringstream ss(linea);
        string campo;
        Curso c;
        getline(ss, c.codigo, ',');
        getline(ss, c.nombre, ',');
        if (!getline(ss, campo, ',')) continue;
        try { c.creditos = stoi(campo); } catch(...) { c.creditos = 0; }
        if (!getline(ss, campo, ',')) continue;
        try { c.semestre = stoi(campo); } catch(...) { c.semestre = 0; }
        if (!getline(ss, c.carrera)) c.carrera = "";
        listaCursos.push_back(c);
    }

    cout << "Cursos cargados: " << listaCursos.size() << "\n";
    archivo.close();
}

void cargarNotas() {

    ifstream archivo("notas.lfp");

    if (!archivo.is_open()) {
        cout << "no se pudo abrir archivo notas\n";
        return;
    }

    string linea;

    while (getline(archivo, linea)) {

        stringstream ss(linea);
        string campo;
        Nota n;
        getline(ss, campo, ',');
        n.carnet = stoi(campo);
        getline(ss, n.codigoCurso, ',');
        getline(ss, campo, ',');
        n.nota = stod(campo);
        listaNotas.push_back(n);
    }

    cout << "Notas cargadas: " << listaNotas.size() << "\n";
    archivo.close();
}

//REPORTES

void reporteEstudiante() {

    if (listaEstudiantes.empty() || listaNotas.empty()) {
        cout << "Debe cargar estudiantes y notas primero\n";
        return;
    }

    int carnetBuscado;
    cout << "Ingrese carnet: ";
    cin >> carnetBuscado;

    Estudiante* estudianteEncontrado = nullptr;

    for (int i = 0; i < listaEstudiantes.size(); i++) {
        if (listaEstudiantes[i].carnet == carnetBuscado) {
            estudianteEncontrado = &listaEstudiantes[i];
            break;
        }
    }

    if (estudianteEncontrado == nullptr) {
        cout << "Estudiante no encontrado\n";
        return;
    }

    cout << "\nREPORTE ACADEMICO \n";
    cout << "Nombre: " << estudianteEncontrado->nombre 
         << " " << estudianteEncontrado->apellido << "\n";
    cout << "Carrera: " << estudianteEncontrado->carrera << "\n\n";

    double suma = 0;
    int contador = 0;

    for (int i = 0; i < listaNotas.size(); i++) {
        if (listaNotas[i].carnet == carnetBuscado) {

            cout << "Curso: " << listaNotas[i].codigoCurso
                 << " | Nota: " << listaNotas[i].nota << "\n";

            suma += listaNotas[i].nota;
            contador++;
        }
    }

    if (contador > 0) {
        cout << "\nPromedio: " << suma / contador << "\n";
    } else {
        cout << "No tiene notas registradas\n";
    }
}

void generarReporte1HTML() {
    ofstream archivo("reporte1.html");
    archivo << "<html><head><style>table,th,td{border:1px solid black;border-collapse:collapse;padding:5px;}</style></head><body>";
    archivo << "<h1>Estadisticas por Curso</h1>";
    archivo << "<table>";
    archivo << "<tr><th>Curso</th><th>Estudiantes</th><th>Promedio</th><th>Max</th><th>Min</th><th>Desv</th>";
    archivo << "<th>P25</th><th>P50 (Mediana)</th><th>P75</th></tr>"; // Añadidos percentiles

    for (const auto& curso : listaCursos) {
        vector<double> notasCurso;
        for (const auto& nota : listaNotas) {
            if (nota.codigoCurso == curso.codigo) {
                notasCurso.push_back(nota.nota);
            }
        }
        if (notasCurso.empty()) continue;

        double promedio = calcularPromedio(notasCurso);
        double mediana = calcularMediana(notasCurso);
        double desviacion = calcularDesviacion(notasCurso);
        double maximo = *max_element(notasCurso.begin(), notasCurso.end());
        double minimo = *min_element(notasCurso.begin(), notasCurso.end());
        double p25 = calcularPercentil(notasCurso, 25);
        double p75 = calcularPercentil(notasCurso, 75);

        archivo << "<tr>";
        archivo << "<td>" << curso.nombre << "</td>";
        archivo << "<td>" << notasCurso.size() << "</td>";
        archivo << "<td>" << fixed << setprecision(2) << promedio << "</td>";
        archivo << "<td>" << maximo << "</td>";
        archivo << "<td>" << minimo << "</td>";
        archivo << "<td>" << desviacion << "</td>";
        archivo << "<td>" << p25 << "</td>";
        archivo << "<td>" << mediana << "</td>";
        archivo << "<td>" << p75 << "</td>";
        archivo << "</tr>";
    }

    archivo << "</table></body></html>";
    archivo.close();
}

void generarReporte2HTML() {

    ofstream archivo("reporte2.html");

    archivo << "<html><body>";
    archivo << "<h1>Rendimiento por Estudiante</h1>";
    archivo << "<table border='1'>";
    archivo << "<tr><th>Carnet</th><th>Nombre</th><th>Carrera</th><th>Semestre</th><th>Promedio</th><th>Aprobados</th><th>Reprobados</th><th>Creditos</th></tr>";

    for (int i = 0; i < listaEstudiantes.size(); i++) {

        vector<double> notasEst;
        int aprobados = 0;
        int reprobados = 0;
        int creditos = 0;

        for (int j = 0; j < listaNotas.size(); j++) {

            if (listaNotas[j].carnet == listaEstudiantes[i].carnet) {

                notasEst.push_back(listaNotas[j].nota);

                if (listaNotas[j].nota >= 61) {
                    aprobados++;

                    for (int k = 0; k < listaCursos.size(); k++) {
                        if (listaCursos[k].codigo == listaNotas[j].codigoCurso) {
                            creditos += listaCursos[k].creditos;
                        }
                    }

                } else {
                    reprobados++;
                }
            }
        }

        if (notasEst.empty()) continue;

        double promedio = calcularPromedio(notasEst);

        archivo << "<tr>";
        archivo << "<td>" << listaEstudiantes[i].carnet << "</td>";
        archivo << "<td>" << listaEstudiantes[i].nombre << " " << listaEstudiantes[i].apellido << "</td>";
        archivo << "<td>" << listaEstudiantes[i].carrera << "</td>";
        archivo << "<td>" << listaEstudiantes[i].semestre << "</td>";
        archivo << "<td>" << promedio << "</td>";
        archivo << "<td>" << aprobados << "</td>";
        archivo << "<td>" << reprobados << "</td>";
        archivo << "<td>" << creditos << "</td>";
        archivo << "</tr>";
    }

    archivo << "</table></body></html>";
    archivo.close();
}

void generarReporte3HTML() {

    ofstream archivo("reporte3.html");

    vector<PromedioEstudiante> ranking;

    for (int i = 0; i < listaEstudiantes.size(); i++) {

        vector<double> notasEst;

        for (int j = 0; j < listaNotas.size(); j++) {
            if (listaNotas[j].carnet == listaEstudiantes[i].carnet) {
                notasEst.push_back(listaNotas[j].nota);
            }
        }

        if (!notasEst.empty()) {
            PromedioEstudiante pe;
            pe.est = listaEstudiantes[i];
            pe.promedio = calcularPromedio(notasEst);
            ranking.push_back(pe);
        }
    }

    sort(ranking.begin(), ranking.end(),
         [](const PromedioEstudiante &a, const PromedioEstudiante &b) {
             return a.promedio > b.promedio;
         });

    archivo << "<html><body>";
    archivo << "<h1>Top 10 Mejores Estudiantes</h1>";
    archivo << "<table border='1'>";
    archivo << "<tr><th>Pos</th><th>Carnet</th><th>Nombre</th><th>Carrera</th><th>Semestre</th><th>Promedio</th></tr>";

    int limite = min(10, (int)ranking.size());

    for (int i = 0; i < limite; i++) {
        archivo << "<tr>";
        archivo << "<td>" << i+1 << "</td>";
        archivo << "<td>" << ranking[i].est.carnet << "</td>";
        archivo << "<td>" << ranking[i].est.nombre << " " << ranking[i].est.apellido << "</td>";
        archivo << "<td>" << ranking[i].est.carrera << "</td>";
        archivo << "<td>" << ranking[i].est.semestre << "</td>";
        archivo << "<td>" << ranking[i].promedio << "</td>";
        archivo << "</tr>";
    }

    archivo << "</table></body></html>";
    archivo.close();
}

void generarReporte4HTML() {

    ofstream archivo("reporte4.html");

    archivo << "<html><body>";
    archivo << "<h1>Cursos con Mayor Reprobacion</h1>";
    archivo << "<table border='1'>";
    archivo << "<tr><th>Codigo</th><th>Curso</th><th>Total</th><th>Aprobados</th><th>Reprobados</th><th>% Reprobacion</th></tr>";

    for (int i = 0; i < listaCursos.size(); i++) {

        int total = 0;
        int aprobados = 0;
        int reprobados = 0;

        for (int j = 0; j < listaNotas.size(); j++) {
            if (listaNotas[j].codigoCurso == listaCursos[i].codigo) {
                total++;

                if (listaNotas[j].nota >= 61)
                    aprobados++;
                else
                    reprobados++;
            }
        }

        if (total == 0) continue;

        double porcentaje = (double)reprobados * 100 / total;

        archivo << "<tr>";
        archivo << "<td>" << listaCursos[i].codigo << "</td>";
        archivo << "<td>" << listaCursos[i].nombre << "</td>";
        archivo << "<td>" << total << "</td>";
        archivo << "<td>" << aprobados << "</td>";
        archivo << "<td>" << reprobados << "</td>";
        archivo << "<td>" << porcentaje << "</td>";
        archivo << "</tr>";
    }

    archivo << "</table></body></html>";
    archivo.close();
}

void generarReporte5HTML() {

    ofstream archivo("reporte5.html");
    archivo << "<html><body>";
    archivo << "<h1>Analisis por Carrera</h1>";
    archivo << "<table border='1'>";
    archivo << "<tr><th>Carrera</th><th>Total Estudiantes</th><th>Promedio General</th>";
    archivo << "<th>Cursos Disponibles</th><th>Distribucion por Semestre (sem:count)</th></tr>";

    // obtener lista única de carreras desde estudiantes
    vector<string> carreras;
    for (auto &e : listaEstudiantes) {
        if (find(carreras.begin(), carreras.end(), e.carrera) == carreras.end())
            carreras.push_back(e.carrera);
    }

    for (auto &carrera : carreras) {
        int totalEst = 0;
        vector<double> notasCarrera;
        set<string> cursosDeCarrera;            // para contar cursos disponibles (sin duplicados)
        vector<int> distrib(11, 0);             // índices 1..10 para semestres

        // contar estudiantes y recolectar notas + distribución por semestre
        for (auto &est : listaEstudiantes) {
            if (est.carrera == carrera) {
                totalEst++;
                distrib[ max(1, min(10, est.semestre)) ]++; // proteger rango
                // recolectar todas las notas de cada estudiante
                for (auto &n : listaNotas) {
                    if (n.carnet == est.carnet) notasCarrera.push_back(n.nota);
                }
            }
        }

        // contar cursos disponibles para esa carrera
        for (auto &cur : listaCursos) {
            if (cur.carrera == carrera) cursosDeCarrera.insert(cur.codigo);
        }

        double promedio = calcularPromedio(notasCarrera);

        // construir string de distribución "1:3,2:5,...", omitimos semestres con 0 si quieres más limpio
        string distribStr;
        bool first = true;
        for (int s = 1; s <= 10; s++) {
            if (distrib[s] > 0) {
                if (!first) distribStr += ", ";
                distribStr += to_string(s) + ":" + to_string(distrib[s]);
                first = false;
            }
        }
        if (distribStr.empty()) distribStr = "—";

        archivo << "<tr>";
        archivo << "<td>" << carrera << "</td>";
        archivo << "<td>" << totalEst << "</td>";
        archivo << "<td>" << promedio << "</td>";
        archivo << "<td>" << cursosDeCarrera.size() << "</td>";
        archivo << "<td>" << distribStr << "</td>";
        archivo << "</tr>";
    }

    archivo << "</table></body></html>";
    archivo.close();

    cout << "Reporte 5 (Analisis por Carrera) generado: reporte5.html\n";
}

//CALCULOS

double calcularPromedio(vector<double> notas) {

    if (notas.empty()) return 0;

    double suma = 0;

    for (int i = 0; i < notas.size(); i++) {
        suma += notas[i];
    }

    return suma / notas.size();
}

double calcularMediana(vector<double> notas) {

    if (notas.empty()) return 0;

    sort(notas.begin(), notas.end());

    int n = notas.size();

    if (n % 2 == 0) {
        return (notas[n/2 - 1] + notas[n/2]) / 2;
    } else {
        return notas[n/2];
    }
}

double calcularDesviacion(vector<double> notas) {

    if (notas.empty()) return 0;

    double promedio = calcularPromedio(notas);
    double suma = 0;

    for (int i = 0; i < notas.size(); i++) {
        suma += pow(notas[i] - promedio, 2);
    }

    return sqrt(suma / notas.size());
}

double calcularPercentil(vector<double> notas, double p) {
    if (notas.empty()) return 0;
    sort(notas.begin(), notas.end());
    double pos = (p / 100.0) * (notas.size() - 1);
    int idx = floor(pos);
    double frac = pos - idx;
    if (idx >= (int)notas.size() - 1) return notas.back();
    return notas[idx] + frac * (notas[idx + 1] - notas[idx]);
}
