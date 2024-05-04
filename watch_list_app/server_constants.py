from typing import Final


class ServerConstants:
    """Utility class that holds various constants."""

    DJANGO_SECRET_KEY_ENV_VAR: Final[str] = 'DJANGO_SECRET_KEY'

    def __new__(cls, *args, **kwargs):
        raise RuntimeError(f'Do not create instances of [{cls.__name__}] class')
