/* Enunciado: Seleccionar todos los empleados
Resultado: Devuelve todos los empleados con todas las columnas de su tabla

*/
SELECT DISTINCT * FROM empleado;

/* Seleccionar sólo los empleados docentes que no sean investigadores
	Resultado: Devuelve los DNI de los empleados docentes y no investigadores
*/
SELECT DISTINCT (dni)
FROM empleado
WHERE dni IN (SELECT dni 
		      FROM DOCENTE 
			  WHERE dni NOT IN (SELECT dni
							  	FROM Investigador));

/* Enunciado: Seleccionar los docentes que sea docentes e investigadores
	Resultado; Devuelve el dni de los empleados docentes e investigadores a la vez
*/
SELECT DISTINCT (dni)
FROM empleado
WHERE dni IN (SELECT dni 
		      FROM DOCENTE 
			  WHERE dni IN (SELECT dni
							  	FROM Investigador));
								 
/* Enunciado: Mostrar todos los empleados cuyo primer apellido sea “Gracia”.
	Resultado: Devuelve el unico empleado con apellido "Gracia", un Investigador
*/
SELECT DISTINCT *
FROM empleado
WHERE apellido1 ='Gracia'; 

/* Enunciado: Seleccionar todas las asignaturas impartidas por dos profesores 
	o más (JOIN + herencia + agrupación + contar)
	Resultado: Devuelve las asignaturas que tienen más de dos profesores asignados

*/
SELECT NombreAsignatura, COUNT(*) AS "Profesores que la imparten"
FROM imparte INNER JOIN Asignatura ON imparte.Cod_Asignatura = Asignatura.Codigo
GROUP BY NombreAsignatura
HAVING COUNT(*) > 1; 

/* Enunciado: Tabla en la que se muestre empleado y carga de trabajo.
				La carga es la suma de asignaturas y proyectos
	Resultado: Muestra la carga de trabajo segun está definida en el enunciado
			para cada empleado.
*/
SELECT codigo, SUM( la_union.TRABAJADO) AS "CARGA DE TRABAJO"
FROM(SELECT Cod_Docente codigo, COUNT(*) AS TRABAJADO
FROM imparte 
GROUP BY Cod_Docente
UNION ALL
SELECT Cod_Investigador codigo, COUNT(*) AS TRABAJADO
FROM participa 
GROUP BY Cod_Investigador) la_union
GROUP BY codigo;

