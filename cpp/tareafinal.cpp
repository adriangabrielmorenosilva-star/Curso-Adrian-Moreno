#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <cctype>

using namespace std;

class Persona {
protected:
    string cedula;
    string nombreCompleto;
    string correo;

public:
    Persona(string cedula, string nombreCompleto, string correo) {
        this->cedula = cedula;
        this->nombreCompleto = nombreCompleto;
        this->correo = correo;
    }

    virtual ~Persona() {}

    string getCedula() const { return cedula; }
    void setCedula(string cedula) { this->cedula = cedula; }

    string getNombreCompleto() const { return nombreCompleto; }
    void setNombreCompleto(string nombreCompleto) { this->nombreCompleto = nombreCompleto; }

    string getCorreo() const { return correo; }
    void setCorreo(string correo) { this->correo = correo; }
};

class Profesor : public Persona {
private:
    string especialidadAcademica;
    string materia;

public:
    Profesor(string cedula, string nombreCompleto, string correo, string especialidad, string materia)
        : Persona(cedula, nombreCompleto, correo) {
        this->especialidadAcademica = especialidad;
        this->materia = materia;
    }

    string getEspecialidadAcademica() const { return especialidadAcademica; }
    void setEspecialidadAcademica(string especialidadAcademica) { this->especialidadAcademica = especialidadAcademica; }

    string getMateria() const { return materia; }
    void setMateria(string materia) { this->materia = materia; }
};


class ProgramaAcademico {
protected:
    string nombreDePrograma;
    string duracion;

public:
    ProgramaAcademico(string nombre, string duracion) {
        this->nombreDePrograma = nombre;
        this->duracion = duracion;
    }

    virtual ~ProgramaAcademico() {}

    string getNombreDePrograma() const { return nombreDePrograma; }

    double calcularPromedio(const vector<double>& notas) const {
        if (notas.empty()) {
            return 0.0;
        }
        double suma = 0;
        for (double nota : notas) {
            suma += nota;
        }
        return suma / notas.size();
    }

    virtual bool verificarAprobacion(const vector<double>& notas) const = 0;
};

class Curso : public ProgramaAcademico {
public:
    Curso() : ProgramaAcademico("Curso", "Corto") {}

    bool verificarAprobacion(const vector<double>& notas) const override {
        if (notas.size() < 3) {
            return false;
        }
        double promedio = calcularPromedio(notas);
        return promedio >= 10.0;
    }
};

class Diplomado : public ProgramaAcademico {
public:
    Diplomado() : ProgramaAcademico("Diplomado", "Mediano") {}

    bool verificarAprobacion(const vector<double>& notas) const override {
        if (notas.size() < 3) {
            return false;
        }
        double promedio = calcularPromedio(notas);
        return promedio >= 14.0;
    }
};

class Bootcamp : public ProgramaAcademico {
public:
    Bootcamp() : ProgramaAcademico("Bootcamp", "Intensivo") {}

    bool verificarAprobacion(const vector<double>& notas) const override {
        if (notas.size() < 3) {
            return false;
        }
        for (double nota : notas) {
            if (nota < 14.0) {
                return false;
            }
        }
        return true;
    }
};

class Alumno : public Persona {
private:
    vector<double> notas;
    ProgramaAcademico* programaAsignado;

    string toLowerTrim(string str) const {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        string trimmed = str.substr(first, (last - first + 1));
        for (char& c : trimmed) {
            c = tolower(static_cast<unsigned char>(c));
        }
        return trimmed;
    }

public:
    Alumno(string cedula, string nombreCompleto, string correo, string tipoPrograma)
        : Persona(cedula, nombreCompleto, correo) {
        this->notas = vector<double>();
        string tipoLower = toLowerTrim(tipoPrograma);
        
        if (tipoLower.find("curso") != string::npos) {
            this->programaAsignado = new Curso();
        } else if (tipoLower.find("diplomado") != string::npos) {
            this->programaAsignado = new Diplomado();
        } else if (tipoLower.find("bootcamp") != string::npos) {
            this->programaAsignado = new Bootcamp();
        } else {
            throw invalid_argument("Tipo de programa invalido: '" + tipoPrograma + "'. Debe ser Curso, Diplomado o Bootcamp.");
        }
    }

