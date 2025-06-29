import os
from pathlib import Path

import yaml

from utils import snake_case_to_camel_case
from yaml_ecs_component import YamlComponent


class YamlSystemResource:
    def __init__(self, data: dict):
        if 'name' not in data:
            raise RuntimeError('Field "name" is required for system\'s resource')
        self.name = data['name']

        self.readonly = False
        if 'readonly' in data:
            self.readonly = data['readonly']

        if 'include_path' not in data:
            raise RuntimeError('Field "include_path" is required for system\'s resource')
        self.include_path = data['include_path']

        self.forward_declaration_only = True
        if 'forward_declaration_only' in data:
            self.forward_declaration_only = data['forward_declaration_only']

    def get_resource_type_name(self) -> str:
        return snake_case_to_camel_case(self.name)

    def get_cpp_field(self) -> str:
        resource_type = self.get_resource_type_name()
        name = resource_type[0].lower() + resource_type[1:]
        if self.readonly:
            resource_type = 'const ' + resource_type
        return f'gsl::not_null<{resource_type}*> _{name}'

    def get_argument_type(self) -> str:
        resource_type = self.get_resource_type_name()
        if self.readonly:
            resource_type = 'const ' + resource_type
        return f'{resource_type}&'

    def get_field_type(self) -> str:
        resource_type = self.get_resource_type_name()
        if self.readonly:
            resource_type = 'const ' + resource_type
        return f'gsl::not_null<{resource_type}*>'

    def get_argument_name(self) -> str:
        resource_type = self.get_resource_type_name()
        return resource_type[0].lower() + resource_type[1:]

    def to_jinja(self):
        return {
            'field_type': self.get_field_type(),
            'type_name': self.get_resource_type_name(),
            'include_path': self.include_path,
            'argument_type': self.get_argument_type(),
            'argument_name': self.get_argument_name(),
            'forward_declaration_only': self.forward_declaration_only
        }


class YamlSystemComponent:
    def __init__(self, data: dict, components_index: dict[str, YamlComponent]):
        if 'name' not in data:
            raise RuntimeError('Field "name" is required for system\'s component')
        name = data['name']
        if name not in components_index:
            raise RuntimeError(f'Component with name {name} was not registed!')

        self.readonly = False
        if 'readonly' in data:
            self.readonly = data['readonly']

        self.excluded = False
        if 'excluded' in data:
            self.excluded = data['excluded']

        self._component = components_index[name]

    def is_tag(self):
        return self._component.is_tag

    def _get_argument_type_and_name(self) -> str:
        component_type = self._component.get_cpp_structure_name()
        name = component_type[0].lower() + component_type[1:]
        if self.readonly:
            component_type = 'const ' + component_type
        return f'{component_type}& {name}'

    def to_jinja(self):
        properties = self._component.to_jinja()
        properties['excluded'] = self.excluded
        properties['argument_type_and_name'] = self._get_argument_type_and_name()
        return properties


