import os

class programaAcademico:
    def __init__(self, nombre, duracion):
        self.nombredePrograma = nombre
        self.duracion = duracion

    def obtener_nombre(self, alumno):
        return self.nombredePrograma

    def calcular_promedio(self, alumno):
        if not alumno.notas:
            return 0.0
        return sum(alumno.notas) / len(alumno.notas)


class curso(programaAcademico):
    def __init__(self):
        super().__init__("Curso", "Corto")

    def verificar_aprobacion(self, alumno):
        if len(alumno.notas) < 3:
            return False
        promedio = sum(alumno.notas) / len(alumno.notas)
        return promedio >= 10.0

class diplomado(programaAcademico):
    def __init__(self):
        super().__init__("Diplomado", "Mediano")

    def verificar_aprobacion(self, alumno):
        if len(alumno.notas) < 3:
            return False
        promedio = sum(alumno.notas) / len(alumno.notas)
        return promedio >= 14.0

class bootcamp(programaAcademico):
    def __init__(self):
        super().__init__("Bootcamp", "Intensivo")

    def verificar_aprobacion(self, alumno):
        if len(alumno.notas) < 3:
            return False
        for nota in alumno.notas:
            if nota < 14.0:
                return False
        return True


class persona:
    def __init__(self, cedula, nombre_completo, correo):
        self.cedula = cedula
        self.nombre_completo = nombre_completo
        self.correo = correo

    def registra_cedula(self, valor):
        self.cedula = valor

    def registra_nombre_completo(self, valor):
        self.nombre_completo = valor

    def registra_correo(self, valor):
        self.correo = valor

class alumno(persona):
    def __init__(self, cedula, nombre_completo, correo, tipo_programa):
        super().__init__(cedula, nombre_completo, correo)
        self.notas = []
        tipo_lower = tipo_programa.lower()
        if "curso" in tipo_lower:
            self.programa_asignado = curso()
        elif "diplomado" in tipo_lower:
            self.programa_asignado = diplomado()
        elif "bootcamp" in tipo_lower:
            self.programa_asignado = bootcamp()
        else:
            self.programa_asignado = curso()

    def agregar_nota(self, nota):
        self.notas.append(nota)

    def calcular_promedio(self):
        if not self.notas:
            return 0.0
        return sum(self.notas) / len(self.notas)

    def inscribirse_en_programa(self, programa):
        self.programa_asignado = programa

class profesor(persona):
    def __init__(self, cedula, nombre_completo, correo, especialidad, materia):
        super().__init__(cedula, nombre_completo, correo)
        self.especialidad_academica = especialidad
        self.materia = materia

    def actualizar_especialidad(self, especialidad):
        self.especialidad_academica = especialidad

    def cambiar_materia(self, materia):
        self.materia = materia

    def evaluar_alumno(self, alumno_obj, nota):
        alumno_obj.agregar_nota(nota)


