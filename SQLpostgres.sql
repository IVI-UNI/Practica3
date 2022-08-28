
SELECT 'DROP DATABASE universidad'
WHERE EXISTS (SELECT FROM pg_database WHERE datname = 'universidad')\gexec

CREATE DATABASE universidad;

\c universidad

CREATE TABLE Asignatura(
	Codigo INT PRIMARY KEY,
	NombreAsignatura text NOT NULL
);
	
CREATE TABLE Empleado(
	DNI TEXT PRIMARY KEY,
	Nombre TEXT ,
	Apellido1 TEXT,
	Apellido2 TEXT
);
	
CREATE TABLE ProyectoInvestigador(
	RefProyecto INT PRIMARY KEY,
	Titulo TEXT ,
	FechaInicio DATE,
	FechaFin DATE 
);
	
CREATE TABLE Docente(
	puestoDocente TEXT,
	PRIMARY KEY (DNI)
)INHERITS (Empleado);

CREATE TABLE Investigador(
	ORCID TEXT,
	PRIMARY KEY (DNI)
)INHERITS (Empleado);

CREATE TABLE PDI(
	PDI_ID SERIAL PRIMARY KEY,
	DNI_Investigador TEXT,
	DNI_Docente TEXT,
  	CONSTRAINT fk_PDI_docente
    FOREIGN KEY(DNI_Docente) 
	REFERENCES Docente(DNI),
	CONSTRAINT fk_PDI_investigador
    FOREIGN KEY(DNI_Investigador) 
	REFERENCES Investigador(DNI)
);

CREATE TABLE imparte(
	Cod_Asignatura INT,
	Cod_Docente TEXT,
	CONSTRAINT fk_docente
    FOREIGN KEY(Cod_Docente) 
	REFERENCES Docente(DNI),
	CONSTRAINT fk_asignatura
    FOREIGN KEY(Cod_Asignatura) 
	REFERENCES Asignatura(Codigo),
	PRIMARY KEY (Cod_Asignatura,Cod_Docente)
);

CREATE TABLE participa(
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

INSERT INTO Asignatura(Codigo, NombreAsignatura)
VALUES(1,'Bases de Datos'), (2, 'Inglés'), 
		(3, 'Ingniería web'), (4, 'Programación I'),
		(5, ' Seguridad informática'), (6, 'Matemáticas');
		
INSERT INTO Empleado (DNI, Nombre, Apellido1, Apellido2)
VALUES ('23854067P', 'Juan', 'Martínez', 'García'),
		('37492099L', 'Marcos', 'Perez', 'Sanchez'),
		('67559902Q', 'Lucas', 'Almeida', 'Cortes'),
		('10284277N', 'Roberto', 'Vaquero', 'Furia'),
		('85065830C', 'Carlos', 'Lopez', 'Castillo');

INSERT INTO ProyectoInvestigador(RefProyecto, Titulo, FechaInicio, FechaFin)
VALUES(3245, 'Horizonte 2030','2022-01-13','2022-08-24'),
		(7321, 'IA en la matemática','2022-03-30','2022-10-15'),
		(5390, 'Accesibilidad de apps bancarias','2021-08-24','2021-08-31'),
		(6113, 'Computación cuántica','2018-08-24','2025-04-30'),
		(2271, 'Biotecnología','2022-01-01','2023-06-20'),
		(0420, 'Grafeno en la electrónica','2023-02-23','2028-09-11');

INSERT INTO Docente (DNI, puestoDocente)
VALUES('23854067P', 'Profesor'), ('37492099L', 'Profesor interino'),
		('85065830C','Jefe de Departamento');

INSERT INTO Investigador (DNI, ORCID)
VALUES ('67559902Q', 'Jefe de proyecto'), ('10284277N','Becario'),
		('37492099L','Becario');

INSERT INTO PDI (DNI_Investigador, DNI_Docente)
VALUES (NULL, '23854067P'), ('67559902Q', NULL);

INSERT INTO imparte(Cod_Asignatura, Cod_Docente)
VALUES ('1', '23854067P'), ('2','23854067P'), ('4','23854067P'), 
		('1','37492099L'), ('3','85065830C'),('5','85065830C'),
		('4','37492099L'), ('6','37492099L'),('2','37492099L'),
		('3','37492099L');
		
INSERT INTO participa(Cod_Proyecto, Cod_Investigador)
VALUES (6113, '67559902Q'), (6113,'10284277N'), (2271,'10284277N'), 
		(2271,'37492099L'), (5390,'37492099L'),(0420,'10284277N'),
		(0420,'37492099L'), (0420,'67559902Q'),(7321,'10284277N'),
		(7321,'67559902Q');