class YamlSystem:
    def __init__(self, data: dict, components_index: dict[str, YamlComponent]):
        if 'name' not in data:
            raise RuntimeError('Field "class" is required for system')
        self.system_name = data['name']

        self.components = []
        self.excluded_components = []
        if 'components' in data:
            for component_entry in data['components']:
                component = YamlSystemComponent(component_entry, components_index)
                if component.excluded:
                    self.excluded_components.append(component)
                else:
                    self.components.append(component)

        self.resources = []
        if 'resources' in data:
            for resource_entry in data['resources']:
                self.resources.append(YamlSystemResource(resource_entry))

        if 'is_ecs' not in data:
            raise RuntimeError('Field "is_ecs" is required for system')
        self.is_ecs = data['is_ecs']

    def get_cpp_structure_name(self) -> str:
        return snake_case_to_camel_case(self.system_name)

    def get_source_filename(self) -> str:
        return f'auto_{self.system_name}.cpp'

    def get_header_filename(self) -> str:
        return f'auto_{self.system_name}.h'

    def get_factory_filename(self) -> str:
        return f'auto_make_{self.system_name}.h'

    def components_to_jinja(self):
        return [c.to_jinja() for c in self.components if not c.is_tag()]

    def tags_to_jinja(self):
        return [c.to_jinja() for c in self.components if c.is_tag()]

    def components_and_tags_to_jinja(self):
        return self.components_to_jinja() + self.tags_to_jinja()

    def excluded_components_to_jinja(self):
        return [c.to_jinja() for c in self.excluded_components]

    def resources_to_jinja(self):
        return [r.to_jinja() for r in self.resources]

    def to_jinja(self):
        return {
            'type_name': self.get_cpp_structure_name(),
            'file_name': self.get_header_filename(),
            'source_file_name': self.get_source_filename(),
            'factory_file_name': self.get_factory_filename(),
            'components': self.components_to_jinja(),
            'tags': self.tags_to_jinja(),
            'components_and_tags': self.components_and_tags_to_jinja(),
            'is_tag_only_system': self.is_tag_only_system(),
            'excluded_components': self.excluded_components_to_jinja(),
            'resources': self.resources_to_jinja(),
            'is_ecs': self.is_ecs
        }

    def is_tag_only_system(self):
        return len(self.components_to_jinja()) == 0


def gather_systems_list(components: list[YamlComponent]):
    input_directory = Path('../sources/simulation/systems_ecs')
    if not input_directory.exists():
        return []

    components_index = {}
    for component in components:
        components_index[component.class_name] = component

    systems = []
    for file in os.listdir(input_directory):
        if not file.endswith('.yaml'):
            continue
        file_path = input_directory / file
        with file_path.open(mode="r", encoding="utf-8") as system_data:
            content = system_data.read()
            system_data = yaml.safe_load(content)
            systems.append(YamlSystem(system_data, components_index))
    return systems


def _generate_systems_sources(environment, systems: list[YamlSystem], output_directory):
    template = environment.get_template("ecs_system_source.jinja")
    for system in systems:
        content = template.render(
            system=system.to_jinja()
        )
        filename = Path(output_directory) / system.get_source_filename()
        with filename.open(mode="w", encoding="utf-8") as system_data:
            system_data.write(content)
            print(f"... wrote {system.get_source_filename()}")


def _generate_systems_headers(environment,
                              systems: list[YamlSystem],
                              output_directory):
    template = environment.get_template("ecs_system_header.jinja")
    for system in systems:
        content = template.render(
            system=system.to_jinja()
        )
        filename = Path(output_directory) / system.get_header_filename()
        with filename.open(mode="w", encoding="utf-8") as system_data:
            system_data.write(content)
            print(f"... wrote {system.get_header_filename()}")


def _generate_systems_factories(environment, systems: list[YamlSystem], output_directory):
    template = environment.get_template("ecs_system_factory_header.jinja")
    for system in systems:
        content = template.render(
            system=system.to_jinja()
        )
        filename = Path(output_directory) / system.get_factory_filename()
        with filename.open(mode="w", encoding="utf-8") as system_data:
            system_data.write(content)
            print(f"... wrote {system.get_factory_filename()}")


def _generate_cmake(systems: list, environment, output_directory: Path):
    cmake_template = environment.get_template("ecs_systems_cmake.jinja")
    cmake_filename = Path(output_directory) / 'CMakeLists.txt'
    with cmake_filename.open(mode="w", encoding="utf-8") as cmake_data:
        systems_yaml_data = []
        for system in systems:
            systems_yaml_data.append(system.to_jinja())
        content = cmake_template.render(
            systems=systems_yaml_data
        )
        cmake_data.write(content)
        print('... wrote CMakeLists.txt for ecs systems')


def generate_systems(components: list[YamlComponent], environment, output_directory):
    systems = gather_systems_list(components)
    _generate_systems_sources(environment, systems, output_directory)
    _generate_systems_headers(environment, systems, output_directory)
    _generate_systems_factories(environment, systems, output_directory)
    _generate_cmake(systems, environment, output_directory)
