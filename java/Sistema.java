import java.io.*;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.List;
import java.util.Stack;


abstract class ProgramaAcademico {
    protected String nombreDePrograma;
    protected String duracion;

    public ProgramaAcademico(String nombre, String duracion) {
        this.nombreDePrograma = nombre;
        this.duracion = duracion;
    }

    public String getNombreDePrograma() { return nombreDePrograma; }

    public double calcularPromedio(Alumno alumno) {
        if (alumno.getNotas().isEmpty()) {
            return 0.0;
        }
        double suma = 0;
        for (double nota : alumno.getNotas()) {
            suma += nota;
        }
        return suma / alumno.getNotas().size();
    }

    public abstract boolean verificarAprobacion(Alumno alumno);
}

class Curso extends ProgramaAcademico {
    public Curso() {
        super("Curso", "Corto");
    }

    @Override
    public boolean verificarAprobacion(Alumno alumno) {
        if (alumno.getNotas().size() < 3) {
            return false;
        }
        double promedio = calcularPromedio(alumno);
        return promedio >= 10.0;
    }
}

class Diplomado extends ProgramaAcademico {
    public Diplomado() {
        super("Diplomado", "Mediano");
    }

    @Override
    public boolean verificarAprobacion(Alumno alumno) {
        if (alumno.getNotas().size() < 3) {
            return false;
        }
        double promedio = calcularPromedio(alumno);
        return promedio >= 14.0;
    }
}

class Bootcamp extends ProgramaAcademico {
    public Bootcamp() {
        super("Bootcamp", "Intensivo");
    }

    @Override
    public boolean verificarAprobacion(Alumno alumno) {
        if (alumno.getNotas().size() < 3) {
            return false;
        }
        for (double nota : alumno.getNotas()) {
            if (nota < 14.0) {
                return false;
            }
        }
        return true;
    }
}


class Persona {
    protected String cedula;
    protected String nombreCompleto;
    protected String correo;

    public Persona(String cedula, String nombreCompleto, String correo) {
        this.cedula = cedula;
        this.nombreCompleto = nombreCompleto;
        this.correo = correo;
    }

    public String getCedula() { return cedula; }
    public void setCedula(String cedula) { this.cedula = cedula; }

    public String getNombreCompleto() { return nombreCompleto; }
    public void setNombreCompleto(String nombreCompleto) { this.nombreCompleto = nombreCompleto; }

    public String getCorreo() { return correo; }
    public void setCorreo(String correo) { this.correo = correo; }
}

class Alumno extends Persona {
    private List<Double> notas;
    private ProgramaAcademico programaAsignado;

    public Alumno(String cedula, String nombreCompleto, String correo, String tipoPrograma) {
        super(cedula, nombreCompleto, correo);
        this.notas = new ArrayList<>();
        String tipoLower = tipoPrograma.toLowerCase();
        if (tipoLower.contains("curso")) {
            this.programaAsignado = new Curso();
        } else if (tipoLower.contains("diplomado")) {
            this.programaAsignado = new Diplomado();
        } else if (tipoLower.contains("bootcamp")) {
            this.programaAsignado = new Bootcamp();
        } else {
            this.programaAsignado = new Curso();
        }
    }

    public List<Double> getNotas() { return notas; }
    public void agregarNota(double nota) { this.notas.add(nota); }

    public ProgramaAcademico getProgramaAsignado() { return programaAsignado; }
    public void setProgramaAsignado(ProgramaAcademico programaAsignado) { this.programaAsignado = programaAsignado; }

    public double calcularPromedio() {
        if (notas.isEmpty()) return 0.0;
        double suma = 0;
        for (double n : notas) {
            suma += n;
        }
        return suma / notas.size();
    }
}

class Profesor extends Persona {
    private String especialidadAcademica;
    private String materia;

    public Profesor(String cedula, String nombreCompleto, String correo, String especialidad, String materia) {
        super(cedula, nombreCompleto, correo);
        this.especialidadAcademica = especialidad;
        this.materia = materia;
    }

    public String getEspecialidadAcademica() { return especialidadAcademica; }
    public void setEspecialidadAcademica(String especialidadAcademica) { this.especialidadAcademica = especialidadAcademica; }

    public String getMateria() { return materia; }
    public void setMateria(String materia) { this.materia = materia; }

    public void evaluarAlumno(Alumno alumnoObj, double nota) {
        alumnoObj.agregarNota(nota);
    }
}


class GestorLogica {
    private List<Alumno> listaAlumnos;
    private List<Profesor> listaProfesores;
    private Stack<String> pilaAccionesNotas;

