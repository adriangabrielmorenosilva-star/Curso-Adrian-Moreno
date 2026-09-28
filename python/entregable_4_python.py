import os

class ProgramaAcademico:
    def __init__(self,nombre_programa):
        self.nombre_programa = nombre_programa

    def evaluar_aprobacion (self,notas):
        if not notas:
            return False, 0.0, "REPROBADO (sin notas)"
        promedio = sum (notas) / len (notas)
        return False, promedio, "REPROBADO"


class curso(ProgramaAcademico):
    def __init__(self) :
        super().__init__("curso")

    def evaluar_aprobacion(self, notas):
        if not notas:
            return False , 0.00, "REPROBADO (Sin notas)"
        promedio = sum (notas) / len (notas)
        aprobado = promedio >= 10.0
        estatus = "Aprobado" if aprobado else "Reprobado"
        return aprobado, promedio , estatus
    
class Diplomado (ProgramaAcademico):
    def __init__(self) :
        super().__init__("Diplomado")

    def evaluar_aprobacion(self, notas):
        if not notas:
            return False , 0.00, "REPROBADO (Sin notas)"
        promedio = sum (notas) / len (notas)
        aprobado = promedio >= 14.0
        estatus = "Aprobado" if aprobado else "Reprobado"
        return aprobado, promedio , estatus
    
class Bootcamp(ProgramaAcademico):
    def __init__(self):
        super().__init__("Bootcamp")

    def evaluar_aprobacion(self, notas):
        if not notas:
            return False, 0.0, "REPROBADO (Sin notas)"
        promedio = sum(notas) / len(notas)
        ninguna_menor_14 = all(n >= 14.0 for n in notas)
        aprobado = (promedio >= 14.0) and ninguna_menor_14
        
        if aprobado:
            estatus = "APROBADO"
        else:
            estatus = "REPROBADO (Regla: Ninguna nota < 14)"
            
        return aprobado, promedio, estatus




class Persona:
    def __init__(self, cedula, nombre):
        self.cedula = cedula.strip()
        self.nombre = nombre.strip()

class Alumno(Persona):
    def __init__(self, cedula, nombre, email, tipo_programa):
        super().__init__(cedula, nombre)
        self.email = email.strip()
        self.tipo_programa = tipo_programa.strip().lower()
        self.notas = []
        
        if self.tipo_programa == "curso":
            self.programa_obj = curso()
        elif self.tipo_programa == "diplomado":
            self.programa_obj = Diplomado()
        elif self.tipo_programa == "bootcamp":
            self.programa_obj = Bootcamp()
        else:
            self.programa_obj = ProgramaAcademico(self.tipo_programa)

    def registrar_nota(self, nota):
        if len(self.notas) < 3:
            self.notas.append(float(nota))
            return True
        return False

    def eliminar_ultima_nota(self):
        if self.notas:
            return self.notas.pop()
        return None

    def evaluar(self):
        return self.programa_obj.evaluar_aprobacion(self.notas)

class profesor (Persona):
    def __init__(self, cedula, nombre, especialidad):
        super () .__init__(cedula, nombre)
        self.especialidad = especialidad.strip()

def guardar_alumnos (alumnos, archivo = "texto.txt"):
    with open(archivo, "w", encoding="utf-8") as f:
        for a in alumnos:
            n1 = a.notas[0] if len(a.notas) > 0 else 0.0
            n2 = a.notas[1] if len(a.notas) > 1 else 0.0
            n3 = a.notas[2] if len(a.notas) > 2 else 0.0
            f.write(f"{a.cedula}, {a.nombre}, {a.email}, {a.tipo_programa}, {n1}, {n2}, {n3}\n")
       

def cargar_alumnos(archivo="alumnos.txt"):
    alumnos = []
    if not os.path.exists(archivo):
        return alumnos
    with open(archivo, "r", encoding="utf-8") as f:
        for linea in f:
            partes = [p.strip() for p in linea.split(",")]
            if len(partes) >= 7:
                cedula, nombre, email, tipo_prog, n1, n2, n3 = partes[:7]
                alumno = Alumno(cedula, nombre, email, tipo_prog)
                try:
                    if float(n1) > 0: alumno.registrar_nota(float(n1))
                    if float(n2) > 0: alumno.registrar_nota(float(n2))
                    if float(n3) > 0: alumno.registrar_nota(float(n3))
                except ValueError:
                    pass
                alumnos.append(alumno)
    return alumnos


def guardar_profesores(profesores, archivo="profesores.txt"):
    with open(archivo, "w", encoding="utf-8") as f:
        for p in profesores:
            f.write(f"{p.cedula}, {p.nombre}, {p.especialidad}\n")


def cargar_profesores(archivo="profesores.txt"):
    profesores = []
    if not os.path.exists(archivo):
        return profesores
    with open(archivo, "r", encoding="utf-8") as f:
        for linea in f:
            partes = [p.strip() for p in linea.split(",")]
            if len(partes) >= 3:
                profesores.append(profesor(partes[0], partes[1], partes[2]))
    return profesores
    



  