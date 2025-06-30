import os
from pathlib import Path

import yaml
from jinja2 import Environment

from utils import get_autogen_directory
from yaml_ecs_component import YamlComponent


def _gather_components_list():
    components = []
    input_directory = Path('../sources/simulation/components')
    if input_directory.exists():
        for file in os.listdir(input_directory):
            if not file.endswith('.yaml'):
                continue
            file_path = input_directory / file
            with file_path.open(mode="r", encoding="utf-8") as component_data:
                content = component_data.read()
                component_data = yaml.safe_load(content)
                components.append(YamlComponent(component_data))
    return components


def _generate_cmake(components: list, environment: Environment,
                    output_directory: Path):
    cmake_template = environment.get_template("ecs_component_list_cmake.jinja")
    cmake_filename = Path(output_directory) / 'CMakeLists.txt'
    with cmake_filename.open(mode="w", encoding="utf-8") as cmake_data:
        content = cmake_template.render(
            components=components,
        )
        cmake_data.write(content)
        print('... wrote CMakeLists.txt')


def generate_components(environment, root_directory: Path) -> list[YamlComponent]:
    output_directory = root_directory / get_autogen_directory()
    template = environment.get_template("ecs_component.jinja")
    components = _gather_components_list()
    for component in components:
        content = template.render(
            component=component.to_jinja()
        )

        filename = Path(output_directory) / component.get_cpp_filename()
        with filename.open(mode="w", encoding="utf-8") as component_data:
            component_data.write(content)
            print(f"... wrote {component.get_cpp_structure_name()} to {component.get_cpp_filename()}")

    _generate_cmake(components, environment, output_directory)
    return components
