<h1 align="center">Proyecto Simulador de hebras Productoras-Consumidoras y un Simulador de Memoria Virtual con Paginación</h1>
<h3 align="center">Sistemas Operativos</h3>
<h2>Instrucciones de Uso</h2>

<p>Para compilar y ejecutar el proyecto, sigue estos pasos desde la carpeta <code>src</code>:</p>

<h3>1. Compilación</h3>
<p>Ingresa el siguiente comando en la terminal:</p>
<pre>
<code>make</code>
</pre>

<h3>2. Ejecución</h3>
<p>Una vez compilado, tienes dos opciones:</p>

<h4>Parte 1: Simulador de hebras Productoras-Consumidoras </h4>
<p>Ejecuta el siguiente comando:</p>
<pre>
<code>./simulapc -p [argumento] -c [argumento] -s [argumento] -t [argumento]</code>
</pre>
<ul>
  <li><code>-p</code>: Número de hebras productoras.</li>
  <li><code>-c</code>: Número de hebras consumidoras.</li>
  <li><code>-s</code>: Tamaño inicial de la cola.</li>
  <li><code>-t</code>: Tiempo máximo de espera de los consumidores.</li>
</ul>

<h4>Parte 2: Simulador de Memoria Virtual</h4>
<p>Ejecuta el siguiente comando:</p>
<pre>
<code>./memoria -m [argumento] -a [argumento] -f [argumento]</code>
</pre>
<ul>
  <li><code>-m</code>: Número de marcos de página.</li>
  <li><code>-a</code>: Algoritmo de reemplazo (<code>FIFO</code>, <code>LRU</code>, <code>lruReloj</code>, <code>OPTIMO</code>).</li>
  <li><code>-f</code>: Archivo con referencias de páginas  (por defecto en el proyecto se encuentra referencias.txt). </li>
</ul>



### Limpieza carpeta src (opcional):
Para limpiar la carpeta src de los archivos generados por el comando make, se debe ingresar el siguiente comando en la terminal (estando en la carpeta src):

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
