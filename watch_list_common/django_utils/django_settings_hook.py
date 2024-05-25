from typing import Any, Callable, List

from watch_list_common.utils.singleton import Singleton

HookFunc = Callable[[], Any]


class DjangoSettingsHook(Singleton):
    """A class to handle hooks that should be executed after Django settings are loaded."""

    __hooks: List[HookFunc] = []
    __django_settings_loaded: bool = False

    @staticmethod
    def django_settings_loaded():
        instance = DjangoSettingsHook()
        if instance.__django_settings_loaded:
            return

        instance.__django_settings_loaded = True

        for hook in instance.__hooks:
            hook()

    @staticmethod
    def register_django_settings_loaded_hook(func: HookFunc) -> None:
        """Register a hook to be executed after Django settings are loaded.

        Args:
            func: The hook function to be executed after Django settings are loaded.

        Returns:
            None
        """
        instance = DjangoSettingsHook()
        instance.__add_django_settings_loaded_hook(func)

    def __add_django_settings_loaded_hook(self, func: HookFunc):
        self.__hooks.append(func)
        if self.__django_settings_loaded:
            func()