    ~Alumno() override {
        if (programaAsignado != nullptr) {
            delete programaAsignado;
            programaAsignado = nullptr;
        }
    }

    const vector<double>& getNotas() const { return notas; }
    void agregarNota(double nota) { this->notas.push_back(nota); }

    double removerUltimaNota() {
        double ultima = notas.back();
        notas.pop_back();
        return ultima;
    }

    ProgramaAcademico* getProgramaAsignado() const { return programaAsignado; }
    void setProgramaAsignado(ProgramaAcademico* programaAsignado) { this->programaAsignado = programaAsignado; }

    double calcularPromedio() const {
        if (notas.empty()) return 0.0;
        double suma = 0;
        for (double n : notas) {
            suma += n;
        }
        return suma / notas.size();
    }
};


class Sistema {
private:
    vector<Alumno*> listaAlumnos;
    vector<Profesor*> listaProfesores;
    stack<string> pilaAccionesNotas;

    const string RUTA_ALUMNOS = "alumnos.txt";
    const string RUTA_PROFESORES = "profesores.txt";
    const string RUTA_CERTIFICADOS = "certificados_pendientes.txt";

    string trim(string str) const {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

    bool equalsIgnoreCase(string a, string b) const {
        string strA = trim(a);
        string strB = trim(b);
        if (strA.size() != strB.size()) return false;
        for (size_t i = 0; i < strA.size(); ++i) {
            if (tolower(static_cast<unsigned char>(strA[i])) != tolower(static_cast<unsigned char>(strB[i]))) {
                return false;
            }
        }
        return true;
    }

    vector<string> split(string s, char delimiter) const {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(trim(token));
        }
        return tokens;
    }

public:
    Sistema() {
        this->listaAlumnos = vector<Alumno*>();
        this->listaProfesores = vector<Profesor*>();
        this->pilaAccionesNotas = stack<string>();

        ofstream(RUTA_ALUMNOS, ios::app);
        ofstream(RUTA_PROFESORES, ios::app);
        ofstream(RUTA_CERTIFICADOS, ios::app);

        cargarDatos();
    }

    ~Sistema() {
        for (Alumno* alu : listaAlumnos) delete alu;
        listaAlumnos.clear();
        for (Profesor* prof : listaProfesores) delete prof;
        listaProfesores.clear();
    }

    void cargarDatos() {
        ifstream archivoAlumnos(RUTA_ALUMNOS);
        if (archivoAlumnos.is_open()) {
            string linea;
            while (getline(archivoAlumnos, linea)) {
                if (trim(linea).empty()) continue;
                vector<string> partes = split(linea, ',');
                if (partes.size() >= 4) {
                    try {
                        Alumno* alu = new Alumno(partes[0], partes[1], partes[2], partes[3]);
                        for (size_t i = 4; i < partes.size(); i++) {
                            if (!partes[i].empty()) {
                                try {
                                    double valNota = stod(partes[i]);
                                    if (valNota >= 0.0 && valNota <= 20.0) {
                                        alu->agregarNota(valNota);
                                    }
                                } catch (...) {}
                            }
                        }
                        listaAlumnos.push_back(alu);
                    } catch (const invalid_argument& e) {
                        cout << "Aviso: Alumno omitido por programa invalido en archivo: " << e.what() << endl;
                    }
                }
            }
            archivoAlumnos.close();
        }

        ifstream archivoProfesores(RUTA_PROFESORES);
        if (archivoProfesores.is_open()) {
            string linea;
            while (getline(archivoProfesores, linea)) {
                if (trim(linea).empty()) continue;
                vector<string> partes = split(linea, ',');
                if (partes.size() >= 5) {
                    Profesor* prof = new Profesor(partes[0], partes[1], partes[2], partes[3], partes[4]);
                    listaProfesores.push_back(prof);
                }
            }
            archivoProfesores.close();
        }
    }

