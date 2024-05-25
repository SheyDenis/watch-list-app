import datetime
import json
from typing import Final, Optional

from django.conf import settings

from watch_list_common.django_utils.django_settings_hook import DjangoSettingsHook


class DatetimeUtils:
    """A utility class for handling datetime objects."""

    DEFAULT_TZ: Final[datetime.timezone] = ...

    def __new__(cls, *args, **kwargs):
        raise RuntimeError(f'Do not create instances of [{cls.__name__}] class')

    @staticmethod
    def parse_datetime(date_str: str, tz: Optional[datetime.timezone] = None) -> datetime.datetime:
        if tz is None:
            tz = DatetimeUtils.DEFAULT_TZ
        return datetime.datetime.fromisoformat(date_str).astimezone(tz)

    @staticmethod
    def format_datetime(dt: datetime.datetime, tz: Optional[datetime.timezone] = None) -> str:
        if tz is None:
            tz = DatetimeUtils.DEFAULT_TZ
        return dt.astimezone(tz).isoformat()

    @staticmethod
    def now(tz: Optional[datetime.timezone] = None) -> datetime.datetime:
        if tz is None:
            tz = DatetimeUtils.DEFAULT_TZ
        return datetime.datetime.now(tz)

    @staticmethod
    def __django_settings_loaded_hook() -> None:
        DatetimeUtils.DEFAULT_TZ = getattr(datetime, settings.TIME_ZONE)


class JSONDatetimeEncoder(json.JSONEncoder):
    """A JSONEncoder that can handle datetime objects."""

    def default(self, obj):
        if isinstance(obj, datetime.datetime):
            return DatetimeUtils.format_datetime(obj, tz=obj.tzinfo)
        return json.JSONEncoder.default(self, obj)


DjangoSettingsHook.register_django_settings_loaded_hook(DatetimeUtils._DatetimeUtils__django_settings_loaded_hook)
