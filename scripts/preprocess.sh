#!/usr/bin/env bash

#!/usr/bin/env bash

# GENERATE FLATBUFFERS SOURCES

BLUE='\033[0;34m'
# shellcheck disable=SC2034
RED='\033[0;31m'
NC='\033[0m' # No Color

SCRIPT_PATH=$(realpath $0)
SCRIPT_DIR=$(dirname "${SCRIPT_PATH}")

KRYPTO_DIR=$(realpath "${SCRIPT_DIR}"/..)
RESOURCES_DIR="${KRYPTO_DIR}"/resources

echo -e "${BLUE}KRYPTO_DIR: ${KRYPTO_DIR}${NC}"

echo -e "${BLUE}"==================="${NC}"
echo -e "${BLUE}"--- FLATBUFFERS ---"${NC}"
echo -e "${BLUE}"==================="${NC}"

rm -rf "${SCRIPT_DIR}"/build && mkdir "${SCRIPT_DIR}"/build && cd "${SCRIPT_DIR}"/build || exit
conan install "${SCRIPT_DIR}" -g virtualenv -g cmake -s build_type=Release --profile "${RESOURCES_DIR}"/build.profile --build=missing
source activate.sh && echo -e "${BLUE}Using $(flatc --version)${NC}" || exit
cd ..

echo -e "${BLUE}"--- COMPILE C++ FLATBUFFERS SOURCES ---"${NC}"
flatc --cpp -o "${KRYPTO_DIR}"/cpp/include/krypto/serialization "${RESOURCES_DIR}"/serialization.fbs || exit
echo -e "${BLUE}"--- COMPILE JAVA FLATBUFFERS SOURCES ---"${NC}"
flatc --java -o "${KRYPTO_DIR}"/java/src/main/java "${RESOURCES_DIR}"/serialization.fbs || exit

rm -r "${SCRIPT_DIR}"/build