def snake_case_to_camel_case(value: str) -> str:
    return ''.join(map(lambda word: word.capitalize(), value.split('_')))