    void guardarAlumnosEnDisco() const {
        ofstream pw(RUTA_ALUMNOS);
        if (!pw.is_open()) return;
        for (const Alumno* alu : listaAlumnos) {
            ostringstream sb;
            sb << alu->getCedula() << ","
               << alu->getNombreCompleto() << ","
               << alu->getCorreo() << ","
               << alu->getProgramaAsignado()->getNombreDePrograma();
            if (!alu->getNotas().empty()) {
                for (double n : alu->getNotas()) {
                    // Fuerza que las notas guarden un decimal exacto (ej. 20.0)
                    sb << "," << fixed << setprecision(1) << n;
                }
            }
            pw << sb.str() << "\n";
        }
        pw.flush();
        pw.close();
    }

    void guardarProfesoresEnDisco() const {
        ofstream pw(RUTA_PROFESORES);
        if (!pw.is_open()) return;
        for (const Profesor* prof : listaProfesores) {
            pw << prof->getCedula() << "," << prof->getNombreCompleto() << "," << prof->getCorreo() << "," << prof->getEspecialidadAcademica() << "," << prof->getMateria() << "\n";
        }
        pw.flush();
        pw.close();
    }

    bool registrarAlumno(Alumno* alu) {
        for (const Alumno* existente : listaAlumnos) {
            if (equalsIgnoreCase(existente->getCedula(), alu->getCedula())) {
                cout << "\nError: Ya existe un alumno registrado con la cedula '" << alu->getCedula() << "'. Registro anulado." << endl;
                delete alu;
                return false;
            }
        }
        listaAlumnos.push_back(alu);
        guardarAlumnosEnDisco();
        cout << "\nAlumno " << alu->getNombreCompleto() << " registrado con exito y guardado en disco!" << endl;
        return true;
    }

    bool registrarProfesor(Profesor* prof) {
        for (const Profesor* existente : listaProfesores) {
            if (equalsIgnoreCase(existente->getCedula(), prof->getCedula())) {
                cout << "\nError: Ya existe un profesor registrado con la cedula '" << prof->getCedula() << "'. Registro anulado." << endl;
                delete prof;
                return false;
            }
        }
        listaProfesores.push_back(prof);
        guardarProfesoresEnDisco();
        cout << "\nProfesor " << prof->getNombreCompleto() << " registrado con exito y guardado en disco!" << endl;
        return true;
    }

    bool registrarNotas(string cedula, double nota) {
        if (nota < 0.0 || nota > 20.0) {
            cout << "Error: Nota no valida (" << fixed << setprecision(1) << nota << "). La calificacion tiene que ser del 0 al 20." << endl;
            return false;
        }

        for (Alumno* alu : listaAlumnos) {
            if (equalsIgnoreCase(alu->getCedula(), cedula)) {
                if (alu->getNotas().size() >= 3) {
                    cout << "Error: El alumno " << alu->getNombreCompleto() << " ya ha llenado las 3 notas permitidas." << endl;
                    return false;
                }
                alu->agregarNota(nota);
                pilaAccionesNotas.push(alu->getCedula());
                guardarAlumnosEnDisco();
                cout << "Nota " << fixed << setprecision(1) << nota << " agregada correctamente a " << alu->getNombreCompleto() << "!" << endl;
                return true;
            }
        }

        cout << "Error: No se encontro ningun alumno registrado con esa cedula." << endl;
        return false;
    }

    void deshacerRegistro() {
        if (pilaAccionesNotas.empty()) {
            cout << "\nNo hay acciones recientes de notas para deshacer." << endl;
            return;
        }
        string cedulaAfectada = pilaAccionesNotas.top();
        pilaAccionesNotas.pop();

        for (Alumno* alu : listaAlumnos) {
            if (equalsIgnoreCase(alu->getCedula(), cedulaAfectada)) {
                if (!alu->getNotas().empty()) {
                    double notaRemovida = alu->removerUltimaNota();
                    guardarAlumnosEnDisco();
                    cout << "\n[Deshacer LIFO]: Se elimino la ultima nota (" << fixed << setprecision(1) << notaRemovida << ") de " << alu->getNombreCompleto() << "." << endl;
                } else {
                    cout << "\nEl alumno " << alu->getNombreCompleto() << " ya no tiene notas para remover." << endl;
                }
                return;
            }
        }
    }

