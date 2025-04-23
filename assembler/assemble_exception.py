

class AssembleException(Exception):

    def __init__(self, file_name: str, line_num: int, message: str):
        super().__init__(f'{message}\n\tat {file_name}:{line_num}')