class sistema:
    def __init__(self):
        self.lista_alumnos = []
        self.lista_profesores = []
        self.certificaciones = []  
        self.pila_acciones_notas = []
        self.cargar_datos()

    def cargar_datos(self):
        if os.path.exists("alumnos.txt"):
            try:
                with open("alumnos.txt", "r", encoding="utf-8") as f:
                    for linea in f:
                        if not linea.strip():
                            continue
                        partes = [p.strip() for p in linea.split(",")]
                        if len(partes) >= 4:
                            cedula, nombre, correo, tipo_prog = partes[0], partes[1], partes[2], partes[3]
                            alu = alumno(cedula, nombre, correo, tipo_prog)
                            # Lee todas las notas disponibles a partir del índice 4
                            for nota_str in partes[4:]:
                                if nota_str:
                                    try:
                                        val_nota = float(nota_str)
                                        if 0.0 <= val_nota <= 20.0:
                                            alu.agregar_nota(val_nota)
                                    except ValueError:
                                        pass
                            self.lista_alumnos.append(alu)
            except Exception as e:
                print(f"Error al leer alumnos.txt: {e}")

        if os.path.exists("profesores.txt"):
            try:
                with open("profesores.txt", "r", encoding="utf-8") as f:
                    for linea in f:
                        if not linea.strip():
                            continue
                        partes = [p.strip() for p in linea.split(",")]
                        if len(partes) >= 5:
                            cedula, nombre, correo, esp, mat = partes[0], partes[1], partes[2], partes[3], partes[4]
                            prof = profesor(cedula, nombre, correo, esp, mat)
                            self.lista_profesores.append(prof)
            except Exception as e:
                print(f"Error al leer profesores.txt: {e}")

    def guardar_alumnos_en_disco(self):
        with open("alumnos.txt", "w", encoding="utf-8") as f:
            for alu in self.lista_alumnos:
                if alu.notas:
                    notas_str = ",".join([str(n) for n in alu.notas])
                    f.write(f"{alu.cedula},{alu.nombre_completo},{alu.correo},{alu.programa_asignado.nombredePrograma},{notas_str}\n")
                else:
                    f.write(f"{alu.cedula},{alu.nombre_completo},{alu.correo},{alu.programa_asignado.nombredePrograma}\n")

    def guardar_profesores_en_disco(self):
        with open("profesores.txt", "w", encoding="utf-8") as f:
            for prof in self.lista_profesores:
                f.write(f"{prof.cedula},{prof.nombre_completo},{prof.correo},{prof.especialidad_academica},{prof.materia}\n")

    def registrar_alumno(self, alu):
        for existente in self.lista_alumnos:
            if existente.cedula.strip().upper() == alu.cedula.strip().upper():
                print(f"\nError: Ya existe un alumno registrado con la cédula '{alu.cedula}'.")
                return False

        self.lista_alumnos.append(alu)
        self.guardar_alumnos_en_disco()
        print(f"\n¡Alumno {alu.nombre_completo} registrado con éxito!")
        return True

    def registrar_profesor(self, prof):
        for existente in self.lista_profesores:
            if existente.cedula.strip().upper() == prof.cedula.strip().upper():
                print(f"\nError: Ya existe un profesor registrado con la cédula '{prof.cedula}'.")
                return False

        self.lista_profesores.append(prof)
        self.guardar_profesores_en_disco()
        print(f"\n¡Profesor {prof.nombre_completo} registrado con éxito!")
        return True

    def registrar_notas(self, cedula, nota):
        if not (0.0 <= nota <= 20.0):
            print(f"Error: Nota no válida ({nota}). La calificación tiene que ser del 0 al 20.")
            return False

        for alu in self.lista_alumnos:
            if alu.cedula.strip().upper() == cedula.strip().upper():
                if len(alu.notas) >= 3:
                    print(f"Error: El alumno {alu.nombre_completo} ya ha llenado las 3 notas permitidas.")
                    return False
                alu.agregar_nota(nota)
                self.pila_acciones_notas.append(alu.cedula)
                self.guardar_alumnos_en_disco()
                print(f"¡Nota {nota} agregada correctamente a {alu.nombre_completo}!")
                return True
                
        print("Error: No se encontró ningún alumno registrado con esa cédula.")
        return False

    def deshacer_registro(self, id_cedula=None):
        if not self.pila_acciones_notas:
            print("\nNo hay acciones recientes de notas para deshacer.")
            return
        cedula_afectada = self.pila_acciones_notas.pop()
        for alu in self.lista_alumnos:
            if alu.cedula.strip().upper() == cedula_afectada.strip().upper():
                if alu.notas:
                    nota_removida = alu.notas.pop()
                    self.guardar_alumnos_en_disco()
                    print(f"\n[Deshacer LIFO]: Se eliminó la última nota ({nota_removida}) de {alu.nombre_completo}.")
                else:
                    print(f"\nEl alumno {alu.nombre_completo} ya no tiene notas para remover.")
                return

    def generar_cola_certificados(self):
        graduandos = []
        for alu in self.lista_alumnos:
            if len(alu.notas) == 3 and alu.programa_asignado.verificar_aprobacion(alu):
                graduandos.append(alu)

        self.certificaciones = graduandos
        with open("certificados_pendientes.txt", "w", encoding="utf-8") as f:
            f.write("===\nREPORTE DE CERTIFICADOS PENDIENTES\n=========================================\n")
            f.write(f"Total de graduandos en cola: {len(graduandos)}\n")
            for idx, grad in enumerate(graduandos, 1):
                prom = grad.calcular_promedio()
                estatus_extra = ""
                if grad.programa_asignado.nombredePrograma.lower() == "bootcamp":
                    estatus_extra = " - Estatus: APROBADO (Cumple regla de ninguna nota < 14)"
                f.write(f"{idx}. [{grad.cedula}] {grad.nombre_completo}\n")
                f.write(f"- Programa: {grad.programa_asignado.nombredePrograma}\n")
                f.write(f"- Promedio Final: {prom:.1f}\n")
                f.write(f"- Estatus: APROBADO{estatus_extra}\n")
            f.write("=========================================\nFin del reporte\n")
        print("\n¡Cola de certificados generada con éxito en 'certificados_pendientes.txt'!")

    def generar_reporte(self):
        print("\n=========================================")
        print(" REPORTE GENERAL - SISTEMA")
        print("=========================================")
        print("--- PROFESORES ---")
        if not self.lista_profesores:
            print("No hay profesores registrados.")
        else:
            for p in self.lista_profesores:
                print(f"[{p.cedula}] {p.nombre_completo} | Correo: {p.correo} | Esp: {p.especialidad_academica} | Mat: {p.materia}")

        print("\n--- ALUMNOS ---")
        if not self.lista_alumnos:
            print("No hay alumnos registrados.")
        else:
            for a in self.lista_alumnos:
                prom = a.calcular_promedio()
                aprobado = a.programa_asignado.verificar_aprobacion(a)
                estatus = "APROBADO" if aprobado else "REPROBADO"
                print(f"[{a.cedula}] {a.nombre_completo} | Prog: {a.programa_asignado.nombredePrograma} | Notas: {a.notas} | Prom: {prom:.1f} | Estatus: {estatus}")
        print("=========================================\n")


