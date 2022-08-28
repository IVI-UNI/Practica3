
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
psql -U postgres universidad -c "INSERT INTO Docente (DNI, Nombre, Apellido1, Apellido2, puestoDocente)
								 VALUES ('48395720N', 'Pedro', 'Sosa', 'Santos', 'Profesor'), 
								 		('94384022Q', 'Marc', 'Conejos', 'Pedraz', 'Profesor interino'),
										('48509910T', 'Enrique', 'Suñen', 'Corlat','Jefe de Departamento'); "

#Inserción de datos en la tabla Investigador
echo "Insertando datos en la tabla Investigador"
psql -U postgres universidad -c "INSERT INTO Investigador (DNI, Nombre, Apellido1, Apellido2, ORCID)
								 VALUES ('99481239I', 'Jhon', 'Smith', 'Anthony', 'Jefe de proyecto'),
								 		('42870012L', 'Andres', 'Berdejo', 'Soler','Becario'),
										('73998241F', 'Dionisio', 'Gracia', 'Portat','Becario'),
										('48509910T', 'Enrique', 'Suñen', 'Corlat','Jefe de Proyecto'); "

#Inserción de datos en la tabla Imparte
echo "Insertando datos en la tabla Imparte"
psql -U postgres universidad -c "INSERT INTO Imparte(Cod_Asignatura, Cod_Docente)
								 VALUES ('1','48395720N'),
								 		('2','94384022Q'),
								 		('4','48395720N'), 
										('1','48509910T'),
										('3','48509910T'),
										('5','48509910T'),
										('4','94384022Q'),
										('6','48509910T'),
										('2','48509910T'),
										('3','48395720N'); "

#Inserción de datos en la tabla Participa
echo "Insertando datos en la tabla Participa"
psql -U postgres universidad -c "INSERT INTO Participa(Cod_Proyecto, Cod_Investigador)
								 VALUES (6113, '99481239I'), 
								 		(6113,'42870012L'), 
								 		(2271,'73998241F'), 
										(2271,'99481239I'), 
										(5390,'42870012L'),
										(0420,'73998241F'),
										(0420,'99481239I'), 
										(0420,'42870012L'),
										(7321,'73998241F'),
										(7321,'99481239I'),
										(7321,'48509910T'); "




		




: << 'OUT'


INSERT INTO PDI (DNI_Investigador, DNI_Docente)
VALUES (NULL, '23854067P'), ('67559902Q', NULL);


		

OUT