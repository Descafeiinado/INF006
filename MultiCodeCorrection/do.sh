#!/bin/bash

RED_BG="\033[41m"
GREEN_BG="\033[42m"
RESET="\033[0m"
CYAN_TEXT="\033[36m"
YELLOW_TEXT="\033[33m"
WHITE_TEXT="\033[37m"

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

  echo -e "${CYAN_TEXT}(resultado obtido):${RESET} $highlighted_origin"
  echo -e "${CYAN_TEXT}(resultado esperado):${RESET} $highlighted_expected"
}

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
  *)
    echo -e "${RED_BG}Erro:${RESET} Opção desconhecida: $1"
    exit 1
    ;;
  esac
done

if [ -z "$CURRENT_LIST" ] || [ -z "$QUESTION_NUMBER" ]; then
  echo -e "${YELLOW_TEXT}Utilização:${RESET} $0 -l <lista> -q <questao>"
  exit 1
fi

ORIGINAL_DIR=$(pwd)
WORKING_DIR="L$CURRENT_LIST/Q$QUESTION_NUMBER"
EXPECTED_FILE="$WORKING_DIR/L${CURRENT_LIST}Q${QUESTION_NUMBER}.expected.out"
INPUT_FILE_NAME="L${CURRENT_LIST}Q${QUESTION_NUMBER}.in"
INPUT_FILE="$WORKING_DIR/$INPUT_FILE_NAME"

if [ ! -f "$EXPECTED_FILE" ]; then
  echo -e "${RED_BG}Erro:${RESET} Arquivo esperado ($EXPECTED_FILE) não encontrado."
  exit 1
fi

if [ ! -f "$INPUT_FILE" ]; then
  echo -e "${RED_BG}Erro:${RESET} Arquivo de entrada ($INPUT_FILE) não encontrado."
  exit 1
fi

SUBFOLDERS=($(find "$WORKING_DIR" -mindepth 1 -maxdepth 1 -type d | sort))

