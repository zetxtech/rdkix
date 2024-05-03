set -e

# Clean and create distribution folder
MINIMALLIB_OUTPUT_PATH="Code/MinimalLib/dist"
rm -rf $MINIMALLIB_OUTPUT_PATH
mkdir -p $MINIMALLIB_OUTPUT_PATH

# Build distribution files
RDKIX_BRANCH=${1:-master}
RDKIX_GIT_URL=${2:-"https://github.com/rdkit/rdkit.git"}
echo "Building distribution files for release $RDKIX_BRANCH from repo $RDKIX_GIT_URL"
DOCKER_BUILDKIT=1 docker build --no-cache -f docker/Dockerfile --build-arg RDKIX_BRANCH=$RDKIX_BRANCH --build-arg RDKIX_GIT_URL=$RDKIX_GIT_URL -o $MINIMALLIB_OUTPUT_PATH .

# Make files executable
chmod a+rwx $MINIMALLIB_OUTPUT_PATH/RDKix_minimal.js
chmod a+rwx $MINIMALLIB_OUTPUT_PATH/RDKix_minimal.wasm

# Log build completed
echo "Build completed"
echo "MinimalLib distribution files are at $MINIMALLIB_OUTPUT_PATH"
