import os
from os import makedirs
from pathlib import Path

from jinja2 import Environment, FileSystemLoader

from generate_ecs_components import generate_components
from generate_ecs_systems import generate_systems


def main():
    environment = Environment(loader=FileSystemLoader("templates/"))

    output_directory = Path('../sources/simulation/components/generated')
    create_sandbox(output_directory)
    generate_components(environment, output_directory)

    output_directory = Path('../sources/simulation/systems_ecs/generated')
    create_sandbox(output_directory)
    generate_systems(environment, output_directory)


def create_sandbox(output_directory):
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
