#!/bin/bash

RED_BG="\033[41m"
GREEN_BG="\033[42m"
RESET="\033[0m"

highlight_diff() {
  local origin="$1"
  local expected="$2"
  local highlighted_origin=""
  local highlighted_expected=""

  for ((i = 0; i < ${#origin} || i < ${#expected}; i++)); do
    char_origin="${origin:i:1}"
    char_expected="${expected:i:1}"

    if [[ "$char_origin" != "$char_expected" ]]; then
      highlighted_origin+="${RED_BG}${char_origin}${RESET}"
      highlighted_expected+="${GREEN_BG}${char_expected}${RESET}"
    else
      highlighted_origin+="$char_origin"
      highlighted_expected+="$char_expected"
    fi
  done

  # Exibir os resultados destacados
  echo -e " (resultado obtido): $highlighted_origin"
  echo -e " (resultado esperado): $highlighted_expected"
}

USE_COMPLEX_DATA=false

while [[ $# -gt 0 ]]; do
  case $1 in
  -l | --list)
    CURRENT_LIST="$2"
    shift 2
    ;;
  -q | --question)
    QUESTION_NUMBER="$2"
    shift 2
    ;;
  -c | --complex)
    USE_COMPLEX_DATA=true
    shift
    ;;
  *)
    echo "Opção desconhecida: $1"
    exit 1
    ;;
  esac
done

if [ -z "$QUESTION_NUMBER" ]; then
  echo "utilização: $0 -q <question_number>"
  exit 1
fi

if [ -z "$USE_COMPLEX_DATA" ]; then
  DATA=false
fi

ORIGINAL_DIR=$(pwd)
WORKING_DIR="L$CURRENT_LIST/Q$QUESTION_NUMBER"

case $USE_COMPLEX_DATA in
true)
  DATA_DIR="complex"
  ;;
*)
  DATA_DIR="basic"
  ;;
esac

cd $WORKING_DIR

SOURCE_FILES=$(find . -iname "*.c" -print)

if [ -z "$SOURCE_FILES" ]; then
  echo "não foram encontrados arquivos de código fonte correspondentes à questão fornecida"
  exit 1
fi

gcc -g -fexec-charset=UTF-8 -finput-charset=UTF-8 $SOURCE_FILES -o RL${CURRENT_LIST}Q${QUESTION_NUMBER}

if [ $? -ne 0 ]; then
  echo "^^^ a compilação do código falhou ^^^"
  exit 1
fi

cp RL${CURRENT_LIST}Q${QUESTION_NUMBER} $ORIGINAL_DIR

cd $DATA_DIR

../RL${CURRENT_LIST}Q${QUESTION_NUMBER}

if [ $? -ne 0 ]; then
  echo "^^^ a execução do código falhou ^^^."
  exit 1
fi

echo "código executado com sucesso."

cd $ORIGINAL_DIR

if [ -z "$WORKING_DIR/$DATA_DIR/L${CURRENT_LIST}Q${QUESTION_NUMBER}.expected.out" ]; then
  echo "não foi encontrado um arquivo correspondente ao resultado esperado."
  exit 1
fi

if [ -f "$WORKING_DIR/$DATA_DIR/L${CURRENT_LIST}Q${QUESTION_NUMBER}.out" ]; then
  echo "arquivo de saída encontrado. tentando comparação com resultado esperado..."
  echo

  diff -q $WORKING_DIR/$DATA_DIR/L${CURRENT_LIST}Q${QUESTION_NUMBER}.out $WORKING_DIR/$DATA_DIR/L${CURRENT_LIST}Q${QUESTION_NUMBER}.expected.out &>/dev/null

  if [ $? -ne 0 ]; then
    echo "resultado: os arquivos diferem em algum ponto."
    echo
    echo "comparação:"
    echo

    LINE_NUMBER=0

    while IFS= read -r LINE_OF_ORIGIN || [[ -n "$LINE_OF_ORIGIN" ]]; do
      IFS= read -r EXPECTED_LINE <&3 || [[ -n "$EXPECTED_LINE" ]]
      ((LINE_NUMBER++))

      if [[ "$LINE_OF_ORIGIN" != "$EXPECTED_LINE" ]]; then
        echo "linha $LINE_NUMBER:"
        highlight_diff "$LINE_OF_ORIGIN" "$EXPECTED_LINE"
        echo
      fi
    done <$WORKING_DIR/$DATA_DIR/L${CURRENT_LIST}Q${QUESTION_NUMBER}.out 3<$WORKING_DIR/$DATA_DIR/L${CURRENT_LIST}Q${QUESTION_NUMBER}.expected.out

    exit 1
  fi

  # echo "resultado: output matches expected output. test passed."
  echo "resultado: a saída é equivalente ao que era esperado. teste bem sucedido!"
else
  echo "não encontramos um arquivo de saída válido. pulando comparação."
  exit 1
fi
