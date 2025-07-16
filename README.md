# 🧠 MiniLang - Interpretador de Linguagem em C

Este projeto é um **interpretador de uma linguagem própria**, chamada *MiniLang*, desenvolvida em linguagem C. O objetivo é ler comandos simples de um arquivo `programa.txt`, interpretar instruções como declarações de variáveis, operações aritméticas, estruturas de controle (`IF`, `LOOP`) e comandos de entrada e saída.

---

## 🛠️ Comandos Suportados

A linguagem MiniLang suporta os seguintes comandos:

| Comando   | Descrição                                                                 |
|-----------|---------------------------------------------------------------------------|
| `SET`     | Declara ou atualiza uma variável com valor numérico ou outra variável.    |
| `ADD`     | Soma dois operandos e armazena em uma variável.                           |
| `SUB`     | Subtrai dois operandos.                                                    |
| `MUL`     | Multiplica dois operandos.                                                 |
| `DIV`     | Divide dois operandos, com verificação de divisão por zero.               |
| `INPUT`   | Solicita que o usuário digite o valor de uma variável.                    |
| `EXIBIR`  | Exibe o valor de uma variável.                                             |
| `IF`      | Executa um bloco condicional com os operadores `<`, `>`, `==`.            |
| `ENDIF`   | Marca o fim de um bloco condicional.                                       |
| `LOOP`    | Repete um bloco de código um número fixo de vezes.                        |
| `ENDLOOP` | Marca o fim de um loop.                                                    |

---

## 🧾 Exemplo de código MiniLang (`programa.txt`)

<details>
<summary>Clique para expandir</summary>

```plaintext

SET A 10
SET B 5
ADD A B C
EXIBIR C
LOOP 2
  SUB C 1 C
  EXIBIR C
ENDLOOP
IF C > 3
  EXIBIR C
ENDIF
INPUT X
MUL C X R
EXIBIR R

```

</details>

---

## 🔍 Como funciona

1. O programa lê todas as linhas do arquivo `programa.txt`.
2. Cada linha é interpretada e executada de acordo com o comando detectado.
3. As variáveis são armazenadas em um array de structs, funcionando como memória.
4. Há checagem de erros para:
   - Comandos inválidos.
   - Número incorreto de argumentos.
   - Uso de variáveis inexistentes.
   - Divisão por zero.
   - Sintaxe incorreta.

---

## ⚙️ Como compilar e rodar

```bash
gcc -o minilang main.c
./minilang
```
---

## 🎤 Apresentação (HTML Slide)

[👉 Slides de Apresentação](https://ramomsouza07.github.io/minilangc/slides.html)


