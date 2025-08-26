# 🔐 Força Bruta com Threads em C

## 🎯 Objetivo

Este programa tem como objetivo **demonstrar a técnica de quebra de senhas por força bruta**, utilizando **threads** para paralelizar as tentativas.  
A senha alvo é fornecida pelo usuário e o programa testa diversas combinações de caracteres até encontrá-la.  
O desempenho é medido variando o número de threads de **1 a 8**, mostrando o tempo gasto em cada execução.

---

## ⚙️ Como executar

### 1. Compilar o código

No terminal (Linux ou WSL):

```bash
gcc -o quebrador_de_senha_paralelo main.c -lpthread
```

### 2. Executar o programa

```bash
./quebrador_de_senha_paralelo
```

### 3. Inserir a senha alvo

Digite a senha que deseja testar.
Exemplo:

```
Digite a senha: igor!
```

O programa mostrará:

- O charset escolhido (números, letras, símbolos, ou combinação)
- A senha encontrada
- O número de tentativas realizadas
- O tempo de execução com cada quantidade de threads

---

## 📊 Exemplo de saída

```
Digite a senha: igor!
Charset escolhido: 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-=[]{}|;:,.<>?
Thread 3 encontrou a senha: igor!
Tentativas realizadas: 125874
Tempo com 1 threads: 46.27 segundos
Tempo com 2 threads: 24.12 segundos
Tempo com 4 threads: 12.07 segundos
Tempo com 8 threads: 6.01 segundos
```

---

## 👥 Integrantes

- Igor Paslauski Pedroso de Oliveira
- Jean Kronbauer de Moura
- Nicolas Castro
- Henrique Froeder

---

## 📚 Tecnologias utilizadas

- **C (padrão C11)**
- **POSIX Threads (pthread)**
- **Funções atômicas (stdatomic.h)**

---

## 🚀 Observações

- O charset utilizado é adaptado automaticamente de acordo com a senha:

  - Apenas números → usa só `0123456789`
  - Apenas letras/símbolos → usa só `abcdefghijklmnopqrstuvwxyz...`
  - Mistura → usa todos os caracteres possíveis

- O código foi projetado para fins **acadêmicos e didáticos**, mostrando na prática a diferença de performance entre execuções **sequenciais** e **paralelas**.
