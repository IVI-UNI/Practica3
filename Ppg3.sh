
#!/bin/sh

echo "Comienza el script en postgres"
echo "Escribe la contraseña del usuario postgres: "
read -s contrasena

export PGPASSWORD=$contrasena

#Creación de la base de datos
echo "Creando la base de datos universidad, eliminando versiones anteriores si existían"
psql -U postgres -c "DROP DATABASE IF EXISTS universidad"
psql -U postgres -c "CREATE DATABASE universidad;"

#Creación de la tabla Asignatura
echo "Creando tabla Asignatura"
psql -U postgres universidad -c "CREATE TABLE Asignatura(
									Codigo INT PRIMARY KEY,
									NombreAsignatura text NOT NULL
								);"

#Creación de la tabla Empleado
echo "Creando tabla Empleado"
psql -U postgres universidad -c " CREATE TABLE Empleado(
									DNI TEXT PRIMARY KEY,
									Nombre TEXT ,
									Apellido1 TEXT,
									Apellido2 TEXT
								);"

#Creación de la tabla Docente
echo "Creando tabla Docente"
psql -U postgres universidad -c " CREATE TABLE Docente(
									puestoDocente TEXT,
									PRIMARY KEY (DNI)
								)INHERITS (Empleado);"

#Creación de la tabla Investigador
echo "Creando tabla Investigador"
psql -U postgres universidad -c "CREATE TABLE Investigador(
									ORCID TEXT,
									PRIMARY KEY (DNI)
								)INHERITS (Empleado); "

#Creación de la tabla ProyectoInvestigador
echo "Creando tabla ProyectoInvestigador"
psql -U postgres universidad -c "CREATE TABLE ProyectoInvestigador(
									RefProyecto INT PRIMARY KEY,
									Titulo TEXT ,
									FechaInicio DATE,
									FechaFin DATE 
								); "

#Creación de la tabla PDI
echo "Creando tabla PDI"
psql -U postgres universidad -c "CREATE TABLE PDI(
									PDI_ID SERIAL PRIMARY KEY,
									ORCID TEXT,
									PuestoDocente TEXT
								)INHERITS(Empleado)"

#Creación de la tabla Imparte
echo "Creando tabla Imparte"
psql -U postgres universidad -c "CREATE TABLE Imparte(
									Cod_Asignatura INT,
									Cod_Docente TEXT,
									CONSTRAINT fk_docente
								    FOREIGN KEY(Cod_Docente) 
									REFERENCES Docente(DNI),
									CONSTRAINT fk_asignatura
								    FOREIGN KEY(Cod_Asignatura) 
									REFERENCES Asignatura(Codigo),
									PRIMARY KEY (Cod_Asignatura,Cod_Docente)
								); "	

#Creación de la tabla Participa
echo "Creando tabla Participa"
psql -U postgres universidad -c "CREATE TABLE Participa(
									Cod_Proyecto INT,
									Cod_Investigador TEXT,
									CONSTRAINT fk_proyecto
								    FOREIGN KEY(Cod_Proyecto) 
									REFERENCES ProyectoInvestigador(RefProyecto),
									CONSTRAINT fk_investigador
								    FOREIGN KEY(Cod_Investigador) 
									REFERENCES Investigador(DNI),
									PRIMARY KEY(Cod_Proyecto,Cod_Investigador)
								);
 "
echo "Creación del trigger y la función para insertar en la tabla PDI"
#Creación de la función que insertará en la tabla PDI cada vez que se inserte en la tabla Docente 
psql -U postgres universidad -c "CREATE OR REPLACE FUNCTION insertar_pdi_docente() RETURNS TRIGGER
                                    AS $\$

                                    BEGIN
                                        INSERT INTO PDI(DNI, Nombre, Apellido1, Apellido2, PuestoDocente) 
                                                VALUES (NEW.DNI, NEW.Nombre, NEW.Apellido1, NEW.Apellido2, NEW.PuestoDocente);                    
                                        RETURN NEW;
                                    END;
                                    $\$ LANGUAGE plpgsql;"

#Creación del trigger para insertar en PDI un docente
psql -U  postgres universidad -c" CREATE OR REPLACE TRIGGER docente_a_PDI
									AFTER INSERT OR UPDATE ON Docente
									FOR EACH ROW 
									EXECUTE PROCEDURE insertar_pdi_docente();"