    public GestorLogica() {
        this.listaAlumnos = new ArrayList<>();
        this.listaProfesores = new ArrayList<>();
        this.pilaAccionesNotas = new Stack<>();
        cargarDatos();
    }

    public void cargarDatos() {
        File archivoAlumnos = new File("java/alumnos.txt");
        if (archivoAlumnos.exists()) {
            try (BufferedReader br = new BufferedReader(new FileReader(archivoAlumnos))) {
                String linea;
                while ((linea = br.readLine()) != null) {
                    if (linea.trim().isEmpty()) continue;
                    String[] partes = linea.split(",");
                    for (int i = 0; i < partes.length; i++) {
                        partes[i] = partes[i].trim();
                    }
                    if (partes.length >= 4) {
                        String cedula = partes[0];
                        String nombre = partes[1];
                        String correo = partes[2];
                        String tipoProg = partes[3];
                        Alumno alu = new Alumno(cedula, nombre, correo, tipoProg);
                        for (int i = 4; i < partes.length; i++) {
                            if (!partes[i].isEmpty()) {
                                try {
                                    double valNota = Double.parseDouble(partes[i]);
                                    if (valNota >= 0.0 && valNota <= 20.0) {
                                        alu.agregarNota(valNota);
                                    }
                                } catch (NumberFormatException e) {
                                    // Ignorar valores mal formateados
                                }
                            }
                        }
                        listaAlumnos.add(alu);
                    }
                }
            } catch (IOException e) {
                System.out.println("Error al leer java/alumnos.txt: " + e.getMessage());
            }
        }

        File archivoProfesores = new File("java/profesores.txt");
        if (archivoProfesores.exists()) {
            try (BufferedReader br = new BufferedReader(new FileReader(archivoProfesores))) {
                String linea;
                while ((linea = br.readLine()) != null) {
                    if (linea.trim().isEmpty()) continue;
                    String[] partes = linea.split(",");
                    for (int i = 0; i < partes.length; i++) {
                        partes[i] = partes[i].trim();
                    }
                    if (partes.length >= 5) {
                        Profesor prof = new Profesor(partes[0], partes[1], partes[2], partes[3], partes[4]);
                        listaProfesores.add(prof);
                    }
                }
            } catch (IOException e) {
                System.out.println("Error al leer java/profesores.txt: " + e.getMessage());
            }
        }
    }

    public void guardarAlumnosEnDisco() {
        try (PrintWriter pw = new PrintWriter(new FileWriter("java/alumnos.txt"))) {
            for (Alumno alu : listaAlumnos) {
                StringBuilder sb = new StringBuilder();
                sb.append(alu.getCedula()).append(",")
                  .append(alu.getNombreCompleto()).append(",")
                  .append(alu.getCorreo()).append(",")
                  .append(alu.getProgramaAsignado().getNombreDePrograma());
                if (!alu.getNotas().isEmpty()) {
                    for (double n : alu.getNotas()) {
                        sb.append(",").append(n);
                    }
                }
                pw.println(sb.toString());
            }
        } catch (IOException e) {
            System.out.println("Error al guardar java/alumnos.txt: " + e.getMessage());
        }
    }

    public void guardarProfesoresEnDisco() {
        try (PrintWriter pw = new PrintWriter(new FileWriter("java/profesores.txt"))) {
            for (Profesor prof : listaProfesores) {
                pw.println(prof.getCedula() + "," + prof.getNombreCompleto() + "," + prof.getCorreo() + "," + prof.getEspecialidadAcademica() + "," + prof.getMateria());
            }
        } catch (IOException e) {
            System.out.println("Error al guardar java/profesores.txt: " + e.getMessage());
        }
    }

    public boolean registrarAlumno(Alumno alu) {
        for (Alumno existente : listaAlumnos) {
            if (existente.getCedula().trim().equalsIgnoreCase(alu.getCedula().trim())) {
                System.out.println("\nError: Ya existe un alumno registrado con la cédula '" + alu.getCedula() + "'.");
                return false;
            }
        }
        listaAlumnos.add(alu);
        guardarAlumnosEnDisco();
        System.out.println("\n¡Alumno " + alu.getNombreCompleto() + " registrado con éxito!");
        return true;
    }

    public boolean registrarProfesor(Profesor prof) {
        for (Profesor existente : listaProfesores) {
            if (existente.getCedula().trim().equalsIgnoreCase(prof.getCedula().trim())) {
                System.out.println("\nError: Ya existe un profesor registrado con la cédula '" + prof.getCedula() + "'.");
                return false;
            }
        }
        listaProfesores.add(prof);
        guardarProfesoresEnDisco();
        System.out.println("\n¡Profesor " + prof.getNombreCompleto() + " registrado con éxito!");
        return true;
    }