if [ ${#SUBFOLDERS[@]} -eq 0 ]; then
  echo -e "${RED_BG}Erro:${RESET} Nenhum subdiretório encontrado para processar."
  exit 1
fi

for FOLDER in "${SUBFOLDERS[@]}"; do
  echo

  EXECUTABLE="$(basename "$FOLDER")"

  echo -e "${YELLOW_TEXT}[Etapa] Copiando arquivo de entrada para ${FOLDER}...${RESET}"
  cp "$INPUT_FILE" "$FOLDER/"

  cd "$FOLDER"

  SOURCE_FILE=$(find . -iname "*.c" -print -quit)

  if [ -z "$SOURCE_FILE" ]; then
    echo -e "${RED_BG}Aviso:${RESET} Nenhum arquivo de código fonte encontrado em $FOLDER. Pulando..."
    cd "$ORIGINAL_DIR"
    continue
  fi

  echo -e "${YELLOW_TEXT}[Etapa] Compilando ${SOURCE_FILE}...${RESET}"
  gcc -g -fexec-charset=UTF-8 -finput-charset=UTF-8 "$SOURCE_FILE" -o "$EXECUTABLE"

  if [ $? -ne 0 ]; then
    echo -e "${RED_BG}Erro:${RESET} A compilação do código em ${FOLDER} falhou."
    rm -f "$INPUT_FILE_NAME"
    cd "$ORIGINAL_DIR"
    continue
  fi

  echo -e "${YELLOW_TEXT}[Etapa] Executando ${EXECUTABLE} com entrada ${INPUT_FILE_NAME}...${RESET}"
  ./$EXECUTABLE

  if [ $? -ne 0 ]; then
    echo -e "${RED_BG}Erro:${RESET} A execução do código em ${FOLDER} falhou."
    rm -f "$INPUT_FILE_NAME"
    cd "$ORIGINAL_DIR"
    continue
  fi

  echo -e "${YELLOW_TEXT}[Etapa] Removendo arquivo de entrada de ${FOLDER} (${INPUT_FILE_NAME})...${RESET}"
  rm -f "$INPUT_FILE_NAME"

  echo -e "${YELLOW_TEXT}[Etapa] Comparando saída com resultado esperado...${RESET}"
  OUTPUT_FILE="L${CURRENT_LIST}Q${QUESTION_NUMBER}.out"

  diff -q "$OUTPUT_FILE" "$ORIGINAL_DIR/$EXPECTED_FILE" &>/dev/null

  if [ $? -ne 0 ]; then
    echo -e "${RED_BG}Resultado:${RESET} Diferenças encontradas em ${FOLDER}. (entre o esperado e o gerado)"
    echo -e "${WHITE_TEXT}Comparação linha por linha:${RESET}"
    echo

    LINE_NUMBER=0

    while IFS= read -r LINE_OF_ORIGIN || [[ -n "$LINE_OF_ORIGIN" ]]; do
      IFS= read -r EXPECTED_LINE <&3 || [[ -n "$EXPECTED_LINE" ]]
      ((LINE_NUMBER++))

      if [[ "$LINE_OF_ORIGIN" != "$EXPECTED_LINE" ]]; then
        echo -e "${CYAN_TEXT}Linha $LINE_NUMBER:${RESET}"
        highlight_diff "$LINE_OF_ORIGIN" "$EXPECTED_LINE"
        echo
      fi
    done <"$OUTPUT_FILE" 3<"$ORIGINAL_DIR/$EXPECTED_FILE"
  else
    echo -e "${GREEN_BG}Resultado:${RESET} Saída de ${FOLDER} é equivalente ao esperado."
  fi

  cd "$ORIGINAL_DIR"

  echo -e "${WHITE_TEXT}Aguardando 5 segundos antes de processar o próximo...${RESET}"
  sleep 5
done

echo

cd $ORIGINAL_DIR

# Comparando saídas de todos os diretórios entre si
echo -e "${YELLOW_TEXT}[Etapa] Comparando todas as saídas entre os diretórios...${RESET}"

for ((i = 0; i < ${#SUBFOLDERS[@]}; i++)); do
  for ((j = i + 1; j < ${#SUBFOLDERS[@]}; j++)); do
    echo -e ""
    FOLDER_A="${SUBFOLDERS[i]}"
    FOLDER_B="${SUBFOLDERS[j]}"
    OUTPUT_A="${ORIGINAL_DIR}/$FOLDER_A/L${CURRENT_LIST}Q${QUESTION_NUMBER}.out"
    OUTPUT_B="${ORIGINAL_DIR}/$FOLDER_B/L${CURRENT_LIST}Q${QUESTION_NUMBER}.out"

    if [ -f "$OUTPUT_A" ] && [ -f "$OUTPUT_B" ]; then
      echo -e "${CYAN_TEXT}Comparando saída de $(basename "$FOLDER_A") com $(basename "$FOLDER_B")...${RESET}"

      diff -q "$OUTPUT_A" "$OUTPUT_B" &>/dev/null

      if [ $? -ne 0 ]; then
        echo -e "${RED_BG}Diferenças encontradas entre $(basename "$FOLDER_A") e $(basename "$FOLDER_B")...${RESET}"
        echo -e ""

        LINE_NUMBER=0

        while IFS= read -r LINE_A || [[ -n "$LINE_A" ]]; do
          IFS= read -r LINE_B <&3 || [[ -n "$LINE_B" ]]
          ((LINE_NUMBER++))

          if [[ "$LINE_A" != "$LINE_B" ]]; then
            echo -e "${CYAN_TEXT}Linha $LINE_NUMBER:${RESET}"
            highlight_diff "$LINE_A" "$LINE_B"
            echo
          fi
        done <"$OUTPUT_A" 3<"$OUTPUT_B"
      else
        echo -e "${GREEN_BG}Saídas de $(basename "$FOLDER_A") e $(basename "$FOLDER_B") são equivalentes.${RESET}"
      fi
    fi
  done
done

echo
echo -e "${GREEN_BG}Processamento concluído.${RESET}"
