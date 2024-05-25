from typing import Dict, Type, TypeVar

T = TypeVar('T')


class Singleton:
    """Singleton base class to ensure only one instance of a class is created."""
    __instances: Dict[Type, 'Singleton'] = {}

    def __new__(cls, *args, **kwargs):
        if cls not in Singleton.__instances:
            Singleton.__instances[cls] = super(Singleton, cls).__new__(cls, *args, **kwargs)

        return Singleton.__instances[cls]

    @classmethod
    def instance(cls: T) -> T:
        return Singleton.__instances[cls]
