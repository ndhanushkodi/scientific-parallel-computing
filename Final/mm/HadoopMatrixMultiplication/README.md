Input Format:
a, 0, 0, 10 (In matrix a, the value at row 0, col 0 is 10.)
a, 3, 4, 10
...
b, 2, 3, 10 (In matrix b, the value at row 2, col 3 is 10.)
...

To run:
In the Final/mm/HadoopMatrixMultiplication directory, make sure there is an input folder with lines as shown in the input format. 

`hadoop com.sun.tools.javac.Main Matrix.java`

`jar cf mm.jar Matrix*.class`

`hadoop jar mm.jar Matrix input output`
