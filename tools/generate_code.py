import os
from os import makedirs
from pathlib import Path

from jinja2 import Environment, FileSystemLoader

from generate_ecs_components import generate_components
from generate_ecs_systems import generate_systems
from utils import get_autogen_directory


def main():
    environment = Environment(loader=FileSystemLoader("templates/"))

    component_roots = [
        Path('../sources/simulation/components')
    ]
    system_roots = [
        Path('../sources/simulation/systems_ecs'),
        Path('../sources/ui/systems_ecs')
    ]

    components = []
    for root in component_roots:
        prepare_output_directory(root)
        components = components + generate_components(environment, root)

    for root in system_roots:
        prepare_output_directory(root)
        generate_systems(components, environment, root)


def prepare_output_directory(root_directory: Path):
    output_directory = root_directory / get_autogen_directory()
    if output_directory.exists():
        for file in os.listdir(output_directory):
            file_path = os.path.join(output_directory, file)
            valid_filename = file.startswith('auto_') or file == 'CMakeLists.txt'
            if not os.path.isfile(file_path) or not valid_filename:
                raise RuntimeError(f'Directory with auto-generated ecs components contains something wrong: {file}')
            os.unlink(file_path)
        output_directory.rmdir()
    makedirs(output_directory)


if __name__ == '__main__':
    main()
