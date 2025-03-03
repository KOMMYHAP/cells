from utils import snake_case_to_camel_case


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

    def _get_jinja_type(self):
        return self.type

    def _get_jinja_name(self):
        if self.array_size:
            return f'{self.name}[{self.array_size}]'
        if self.array_size_constant:
            cpp_constant_name = snake_case_to_camel_case(self.array_size_constant)
            return f'{self.name}[{cpp_constant_name}]'
        return self.name

    def to_jinja(self):
        return {
            'type_name': self._get_jinja_type(),
            'field_name': self._get_jinja_name()
        }


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

    def to_jinja(self):
        return {
            'type_name': self.type,
            'field_name': snake_case_to_camel_case(self.name),
            'value': self.value
        }


class YamlComponent:
    def __init__(self, data: dict):
        if 'name' not in data:
            raise RuntimeError('Field "name" is required for component')
        self.class_name = data['name']

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

        self.is_tag = False
        if 'is_tag' in data:
            self.is_tag = data['is_tag']
            if not isinstance(self.is_tag, bool):
                raise RuntimeError('Field "is_tag" must have "bool" type')

    def get_cpp_structure_name(self):
        return snake_case_to_camel_case(self.class_name)

    def get_cpp_filename(self):
        return f'auto_{self.class_name}.h'

    def get_jinja_fields(self):
        fields = []
        for field in self.fields:
            fields.append(field.to_jinja())
        return fields

    def get_jinja_constants(self):
        constants = []
        for constant in self.constants:
            constants.append(constant.to_jinja())
        return constants

    def to_jinja(self):
        return {
            'type_name': self.get_cpp_structure_name(),
            'file_name': self.get_cpp_filename(),
            'fields': self.get_jinja_fields(),
            'constants': self.get_jinja_constants(),
            'spaceship_required': self.spaceship_required,
            'is_tag': self.is_tag
        }
