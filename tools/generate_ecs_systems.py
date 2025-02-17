import os
import re
from os import makedirs
from pathlib import Path

from jinja2 import Environment, FileSystemLoader


class EcsComponentField:
    def __init__(self, field_type: str, name: str):
        self.field_type = field_type
        self.name = name


class EcsComponent:
    def __init__(self, name: str):
        self.fields = []
        self._name = name

    def add_field(self, field_type: str, name: str):
        self.fields.append(EcsComponentField(field_type, name))

    def get_cpp_filename(self):
        return f'auto_{self._name.lower()}.h'

    def get_cpp_structure_name(self):
        return ''.join(map(lambda word: word.capitalize(), self._name.split('_')))


class LuaComponentRegex:
    def __init__(self):
        self.class_name = re.compile(r'(---@class (?P<CLASS_NAME>[a-z_]+))')
        self.field_name = re.compile(r'(---@field (?P<FIELD_NAME>[a-z0-9_]+)\s+(?P<FIELD_TYPE>[a-z0-9_]+))')


def parse_component(component_regex: LuaComponentRegex, content: str) -> EcsComponent | None:
    class_name_list = component_regex.class_name.findall(content)
    if len(class_name_list) != 1:
        print(f'Exactly one class name allowed for component: pattern "{component_regex.class_name}"')
        return None
    class_name = class_name_list[0][1]

    fields_list = component_regex.field_name.findall(content)
    if len(fields_list) == 0:
        print(f'At least one field must exist: pattern "{component_regex.field_name}"')
        return None

    component = EcsComponent(class_name)
    for field_entry in fields_list:
        field_name = field_entry[1]
        field_type = field_entry[2]
        component.add_field(field_type, field_name)
    return component


def gather_components_list():
    components = []
    component_regex = LuaComponentRegex()
    input_directory = Path('../sources/simulation/components')
    if input_directory.exists():
        for file in os.listdir(input_directory):
            if not file.endswith('.lua'):
                continue
            file_path = input_directory / file
            with file_path.open(mode="r", encoding="utf-8") as component_data:
                component = parse_component(component_regex, component_data.read())
                if not component:
                    break
                components.append(component)
    return components


def main():
    output_directory = Path('../sources/simulation/components/generated')
    if output_directory.exists():
        for file in os.listdir(output_directory):
            file_path = os.path.join(output_directory, file)
            valid_filename = file.startswith('auto_') or file == 'CMakeLists.txt'
            if not os.path.isfile(file_path) or not valid_filename:
                raise RuntimeError(f'Directory with auto-generated ecs components contains something wrong: {file}')
            os.unlink(file_path)
        output_directory.rmdir()
    makedirs(output_directory)

    components = gather_components_list()

    environment = Environment(loader=FileSystemLoader("templates/"), trim_blocks=True, lstrip_blocks=True)
    template = environment.get_template("ecs_component.jinja")

    for component in components:
        content = template.render(
            name=component.get_cpp_structure_name(),
            fields=component.fields)

        filename = Path(output_directory) / component.get_cpp_filename()
        with filename.open(mode="w", encoding="utf-8") as component_data:
            component_data.write(content)
            print(f"... wrote {component.get_cpp_structure_name()} to {component.get_cpp_filename()}")

    cmake_template = environment.get_template("ecs_component_list_cmake.jinja")
    cmake_filename = Path(output_directory) / 'CMakeLists.txt'
    with cmake_filename.open(mode="w", encoding="utf-8") as cmake_data:
        component_filename_list = []
        for component in components:
            component_filename_list.append(component.get_cpp_filename())
        content = cmake_template.render(components=component_filename_list)
        cmake_data.write(content)
        print('... wrote CMakeLists.txt')


if __name__ == '__main__':
    main()
