def snake_case_to_camel_case(value: str) -> str:
    return ''.join(map(lambda word: word.capitalize(), value.split('_')))


def decapitalize(value: str) -> str:
    return value[0].lower() + value[1:]


def get_autogen_directory() -> str:
    return 'generated'