#Creación de la función que insertará en la tabla PDI cada vez que se inserte en la tabla Investigador 
psql -U postgres universidad -c "CREATE OR REPLACE FUNCTION insertar_pdi_Investigador() RETURNS TRIGGER
                                    AS $\$

                                    BEGIN
                                        INSERT INTO PDI(DNI, Nombre, Apellido1, Apellido2, ORCID) 
                                                VALUES (NEW.DNI, NEW.Nombre, NEW.Apellido1, NEW.Apellido2, NEW.ORCID);                    
                                        RETURN NEW;
                                    END;
                                    $\$ LANGUAGE plpgsql;"

#Creación del trigger para insertar en PDI un docente
psql -U  postgres universidad -c" CREATE OR REPLACE TRIGGER investigador_a_PDI
									AFTER INSERT OR UPDATE ON Investigador
									FOR EACH ROW 
									EXECUTE PROCEDURE insertar_pdi_Investigador();"

#Inserción de datos en la tabla Asignatura
echo "Insertando datos en la tabla Asignatura"
psql -U postgres universidad -c "INSERT INTO Asignatura(Codigo, NombreAsignatura)
								 VALUES (1,'Bases de Datos'), (2, 'Inglés'), 
										(3, 'Ingniería web'), (4, 'Programación I'),
										(5, ' Seguridad informática'), (6, 'Matemáticas'); "

#Inserción de datos en la tabla Empleado
echo "Insertando datos en la tabla Empleado"
psql -U postgres universidad -c "INSERT INTO Empleado (DNI, Nombre, Apellido1, Apellido2)
								 VALUES ('23854067P', 'Juan', 'Martínez', 'García'),
										('37492099L', 'Marcos', 'Perez', 'Sanchez'),
										('67559902Q', 'Lucas', 'Almeida', 'Cortes'),
										('10284277N', 'Roberto', 'Vaquero', 'Furia'),
										('85065830C', 'Carlos', 'Lopez', 'Castillo'); "


#Inserción de datos en la tabla ProyectoInvestigador
echo "Insertando datos en la tabla ProyectoInvestigador"
psql -U postgres universidad -c "INSERT INTO ProyectoInvestigador(RefProyecto, Titulo, FechaInicio, FechaFin)
								 VALUES (3245, 'Horizonte 2030','2022-01-13','2022-08-24'),
										(7321, 'IA en la matemática','2022-03-30','2022-10-15'),
										(5390, 'Accesibilidad de apps bancarias','2021-08-24','2021-08-31'),
										(6113, 'Computación cuántica','2018-08-24','2025-04-30'),
										(2271, 'Biotecnología','2022-01-01','2023-06-20'),
										(0420, 'Grafeno en la electrónica','2023-02-23','2028-09-11');
 "

#Inserción de datos en la tabla Docente
echo "Insertando datos en la tabla Docente"
psql -U postgres universidad -c "INSERT INTO Docente (DNI, puestoDocente)
								 VALUES ('23854067P', 'Profesor'), 
								 		('37492099L', 'Profesor interino'),
										('85065830C','Jefe de Departamento'); "

#Inserción de datos en la tabla Investigador
echo "Insertando datos en la tabla Investigador"
psql -U postgres universidad -c "INSERT INTO Investigador (DNI, ORCID)
								 VALUES ('67559902Q', 'Jefe de proyecto'),
								 		('10284277N','Becario'),
										('37492099L','Becario'); "

#Inserción de datos en la tabla Imparte
echo "Insertando datos en la tabla Imparte"
psql -U postgres universidad -c "INSERT INTO Imparte(Cod_Asignatura, Cod_Docente)
								 VALUES ('1', '23854067P'),
								 		('2','23854067P'),
								 		('4','23854067P'), 
										('1','37492099L'),
										('3','85065830C'),
										('5','85065830C'),
										('4','37492099L'),
										('6','37492099L'),
										('2','37492099L'),
										('3','37492099L'); "

#Inserción de datos en la tabla Participa
echo "Insertando datos en la tabla Participa"
psql -U postgres universidad -c "INSERT INTO Participa(Cod_Proyecto, Cod_Investigador)
								 VALUES (6113, '67559902Q'), 
								 		(6113,'10284277N'), 
								 		(2271,'10284277N'), 
										(2271,'37492099L'), 
										(5390,'37492099L'),
										(0420,'10284277N'),
										(0420,'37492099L'), 
										(0420,'67559902Q'),
										(7321,'10284277N'),
										(7321,'67559902Q'); "




		




: << 'OUT'


INSERT INTO PDI (DNI_Investigador, DNI_Docente)
VALUES (NULL, '23854067P'), ('67559902Q', NULL);


		

OUT