import re
from pathlib import Path
import shutil
from typing import List

def replace_names(dir_path: Path):
    for item in dir_path.iterdir():
        if item.is_dir():
            replace_names(item)
        new_name = item.name
        new_name = new_name.replace("rdkit", "rdkix")
        new_name = new_name.replace("RDKit", "RDKix")
        new_name = new_name.replace('Rdkit', 'Rdkix')
        new_name = new_name.replace('RDKIT', 'RDKIX')
        new_name = new_name.replace('RDkit', 'RDkix')
        if new_name != item.name:
            target = item.parent / new_name
            if target.exists():
                raise FileExistsError(f'can not rename, target {target} already exists')
            shutil.move(item, item.parent / new_name)

def replace_keywords(directory: Path, pattern: str, replacement: str, without_suffix: List[str] = ['.md', '.rst']):
    for path in Path(directory).rglob('*'):
        if any(p.name == '.git' for p in path.parents):
            continue
        if path.absolute() == Path(__file__).absolute():
            continue
        if path.is_dir():
            continue
        if without_suffix and path.suffix in without_suffix:
            continue
        try:
            content = path.read_text(encoding='utf-8')
        except UnicodeDecodeError:
            continue
        content_new = re.sub(pattern, replacement, content, flags=re.MULTILINE)
        if content != content_new:
            path.write_text(content_new)

if __name__ == '__main__':
    replace_names(Path.cwd())
    replace_keywords(Path.cwd(), 'rdkit', 'rdkix')
    replace_keywords(Path.cwd(), 'RDKit', 'RDKix')
    replace_keywords(Path.cwd(), 'Rdkit', 'Rdkix')
    replace_keywords(Path.cwd(), 'RDKIT', 'RDKIX')
    replace_keywords(Path.cwd(), 'RDkit', 'RDkix')
    replace_keywords(Path.cwd(), 'rdkix.org', 'rdkit.org')
    replace_keywords(Path.cwd(), 'github.com/rdkix/rdkix', 'github.com/rdkit/rdkit')
    replace_keywords(Path.cwd(), 'github.com/rdkix', 'github.com/rdkit')
    
    # Code/MinimalLib/docker/Dockerfile
    replace_keywords(
        Path.cwd(),
        r'RDKIT_GIT_URL="https://github.com/rdkit/rdkit.git"',
        'RDKIX_GIT_URL="https://github.com/zetxtech/rdkix.git"'
    )
    
    # Code/MinimalLib/scripts/build_rdkixjs.sh
    replace_keywords(
        Path.cwd(),
        r'RDKIT_GIT_URL=\${2:-"https://github.com/rdkit/rdkit.git"}',
        'RDKIX_GIT_URL=${2:-"https://github.com/zetxtech/rdkix.git"}'
    )
    
    # External/RingFamilies/CMakeLists.txt
    replace_keywords(
        Path.cwd(),
        'github.com/rareylab/RingDecomposerLib',
        'github.com/rdkix/RingDecomposerLib'
    )
 
    # External/AvalonTools/CMakeLists.txt
    replace_keywords(
        Path.cwd(),
        r'AVALONTOOLS_URL "https://github.com/rdkit',
        'AVALONTOOLS_URL "https://github.com/rdkix'
    )
    
    # Code/cmake/Modules/RDKitUtils.cmake
    replace_keywords(
        Path.cwd(),
        r'if \(NOT "\${md5chksum}" STREQUAL ""\)',
        'if (FALSE)'
    )