import os
from os import makedirs
from pathlib import Path

import yaml
from jinja2 import Environment, FileSystemLoader


def _snake_case_to_camel_case(value: str):
    return ''.join(map(lambda word: word.capitalize(), value.split('_')))


class YamlComponentField:
    def __init__(self, data: dict):
        if 'name' not in data:
            raise RuntimeError('Field "name" is required for component\'s field')
        self.name = data['name']
        if not isinstance(self.name, str):
            raise RuntimeError('Field "name" must have "string" type')

        if 'type' not in data:
            raise RuntimeError('Field "type" is required for component\'s field')
        self.type = data['type']
        if not isinstance(self.type, str):
            raise RuntimeError('Field "type" must have "string" type')

        self.array_size = None
        if 'array_size' in data:
            self.array_size = data['array_size']
            if not isinstance(self.array_size, int):
                raise RuntimeError("array_size must be integer type")

        self.array_size_constant = None
        if 'array_size_constant' in data:
            self.array_size_constant = data['array_size_constant']
            if not isinstance(self.array_size_constant, str):
                raise RuntimeError("array_size_constant must be string type and contents name of declared constant")

        if self.array_size and self.array_size_constant:
            raise RuntimeError(
                'Only one of these fields can exist on component\'s field: "array_size" or "array_size_constant"')

    def get_jinja_type(self):
        return self.type

    def get_jinja_name(self):
        if self.array_size:
            return f'{self.name}[{self.array_size}]'
        if self.array_size_constant:
            cpp_constant_name = _snake_case_to_camel_case(self.array_size_constant)
            return f'{self.name}[{cpp_constant_name}]'
        return self.name


class YamlComponentConstant:
    def __init__(self, data: dict):
        if 'name' not in data:
            raise RuntimeError('Field "name" is required for component\'s constant')
        self.name = data['name']

        if 'type' not in data:
            raise RuntimeError('Field "type" is required for component\'s constant')
        self.type = data['type']

        if 'value' not in data:
            raise RuntimeError('Field "type" is required for component\'s constant')
        self.value = data['value']


class YamlComponentClass:
    def __init__(self, data: dict):
        if 'name' not in data:
            raise RuntimeError('Field "name" is required for component\'s class')
        self.name = data['name']
        if not isinstance(self.name, str):
            raise RuntimeError('Field "name" must have "string" type')


class YamlComponent:
    def __init__(self, data: dict):
        if 'class' not in data:
            raise RuntimeError('Field "class" is required for component')
        self.class_name = YamlComponentClass(data['class'])

        self.fields = []
        if 'fields' in data:
            for field_entry in data['fields']:
                self.fields.append(YamlComponentField(field_entry))

        self.constants = []
        if 'constants' in data:
            for constant_entry in data['constants']:
                self.constants.append(YamlComponentConstant(constant_entry))

        self.spaceship_required = False
        if 'spaceship_operator' in data:
            self.spaceship_required = data['spaceship_operator']
            if not isinstance(self.spaceship_required, bool):
                raise RuntimeError('Field "spaceship_operator" must have "bool" type')

    def get_cpp_structure_name(self):
        return _snake_case_to_camel_case(self.class_name.name)

    def get_cpp_filename(self):
        return f'auto_{self.class_name.name}.h'

    def get_jinja_fields(self):
        fields = []
        for field in self.fields:
            fields.append({
                'type': field.get_jinja_type(),
                'name': field.get_jinja_name()
            })
        return fields

    def get_jinja_constants(self):
        constants = []
        for constant in self.constants:
            constants.append({
                'type': constant.type,
                'name': _snake_case_to_camel_case(constant.name),
                'value': constant.value
            })
        return constants


def gather_components_list():
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


def main():
    output_directory = Path('../sources/simulation/components/generated')
    create_sandbox(output_directory)

    environment = Environment(loader=FileSystemLoader("templates/"), trim_blocks=True, lstrip_blocks=True)
    components = generate_components(environment, output_directory)
    generate_cmake(components, environment, output_directory)


def generate_cmake(components: list, environment: Environment,
                   output_directory: Path):
    cmake_template = environment.get_template("ecs_component_list_cmake.jinja")
    cmake_filename = Path(output_directory) / 'CMakeLists.txt'
    with cmake_filename.open(mode="w", encoding="utf-8") as cmake_data:
        component_filename_list = []
        for component in components:
            component_filename_list.append(component.get_cpp_filename())
        content = cmake_template.render(
            components=component_filename_list,
        )
        cmake_data.write(content)
        print('... wrote CMakeLists.txt')


def generate_components(environment, output_directory):
    template = environment.get_template("ecs_component.jinja")
    components = gather_components_list()
    for component in components:
        content = template.render(
            name=component.get_cpp_structure_name(),
            fields=component.get_jinja_fields(),
            constants=component.get_jinja_constants(),
            spaceship_required=component.spaceship_required
        )

        filename = Path(output_directory) / component.get_cpp_filename()
        with filename.open(mode="w", encoding="utf-8") as component_data:
            component_data.write(content)
            print(f"... wrote {component.get_cpp_structure_name()} to {component.get_cpp_filename()}")
    return components


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
