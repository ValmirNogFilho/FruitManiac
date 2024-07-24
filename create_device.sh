#!/bin/bash
SCRIPT_DIR=$(dirname "$(realpath "$0")")
# Verifica se o módulo foi passado como argumento
if [ $# -ne 1 ]; then
  echo "Uso: $0 <nome_do_modulo>"
  exit 1
fi

MODULE_NAME=$1

# Obtém o major number do módulo
major=$(awk "\$2 == \"$MODULE_NAME\" {print \$1}" /proc/devices)

if [ -z "$major" ]; then
  echo "Erro: Major number não encontrado para $MODULE_NAME."
  exit 1
fi

# Cria o dispositivo usando mknod
sudo mknod /dev/$MODULE_NAME c $major 0

echo "Dispositivo /dev/$MODULE_NAME criado com sucesso."