    public boolean registrarNotas(String cedula, double nota) {
        if (nota < 0.0 || nota > 20.0) {
            System.out.println("Error: Nota no válida (" + nota + "). La calificación tiene que ser del 0 al 20.");
            return false;
        }

        for (Alumno alu : listaAlumnos) {
            if (alu.getCedula().trim().equalsIgnoreCase(cedula.trim())) {
                if (alu.getNotas().size() >= 3) {
                    System.out.println("Error: El alumno " + alu.getNombreCompleto() + " ya ha llenado las 3 notas permitidas.");
                    return false;
                }
                alu.agregarNota(nota);
                pilaAccionesNotas.push(alu.getCedula());
                guardarAlumnosEnDisco();
                System.out.println("¡Nota " + nota + " agregada correctamente a " + alu.getNombreCompleto() + "!");
                return true;
            }
        }

        System.out.println("Error: No se encontró ningún alumno registrado con esa cédula.");
        return false;
    }

    public void deshacerRegistro() {
        if (pilaAccionesNotas.isEmpty()) {
            System.out.println("\nNo hay acciones recientes de notas para deshacer.");
            return;
        }
        String cedulaAfectada = pilaAccionesNotas.pop();
        for (Alumno alu : listaAlumnos) {
            if (alu.getCedula().trim().equalsIgnoreCase(cedulaAfectada.trim())) {
                if (!alu.getNotas().isEmpty()) {
                    double notaRemovida = alu.getNotas().remove(alu.getNotas().size() - 1);
                    guardarAlumnosEnDisco();
                    System.out.println("\n[Deshacer LIFO]: Se eliminó la última nota (" + notaRemovida + ") de " + alu.getNombreCompleto() + ".");
                } else {
                    System.out.println("\nEl alumno " + alu.getNombreCompleto() + " ya no tiene notas para remover.");
                }
                return;
            }
        }
    }

    public void generarColaCertificados() {
        List<Alumno> graduandos = new ArrayList<>();
        for (Alumno alu : listaAlumnos) {
            if (alu.getNotas().size() == 3 && alu.getProgramaAsignado().verificarAprobacion(alu)) {
                graduandos.add(alu);
            }
        }

        try (PrintWriter pw = new PrintWriter(new FileWriter("java/certificados_pendientes.txt"))) {
            pw.println("===");
            pw.println("REPORTE DE CERTIFICADOS PENDIENTES");
            pw.println("=========================================");
            pw.println("Total de graduandos en cola: " + graduandos.size());
            int idx = 1;
            for (Alumno grad : graduandos) {
                double prom = grad.calcularPromedio();
                String estatusExtra = "";
                if (grad.getProgramaAsignado().getNombreDePrograma().equalsIgnoreCase("bootcamp")) {
                    estatusExtra = " - Estatus: APROBADO (Cumple regla de ninguna nota < 14)";
                }
                pw.println(idx + ". [" + grad.getCedula() + "] " + grad.getNombreCompleto());
                pw.println("- Programa: " + grad.getProgramaAsignado().getNombreDePrograma());
                pw.println(String.format("- Promedio Final: %.1f", prom));
                pw.println("- Estatus: APROBADO" + estatusExtra);
                idx++;
            }
            pw.println("=========================================");
            pw.println("* Fin del reporte - Generado por SGA-DO *");
        } catch (IOException e) {
            System.out.println("Error al escribir java/certificados_pendientes.txt: " + e.getMessage());
        }
        System.out.println("\n¡Cola de certificados generada con éxito en 'java/certificados_pendientes.txt'!");
    }

    public void generarReporte() {
        System.out.println("\n=========================================");
        System.out.println(" REPORTE GENERAL - SISTEMA");
        System.out.println("=========================================");
        System.out.println("--- PROFESORES ---");
        if (listaProfesores.isEmpty()) {
            System.out.println("No hay profesores registrados.");
        } else {
            for (Profesor p : listaProfesores) {
                System.out.println("[" + p.getCedula() + "] " + p.getNombreCompleto() + " | Correo: " + p.getCorreo() + " | Esp: " + p.getEspecialidadAcademica() + " | Mat: " + p.getMateria());
            }
        }

        System.out.println("\n--- ALUMNOS ---");
        if (listaAlumnos.isEmpty()) {
            System.out.println("No hay alumnos registrados.");
        } else {
            for (Alumno a : listaAlumnos) {
                double prom = a.calcularPromedio();
                boolean aprobado = a.getProgramaAsignado().verificarAprobacion(a);
                String estatus = aprobado ? "APROBADO" : "REPROBADO";
                System.out.println(String.format("[%s] %s | Prog: %s | Notas: %s | Prom: %.1f | Estatus: %s",
                        a.getCedula(), a.getNombreCompleto(), a.getProgramaAsignado().getNombreDePrograma(), a.getNotas().toString(), prom, estatus));
            }
        }
        System.out.println("=========================================\n");
    }
}


