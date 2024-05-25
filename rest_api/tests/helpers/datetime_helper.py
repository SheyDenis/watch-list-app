import datetime
from typing import Optional


class DatetimeEq:
    """This class is used to compare a datetime object with a datetime string or another datetime object with a timedelta."""

    def __init__(self, dt: datetime, td: Optional[datetime.timedelta] = None):
        """Initializes the DatetimeEq class with the datetime object and the timedelta object.

        Args:
            dt: The datetime object to compare.
            td: Optional timedelta offset from the datetime.
        """
        self._datetime: datetime.datetime = dt
        self._timedelta: datetime.timedelta = td

    def __instancecheck__(self, instance):
        return isinstance(instance, datetime.datetime)

    def __eq__(self, other: datetime):
        if isinstance(other, str):
            try:
                other = datetime.datetime.fromisoformat(other)
            except:
                pass
        if not isinstance(other, datetime.datetime):
            return False

        if self._timedelta is None:
            return self._datetime == other
        return self._datetime == other or (self._datetime + self._timedelta >= other >= self._datetime - self._timedelta)

    def __repr__(self):
        if self._timedelta is None:
            return self._datetime.__repr__()
        return f'{self._datetime.__repr__()} +- {self._timedelta}'
