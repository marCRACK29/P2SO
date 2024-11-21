<h1 align="center">Proyecto Simulador de hebras Productoras-Consumidoras y un Simulador de Memoria Virtual con Paginación</h1>
<h3 align="center">Sistemas Operativos</h3>
<h2>Integrantes: </h2>
<ul>
  <li>Jesús Guevara Salcedo</li>
  <li>Rodrigo Bascuñán León</li>
  <li>Gabriel Castillo Castillo</li>
  <li>Marcos Martínez Rojas</li>
</ul>
<h2>Instrucciones de Uso</h2>

<p>Para compilar y ejecutar el proyecto, sigue estos pasos desde la carpeta raíz: </p>

<h3>1. Compilación</h3>
<p>Ingrese el siguiente comando en la terminal:</p>
<pre>
<code>make</code>
</pre>
<p>Este comando compilará los dos programas: <code>simulapc</code> para el problema de productor-consumidor y <code>memoria</code> para el problema de memoria virtual.</p>

<h3>2. Ejecución</h3>
<p>Una vez compilado, tienes dos opciones:</p>

<h4>Parte 1: Simulador de hebras Productoras-Consumidoras </h4>
<p>Ejecuta el siguiente comando:</p>
<pre>
<code>./simulapc -p [int] -c [int] -s [int] -t [int]</code>
</pre>
<ul>
  <li><code>-p</code>: Número de hebras productoras.</li>
  <li><code>-c</code>: Número de hebras consumidoras.</li>
  <li><code>-s</code>: Tamaño inicial de la cola.</li>
  <li><code>-t</code>: Tiempo máximo de espera en segundos para los consumidores.</li>
</ul>
Una vez ejecutado, podrá ver el comportamiento de las hebras productoras y consumidoras en la terminal, además de un archivo log.txt con la misma información. 

<h4>Parte 2: Simulador de Memoria Virtual</h4>
<p>Ejecuta el siguiente comando:</p>
<pre>
<code>./memoria -m [int] -a [argumento] -f [argumento]</code>
</pre>
<ul>
  <li><code>-m</code>: Número de marcos de página.</li>
  <li><code>-a</code>: Algoritmo de reemplazo (<code>FIFO</code>, <code>LRU</code>, <code>LRUR</code>, <code>OPTIMO</code>).</li>
  <li><code>-f</code>: Archivo .txt con referencias de páginas  (por defecto en el proyecto se encuentra referencias.txt). </li>
</ul>
Una vez ejecutado, podrá ver el número de fallos de página dependiendo de la cantidad de marcos y el algoritmo de reemplazo seleccionado. 


<h3>3. Limpieza de archivos ejecutables (opcional): </h3>
Para limpiar la carpeta src de los archivos generados por el comando make, se debe ingresar el siguiente comando en la terminal (estando en la carpeta raiz):

```sh
make clean
```

<h3 align="left">Herramientas utilizadas: </h3>
<p align="left"> 
  <p align="left"> <a href="https://www.w3schools.com/cpp/" target="_blank" rel="noreferrer"> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/cplusplus/cplusplus-original.svg" alt="cplusplus" width="40" height="40"/> </a> <a href="https://git-scm.com/" target="_blank" rel="noreferrer"> <img src="https://www.vectorlogo.zone/logos/git-scm/git-scm-icon.svg" alt="git" width="40" height="40"/> </a> <a href="https://www.linux.org/" target="_blank" rel="noreferrer"> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/linux/linux-original.svg" alt="linux" width="40"  

  <a href="https://valgrind.org/" target="_blank" rel="noreferrer"> 
    <img src="https://upload.wikimedia.org/wikipedia/en/thumb/f/f9/Valgrind_logo.png/200px-Valgrind_logo.png" alt="Valgrind" width="40" height="40"/> 
  </a> 
    <a href="https://www.gnu.org/software/make/" target="_blank" rel="noreferrer"> 
        <img src="https://static1.makeuseofimages.com/wordpress/wp-content/uploads/2023/05/the-popular-makefile-logo.jpg" alt="Make" width="60" height="40"/>
    </a>
</p>