# --- MENÚ INTERACTIVO ---
def main():
    s = sistema()
    while True:
        print("\n=========================================")
        print("=== SISTEMA DIPLOMADOSONLINE ===")
        print("=========================================")
        print("1. Registrar Alumno")
        print("2. Registrar Profesor")
        print("3. Registrar Notas a un Alumno")
        print("4. Deshacer Último Registro de Nota")
        print("5. Generar Cola de Certificados")
        print("6. Mostrar Reporte General")
        print("7. Salir")
        
        try:
            opcion = input("Seleccione una opción (1-7): ").strip()
            if not opcion:
                print("Error: Debe ingresar una opción válida.")
                continue
            opcion_int = int(opcion)
        except ValueError:
            print("Error: Ingrese un valor numérico válido.")
            continue

        if opcion_int == 1:
            print("\n--- REGISTRAR ALUMNO ---")
            cedula = input("Ingrese Cédula/ID (Ej. V-999): ").strip()
            nombre = input("Ingrese Nombre Completo: ").strip()
            correo = input("Ingrese Correo Electrónico: ").strip()
            tipo_prog = input("Ingrese Tipo de Programa (Curso, Diplomado, Bootcamp): ").strip()
            nuevo_alumno = alumno(cedula, nombre, correo, tipo_prog)
            s.registrar_alumno(nuevo_alumno)

        elif opcion_int == 2:
            print("\n--- REGISTRAR PROFESOR ---")
            cedula = input("Ingrese Cédula/ID (Ej. V-999): ").strip()
            nombre = input("Ingrese Nombre Completo: ").strip()
            correo = input("Ingrese Correo Electrónico: ").strip()
            especialidad = input("Ingrese Especialidad Académica: ").strip()
            materia = input("Ingrese Materia Asignada: ").strip()
            nuevo_profesor = profesor(cedula, nombre, correo, especialidad, materia)
            s.registrar_profesor(nuevo_profesor)

        elif opcion_int == 3:
            print("\n--- REGISTRAR NOTAS A UN ALUMNO ---")
            cedula = input("Ingrese Cédula del Alumno (Ej. V-999): ").strip()
            try:
                nota = float(input("Ingrese la calificación (0-20): "))
                s.registrar_notas(cedula, nota)
            except ValueError:
                print("Error: Ingrese un valor numérico válido para la nota.")

        elif opcion_int == 4:
            print("\n--- DESHACER ÚLTIMO REGISTRO DE NOTA ---")
            s.deshacer_registro()

        elif opcion_int == 5:
            print("\n--- GENERAR COLA DE CERTIFICADOS ---")
            s.generar_cola_certificados()

        elif opcion_int == 6:
            s.generar_reporte()

        elif opcion_int == 7:
            print("\nSaliendo del sistema de forma segura. ¡Hasta luego!")
            break
        else:
            print("Opción inválida. Seleccione un número entre 1 y 7.")

if __name__ == "__main__":
    main()