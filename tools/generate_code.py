import os
from os import makedirs
from pathlib import Path

from jinja2 import Environment, FileSystemLoader

from generate_ecs_components import generate_components
from generate_ecs_systems import generate_systems
from utils import get_autogen_directory


def main():
    environment = Environment(
        loader=FileSystemLoader("templates/"),
        trim_blocks=True,
        lstrip_blocks=True,
    )

    component_entries = [
        {
            'root': Path('../sources/simulation/components'),
            'name': 'simulation_components'
        },
        {
            'root': Path('../sources/projects/conway_life/components'),
            'name': 'conway_life_components'
        }
    ]
    system_entries = [
        {
            'root': Path('../sources/simulation/systems_ecs'),
            'name': 'simulation_systems'
        },
        {
            'root': Path('../sources/ui/systems_ecs'),
            'name': 'ui_systems'
        },
        {
            'root': Path('../sources/projects/conway_life/systems'),
            'name': 'conway_life_systems'
        }
    ]

    existing_components = []
    for entry in component_entries:
        root = entry['root']
        name = entry['name']
        prepare_output_directory(root)
        existing_components = existing_components + generate_components(environment, root, name)

    for entry in system_entries:
        root = entry['root']
        name = entry['name']
        prepare_output_directory(root)
        generate_systems(existing_components, environment, root, name)


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