public class Sistema {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        GestorLogica sistema = new GestorLogica();

        while (true) {
            System.out.println("\n=========================================");
            System.out.println("=== SGA-DO: SISTEMA DIPLOMADOSONLINE ===");
            System.out.println("=========================================");
            System.out.println("1. Registrar Alumno");
            System.out.println("2. Registrar Profesor");
            System.out.println("3. Registrar Notas a un Alumno");
            System.out.println("4. Deshacer Ultimo Registro de Nota");
            System.out.println("5. Generar Cola de Certificados");
            System.out.println("6. Mostrar Reporte General");
            System.out.println("7. Salir");
            System.out.print("Seleccione una opción (1-7): ");

            String opcionStr = scanner.nextLine().trim();
            if (opcionStr.isEmpty()) {
                System.out.println("Error: Debe ingresar una opción válida.");
                continue;
            }

            int opcionInt;
            try {
                opcionInt = Integer.parseInt(opcionStr);
            } catch (NumberFormatException e) {
                System.out.println("Error: Ingrese un valor numérico válido.");
                continue;
            }

            switch (opcionInt) {
                case 1:
                    System.out.println("\n--- REGISTRAR ALUMNO ---");
                    System.out.print("Ingrese Cédula/ID (Ej. V-999): ");
                    String cedulaAlu = scanner.nextLine().trim();
                    System.out.print("Ingrese Nombre Completo: ");
                    String nombreAlu = scanner.nextLine().trim();
                    System.out.print("Ingrese Correo Electrónico: ");
                    String correoAlu = scanner.nextLine().trim();
                    System.out.print("Ingrese Tipo de Programa (Curso, Diplomado, Bootcamp): ");
                    String tipoProg = scanner.nextLine().trim();
                    Alumno nuevoAlumno = new Alumno(cedulaAlu, nombreAlu, correoAlu, tipoProg);
                    sistema.registrarAlumno(nuevoAlumno);
                    break;

                case 2:
                    System.out.println("\n--- REGISTRAR PROFESOR ---");
                    System.out.print("Ingrese Cédula/ID (Ej. V-999): ");
                    String cedulaProf = scanner.nextLine().trim();
                    System.out.print("Ingrese Nombre Completo: ");
                    String nombreProf = scanner.nextLine().trim();
                    System.out.print("Ingrese Correo Electrónico: ");
                    String correoProf = scanner.nextLine().trim();
                    System.out.print("Ingrese Especialidad Académica: ");
                    String especialidad = scanner.nextLine().trim();
                    System.out.print("Ingrese Materia Asignada: ");
                    String materia = scanner.nextLine().trim();
                    Profesor nuevoProfesor = new Profesor(cedulaProf, nombreProf, correoProf, especialidad, materia);
                    sistema.registrarProfesor(nuevoProfesor);
                    break;

                case 3:
                    System.out.println("\n--- REGISTRAR NOTAS A UN ALUMNO ---");
                    System.out.print("Ingrese Cédula del Alumno (Ej. V-999): ");
                    String cedulaNota = scanner.nextLine().trim();
                    System.out.print("Ingrese la calificación (0-20): ");
                    try {
                        double nota = Double.parseDouble(scanner.nextLine().trim());
                        sistema.registrarNotas(cedulaNota, nota);
                    } catch (NumberFormatException e) {
                        System.out.println("Error: Ingrese un valor numérico válido para la nota.");
                    }
                    break;

                case 4:
                    System.out.println("\n--- DESHACER ÚLTIMO REGISTRO DE NOTA ---");
                    sistema.deshacerRegistro();
                    break;

                case 5:
                    System.out.println("\n--- GENERAR COLA DE CERTIFICADOS ---");
                    sistema.generarColaCertificados();
                    break;

                case 6:
                    sistema.generarReporte();
                    break;

                case 7:
                    System.out.println("\nSaliendo del sistema de forma segura. ¡Hasta luego!");
                    scanner.close();
                    return;

                default:
                    System.out.println("Opción inválida. Seleccione un número entre 1 y 7.");
                    break;
            }
        }
    }
}