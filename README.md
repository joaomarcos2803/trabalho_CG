# Grafos com OpenGl moderno

Animação com OpenGl 3.3+ dos algoritmos de busca em largura e profundidade em grafos com linguagem C++.
---
## Sobre

O objetivo da aplicação desenvolvida é simular, por meio de animações, a execução do algoritmo BFS e DFS. Além disso, utilizamos cubos e linhas como modelos 3D para simular os vértices e as arestas de um grafo. A aplicação permite com que o usuário realize translação, projeção e iluminação.

As orientações estão divididas nos seguintes tópicos:

- [Funcionalidades](#funcionalidades-gear)
- [Pré-requisitos e configuração](#pré-requisitos-e-configuração-hammer_and_wrench)
- [Layout](#layout-art)
- [Tecnologias](#tecnologias-technologist)
- [Contribuidores](#contribuidores)

---
## Funcionalidades :gear:

 - [x] Desenhar mais de um objeto na tela;
 - [x] Alterar a cor de cada objeto separadamente;
 - [x] Conectar os cubos com linhas;
 - [x] Animar um algoritmo de DFS;
 - [x] Animar um algoritmos de BFS;
 - [x] q ou Q: fecha o programa;
 - [x] n ou N: executa a animação do algoritmo DFS;
 - [x] m ou M: executa a animação do algoritmo BFS;
 - [x] z ou Z: aumenta o zoom da projeção;
 - [x] x ou X: diminui o zoom da projeção;
 - [x] p ou P: altera os cubos para a translação, zoom e iluminação iniciais;
 - [x] w ou W: translação positiva dos cubos no eixo Y;
 - [x] a ou A: translação negativa dos cubos no eixo X;
 - [x] s ou S: translação negativa dos cubos no eixo Y;
 - [x] d ou D: translação positiva dos cubos no eixo X.

---
## Pré-requisitos e configuração :hammer_and_wrench:
No geral, para executar a aplicação é recomendado que o sistema já possua:
 - [x] Biblioteca do OpenGL 3.3+;
 - [x] Compilador da linguagem C++ (g++ 12.2+);
 - [x] GNU Make 4.3;
 - [x] Git 2.38.1;

Para executar a aplicação siga os comandos abaixo.

```bash

# Clone este repositório com
$ git clone https://github.com/joaomarcos2803/trabalho_CG
# ou
$ git clone git@github.com:joaomarcos2803/trabalho_CG.git

# Acesse a pasta do projeto no seu terminal/cmd
$ cd trabalho_CG

# Rode o comando make
$ make

# Será criada um tela e para executar os algoritmos pressione n e m

```

---
## Layout :art:
<p align="center">
  <img alt="OpenGL" title="#First" src="https://i.imgur.com/CGI4F1R.png" width="200px">

  <img alt="OpenGL" title="#BFS" src="https://i.imgur.com/XeniZbP.png" width="200px">

  <img alt="OpenGL" title="#DFS" src="https://i.imgur.com/h46bMCZ.png" width="200px">
</p>
    
---
## Tecnologias :technologist:
    O ponto de início deste projeto foi uma aplicação com OpenGl, as dependências utilizadas estão presentes nos import do código fonte. 
---
Aplicação:

    OpenGl 3.3+:
    - Glew
    - Freeglut
    - Glm
    - Gtc
    - Gtx

---
Utilitários:

    Dev
    - Visual Studio Code 1.73
    - Makefile
---  

## Contribuidores

<table>
  <tr>
    <td align="center"><a href="https://github.com/darlosss"><img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/72506461?v=4" width="100px;" alt=""/><br /><sub><b>Carlos Eduardo</b></sub></a><br /><a href="https://github.com/darlosss/repime" title="RepiMe">:technologist:</a></td>
    <td align="center"><a href="https://github.com/MatMB115"><img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/63670910?v=4" width="100px;" alt=""/><br /><sub><b>Matheus Martins</b></sub></a><br /><a href="https://github.com/MatMB115/repime" title="RepiMe">:technologist:</a></td>
    <td align="center"><a href="https://github.com/joaomarcos2803"><img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/103376456?v=4" width="100px;" alt=""/><br /><sub><b>João Marcos</b></sub></a><br /><a href="https://github.com/joaomarcos2803" title="RepiMe">:technologist:</a></td>
    <td align="center"><a href="https://github.com/adriano-12"><img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/66391807?v=4" width="100px;" alt=""/><br /><sub><b>Adriano Lucas</b></sub></a><br /><a href="https://github.com/adriano-12" title="RepiMe">:technologist:</a></td>
  </tr>
</table>
