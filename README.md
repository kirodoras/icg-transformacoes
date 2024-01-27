# Transformações no Plano com interpolação linear

Este é um projeto simples em C para transformação no plano em imagens PPM utilizando uma matriz de transformação. Foi desenvolvido como parte do curso no Departamento de Matemática da Universidade Federal de Sergipe, sob a supervisão do Prof. Evilson Vieira.

## Compilação e Execução

Para compilar o projeto, utilize o seguinte comando:

```bash
gcc -o tnp tnp.c lib/matriz.c
```

Para executar o programa, use o seguinte comando, substituindo "input.ppm" pelo nome do arquivo de entrada desejado:

```bash
./tnp input.ppm
```

O programa gera um arquivo de saída chamado "output.ppm" que contém a imagem transformada.

## Resultados

- A imagem foi ampliada usando transformações no plano.

#### Imagem com interpolação linear:

<div>
    <img src="./imagens/output-i.png">
</div>

#### Imagem sem interpolação linear:

<div>
    <img src="./imagens/output-s.png">
</div>