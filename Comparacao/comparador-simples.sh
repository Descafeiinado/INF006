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

{
  ORIGINAL_DIR=$(pwd)

  GABARITO="julio_rodando_com_gabarito.out"
  PROPRIO="julio_rodando_proprio_codigo.out"

  if [ -z "$PROPRIO" ] || [ -z "$GABARITO" ]; then
    echo -e "${RED_BG}Erro:${RESET} não foram encontrados alguns dos arquivos correspondentes."
    exit 1
  fi

  echo -e "${YELLOW_TEXT}[Etapa] tentando comparação entre os resultados...${RESET}"
  echo

  LINE_NUMBER=0
  DIFFERENT_LINES=0

  while IFS= read -r LINE_OF_ORIGIN || [[ -n "$LINE_OF_ORIGIN" ]]; do
    IFS= read -r EXPECTED_LINE <&3 || [[ -n "$EXPECTED_LINE" ]]
    ((LINE_NUMBER++))

    if [[ "$LINE_OF_ORIGIN" != "$EXPECTED_LINE" ]]; then
      ((DIFFERENT_LINES++))

      echo -e "${RED_BG}Diferença:${RESET} ${WHITE_TEXT}na linha $LINE_NUMBER${RESET}"
      highlight_diff "$LINE_OF_ORIGIN" "$EXPECTED_LINE"
      echo
    else
      echo -e "${CYAN_TEXT}linha $LINE_NUMBER: ${RESET}resultados equivalentes"
    fi
  done <$PROPRIO 3<$GABARITO

  if [ $DIFFERENT_LINES -gt 0 ]; then
    echo -e "${RED_BG}Resultado:${RESET} diferenças encontradas. TESTE FALHOU. (${DIFFERENT_LINES} diferenças)"
  else
    echo -e "${GREEN_BG}Resultado:${RESET} os resultados são equivalentes. TESTE BEM SUCEDIDO!"
  fi
} | tee resultado.log
