-------------------------------------------------------------------------------
                 Tops - A mesh data structures with Topological Operators
                                     Version 1.0
                                    January, 2006
 
                            Luiz Velho <lvelho@impa.br>
                            Esdras Medeiros <esdras@impa.br>
		           Hélio Lopes <lopes@mat.puc-rio.br>


 Copyright (C) 2006.  See "GPL.txt" for details.


 For the latest version, up-to-date 
 information and ducumentation please visit
 
    http://www.impa.br/~esdras/tops


 Luiz Velho, Esdras Medeiros 
 IMPA - Instituto de Matematica Pura e Aplicada 
 Estrada Dona Castorina, 110
 Rio de Janeiro, RJ 
 22460-320, Brazil 

 Hélio Lopes
 Puc-Rio - Pontifícia Universidade Católica do Rio de Janeiro 
 Departamento de Matemática
 Rua Marquês de São Vicente, 255
 Rio de Janeiro, RJ
 22453-900, Brazil

------------------------------------------------------------------------------------

OVERVIEW
--------

Tops is unified framework
for basic operations on combinatorial 2-manifolds with 
or without boundary. There are two kinds of primitive operators
on the underlying meshes:
operators which change the topological characteristic or nature 
of the mesh and operators which just modify its combinatorial
structure. Such operators  provide a complete set of elementary operations
for mesh modification.
The mesh representation is based on a half-edge data structure
without any extra storage requirements.


BUILDING & INSTALLATION
-----------------------

To build the library and test application:

        (1) cd tops_1.0
        (2) make install

To run the demo application:

        (1) cd bin
        (2) demo


------------------------------------------------------------------------
        