    void generarColaCertificados() const {
        queue<Alumno*> colaGraduandos;
        for (Alumno* alu : listaAlumnos) {
            if (alu->getNotas().size() == 3 && alu->getProgramaAsignado()->verificarAprobacion(alu->getNotas())) {
                colaGraduandos.push(alu);
            }
        }

        ofstream pw(RUTA_CERTIFICADOS);
        if (!pw.is_open()) return;

        pw << "===\n";
        pw << "REPORTE DE CERTIFICADOS PENDIENTES\n";
        pw << "=========================================\n";
        pw << "Total de graduandos en cola: " << colaGraduandos.size() << "\n";
        int idx = 1;
        while (!colaGraduandos.empty()) {
            Alumno* grad = colaGraduandos.front();
            colaGraduandos.pop();

            double prom = grad->calcularPromedio();
            string estatusExtra = "";
            if (equalsIgnoreCase(grad->getProgramaAsignado()->getNombreDePrograma(), "bootcamp")) {
                estatusExtra = " - Estatus: APROBADO (Cumple regla de ninguna nota < 14)";
            }
            pw << idx << ". [" << grad->getCedula() << "] " << grad->getNombreCompleto() << "\n";
            pw << "- Programa: " << grad->getProgramaAsignado()->getNombreDePrograma() << "\n";
            pw << "- Promedio Final: " << fixed << setprecision(1) << prom << "\n";
            pw << "- Estatus: APROBADO" << estatusExtra << "\n";
            idx++;
        }
        pw << "=========================================\n";
        pw << "* Fin del reporte - Generado por SGA-DO *\n";
        pw.flush();
        pw.close();
        cout << "\nCola de certificados generada con exito en '" << RUTA_CERTIFICADOS << "'!" << endl;
    }

    void generarReporte() const {
        cout << "\n=========================================" << endl;
        cout << " REPORTE GENERAL - SISTEMA" << endl;
        cout << "=========================================" << endl;
        cout << "--- PROFESORES ---" << endl;
        if (listaProfesores.empty()) {
            cout << "No hay profesores registrados." << endl;
        } else {
            for (const Profesor* p : listaProfesores) {
                cout << "[" << p->getCedula() << "] " << p->getNombreCompleto() << " | Correo: " << p->getCorreo() << " | Esp: " << p->getEspecialidadAcademica() << " | Mat: " << p->getMateria() << endl;
            }
        }

        cout << "\n--- ALUMNOS ---" << endl;
        if (listaAlumnos.empty()) {
            cout << "No hay alumnos registrados." << endl;
        } else {
            for (const Alumno* a : listaAlumnos) {
                double prom = a->calcularPromedio();
                bool aprobado = a->getProgramaAsignado()->verificarAprobacion(a->getNotas());
                string estatus = aprobado ? "APROBADO" : "REPROBADO";
                
                cout << "[" << a->getCedula() << "] " << a->getNombreCompleto() 
                     << " | Prog: " << a->getProgramaAsignado()->getNombreDePrograma() 
                     << " | Notas: [";
                
                const vector<double>& notas = a->getNotas();
                for (size_t i = 0; i < notas.size(); ++i) {
                    
                    cout << fixed << setprecision(1) << notas[i] << (i < notas.size() - 1 ? ", " : "");
                }
                cout << "] | Prom: " << fixed << setprecision(1) << prom << " | Estatus: " << estatus << endl;
            }
        }
        cout << "=========================================\n" << endl;
    }

