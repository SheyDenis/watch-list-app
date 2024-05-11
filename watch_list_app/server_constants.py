import os
from typing import Final, Iterable, List

from pydantic import SecretStr

from watch_list_common.utils import str2bool


class ServerConstants:
    """Utility class that holds various constants."""

    DJANGO_DEBUG_ENV_VAR: Final[str] = 'DJANGO_DEBUG'
    DJANGO_SECRET_KEY_ENV_VAR: Final[str] = 'DJANGO_SECRET_KEY'
    DJANGO_SECRET_KEY_FALLBACKS_ENV_VAR: Final[str] = 'DJANGO_SECRET_KEY_FALLBACKS'

    def __new__(cls, *args, **kwargs):
        raise RuntimeError(f'Do not create instances of [{cls.__name__}] class')

    @staticmethod
    def django_secret_key() -> SecretStr:
        return SecretStr(os.environ[ServerConstants.DJANGO_SECRET_KEY_ENV_VAR])

    @staticmethod
    def django_secret_key_fallbacks() -> Iterable[SecretStr]:
        if os.environ.get(ServerConstants.DJANGO_SECRET_KEY_FALLBACKS_ENV_VAR, None) is None:
            return []

        res: List[SecretStr] = [SecretStr(v) for v in os.environ[ServerConstants.DJANGO_SECRET_KEY_FALLBACKS_ENV_VAR].split(' ')]
        return res

    @staticmethod
    def django_debug() -> bool:
        return str2bool(os.environ.get(ServerConstants.DJANGO_DEBUG_ENV_VAR, 'False'))
