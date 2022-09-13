# Parallel Programming

Desenvolva uma aplicação utilizando a linguagem de programação C e a biblioteca OpenMPI para desenvolver um programa que realize a soma de um vetor de números aleatórios de forma distribuída.

- O vetor deve conter 1000 valores inteiros gerados de forma aleatória entre os números 1 e 100.
- O vetor deverá ser gerado pelo processo com rank zero e deverá ser distribuído através dos mecanismos de comunicação do MPI.
- O processo de soma do vetor deve ser distribuído igualmente entre todos os processos disponíveis para execução.
- O processo de rank zero é responsável por realizar o somatório final, agregando os resultados obtidos por cada um dos processos distribuídos.

**Deverão ser desenvolvidas duas versões do código:**
- Uma versão utilizando as primitivas MPI_Send e MPI_Recv.
- Uma versão utilizando as primitivas MPI_Scatter e MPI_Gather.

**Instalação de dependências:**
```
sudo apt install build-essential openmpi-bin libopenmpi-dev
```

**MPI_Send e MPI_Recv:**
```
mpicc send-recv.c -o sendrecv && mpiexec sendrecv
```

**MPI_Scatter e MPI_Gather:**
```
mpicc scatter-gather.c -o scattergather && mpiexec scattergather
```