    static void ejecutarMenu() {
        Sistema* sistema = new Sistema();

        while (true) {
            cout << "\n=========================================" << endl;
            cout << "=== SGA-DO: SISTEMA DIPLOMADOSONLINE ===" << endl;
            cout << "=========================================" << endl;
            cout << "1. Registrar Alumno" << endl;
            cout << "2. Registrar Profesor" << endl;
            cout << "3. Registrar Notas a un Alumno" << endl;
            cout << "4. Deshacer Ultimo Registro de Nota" << endl;
            cout << "5. Generar Cola de Certificados" << endl;
            cout << "6. Mostrar Reporte General" << endl;
            cout << "7. Salir" << endl;
            cout << "Seleccione una opcion (1-7): ";

            string opcionStr;
            getline(cin, opcionStr);
            size_t first = opcionStr.find_first_not_of(" \t\r\n");
            if (first == string::npos) {
                cout << "Error: Debe ingresar una opcion valida." << endl;
                continue;
            }
            size_t last = opcionStr.find_last_not_of(" \t\r\n");
            opcionStr = opcionStr.substr(first, (last - first + 1));

            if (opcionStr.empty()) {
                cout << "Error: Debe ingresar una opcion valida." << endl;
                continue;
            }

            int opcionInt = 0;
            try {
                size_t pos;
                opcionInt = stoi(opcionStr, &pos);
                if (pos != opcionStr.length()) {
                    throw invalid_argument("");
                }
            } catch (...) {
                cout << "Error: Ingrese un valor numerico valido." << endl;
                continue;
            }

            switch (opcionInt) {
                case 1: {
                    cout << "\n--- REGISTRAR ALUMNO ---" << endl;
                    cout << "Ingrese Cedula/ID (Ej. V-999): ";
                    string cedulaAlu; getline(cin, cedulaAlu);
                    cout << "Ingrese Nombre Completo: ";
                    string nombreAlu; getline(cin, nombreAlu);
                    cout << "Ingrese Correo Electronico: ";
                    string correoAlu; getline(cin, correoAlu);
                    cout << "Ingrese Tipo de Programa (Curso, Diplomado, Bootcamp): ";
                    string tipoProg; getline(cin, tipoProg);
                    
                    try {
                        Alumno* nuevoAlumno = new Alumno(cedulaAlu, nombreAlu, correoAlu, tipoProg);
                        sistema->registrarAlumno(nuevoAlumno);
                    } catch (const invalid_argument& e) {
                        cout << "\n[Error de Registro]: " << e.what() << endl;
                        cout << ">>> La opcion ingresada no es valida. El registro del alumno ha sido anulado. <<<" << endl;
                    }
                    break;
                }

                case 2: {
                    cout << "\n--- REGISTRAR PROFESOR ---" << endl;
                    cout << "Ingrese Cedula/ID (Ej. V-999): ";
                    string cedulaProf; getline(cin, cedulaProf);
                    cout << "Ingrese Nombre Completo: ";
                    string nombreProf; getline(cin, nombreProf);
                    cout << "Ingrese Correo Electronico: ";
                    string correoProf; getline(cin, correoProf);
                    cout << "Ingrese Especialidad Academica: ";
                    string especialidad; getline(cin, especialidad);
                    cout << "Ingrese Materia Asignada: ";
                    string materia; getline(cin, materia);
                    Profesor* nuevoProfesor = new Profesor(cedulaProf, nombreProf, correoProf, especialidad, materia);
                    sistema->registrarProfesor(nuevoProfesor);
                    break;
                }

                case 3: {
                    cout << "\n--- REGISTRAR NOTAS A UN ALUMNO ---" << endl;
                    cout << "Ingrese Cedula del Alumno (Ej. V-999): ";
                    string cedulaNota; getline(cin, cedulaNota);
                    cout << "Ingrese la calificacion (0-20): ";
                    string notaStr; getline(cin, notaStr);
                    try {
                        double nota = stod(notaStr);
                        sistema->registrarNotas(cedulaNota, nota);
                    } catch (...) {
                        cout << "Error: Ingrese un valor numerico valido para la nota." << endl;
                    }
                    break;
                }

                case 4:
                    cout << "\n--- DESHACER ULTIMO REGISTRO DE NOTA ---" << endl;
                    sistema->deshacerRegistro();
                    break;

                case 5:
                    cout << "\n--- GENERAR COLA DE CERTIFICADOS ---" << endl;
                    sistema->generarColaCertificados();
                    break;

                case 6:
                    sistema->generarReporte();
                    break;

                case 7:
                    cout << "\nSaliendo del sistema de forma segura. Hasta luego!" << endl;
                    delete sistema;
                    return;

                default:
                    cout << "Opcion invalida. Seleccione un numero entre 1 y 7." << endl;
                    break;
            }
        }
    }
};


int main() {
    Sistema::ejecutarMenu();
    return 0;
}